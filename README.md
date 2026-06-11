# assembleCar

C++로 구현된 자동차 조립 시뮬레이터입니다.  
사용자가 차량 타입과 부품을 단계별로 선택하면, 조합 유효성을 검사하여 차량을 완성합니다.

---

## 기능

1. **차량 타입 선택** — Sedan / SUV / Truck, `0` 입력으로 프로그램 종료
2. **부품 선택** — 엔진 · 제동장치 · 조향장치를 각각 선택 (`0` 입력으로 이전 단계 이동)
3. **조합 유효성 검사** — 아래 제한 조건을 위반한 조합은 동작 불가
4. **RUN / Test** — 완성 차량 실행 또는 조합 검사 결과 확인

### 부품 목록

| 부품 | 선택지 |
|---|---|
| 엔진 | GM · TOYOTA · WIA |
| 제동장치 | MANDO · CONTINENTAL · BOSCH |
| 조향장치 | BOSCH · MOBIS |

### 조합 제한 조건

| 조건 | 내용 |
|---|---|
| 제한 1 | BOSCH 제동장치를 사용하면 조향장치도 반드시 BOSCH 사용 |
| 제한 2 | Sedan에 CONTINENTAL 제동장치 사용 불가 |
| 제한 3 | SUV에 TOYOTA 엔진 사용 불가 |
| 제한 4 | Truck에 WIA 엔진 사용 불가 |
| 제한 5 | Truck에 MANDO 제동장치 사용 불가 |

---

## 빌드 및 실행

Visual Studio 2022 기준 (`.slnx` 솔루션 파일 사용)

```
# Release 빌드 — 인터랙티브 시뮬레이터 실행
x64/Debug/assembleCar.exe

# Debug 빌드 — GoogleMock 유닛 테스트 실행
#ifdef _DEBUG 블록이 활성화되어 RUN_ALL_TESTS() 실행
```

### 의존성

- [GoogleMock 1.11.0](https://www.nuget.org/packages/gmock/1.11.0) — NuGet 패키지로 포함 (`packages/`)

---

## 프로젝트 구조

```
assembleCar/
├── main.cpp                  # 진입점 (Release: 시뮬레이터 / Debug: 테스트)
├── assembleCar.vcxproj       # Visual Studio 프로젝트 파일
├── assembleCar.slnx          # 솔루션 파일
├── packages/                 # NuGet 패키지 (gmock 1.11.0)
├── docs/                     # 요구사항 문서
└── temp_docs/                # 리팩토링 계획 문서
```

---

## 리팩토링 계획

현재 코드는 절차지향(C-style)으로 구현되어 있습니다.  
객체지향 설계로의 전환 계획은 아래 문서를 참고하세요.

→ **[temp_docs/PLAN.md](temp_docs/PLAN.md)**

| Phase | 범위 | 문서 |
|---|---|---|
| PHASE 1 | Method Level — 함수·변수 단위 품질 개선 | [PHASE1.md](temp_docs/PHASE1.md) |
| PHASE 2 | Class Level — 절차지향 → 객체지향 전환 | [PHASE2.md](temp_docs/PHASE2.md) |
| PHASE 3 | Unit Test — GoogleMock 기반 테스트 작성 | [PHASE3.md](temp_docs/PHASE3.md) |
