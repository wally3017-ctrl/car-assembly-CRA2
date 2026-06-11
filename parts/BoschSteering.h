#pragma once
#include "ISteeringSystem.h"

class BoschSteering : public ISteeringSystem {
public:
    std::string getName() const override { return "Bosch"; }
};
