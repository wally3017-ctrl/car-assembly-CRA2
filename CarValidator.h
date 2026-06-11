#pragma once
#include <functional>
#include <string>
#include <vector>
#include "Car.h"

class CarValidator {
public:
    bool        isValid(const Car& car) const;
    std::string getInvalidReason(const Car& car) const;

private:
    struct ValidationRule {
        std::string errorMessage;
        std::function<bool(const Car&)> check;
    };

    static const std::vector<ValidationRule> rules_;
};
