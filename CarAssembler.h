#pragma once
#include <functional>
#include <string>
#include <vector>
#include "Car.h"
#include "ConsoleUI.h"
#include "CarValidator.h"

class CarAssembler {
public:
    explicit CarAssembler(ConsoleUI& ui, const CarValidator& validator);
    void run();

private:
    struct AssemblyStep {
        void (ConsoleUI::*showMenu)() const;
        int  maxChoice;
        std::function<void(Car&, int)>          applyChoice;
        std::function<std::string(const Car&)>  confirmMsg;
    };

    Car  buildCar();
    void handleRunTest(const Car& car);

    ConsoleUI&          ui_;
    const CarValidator& validator_;

    static const std::vector<AssemblyStep> steps_;
};
