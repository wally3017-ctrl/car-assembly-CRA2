#pragma once
#include "IBrakeSystem.h"

class MandoBrake : public IBrakeSystem {
public:
    std::string getName() const override { return "Mando"; }
};
