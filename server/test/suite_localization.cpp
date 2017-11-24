
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "localization.hpp"

namespace ins_service
{

class LocalizationFixture : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        //spdlog::set_level(spdlog::level::debug);
    }

protected:
	Localization localization_;
};

TEST_F(LocalizationFixture, Init)
{
}

} // namespace !ins_service