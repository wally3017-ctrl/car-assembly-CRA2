#pragma once
#include <string>

class IEngine {
public:
    virtual ~IEngine() = default;
    virtual std::string getName() const = 0;
    virtual bool isBroken() const { return false; }
};
