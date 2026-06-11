#pragma once
#include <string>

class ISteeringSystem {
public:
    virtual ~ISteeringSystem() = default;
    virtual std::string getName() const = 0;
};
