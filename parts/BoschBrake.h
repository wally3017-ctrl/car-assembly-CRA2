#pragma once
#include "IBrakeSystem.h"

class BoschBrake : public IBrakeSystem {
public:
    std::string getName() const override { return "Bosch"; }
};
