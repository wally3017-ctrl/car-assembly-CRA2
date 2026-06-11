#include "gmock/gmock.h"
#include "../Car.h"
#include "../CarValidator.h"
#include "../parts/GmEngine.h"
#include "../parts/ToyotaEngine.h"
#include "../parts/WiaEngine.h"
#include "../parts/MandoBrake.h"
#include "../parts/ContinentalBrake.h"
#include "../parts/BoschBrake.h"
#include "../parts/BoschSteering.h"
#include "../parts/MobisSteering.h"
#include "../types/Sedan.h"
#include "../types/Suv.h"
#include "../types/Truck.h"

class CarValidatorTest : public ::testing::Test {
protected:
    CarValidator validator;
};

// TC1: Bosch brake + Mobis steering -> isValid() == false
TEST_F(CarValidatorTest, BoschBrake_requires_BoschSteering) {
    Car car;
    car.carType        = std::make_unique<Sedan>();
    car.engine         = std::make_unique<GmEngine>();
    car.brakeSystem    = std::make_unique<BoschBrake>();
    car.steeringSystem = std::make_unique<MobisSteering>();
    EXPECT_FALSE(validator.isValid(car));
}

// TC2: Bosch brake + Bosch steering -> isValid() == true
TEST_F(CarValidatorTest, BoschBrake_with_BoschSteering_is_valid) {
    Car car;
    car.carType        = std::make_unique<Sedan>();
    car.engine         = std::make_unique<GmEngine>();
    car.brakeSystem    = std::make_unique<BoschBrake>();
    car.steeringSystem = std::make_unique<BoschSteering>();
    EXPECT_TRUE(validator.isValid(car));
}

// TC3: Sedan + Continental brake -> isValid() == false
TEST_F(CarValidatorTest, Sedan_with_Continental_is_invalid) {
    Car car;
    car.carType        = std::make_unique<Sedan>();
    car.engine         = std::make_unique<GmEngine>();
    car.brakeSystem    = std::make_unique<ContinentalBrake>();
    car.steeringSystem = std::make_unique<MobisSteering>();
    EXPECT_FALSE(validator.isValid(car));
}

// TC4: Sedan + Mando brake -> isValid() == true
TEST_F(CarValidatorTest, Sedan_with_Mando_is_valid) {
    Car car;
    car.carType        = std::make_unique<Sedan>();
    car.engine         = std::make_unique<GmEngine>();
    car.brakeSystem    = std::make_unique<MandoBrake>();
    car.steeringSystem = std::make_unique<MobisSteering>();
    EXPECT_TRUE(validator.isValid(car));
}

// TC5: SUV + TOYOTA engine -> isValid() == false
TEST_F(CarValidatorTest, SUV_with_Toyota_engine_is_invalid) {
    Car car;
    car.carType        = std::make_unique<Suv>();
    car.engine         = std::make_unique<ToyotaEngine>();
    car.brakeSystem    = std::make_unique<MandoBrake>();
    car.steeringSystem = std::make_unique<MobisSteering>();
    EXPECT_FALSE(validator.isValid(car));
}

// TC6: Truck + WIA engine -> isValid() == false
TEST_F(CarValidatorTest, Truck_with_WIA_engine_is_invalid) {
    Car car;
    car.carType        = std::make_unique<Truck>();
    car.engine         = std::make_unique<WiaEngine>();
    car.brakeSystem    = std::make_unique<MandoBrake>();
    car.steeringSystem = std::make_unique<MobisSteering>();
    EXPECT_FALSE(validator.isValid(car));
}

// TC7: Truck + Mando brake -> isValid() == false
TEST_F(CarValidatorTest, Truck_with_Mando_brake_is_invalid) {
    Car car;
    car.carType        = std::make_unique<Truck>();
    car.engine         = std::make_unique<GmEngine>();
    car.brakeSystem    = std::make_unique<MandoBrake>();
    car.steeringSystem = std::make_unique<MobisSteering>();
    EXPECT_FALSE(validator.isValid(car));
}

// TC8: Sedan + GM + Mando + Mobis -> isValid() == true (all checks pass)
TEST_F(CarValidatorTest, valid_combination_passes_all_checks) {
    Car car;
    car.carType        = std::make_unique<Sedan>();
    car.engine         = std::make_unique<GmEngine>();
    car.brakeSystem    = std::make_unique<MandoBrake>();
    car.steeringSystem = std::make_unique<MobisSteering>();
    EXPECT_TRUE(validator.isValid(car));
}

