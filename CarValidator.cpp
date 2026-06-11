#include "CarValidator.h"

bool CarValidator::isValid(const Car& car) const {
    return checkSedanContinental(car)
        && checkSuvToyota(car)
        && checkTruckWia(car)
        && checkTruckMando(car)
        && checkBrakeSteeringCompatibility(car);
}

std::string CarValidator::getInvalidReason(const Car& car) const {
    if (!checkSedanContinental(car))
        return "Sedan에는 Continental 제동장치 사용 불가";
    if (!checkSuvToyota(car))
        return "SUV에는 TOYOTA 엔진 사용 불가";
    if (!checkTruckWia(car))
        return "Truck에는 WIA 엔진 사용 불가";
    if (!checkTruckMando(car))
        return "Truck에는 Mando 제동장치 사용 불가";
    if (!checkBrakeSteeringCompatibility(car))
        return "Bosch 제동장치에는 Bosch 조향장치 이외 사용 불가";
    return "";
}

bool CarValidator::checkBrakeSteeringCompatibility(const Car& car) const {
    return !(car.brakeSystem->getName() == "Bosch" &&
             car.steeringSystem->getName() != "Bosch");
}

bool CarValidator::checkSedanContinental(const Car& car) const {
    return !(car.carType->getName() == "Sedan" &&
             car.brakeSystem->getName() == "Continental");
}

bool CarValidator::checkSuvToyota(const Car& car) const {
    return !(car.carType->getName() == "SUV" &&
             car.engine->getName() == "TOYOTA");
}

bool CarValidator::checkTruckWia(const Car& car) const {
    return !(car.carType->getName() == "Truck" &&
             car.engine->getName() == "WIA");
}

bool CarValidator::checkTruckMando(const Car& car) const {
    return !(car.carType->getName() == "Truck" &&
             car.brakeSystem->getName() == "Mando");
}
