# PHASE 2 : Class Level 리팩토링

> 범위: 절차지향 구조를 객체지향 설계로 전환. 전역 상태 제거, 책임 분리, 확장성 확보를 목표로 한다.  
> 선행 조건: [PHASE 1](PHASE1.md) 완료 후 진행한다.

---

## 2-1. 전역 `stack[]` 배열 → `Car` 클래스로 교체

**문제**
`int stack[10]` 전역 배열이 암묵적 인덱스(`stack[CarType_Q]`, `stack[Engine_Q]` 등)로 차량 상태를 관리한다. 인덱스 범위를 벗어난 접근을 컴파일 타임에 감지할 수 없고, 의미를 파악하려면 `QuestionType` enum을 항상 함께 참조해야 한다.

**변경 내용**
선택된 부품을 명시적 멤버로 가지는 `Car` 클래스를 도입하고 전역 `stack[]`을 제거한다.

```cpp
// Car.h
class Car {
public:
    CarType        carType;
    Engine         engine;
    BrakeSystem    brakeSystem;
    SteeringSystem steeringSystem;
};
```

---

## 2-2. 검증 로직 → `CarValidator` 클래스로 분리

**문제**
`isValidCheck()` / `getInvalidReason()` 이 `main.cpp` 내 전역 함수로 존재하여 재사용·테스트가 어렵다.

**변경 내용**
`CarValidator` 클래스로 추출한다. PDF 23페이지의 5가지 제한 조건을 각각 private 메서드로 분리하여 조건 추가 시 영향 범위를 최소화한다.

```cpp
// CarValidator.h
class CarValidator {
public:
    bool        isValid(const Car& car) const;
    std::string getInvalidReason(const Car& car) const;

private:
    bool checkBrakeSteeringCompatibility(const Car& car) const;  // 제한조건1: Bosch 제동 ↔ Bosch 조향
    bool checkSedanContinental(const Car& car) const;            // 제한조건2: Sedan + Continental 불가
    bool checkSuvToyota(const Car& car) const;                   // 제한조건3: SUV + TOYOTA 불가
    bool checkTruckWia(const Car& car) const;                    // 제한조건4: Truck + WIA 불가
    bool checkTruckMando(const Car& car) const;                  // 제한조건5: Truck + MANDO 불가
};
```

---

## 2-3. 부품별 추상 인터페이스 도입

**목적**
향후 새로운 부품 추가 시 기존 코드 수정 없이 확장 가능하도록 Open/Closed Principle을 적용한다.

**변경 내용**

```cpp
// parts/IEngine.h
class IEngine {
public:
    virtual ~IEngine() = default;
    virtual std::string getName() const = 0;
    virtual bool isBroken()      const { return false; }
};

// parts/IBrakeSystem.h
class IBrakeSystem {
public:
    virtual ~IBrakeSystem() = default;
    virtual std::string getName() const = 0;
};

// parts/ISteeringSystem.h
class ISteeringSystem {
public:
    virtual ~ISteeringSystem() = default;
    virtual std::string getName() const = 0;
};
```

구체 클래스 예시:

| 인터페이스 | 구체 클래스 |
|---|---|
| `IEngine` | `GmEngine`, `ToyotaEngine`, `WiaEngine`, `BrokenEngine` |
| `IBrakeSystem` | `MandoBrake`, `ContinentalBrake`, `BoschBrake` |
| `ISteeringSystem` | `BoschSteering`, `MobisSteering` |

---

## 2-4. 자동차 타입 → `ICarType` 인터페이스 + 구체 클래스

**목적**
PDF 21페이지 "향후에 타입이 더 추가될 수 있다" 요건에 대응한다. 새 타입 추가 시 `switch`/`if-else` 체인 수정 없이 클래스만 추가하면 된다.

**변경 내용**

```cpp
// types/ICarType.h
class ICarType {
public:
    virtual ~ICarType() = default;
    virtual std::string getName() const = 0;
};

// types/Sedan.h
class Sedan : public ICarType {
public:
    std::string getName() const override { return "Sedan"; }
};

// types/Suv.h / types/Truck.h — 동일한 패턴
```

---

## 2-5. 조립 흐름 → `CarAssembler` 클래스로 분리

**문제**
step 관리, 사용자 입력 파싱, `Car` 객체 구성이 모두 `main()` 루프 안에 직접 구현되어 있다.

**변경 내용**
`CarAssembler` 클래스가 단계 전환과 `Car` 객체 구성을 담당한다. `main()`은 `CarAssembler` 객체를 생성하고 `run()`을 호출하는 역할만 수행한다.

```cpp
// CarAssembler.h
class CarAssembler {
public:
    explicit CarAssembler(ConsoleUI& ui, CarValidator& validator);
    void run();          // 메인 루프 (차량 선택 → 조립 → 실행/테스트)

private:
    Car             buildCar();
    void            handleRunTest(const Car& car);

    ConsoleUI&      ui_;
    CarValidator&   validator_;
};
```

---

## 2-6. UI/출력 → `ConsoleUI` 클래스로 분리

**문제**
`printf` 기반 출력과 `fgets`/`strtol` 입력 파싱이 조립 로직과 뒤섞여 있다. UI 변경 시 비즈니스 로직 파일을 수정해야 한다.

**변경 내용**
화면 출력, 입력 파싱, 에러 메시지를 `ConsoleUI` 클래스로 분리한다.  
C `printf` → `std::cout`, `fgets`/`strtol` → 타입 안전한 `readInt()` 메서드로 교체한다.

```cpp
// ConsoleUI.h
class ConsoleUI {
public:
    void showCarTypeMenu()       const;
    void showEngineMenu()        const;
    void showBrakeSystemMenu()   const;
    void showSteeringSystemMenu() const;
    void showRunTestMenu()       const;
    void showError(const std::string& msg) const;
    void showCarInfo(const Car& car)       const;

    int  readInt(int min, int max);  // 범위 검증 포함
};
```

---

## 목표 파일 구조

```
assembleCar/
├── main.cpp                      // 시뮬레이터 진입점 (테스트 코드 없음)
├── Car.h
├── CarValidator.h / CarValidator.cpp
├── CarAssembler.h / CarAssembler.cpp
├── ConsoleUI.h    / ConsoleUI.cpp
├── parts/
│   ├── IEngine.h
│   ├── IBrakeSystem.h
│   ├── ISteeringSystem.h
│   ├── GmEngine.h / ToyotaEngine.h / WiaEngine.h / BrokenEngine.h
│   ├── MandoBrake.h / ContinentalBrake.h / BoschBrake.h
│   └── BoschSteering.h / MobisSteering.h
├── types/
│   ├── ICarType.h
│   └── Sedan.h / Suv.h / Truck.h
└── tests/                        // PHASE 3에서 추가 — 테스트 진입점 및 테스트 파일
    ├── test_main.cpp
    ├── CarValidatorTest.cpp
    └── CarAssemblerTest.cpp
```
