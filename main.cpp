#ifdef _DEBUG

#include "gmock/gmock.h"

int main()
{
    testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

#else

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <chrono>
#include <thread>

#define CLEAR_SCREEN "\033[H\033[2J"

int stack[10];

void selectCarType(int answer);
void selectEngine(int answer);
void selectBrakeSystem(int answer);
void selectSteeringSystem(int answer);
void printCarInfo();
void runProducedCar();
void testProducedCar();
void delay(int ms);
bool isValidCheck();
const char* getInvalidReason();

enum QuestionType
{
    CarType_Q,
    Engine_Q,
    brakeSystem_Q,
    SteeringSystem_Q,
    Run_Test,
};

enum CarType
{
    SEDAN = 1,
    SUV,
    TRUCK
};

enum Engine
{
    GM = 1,
    TOYOTA,
    WIA,
    BROKEN_ENGINE
};

enum brakeSystem
{
    MANDO = 1,
    CONTINENTAL,
    BOSCH_B
};

enum SteeringSystem
{
    BOSCH_S = 1,
    MOBIS
};

// 1-2: busy-wait 제거 → sleep_for 교체
void delay(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main()
{
    char buf[100];
    int step = CarType_Q;

    while (1)
    {
        if (step == CarType_Q)
        {
            printf(CLEAR_SCREEN);
            printf("        ______________\n");
            printf("       /|            | \n");
            printf("  ____/_|_____________|____\n");
            printf(" |                      O  |\n");
            printf(" '-(@)----------------(@)--'\n");
            printf("===============================\n");
            printf("어떤 차량 타입을 선택할까요?\n");
            printf("1. Sedan\n");
            printf("2. SUV\n");
            printf("3. Truck\n");
        }
        else if (step == Engine_Q)
        {
            printf(CLEAR_SCREEN);
            printf("어떤 엔진을 탑재할까요?\n");
            printf("0. 뒤로가기\n");
            printf("1. GM\n");
            printf("2. TOYOTA\n");
            printf("3. WIA\n");
            printf("4. 고장난 엔진\n");
        }
        else if (step == brakeSystem_Q)
        {
            printf(CLEAR_SCREEN);
            printf("어떤 제동장치를 선택할까요?\n");
            printf("0. 뒤로가기\n");
            printf("1. MANDO\n");
            printf("2. CONTINENTAL\n");
            printf("3. BOSCH\n");
        }
        else if (step == SteeringSystem_Q)
        {
            printf(CLEAR_SCREEN);
            printf("어떤 조향장치를 선택할까요?\n");
            printf("0. 뒤로가기\n");
            printf("1. BOSCH\n");
            printf("2. MOBIS\n");
        }
        else if (step == Run_Test)
        {
            printf(CLEAR_SCREEN);
            printf("멋진 차량이 완성되었습니다.\n");
            printf("어떤 동작을 할까요?\n");
            printf("0. 처음 화면으로 돌아가기\n");
            printf("1. RUN\n");
            printf("2. Test\n");
        }
        printf("===============================\n");

        printf("INPUT > ");
        fgets(buf, sizeof(buf), stdin);

        char* context = nullptr;
        strtok_s(buf, "\r", &context);
        strtok_s(buf, "\n", &context);

        if (!strcmp(buf, "exit"))
        {
            printf("바이바이\n");
            break;
        }

        char* checkNumber;
        int answer = strtol(buf, &checkNumber, 10);

        if (*checkNumber != '\0')
        {
            printf("ERROR :: 숫자만 입력 가능\n");
            delay(800);
            continue;
        }

        if (step == CarType_Q && !(answer >= 1 && answer <= 3))
        {
            printf("ERROR :: 차량 타입은 1 ~ 3 범위만 선택 가능\n");
            delay(800);
            continue;
        }

        if (step == Engine_Q && !(answer >= 0 && answer <= 4))
        {
            printf("ERROR :: 엔진은 1 ~ 4 범위만 선택 가능\n");
            delay(800);
            continue;
        }

        if (step == brakeSystem_Q && !(answer >= 0 && answer <= 3))
        {
            printf("ERROR :: 제동장치는 1 ~ 3 범위만 선택 가능\n");
            delay(800);
            continue;
        }

        if (step == SteeringSystem_Q && !(answer >= 0 && answer <= 2))
        {
            printf("ERROR :: 조향장치는 1 ~ 2 범위만 선택 가능\n");
            delay(800);
            continue;
        }

        if (step == Run_Test && !(answer >= 0 && answer <= 2))
        {
            printf("ERROR :: Run 또는 Test 중 하나를 선택 필요\n");
            delay(800);
            continue;
        }

        if (answer == 0 && step == Run_Test)
        {
            step = CarType_Q;
            continue;
        }

        if (answer == 0 && step >= 1)
        {
            step -= 1;
            continue;
        }

        if (step == CarType_Q)
        {
            selectCarType(answer);
            delay(800);
            step = Engine_Q;
        }
        else if (step == Engine_Q)
        {
            selectEngine(answer);
            delay(800);
            step = brakeSystem_Q;
        }
        else if (step == brakeSystem_Q)
        {
            // 1-3: selectbrakeSystem → selectBrakeSystem
            selectBrakeSystem(answer);
            delay(800);
            step = SteeringSystem_Q;
        }
        else if (step == SteeringSystem_Q)
        {
            selectSteeringSystem(answer);
            delay(800);
            step = Run_Test;
        }
        else if (step == Run_Test && answer == 1)
        {
            runProducedCar();
            delay(2000);
        }
        else if (step == Run_Test && answer == 2)
        {
            printf("Test...\n");
            delay(1500);
            testProducedCar();
            delay(2000);
        }
    }
}

void selectCarType(int answer)
{
    stack[CarType_Q] = answer;
    // 1-4: magic number → enum 값
    if (answer == SEDAN)
        printf("차량 타입으로 Sedan을 선택하셨습니다.\n");
    if (answer == SUV)
        printf("차량 타입으로 SUV을 선택하셨습니다.\n");
    if (answer == TRUCK)
        printf("차량 타입으로 Truck을 선택하셨습니다.\n");
}

void selectEngine(int answer)
{
    stack[Engine_Q] = answer;
    // 1-4: magic number → enum 값
    if (answer == GM)
        printf("GM 엔진을 선택하셨습니다.\n");
    if (answer == TOYOTA)
        printf("TOYOTA 엔진을 선택하셨습니다.\n");
    if (answer == WIA)
        printf("WIA 엔진을 선택하셨습니다.\n");
    if (answer == BROKEN_ENGINE)
        printf("고장난 엔진을 선택하셨습니다.\n");
}

// 1-3: selectbrakeSystem → selectBrakeSystem
void selectBrakeSystem(int answer)
{
    stack[brakeSystem_Q] = answer;
    // 1-4: magic number → enum 값
    if (answer == MANDO)
        printf("MANDO 제동장치를 선택하셨습니다.\n");
    if (answer == CONTINENTAL)
        printf("CONTINENTAL 제동장치를 선택하셨습니다.\n");
    if (answer == BOSCH_B)
        printf("BOSCH 제동장치를 선택하셨습니다.\n");
}

void selectSteeringSystem(int answer)
{
    stack[SteeringSystem_Q] = answer;
    // 1-4: magic number → enum 값
    if (answer == BOSCH_S)
        printf("BOSCH 조향장치를 선택하셨습니다.\n");
    if (answer == MOBIS)
        printf("MOBIS 조향장치를 선택하셨습니다.\n");
}

// 1-1: int → bool, dead code(return true) 제거(1-5)
bool isValidCheck()
{
    if (stack[CarType_Q] == SEDAN && stack[brakeSystem_Q] == CONTINENTAL)
        return false;
    if (stack[CarType_Q] == SUV && stack[Engine_Q] == TOYOTA)
        return false;
    if (stack[CarType_Q] == TRUCK && stack[Engine_Q] == WIA)
        return false;
    if (stack[CarType_Q] == TRUCK && stack[brakeSystem_Q] == MANDO)
        return false;
    if (stack[brakeSystem_Q] == BOSCH_B && stack[SteeringSystem_Q] != BOSCH_S)
        return false;
    return true;
}

// 1-1: testProducedCar() 중복 조건 제거를 위한 실패 원인 반환
const char* getInvalidReason()
{
    if (stack[CarType_Q] == SEDAN && stack[brakeSystem_Q] == CONTINENTAL)
        return "Sedan에는 Continental 제동장치 사용 불가";
    if (stack[CarType_Q] == SUV && stack[Engine_Q] == TOYOTA)
        return "SUV에는 TOYOTA 엔진 사용 불가";
    if (stack[CarType_Q] == TRUCK && stack[Engine_Q] == WIA)
        return "Truck에는 WIA 엔진 사용 불가";
    if (stack[CarType_Q] == TRUCK && stack[brakeSystem_Q] == MANDO)
        return "Truck에는 Mando 제동장치 사용 불가";
    if (stack[brakeSystem_Q] == BOSCH_B && stack[SteeringSystem_Q] != BOSCH_S)
        return "Bosch 제동장치에는 Bosch 조향장치 이외 사용 불가";
    return "";
}

// 1-6: runProducedCar()에서 출력 로직 분리
void printCarInfo()
{
    if (stack[CarType_Q] == SEDAN)
        printf("Car Type : Sedan\n");
    if (stack[CarType_Q] == SUV)
        printf("Car Type : SUV\n");
    if (stack[CarType_Q] == TRUCK)
        printf("Car Type : Truck\n");
    if (stack[Engine_Q] == GM)
        printf("Engine : GM\n");
    if (stack[Engine_Q] == TOYOTA)
        printf("Engine : TOYOTA\n");
    if (stack[Engine_Q] == WIA)
        printf("Engine : WIA\n");
    if (stack[brakeSystem_Q] == MANDO)
        printf("Brake System : Mando\n");
    if (stack[brakeSystem_Q] == CONTINENTAL)
        printf("Brake System : Continental\n");
    if (stack[brakeSystem_Q] == BOSCH_B)
        printf("Brake System : Bosch\n");
    if (stack[SteeringSystem_Q] == BOSCH_S)
        printf("SteeringSystem : Bosch\n");
    if (stack[SteeringSystem_Q] == MOBIS)
        printf("SteeringSystem : Mobis\n");
}

// 1-6: 유효성 검사 / 엔진 고장 / 출력 책임 분리
void runProducedCar()
{
    if (!isValidCheck())
    {
        printf("자동차가 동작되지 않습니다\n");
        return;
    }
    if (stack[Engine_Q] == BROKEN_ENGINE)
    {
        printf("엔진이 고장나있습니다.\n");
        printf("자동차가 움직이지 않습니다.\n");
        return;
    }
    printCarInfo();
    printf("자동차가 동작됩니다.\n");
}

// 1-1: 중복 조건 제거 → isValidCheck() / getInvalidReason() 재사용
void testProducedCar()
{
    if (!isValidCheck())
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("%s\n", getInvalidReason());
    }
    else
    {
        printf("자동차 부품 조합 테스트 결과 : PASS\n");
    }
}

#endif