// --- PASS cases for conditions that had only FAIL cases ---

// TC9: SUV + GM (non-TOYOTA) -> isValid() == true (condition 2b PASS)
TEST_F(CarValidatorTest, SUV_with_non_Toyota_engine_is_valid) {
    Car car;
    car.carType        = std::make_unique<Suv>();
    car.engine         = std::make_unique<GmEngine>();
    car.brakeSystem    = std::make_unique<MandoBrake>();
    car.steeringSystem = std::make_unique<MobisSteering>();
    EXPECT_TRUE(validator.isValid(car));
}

// TC10: Truck + GM + Continental + Mobis -> isValid() == true (conditions 2c, 2d both PASS)
TEST_F(CarValidatorTest, Truck_with_non_WIA_non_Mando_is_valid) {
    Car car;
    car.carType        = std::make_unique<Truck>();
    car.engine         = std::make_unique<GmEngine>();
    car.brakeSystem    = std::make_unique<ContinentalBrake>();
    car.steeringSystem = std::make_unique<MobisSteering>();
    EXPECT_TRUE(validator.isValid(car));
}

// --- getInvalidReason() message verification ---

// TC11: Bosch brake + Mobis steering -> reason mentions "Bosch"
TEST_F(CarValidatorTest, getInvalidReason_Bosch_Mobis_mentions_Bosch) {
    Car car;
    car.carType        = std::make_unique<Sedan>();
    car.engine         = std::make_unique<GmEngine>();
    car.brakeSystem    = std::make_unique<BoschBrake>();
    car.steeringSystem = std::make_unique<MobisSteering>();
    EXPECT_THAT(validator.getInvalidReason(car), testing::HasSubstr("Bosch"));
}

// TC12: Sedan + Continental -> reason mentions "Continental"
TEST_F(CarValidatorTest, getInvalidReason_Sedan_Continental_mentions_Continental) {
    Car car;
    car.carType        = std::make_unique<Sedan>();
    car.engine         = std::make_unique<GmEngine>();
    car.brakeSystem    = std::make_unique<ContinentalBrake>();
    car.steeringSystem = std::make_unique<MobisSteering>();
    EXPECT_THAT(validator.getInvalidReason(car), testing::HasSubstr("Continental"));
}

// TC13: SUV + TOYOTA -> reason mentions "TOYOTA"
TEST_F(CarValidatorTest, getInvalidReason_SUV_Toyota_mentions_TOYOTA) {
    Car car;
    car.carType        = std::make_unique<Suv>();
    car.engine         = std::make_unique<ToyotaEngine>();
    car.brakeSystem    = std::make_unique<MandoBrake>();
    car.steeringSystem = std::make_unique<MobisSteering>();
    EXPECT_THAT(validator.getInvalidReason(car), testing::HasSubstr("TOYOTA"));
}

// TC14: Truck + WIA -> reason mentions "WIA"
TEST_F(CarValidatorTest, getInvalidReason_Truck_WIA_mentions_WIA) {
    Car car;
    car.carType        = std::make_unique<Truck>();
    car.engine         = std::make_unique<WiaEngine>();
    car.brakeSystem    = std::make_unique<ContinentalBrake>();
    car.steeringSystem = std::make_unique<MobisSteering>();
    EXPECT_THAT(validator.getInvalidReason(car), testing::HasSubstr("WIA"));
}

// TC15: Truck + Mando -> reason mentions "Mando"
TEST_F(CarValidatorTest, getInvalidReason_Truck_Mando_mentions_Mando) {
    Car car;
    car.carType        = std::make_unique<Truck>();
    car.engine         = std::make_unique<GmEngine>();
    car.brakeSystem    = std::make_unique<MandoBrake>();
    car.steeringSystem = std::make_unique<MobisSteering>();
    EXPECT_THAT(validator.getInvalidReason(car), testing::HasSubstr("Mando"));
}

// TC16: Valid car -> getInvalidReason returns empty string
TEST_F(CarValidatorTest, getInvalidReason_empty_for_valid_car) {
    Car car;
    car.carType        = std::make_unique<Sedan>();
    car.engine         = std::make_unique<GmEngine>();
    car.brakeSystem    = std::make_unique<MandoBrake>();
    car.steeringSystem = std::make_unique<MobisSteering>();
    EXPECT_TRUE(validator.getInvalidReason(car).empty());
}
