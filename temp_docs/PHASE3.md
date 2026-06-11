# PHASE 3 : 유닛 테스트 추가

> 범위: GoogleMock 프레임워크를 활용하여 Phase 2에서 분리된 클래스들의 동작을 검증하는 유닛 테스트를 작성한다.  
> 선행 조건: [PHASE 2](PHASE2.md) 완료 후 진행한다.

---

## 3-1. 테스트 진입점 분리 — `main.cpp`에서 `tests/test_main.cpp` 분리

**문제**
현재 `main.cpp`는 `#ifdef _DEBUG` / `#else` 분기로 테스트 러너와 시뮬레이터 진입점을 한 파일에 공존시키고 있다.

```cpp
// 현재 main.cpp — 두 가지 역할이 혼재
#ifdef _DEBUG
#include "gmock/gmock.h"
int main() { testing::InitGoogleMock(); return RUN_ALL_TESTS(); }
#else
// ... 시뮬레이터 전체 코드
#endif
```

이 구조의 문제점:
- `main.cpp` 하나가 시뮬레이터 진입점과 테스트 진입점 두 가지 책임을 가짐 (SRP 위반)
- 테스트 코드와 제품 코드의 경계가 모호
- 테스트 파일만 별도로 빌드·관리하기 어려움

**변경 내용**

`#ifdef _DEBUG` 블록을 `main.cpp`에서 완전히 제거하고, 테스트 전용 진입점 파일을 신규 생성한다.

```cpp
// main.cpp (변경 후) — 시뮬레이터 진입점만 담당
#include "CarAssembler.h"
#include "ConsoleUI.h"
#include "CarValidator.h"

int main() {
    ConsoleUI     ui;
    CarValidator  validator;
    CarAssembler  assembler(ui, validator);
    assembler.run();
    return 0;
}
```

```cpp
// tests/test_main.cpp (신규) — 테스트 진입점만 담당
#include "gmock/gmock.h"

int main(int argc, char** argv) {
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
```

**빌드 설정 (`assembleCar.vcxproj`)**

Visual Studio 프로젝트에서 빌드 구성별로 포함 파일을 분리한다.

| 빌드 구성 | 포함 파일 | 제외 파일 |
|---|---|---|
| Release | `main.cpp` | `tests/*.cpp` |
| Debug | `tests/test_main.cpp`, `tests/*Test.cpp` | `main.cpp` |

vcxproj 내 파일별 `ExcludedFromBuild` 조건으로 제어한다:

```xml
<!-- assembleCar.vcxproj -->
<ClCompile Include="main.cpp">
  <ExcludedFromBuild Condition="'$(Configuration)'=='Debug'">true</ExcludedFromBuild>
</ClCompile>
<ClCompile Include="tests\test_main.cpp">
  <ExcludedFromBuild Condition="'$(Configuration)'=='Release'">true</ExcludedFromBuild>
</ClCompile>
```

---

## 3-2. `CarValidator` 테스트 — 5가지 제한 조건 검증

PDF 23페이지의 제한 조건을 기반으로 각 조건별 `PASS`/`FAIL` 케이스를 모두 작성한다.

### 테스트 파일: `tests/CarValidatorTest.cpp`

| 테스트 케이스 | 설명 | 기대 결과 |
|---|---|---|
| `BoschBrake_requires_BoschSteering` | Bosch 제동 + Mobis 조향 | `isValid() == false` |
| `BoschBrake_with_BoschSteering_is_valid` | Bosch 제동 + Bosch 조향 | `isValid() == true` |
| `Sedan_with_Continental_is_invalid` | Sedan + Continental 제동 | `isValid() == false` |
| `Sedan_with_Mando_is_valid` | Sedan + Mando 제동 | `isValid() == true` |
| `SUV_with_Toyota_engine_is_invalid` | SUV + TOYOTA 엔진 | `isValid() == false` |
| `Truck_with_WIA_engine_is_invalid` | Truck + WIA 엔진 | `isValid() == false` |
| `Truck_with_Mando_brake_is_invalid` | Truck + MANDO 제동 | `isValid() == false` |
| `valid_combination_passes_all_checks` | Sedan + GM + Mando + Mobis | `isValid() == true` |

```cpp
// tests/CarValidatorTest.cpp
#include "gmock/gmock.h"
#include "../Car.h"
#include "../CarValidator.h"

class CarValidatorTest : public ::testing::Test {
protected:
    CarValidator validator;
};

TEST_F(CarValidatorTest, BoschBrake_requires_BoschSteering) {
    Car car{ CarType::SEDAN, Engine::GM, BrakeSystem::BOSCH, SteeringSystem::MOBIS };
    EXPECT_FALSE(validator.isValid(car));
}

TEST_F(CarValidatorTest, Sedan_with_Continental_is_invalid) {
    Car car{ CarType::SEDAN, Engine::GM, BrakeSystem::CONTINENTAL, SteeringSystem::MOBIS };
    EXPECT_FALSE(validator.isValid(car));
}

TEST_F(CarValidatorTest, valid_combination_passes_all_checks) {
    Car car{ CarType::SEDAN, Engine::GM, BrakeSystem::MANDO, SteeringSystem::MOBIS };
    EXPECT_TRUE(validator.isValid(car));
}

// ... (나머지 케이스 동일 패턴)
```

---

## 3-3. `CarAssembler` 테스트 — 조립 단계 전환 검증

`ConsoleUI`를 Mock으로 대체하여 입력 시나리오별 조립 결과를 검증한다.

### 테스트 파일: `tests/CarAssemblerTest.cpp`

```cpp
// tests/CarAssemblerTest.cpp
#include "gmock/gmock.h"
#include "../CarAssembler.h"
#include "../ConsoleUI.h"

class MockConsoleUI : public ConsoleUI {
public:
    MOCK_METHOD(int, readInt, (int min, int max), (override));
    MOCK_METHOD(void, showCarTypeMenu,        (), (const, override));
    MOCK_METHOD(void, showEngineMenu,         (), (const, override));
    MOCK_METHOD(void, showBrakeSystemMenu,    (), (const, override));
    MOCK_METHOD(void, showSteeringSystemMenu, (), (const, override));
};
```

| 테스트 케이스 | 시나리오 | 검증 내용 |
|---|---|---|
| `assemble_returns_correct_car_type` | 사용자가 1(Sedan) 선택 | `car.carType == CarType::SEDAN` |
| `assemble_returns_correct_engine` | 사용자가 2(TOYOTA) 선택 | `car.engine == Engine::TOYOTA` |
| `back_navigation_returns_to_previous_step` | 이전 단계에서 0 입력 | 이전 메뉴 재표시 |
| `invalid_input_shows_error_and_retries` | 범위 외 숫자 입력 | `showError()` 호출 후 재입력 요청 |

---

## 테스트 파일 구조

```
assembleCar/
├── main.cpp                     // 시뮬레이터 진입점만 담당 (#ifdef 제거)
└── tests/
    ├── test_main.cpp            // 테스트 진입점 (InitGoogleMock + RUN_ALL_TESTS)
    ├── CarValidatorTest.cpp     // 제한 조건 5가지 × PASS/FAIL = 최소 8개 케이스
    └── CarAssemblerTest.cpp     // 조립 흐름 × 입력 시나리오 = 최소 4개 케이스
```
