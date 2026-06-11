# assembleCar 리팩토링 계획

## 현재 코드 문제점 (Legacy System)

PDF 24페이지 기준 기존 시스템의 문제점:
- 절차지향식 코드로 유지보수가 어려운 구조 (단일 `main.cpp`에 모든 로직 집중)
- 안전하지 않은 문법 사용 (`printf`, C-style 문자열, 전역 배열 `int stack[10]`)
- 확장성이 고려되지 않음 (새 차량 타입·부품 추가 시 함수 전체 수정 필요)
- 유닛 테스트 없음 (GoogleMock 의존성만 존재, 실제 테스트 코드 없음)

---

## 리팩토링 계획 — 작은 변경(Method Level) → 큰 변경(Class Level)

| Phase | 범위 | 상세 |
|---|---|---|
| [PHASE 1](PHASE1.md) | Method Level | 함수·변수 단위 국소 수정 |
| [PHASE 2](PHASE2.md) | Class Level | 절차지향 → 객체지향 구조 전환 |
| [PHASE 3](PHASE3.md) | Unit Test | GoogleMock 기반 유닛 테스트 작성 |

---

## [PHASE 1 : Method Level 리팩토링](PHASE1.md)

> 클래스 구조 변경 없이 기존 함수·변수 단위의 코드 품질을 개선한다.

- **1-1.** 중복 조건 로직 제거 — `isValidCheck()` / `testProducedCar()` 통합
- **1-2.** `delay()` Busy-wait 제거 → `std::this_thread::sleep_for()` 교체
- **1-3.** 네이밍 컨벤션 통일 (`selectbrakeSystem` → `selectBrakeSystem` 등)
- **1-4.** Magic Number 제거 — 숫자 리터럴을 enum 값으로 교체
- **1-5.** Dead Code 제거 (`isValidCheck()` 내 도달 불가 `return true;`)
- **1-6.** `runProducedCar()` 출력 로직 → `printCarInfo()` 함수 분리

→ [PHASE1.md 상세 보기](PHASE1.md)

---

## [PHASE 2 : Class Level 리팩토링](PHASE2.md)

> 절차지향 구조를 객체지향 설계로 전환한다. 전역 상태 제거, 책임 분리, 확장성 확보가 목표.  
> 선행 조건: PHASE 1 완료

- **2-1.** 전역 `stack[]` 배열 → `Car` 클래스로 교체
- **2-2.** 검증 로직 → `CarValidator` 클래스로 분리
- **2-3.** 부품별 추상 인터페이스 도입 (`IEngine`, `IBrakeSystem`, `ISteeringSystem`)
- **2-4.** 자동차 타입 → `ICarType` 인터페이스 + 구체 클래스 (`Sedan`, `Suv`, `Truck`)
- **2-5.** 조립 흐름 → `CarAssembler` 클래스로 분리
- **2-6.** UI/출력 → `ConsoleUI` 클래스로 분리 (`printf` → `std::cout`)
- **2-6a.** 첫 화면(차량 타입 선택)에 `0. 종료` 옵션 추가 — `requestExit()` 메서드로 `CarAssembler` 연동

→ [PHASE2.md 상세 보기](PHASE2.md)

---

## [PHASE 3 : 유닛 테스트 추가](PHASE3.md)

> GoogleMock 프레임워크로 Phase 2에서 분리된 클래스들의 동작을 검증한다.  
> 선행 조건: PHASE 2 완료

- **3-1.** 테스트 진입점 분리 — `main.cpp`의 `#ifdef _DEBUG` 블록 제거 → `tests/test_main.cpp` 신규 생성, vcxproj 빌드 구성별 포함 파일 분리
- **3-2.** `CarValidator` 테스트 — 5가지 제한 조건 × PASS/FAIL 케이스 (최소 8개)
- **3-3.** `CarAssembler` 테스트 — Mock UI로 조립 단계 전환·입력 시나리오 검증 (최소 4개)

→ [PHASE3.md 상세 보기](PHASE3.md)

---

## 최종 파일 구조 (목표)

```
assembleCar/
├── main.cpp
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
└── tests/
    ├── test_main.cpp             # 테스트 진입점 (InitGoogleMock + RUN_ALL_TESTS)
    ├── CarValidatorTest.cpp
    └── CarAssemblerTest.cpp
```
