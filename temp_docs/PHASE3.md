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

PDF 23페이지의 제한 조건을 기반으로 각 조건별 `isValid()` PASS/FAIL 케이스와 `getInvalidReason()` 메시지 검증을 모두 작성한다.

### 테스트 파일: `tests/CarValidatorTest.cpp`

#### isValid() — 16개 케이스

| # | 테스트 케이스 | 조합 | 기대 결과 |
|---|---|---|---|
| TC1 | `BoschBrake_requires_BoschSteering` | Sedan + GM + **Bosch** 제동 + Mobis 조향 | `isValid() == false` |
| TC2 | `BoschBrake_with_BoschSteering_is_valid` | Sedan + GM + **Bosch** 제동 + **Bosch** 조향 | `isValid() == true` |
| TC3 | `Sedan_with_Continental_is_invalid` | **Sedan** + GM + **Continental** 제동 + Mobis | `isValid() == false` |
| TC4 | `Sedan_with_Mando_is_valid` | **Sedan** + GM + Mando 제동 + Mobis | `isValid() == true` |
| TC5 | `SUV_with_Toyota_engine_is_invalid` | **SUV** + **TOYOTA** + Mando + Mobis | `isValid() == false` |
| TC6 | `Truck_with_WIA_engine_is_invalid` | **Truck** + **WIA** + Mando + Mobis | `isValid() == false` |
| TC7 | `Truck_with_Mando_brake_is_invalid` | **Truck** + GM + **Mando** + Mobis | `isValid() == false` |
| TC8 | `valid_combination_passes_all_checks` | Sedan + GM + Mando + Mobis | `isValid() == true` |
| TC9 | `SUV_with_non_Toyota_engine_is_valid` | **SUV** + **GM** + Mando + Mobis | `isValid() == true` |
| TC10 | `Truck_with_non_WIA_non_Mando_is_valid` | **Truck** + **GM** + **Continental** + Mobis | `isValid() == true` |
| TC11 | `getInvalidReason_Bosch_Mobis_mentions_Bosch` | Sedan + GM + Bosch + Mobis | 메시지에 "Bosch" 포함 |
| TC12 | `getInvalidReason_Sedan_Continental_mentions_Continental` | Sedan + GM + Continental + Mobis | 메시지에 "Continental" 포함 |
| TC13 | `getInvalidReason_SUV_Toyota_mentions_TOYOTA` | SUV + TOYOTA + Mando + Mobis | 메시지에 "TOYOTA" 포함 |
| TC14 | `getInvalidReason_Truck_WIA_mentions_WIA` | Truck + WIA + Continental + Mobis | 메시지에 "WIA" 포함 |
| TC15 | `getInvalidReason_Truck_Mando_mentions_Mando` | Truck + GM + Mando + Mobis | 메시지에 "Mando" 포함 |
| TC16 | `getInvalidReason_empty_for_valid_car` | Sedan + GM + Mando + Mobis | `getInvalidReason() == ""` |

> TC11~TC15: `EXPECT_THAT(validator.getInvalidReason(car), testing::HasSubstr("키워드"))` 패턴으로 검증

---

## 3-3. `CarAssembler` 테스트 — 조립 단계 전환 검증

`ConsoleUI`를 `NiceMock`으로 대체하여 입력 시나리오별 조립 결과를 검증한다.  
`showMessage`, `delay` 등 비검증 메서드는 `NiceMock`이 묵시적으로 처리한다.

### MockConsoleUI

```cpp
class MockConsoleUI : public ConsoleUI {
public:
    MOCK_METHOD(int,  readInt,               (int, int),           (override));
    MOCK_METHOD(void, showCarTypeMenu,       (),                   (const, override));
    MOCK_METHOD(void, showEngineMenu,        (),                   (const, override));
    MOCK_METHOD(void, showBrakeSystemMenu,   (),                   (const, override));
    MOCK_METHOD(void, showSteeringSystemMenu,(),                   (const, override));
    MOCK_METHOD(void, showRunTestMenu,       (),                   (const, override));
    MOCK_METHOD(void, showMessage,           (const std::string&), (const, override));
    MOCK_METHOD(void, showError,             (const std::string&), (const, override));
    MOCK_METHOD(void, showCarInfo,           (const Car&),         (const, override));
    MOCK_METHOD(void, delay,                 (int),                (const, override));
};
```

### 테스트 파일: `tests/CarAssemblerTest.cpp`

| # | 테스트 케이스 | 입력 시나리오 | 검증 내용 |
|---|---|---|---|
| TC1 | `SelectSedan_displaysSedan` | Sedan(1)→GM(1)→Mando(1)→Mobis(2)→Exit | `showMessage("...Sedan...")` 호출 확인 |
| TC2 | `SelectToyotaEngine_displaysToyota` | Sedan(1)→TOYOTA(2)→Mando(1)→Mobis(2)→Exit | `showMessage("TOYOTA...")` 호출 확인 |
| TC3 | `BackNavigation_returns_to_CarType` | Sedan(1)→ENGINE back(0)→Exit(0) | `showCarTypeMenu()` 2회 호출 확인 |
| TC4 | `ExitOption_terminates_run` | CarType에서 Exit(0) | `showEngineMenu()` 0회 호출 확인 |
| TC5 | `BrokenEngine_run_does_not_show_carInfo` | Sedan+BrokenEngine(4)+Mando+Mobis→RUN(1)→Exit | `showCarInfo()` 0회 호출 확인 |
| TC6 | `InvalidCar_test_shows_FAIL` | Sedan+GM+Continental+Mobis→Test(2)→Exit | `showMessage("...FAIL...")` 호출 확인 |
| TC7 | `ValidCar_test_shows_PASS` | Sedan+GM+Mando+Mobis→Test(2)→Exit | `showMessage("...PASS...")` 호출 확인 |

---

## 테스트 파일 구조

```
assembleCar/
├── main.cpp                     // 시뮬레이터 진입점만 담당 (#ifdef 제거)
└── tests/
    ├── test_main.cpp            // 테스트 진입점 (InitGoogleMock + RUN_ALL_TESTS)
    ├── CarValidatorTest.cpp     // 제한 조건 5가지 × isValid/getInvalidReason = 16개 케이스
    └── CarAssemblerTest.cpp     // 조립 흐름 × 입력 시나리오 = 7개 케이스
```

> **총 테스트: 23개** (CarValidatorTest 16 + CarAssemblerTest 7)
