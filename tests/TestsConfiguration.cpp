//
// Created by chema on 3/26/24.
//

#include "TestsConfiguration.h"
#include "Config.h"

TEST_F(TestsConfiguration, TestDeserialize)
{
    auto config = Config::from_json_file("../configs/example.json");
    config.print_config();

    EXPECT_EQ(config.measure_interval, 3);
    EXPECT_TRUE(config.measure_cpu);
    EXPECT_TRUE(config.measure_memory);
    EXPECT_EQ(config.measure_memory_units, "MB");

}