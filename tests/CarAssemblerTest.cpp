#include "gmock/gmock.h"
#include "../CarAssembler.h"
#include "../CarValidator.h"
#include "../ConsoleUI.h"

using ::testing::Return;
using ::testing::_;
using ::testing::AtLeast;
using ::testing::NiceMock;

class MockConsoleUI : public ConsoleUI {
public:
    MOCK_METHOD(int,  readInt,               (int, int),              (override));
    MOCK_METHOD(void, showCarTypeMenu,       (),                      (const, override));
    MOCK_METHOD(void, showEngineMenu,        (),                      (const, override));
    MOCK_METHOD(void, showBrakeSystemMenu,   (),                      (const, override));
    MOCK_METHOD(void, showSteeringSystemMenu,(),                      (const, override));
    MOCK_METHOD(void, showRunTestMenu,       (),                      (const, override));
    MOCK_METHOD(void, showMessage,           (const std::string&),    (const, override));
    MOCK_METHOD(void, showError,             (const std::string&),    (const, override));
    MOCK_METHOD(void, showCarInfo,           (const Car&),            (const, override));
    MOCK_METHOD(void, delay,                 (int),                   (const, override));
};

class CarAssemblerTest : public ::testing::Test {
protected:
    CarValidator validator;
};

// TC1: Select Sedan(1) -> showMessage contains "Sedan"
// Input: readInt(0,3)=1(Sedan), readInt(0,4)=1(GM), readInt(0,3)=1(Mando),
//        readInt(0,2)=2(Mobis), readInt(0,2)=0(exit RunTest), readInt(0,3)=0(exit)
TEST_F(CarAssemblerTest, SelectSedan_displaysSedan) {
    NiceMock<MockConsoleUI> mockUI;
    // catch-all first (tried second due to LIFO), specific last (tried first)
    EXPECT_CALL(mockUI, showMessage(_)).Times(testing::AnyNumber());
    EXPECT_CALL(mockUI, showMessage(testing::HasSubstr("Sedan"))).Times(AtLeast(1));
    EXPECT_CALL(mockUI, readInt(0, 3))
        .WillOnce(Return(1))
        .WillOnce(Return(1))
        .WillOnce(Return(0));
    EXPECT_CALL(mockUI, readInt(0, 4)).WillOnce(Return(1));
    EXPECT_CALL(mockUI, readInt(0, 2))
        .WillOnce(Return(2))
        .WillOnce(Return(0));

    CarAssembler assembler(mockUI, validator);
    assembler.run();
}

// TC2: Select TOYOTA engine(2) -> showMessage contains "TOYOTA"
// Input: readInt(0,3)=1(Sedan), readInt(0,4)=2(TOYOTA), readInt(0,3)=1(Mando),
//        readInt(0,2)=2(Mobis), readInt(0,2)=0(exit RunTest), readInt(0,3)=0(exit)
TEST_F(CarAssemblerTest, SelectToyotaEngine_displaysToyota) {
    NiceMock<MockConsoleUI> mockUI;
    // catch-all first (tried second due to LIFO), specific last (tried first)
    EXPECT_CALL(mockUI, showMessage(_)).Times(testing::AnyNumber());
    EXPECT_CALL(mockUI, showMessage(testing::HasSubstr("TOYOTA"))).Times(AtLeast(1));
    EXPECT_CALL(mockUI, readInt(0, 3))
        .WillOnce(Return(1))
        .WillOnce(Return(1))
        .WillOnce(Return(0));
    EXPECT_CALL(mockUI, readInt(0, 4)).WillOnce(Return(2));
    EXPECT_CALL(mockUI, readInt(0, 2))
        .WillOnce(Return(2))
        .WillOnce(Return(0));

    CarAssembler assembler(mockUI, validator);
    assembler.run();
}

// TC3: Back navigation at ENGINE step(0) -> showCarTypeMenu shown twice
// Input: readInt(0,3)=1(Sedan), readInt(0,4)=0(back), readInt(0,3)=0(exit)
TEST_F(CarAssemblerTest, BackNavigation_returns_to_CarType) {
    NiceMock<MockConsoleUI> mockUI;
    EXPECT_CALL(mockUI, showCarTypeMenu()).Times(2);
    EXPECT_CALL(mockUI, readInt(0, 3))
        .WillOnce(Return(1))
        .WillOnce(Return(0));
    EXPECT_CALL(mockUI, readInt(0, 4)).WillOnce(Return(0));

    CarAssembler assembler(mockUI, validator);
    assembler.run();
}

// TC4: Exit option at CarType(0) -> run() returns without showing engine menu
// Input: readInt(0,3)=0(exit)
TEST_F(CarAssemblerTest, ExitOption_terminates_run) {
    NiceMock<MockConsoleUI> mockUI;
    EXPECT_CALL(mockUI, showEngineMenu()).Times(0);
    EXPECT_CALL(mockUI, readInt(0, 3)).WillOnce(Return(0));

    CarAssembler assembler(mockUI, validator);
    assembler.run();
}
