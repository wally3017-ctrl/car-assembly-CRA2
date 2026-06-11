#pragma once
#include "Car.h"
#include "ConsoleUI.h"
#include "CarValidator.h"

class CarAssembler {
public:
    explicit CarAssembler(ConsoleUI& ui, const CarValidator& validator);
    void run();

private:
    Car  buildCar();
    void handleRunTest(const Car& car);

    ConsoleUI&          ui_;
    const CarValidator& validator_;
};
