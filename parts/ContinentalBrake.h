#pragma once
#include "IBrakeSystem.h"

class ContinentalBrake : public IBrakeSystem {
public:
    std::string getName() const override { return "Continental"; }
};
