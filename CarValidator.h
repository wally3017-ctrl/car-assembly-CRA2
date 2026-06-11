#pragma once
#include <string>
#include "Car.h"

class CarValidator {
public:
    bool        isValid(const Car& car) const;
    std::string getInvalidReason(const Car& car) const;

private:
    bool checkBrakeSteeringCompatibility(const Car& car) const;
    bool checkSedanContinental(const Car& car) const;
    bool checkSuvToyota(const Car& car) const;
    bool checkTruckWia(const Car& car) const;
    bool checkTruckMando(const Car& car) const;
};
