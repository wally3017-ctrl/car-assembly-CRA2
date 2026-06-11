#pragma once
#include <string>

class IBrakeSystem {
public:
    virtual ~IBrakeSystem() = default;
    virtual std::string getName() const = 0;
};
