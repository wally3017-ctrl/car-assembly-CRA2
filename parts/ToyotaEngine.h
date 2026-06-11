#pragma once
#include "IEngine.h"

class ToyotaEngine : public IEngine {
public:
    std::string getName() const override { return "TOYOTA"; }
};
