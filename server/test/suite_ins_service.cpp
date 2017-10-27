
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ins_service.hpp"


namespace ins_service
{

class IndoorNavigationServiceFixture : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        Pistache::Port port(9080);
        Pistache::Address addr(Pistache::Ipv4::any(), port);
        IndoorNavigationService ins_service_(addr);
    }

protected:

};

TEST_F(IndoorNavigationServiceFixture, Init)
{
}

} // namespace !ins_service