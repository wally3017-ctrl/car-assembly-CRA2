#pragma once
#include "IEngine.h"

class GmEngine : public IEngine {
public:
    std::string getName() const override { return "GM"; }
};
