#include "ConsoleUI.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

static const char* CLEAR_SCREEN = "\033[H\033[2J";

void ConsoleUI::delay(int ms) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void ConsoleUI::showCarTypeMenu() const {
    std::cout << CLEAR_SCREEN;
    std::cout << "        ______________\n";
    std::cout << "       /|            | \n";
    std::cout << "  ____/_|_____________|____\n";
    std::cout << " |                      O  |\n";
    std::cout << " '-(@)----------------(@)--'\n";
    std::cout << "===============================\n";
    std::cout << "어떤 차량 타입을 선택할까요?\n";
    std::cout << "0. 종료\n";
    std::cout << "1. Sedan\n";
    std::cout << "2. SUV\n";
    std::cout << "3. Truck\n";
    std::cout << "===============================\n";
}

void ConsoleUI::showEngineMenu() const {
    std::cout << CLEAR_SCREEN;
    std::cout << "어떤 엔진을 탑재할까요?\n";
    std::cout << "0. 뒤로가기\n";
    std::cout << "1. GM\n";
    std::cout << "2. TOYOTA\n";
    std::cout << "3. WIA\n";
    std::cout << "4. 고장난 엔진\n";
    std::cout << "===============================\n";
}

void ConsoleUI::showBrakeSystemMenu() const {
    std::cout << CLEAR_SCREEN;
    std::cout << "어떤 제동장치를 선택할까요?\n";
    std::cout << "0. 뒤로가기\n";
    std::cout << "1. MANDO\n";
    std::cout << "2. CONTINENTAL\n";
    std::cout << "3. BOSCH\n";
    std::cout << "===============================\n";
}

void ConsoleUI::showSteeringSystemMenu() const {
    std::cout << CLEAR_SCREEN;
    std::cout << "어떤 조향장치를 선택할까요?\n";
    std::cout << "0. 뒤로가기\n";
    std::cout << "1. BOSCH\n";
    std::cout << "2. MOBIS\n";
    std::cout << "===============================\n";
}

void ConsoleUI::showRunTestMenu() const {
    std::cout << CLEAR_SCREEN;
    std::cout << "멋진 차량이 완성되었습니다.\n";
    std::cout << "어떤 동작을 할까요?\n";
    std::cout << "0. 처음 화면으로 돌아가기\n";
    std::cout << "1. RUN\n";
    std::cout << "2. Test\n";
    std::cout << "===============================\n";
}

void ConsoleUI::showMessage(const std::string& msg) const {
    std::cout << msg << "\n";
}

void ConsoleUI::showError(const std::string& msg) const {
    std::cout << "ERROR :: " << msg << "\n";
}

void ConsoleUI::showCarInfo(const Car& car) const {
    std::cout << "Car Type : "      << car.carType->getName()       << "\n";
    std::cout << "Engine : "        << car.engine->getName()        << "\n";
    std::cout << "Brake System : "  << car.brakeSystem->getName()   << "\n";
    std::cout << "SteeringSystem : "<< car.steeringSystem->getName()<< "\n";
}

int ConsoleUI::readInt(int min, int max) {
    std::string line;
    while (true) {
        std::cout << "INPUT > ";
        std::getline(std::cin, line);

        if (line == "exit") {
            exitRequested_ = true;
            return 0;
        }

        try {
            size_t pos;
            int answer = std::stoi(line, &pos);
            if (pos != line.size()) {
                showError("숫자만 입력 가능");
                delay(800);
                continue;
            }
            if (answer < min || answer > max) {
                showError(std::to_string(min) + " ~ " + std::to_string(max) + " 범위만 선택 가능");
                delay(800);
                continue;
            }
            return answer;
        }
        catch (...) {
            showError("숫자만 입력 가능");
            delay(800);
        }
    }
}
