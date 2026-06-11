#pragma once
#include "IEngine.h"

class BrokenEngine : public IEngine {
public:
    std::string getName() const override { return "Broken Engine"; }
    bool isBroken() const override { return true; }
};
