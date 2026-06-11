#include "CarValidator.h"

const std::vector<CarValidator::ValidationRule> CarValidator::rules_ = {
    {
        "Bosch 제동장치에는 Bosch 조향장치 이외 사용 불가",
        [](const Car& car) {
            return !(car.brakeSystem->getName() == "Bosch" &&
                     car.steeringSystem->getName() != "Bosch");
        }
    },
    {
        "Sedan에는 Continental 제동장치 사용 불가",
        [](const Car& car) {
            return !(car.carType->getName() == "Sedan" &&
                     car.brakeSystem->getName() == "Continental");
        }
    },
    {
        "SUV에는 TOYOTA 엔진 사용 불가",
        [](const Car& car) {
            return !(car.carType->getName() == "SUV" &&
                     car.engine->getName() == "TOYOTA");
        }
    },
    {
        "Truck에는 WIA 엔진 사용 불가",
        [](const Car& car) {
            return !(car.carType->getName() == "Truck" &&
                     car.engine->getName() == "WIA");
        }
    },
    {
        "Truck에는 Mando 제동장치 사용 불가",
        [](const Car& car) {
            return !(car.carType->getName() == "Truck" &&
                     car.brakeSystem->getName() == "Mando");
        }
    }
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
