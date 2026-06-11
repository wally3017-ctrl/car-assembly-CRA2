#include "CarAssembler.h"
#include "types/Sedan.h"
#include "types/Suv.h"
#include "types/Truck.h"
#include "parts/GmEngine.h"
#include "parts/ToyotaEngine.h"
#include "parts/WiaEngine.h"
#include "parts/BrokenEngine.h"
#include "parts/MandoBrake.h"
#include "parts/ContinentalBrake.h"
#include "parts/BoschBrake.h"
#include "parts/BoschSteering.h"
#include "parts/MobisSteering.h"

CarAssembler::CarAssembler(ConsoleUI& ui, const CarValidator& validator)
    : ui_(ui), validator_(validator) {}

void CarAssembler::run() {
    while (true) {
        Car car = buildCar();
        if (ui_.isExitRequested()) return;
        handleRunTest(car);
        if (ui_.isExitRequested()) return;
    }
}

Car CarAssembler::buildCar() {
    enum class Step { CAR_TYPE, ENGINE, BRAKE, STEERING, DONE };
    Car car;
    Step step = Step::CAR_TYPE;

    while (step != Step::DONE) {
        if (ui_.isExitRequested()) return car;

        switch (step) {
        case Step::CAR_TYPE: {
            ui_.showCarTypeMenu();
            int answer = ui_.readInt(0, 3);
            if (ui_.isExitRequested()) return car;
            if (answer == 0) { ui_.requestExit(); return car; }
            switch (answer) {
            case 1: car.carType = std::make_unique<Sedan>(); break;
            case 2: car.carType = std::make_unique<Suv>();   break;
            case 3: car.carType = std::make_unique<Truck>(); break;
            }
            ui_.showMessage("차량 타입으로 " + car.carType->getName() + "을(를) 선택하셨습니다.");
            ui_.delay(800);
            step = Step::ENGINE;
            break;
        }
        case Step::ENGINE: {
            ui_.showEngineMenu();
            int answer = ui_.readInt(0, 4);
            if (ui_.isExitRequested()) return car;
            if (answer == 0) { step = Step::CAR_TYPE; break; }
            switch (answer) {
            case 1: car.engine = std::make_unique<GmEngine>();      break;
            case 2: car.engine = std::make_unique<ToyotaEngine>();  break;
            case 3: car.engine = std::make_unique<WiaEngine>();     break;
            case 4: car.engine = std::make_unique<BrokenEngine>();  break;
            }
            ui_.showMessage(car.engine->getName() + " 엔진을 선택하셨습니다.");
            ui_.delay(800);
            step = Step::BRAKE;
            break;
        }
        case Step::BRAKE: {
            ui_.showBrakeSystemMenu();
            int answer = ui_.readInt(0, 3);
            if (ui_.isExitRequested()) return car;
            if (answer == 0) { step = Step::ENGINE; break; }
            switch (answer) {
            case 1: car.brakeSystem = std::make_unique<MandoBrake>();       break;
            case 2: car.brakeSystem = std::make_unique<ContinentalBrake>(); break;
            case 3: car.brakeSystem = std::make_unique<BoschBrake>();       break;
            }
            ui_.showMessage(car.brakeSystem->getName() + " 제동장치를 선택하셨습니다.");
            ui_.delay(800);
            step = Step::STEERING;
            break;
        }
        case Step::STEERING: {
            ui_.showSteeringSystemMenu();
            int answer = ui_.readInt(0, 2);
            if (ui_.isExitRequested()) return car;
            if (answer == 0) { step = Step::BRAKE; break; }
            switch (answer) {
            case 1: car.steeringSystem = std::make_unique<BoschSteering>(); break;
            case 2: car.steeringSystem = std::make_unique<MobisSteering>(); break;
            }
            ui_.showMessage(car.steeringSystem->getName() + " 조향장치를 선택하셨습니다.");
            ui_.delay(800);
            step = Step::DONE;
            break;
        }
        default: break;
        }
    }

    return car;
}

void CarAssembler::handleRunTest(const Car& car) {
    while (true) {
        ui_.showRunTestMenu();
        int answer = ui_.readInt(0, 2);
        if (ui_.isExitRequested() || answer == 0) return;

        if (answer == 1) {
            if (!validator_.isValid(car)) {
                ui_.showMessage("자동차가 동작되지 않습니다");
            }
            else if (car.engine->isBroken()) {
                ui_.showMessage("엔진이 고장나있습니다.");
                ui_.showMessage("자동차가 움직이지 않습니다.");
            }
            else {
                ui_.showCarInfo(car);
                ui_.showMessage("자동차가 동작됩니다.");
            }
            ui_.delay(2000);
        }
        else if (answer == 2) {
            ui_.showMessage("Test...");
            ui_.delay(1500);
            if (!validator_.isValid(car)) {
                ui_.showMessage("자동차 부품 조합 테스트 결과 : FAIL");
                ui_.showMessage(validator_.getInvalidReason(car));
            }
            else {
                ui_.showMessage("자동차 부품 조합 테스트 결과 : PASS");
            }
            ui_.delay(2000);
        }
    }
}
