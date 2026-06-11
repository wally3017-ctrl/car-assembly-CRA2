#pragma once
#include "ISteeringSystem.h"

class MobisSteering : public ISteeringSystem {
public:
    std::string getName() const override { return "Mobis"; }
};
