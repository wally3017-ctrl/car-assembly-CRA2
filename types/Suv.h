#pragma once
#include "ICarType.h"

class Suv : public ICarType {
public:
    std::string getName() const override { return "SUV"; }
};
