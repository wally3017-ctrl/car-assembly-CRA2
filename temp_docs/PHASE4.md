# PHASE 4 : Pattern Level 리팩토링

> 범위: 반복 코드 구조를 데이터 테이블로 대체한다. 새 부품·규칙 추가 시 기존 제어 흐름을 변경하지 않고 테이블만 확장하면 된다.  
> 선행 조건: [PHASE 3](PHASE3.md) 완료 후 진행한다.

---

## 4-1. `CarAssembler::buildCar()` 반복 단계 구조 제거

**문제**

CAR_TYPE → ENGINE → BRAKE → STEERING 각 단계가 동일한 패턴을 4번 반복한다.

```
showMenu() → readInt(0, N) → isExitRequested() → switch(answer) → make_unique<X>()
           → showMessage() → delay(800) → 다음 단계로 이동
```

새 부품 카테고리를 추가하려면 `switch` 분기, `readInt` 범위, 확인 메시지를 각각 수정해야 하는 **산탄총 수술(Shotgun Surgery)** 문제가 있다.

**변경 내용**

각 단계의 데이터를 `AssemblyStep` 구조체로 선언하고, `steps_` 정적 테이블에 모은다.  
`buildCar()`는 테이블을 인덱스로 순회하며 동일한 로직을 단 한 번만 구현한다.

```cpp
// CarAssembler.h — private 중첩 구조체
struct AssemblyStep {
    void (ConsoleUI::*showMenu)() const;          // 메뉴 표시 메서드 포인터
    int  maxChoice;                               // readInt 상한 (4-2에서 매직 넘버 제거)
    std::function<void(Car&, int)>          applyChoice;  // 선택 적용
    std::function<std::string(const Car&)>  confirmMsg;   // 확인 메시지 생성
};

static const std::vector<AssemblyStep> steps_;
```

```cpp
// CarAssembler.cpp — 단계별 데이터 선언
const std::vector<CarAssembler::AssemblyStep> CarAssembler::steps_ = {
    { &ConsoleUI::showCarTypeMenu,    3, /* applyChoice */, /* confirmMsg */ },
    { &ConsoleUI::showEngineMenu,     4, /* applyChoice */, /* confirmMsg */ },
    { &ConsoleUI::showBrakeSystemMenu,3, /* applyChoice */, /* confirmMsg */ },
    { &ConsoleUI::showSteeringSystemMenu, 2, /* applyChoice */, /* confirmMsg */ },
};
```

```cpp
// CarAssembler.cpp — buildCar() 단순화
Car CarAssembler::buildCar() {
    Car car;
    int stepIdx = 0;

    while (stepIdx < static_cast<int>(steps_.size())) {
        if (ui_.isExitRequested()) return car;

        const auto& step = steps_[stepIdx];
        (ui_.*step.showMenu)();
        int answer = ui_.readInt(0, step.maxChoice);
        if (ui_.isExitRequested()) return car;

        if (answer == 0) {
            if (stepIdx == 0) { ui_.requestExit(); return car; }
            --stepIdx;
            continue;
        }

        step.applyChoice(car, answer);
        ui_.showMessage(step.confirmMsg(car));
        ui_.delay(800);
        ++stepIdx;
    }

    return car;
}
```

**효과**

- 새 조립 단계 추가: `steps_` 테이블에 항목 1개 추가만으로 완료
- `buildCar()` 본문은 변경 불필요

---

## 4-2. 매직 넘버 제거

**문제**

`buildCar()` 내 `readInt(0, 3)`, `readInt(0, 4)`, `readInt(0, 3)`, `readInt(0, 2)` 호출의 상한값이 메뉴 항목 수와 암묵적으로 결합되어 있다.  
메뉴에 항목을 추가하면 `ConsoleUI`와 `CarAssembler` 두 파일을 동시에 수정해야 하는 위험이 있다.

**변경 내용**

각 단계의 `readInt` 상한값을 `AssemblyStep::maxChoice` 필드로 선언한다.  
`buildCar()`는 `ui_.readInt(0, step.maxChoice)`로 호출하여 숫자 리터럴이 단 한 곳에만 존재하게 된다.

```
변경 전 — 4곳에 분산
  readInt(0, 3)  // CAR_TYPE
  readInt(0, 4)  // ENGINE
  readInt(0, 3)  // BRAKE
  readInt(0, 2)  // STEERING

변경 후 — 테이블에 집약
  steps_[0].maxChoice = 3
  steps_[1].maxChoice = 4
  steps_[2].maxChoice = 3
  steps_[3].maxChoice = 2
```

---

## 4-3. `CarValidator` 검증 메서드 중복 제거

**문제**

5개 개별 private 메서드가 모두 `!(A->getName() == "X" && B->getName() == "Y")` 구조를 반복한다.  
새 제한 조건을 추가하려면 ① private 메서드 추가, ② `isValid()` 수정, ③ `getInvalidReason()` 수정 세 곳을 동시에 변경해야 한다.

**변경 내용**

제한 조건을 `ValidationRule` 구조체 배열로 선언한다.  
`isValid()`와 `getInvalidReason()`은 테이블을 순회하는 단일 구현으로 통합된다.

```cpp
// CarValidator.h — private 중첩 구조체
struct ValidationRule {
    std::string errorMessage;
    std::function<bool(const Car&)> check;
};

static const std::vector<ValidationRule> rules_;
```

```cpp
// CarValidator.cpp — 규칙 테이블
const std::vector<CarValidator::ValidationRule> CarValidator::rules_ = {
    { "Bosch 제동장치에는 Bosch 조향장치 이외 사용 불가",
      [](const Car& car){ return !(car.brakeSystem->getName() == "Bosch" &&
                                   car.steeringSystem->getName() != "Bosch"); } },
    { "Sedan에는 Continental 제동장치 사용 불가",
      [](const Car& car){ return !(car.carType->getName() == "Sedan" &&
                                   car.brakeSystem->getName() == "Continental"); } },
    // ... 5개 규칙
};

bool CarValidator::isValid(const Car& car) const {
    for (const auto& rule : rules_)
        if (!rule.check(car)) return false;
    return true;
}

std::string CarValidator::getInvalidReason(const Car& car) const {
    for (const auto& rule : rules_)
        if (!rule.check(car)) return rule.errorMessage;
    return "";
}
```

**효과**

- 새 제한 조건 추가: `rules_` 테이블에 항목 1개 추가만으로 완료
- `isValid()` / `getInvalidReason()` 본문은 변경 불필요

---

## 변경 파일 요약

| 파일 | 변경 내용 |
|---|---|
| `CarAssembler.h` | `AssemblyStep` nested struct 추가, `steps_` static 멤버 선언 |
| `CarAssembler.cpp` | `steps_` 테이블 정의, `buildCar()` 인덱스 순회 방식으로 재작성 |
| `CarValidator.h` | `ValidationRule` nested struct 추가, `rules_` static 멤버 선언, 5개 private 메서드 제거 |
| `CarValidator.cpp` | `rules_` 테이블 정의, `isValid()` / `getInvalidReason()` 루프 방식으로 단순화 |

> 23개 유닛 테스트 전부 통과 확인 (Release / Debug 빌드 모두 오류 없음)
