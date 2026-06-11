#pragma once
#include <memory>
#include "types/ICarType.h"
#include "parts/IEngine.h"
#include "parts/IBrakeSystem.h"
#include "parts/ISteeringSystem.h"

class Car {
public:
    std::unique_ptr<ICarType>        carType;
    std::unique_ptr<IEngine>         engine;
    std::unique_ptr<IBrakeSystem>    brakeSystem;
    std::unique_ptr<ISteeringSystem> steeringSystem;
};
