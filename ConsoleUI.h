#pragma once
#include <string>
#include "Car.h"

class ConsoleUI {
public:
    virtual ~ConsoleUI() = default;

    virtual void showCarTypeMenu() const;
    virtual void showEngineMenu() const;
    virtual void showBrakeSystemMenu() const;
    virtual void showSteeringSystemMenu() const;
    virtual void showRunTestMenu() const;
    virtual void showMessage(const std::string& msg) const;
    virtual void showError(const std::string& msg) const;
    virtual void showCarInfo(const Car& car) const;
    virtual void delay(int ms) const;

    virtual int readInt(int min, int max);

    bool isExitRequested() const { return exitRequested_; }
    void requestExit()           { exitRequested_ = true;  }

private:
    bool exitRequested_ = false;
};
