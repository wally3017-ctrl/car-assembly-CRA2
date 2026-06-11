#pragma once
#include <string>

class ICarType {
public:
    virtual ~ICarType() = default;
    virtual std::string getName() const = 0;
};
