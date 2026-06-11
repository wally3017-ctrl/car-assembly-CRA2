#pragma once
#include "ICarType.h"

class Sedan : public ICarType {
public:
    std::string getName() const override { return "Sedan"; }
};
