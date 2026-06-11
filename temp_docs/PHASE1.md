# PHASE 1 : Method Level 리팩토링

> 범위: 기존 함수/변수 단위의 국소적 수정. 클래스 구조 변경 없이 코드 품질을 개선한다.

---

## 1-1. 중복 조건 로직 제거 — `isValidCheck()` / `testProducedCar()`

**문제**
`isValidCheck()`와 `testProducedCar()` 두 함수가 동일한 5가지 조합 금지 조건을 각각 독립적으로 구현하고 있어, 조건이 변경될 때 두 곳을 동시에 수정해야 하는 유지보수 위험이 있다.

**변경 내용**
- `isValidCheck()` 반환 타입 `int` → `bool` 로 수정
- `testProducedCar()` 내부의 5개 조건 분기를 제거하고 `isValidCheck()` 호출로 교체
- 실패 원인 문자열 반환을 위한 `getInvalidReason()` 함수를 신규 추출

```cpp
// 변경 전
int isValidCheck() { /* 조건 5개 나열 */ }
void testProducedCar() { /* 동일한 조건 5개 나열 */ }

// 변경 후
bool isValidCheck();
const char* getInvalidReason();
void testProducedCar() {
    if (!isValidCheck())
        printf("자동차 부품 조합 테스트 결과 : FAIL\n%s\n", getInvalidReason());
    else
        printf("자동차 부품 조합 테스트 결과 : PASS\n");
}
```

---

## 1-2. `delay()` 함수 교체 — Busy-wait 제거

**문제**
`volatile int sum`을 이용한 3중 루프 busy-wait은 CPU를 100% 점유하며, 하드웨어·최적화 수준에 따라 실제 대기 시간이 달라지는 비결정적 동작을 한다.

**변경 내용**
`<chrono>` + `<thread>` 기반 `std::this_thread::sleep_for()` 로 교체한다.

```cpp
// 변경 전
void delay(int ms) {
    volatile int sum = 0;
    for (int i = 0; i < 1000; i++)
        for (int j = 0; j < 1000; j++)
            for (int t = 0; t < ms; t++)
                sum++;
}

// 변경 후
#include <chrono>
#include <thread>
void delay(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
```

---

## 1-3. 네이밍 컨벤션 통일

**문제**
- `selectbrakeSystem` 함수명만 소문자 `b`로 시작하여 `selectEngine`, `selectSteeringSystem`과 불일치
- `BOSCH_B` / `BOSCH_S` 처럼 같은 이름에 suffix를 붙여 enum 내 충돌을 우회하는 방식은 가독성이 낮음

**변경 내용**
- `selectbrakeSystem` → `selectBrakeSystem` 으로 이름 변경 (호출부 포함 전체 반영)
- `BOSCH_B` / `BOSCH_S` 는 Phase 2에서 enum을 분리하여 근본적으로 해결 (이 단계에서는 이름만 명확하게 수정)

---

## 1-4. Magic Number 제거 — enum 값으로 교체

**문제**
`runProducedCar()` 내부에서 `stack[CarType_Q] == 1`, `stack[Engine_Q] == 4` 처럼 숫자 리터럴로 비교하여 의미 파악이 어렵다.

**변경 내용**
이미 정의된 `enum CarType`, `enum Engine`, `enum brakeSystem`, `enum SteeringSystem` 값을 사용하도록 교체한다.

```cpp
// 변경 전
if (stack[CarType_Q] == 1)  printf("Car Type : Sedan\n");
if (stack[Engine_Q]  == 4)  printf("엔진이 고장나있습니다.\n");

// 변경 후
if (stack[CarType_Q] == SEDAN)         printf("Car Type : Sedan\n");
if (stack[Engine_Q]  == BROKEN_ENGINE) printf("엔진이 고장나있습니다.\n");
```

---

## 1-5. Dead Code 제거

**문제**
`isValidCheck()` 함수 말미에 `else` 블록 이후 절대 도달할 수 없는 `return true;`가 존재한다 (`main.cpp` line 323).

**변경 내용**
해당 라인을 제거한다.

---

## 1-6. `runProducedCar()` 출력 로직 분리

**문제**
`runProducedCar()` 한 함수에 ① 유효성 검사, ② 엔진 고장 확인, ③ 부품 목록 출력이 모두 혼재되어 있다.

**변경 내용**
부품 정보 출력 로직을 `printCarInfo()` 함수로 추출한다.

```cpp
void printCarInfo();   // 차량 타입, 엔진, 제동장치, 조향장치 이름 출력

void runProducedCar() {
    if (!isValidCheck()) { printf("자동차가 동작되지 않습니다\n"); return; }
    if (stack[Engine_Q] == BROKEN_ENGINE) { printf("엔진이 고장나있습니다.\n..."); return; }
    printCarInfo();
    printf("자동차가 동작됩니다.\n");
}
```
