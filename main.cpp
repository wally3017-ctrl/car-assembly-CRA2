#include "CarAssembler.h"
#include "CarValidator.h"
#include "ConsoleUI.h"

int main()
{
    ConsoleUI    ui;
    CarValidator validator;
    CarAssembler assembler(ui, validator);
    assembler.run();
    return 0;
}