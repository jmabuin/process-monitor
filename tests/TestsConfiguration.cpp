//
// Created by chema on 3/26/24.
//

#include "TestsConfiguration.h"
#include "Config.h"

TEST_F(TestsConfiguration, TestDeserialize)
{
    auto config = Config::from_json_file("../configs/example.json");
    config.print_config();

    EXPECT_EQ(config.MeasureInterval, 3);
    EXPECT_TRUE(config.MeasureCpu);
    EXPECT_TRUE(config.MeasureMemory);
    EXPECT_EQ(config.MeasureMemoryUnits, "MB");

}