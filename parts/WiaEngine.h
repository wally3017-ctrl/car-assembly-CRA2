#pragma once
#include "IEngine.h"

class WiaEngine : public IEngine {
public:
    std::string getName() const override { return "WIA"; }
};
