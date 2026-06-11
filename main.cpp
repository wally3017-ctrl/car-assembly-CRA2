#ifdef _DEBUG

#include "gmock/gmock.h"

int main()
{
    testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

#else

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

#endif