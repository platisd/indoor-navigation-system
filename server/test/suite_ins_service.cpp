
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ins_service.hpp"
#include "mock_data_store.hpp"
#include "mock_lib_wrapper.hpp"
#include "mock_localization.hpp"

using namespace ::testing;

namespace ins_service
{

class IndoorNavigationServiceFixture : public Test
{
public:
    virtual void SetUp()
    {
        g_mocked_data_store_  = &mock_data_store_;
        g_mocked_lib_wrapper_ = &mock_lib_wrapper_;
        //spdlog::set_level(spdlog::level::debug);
    }

    std::shared_ptr<Pistache::Http::Endpoint> GetHttpEndpoint()
    {
        return ins_service_.http_end_point_;
    }

    void SetupRoutes()
    {
        ins_service_.SetupRoutes();
    }


protected:
    NiceMock<MockDataStore>  mock_data_store_;
    NiceMock<MockLibWrapper> mock_lib_wrapper_;
    IndoorNavigationService  ins_service_;
};

/*
 * TEST: Init
 * EXPECT: Initialize DataStore
 */
TEST_F(IndoorNavigationServiceFixture, Init_WillCallDataStoreInit)
{
    EXPECT_CALL(mock_data_store_, Init(_)).Times(1);
    Pistache::Port    port(9080);
    Pistache::Address addr(Pistache::Ipv4::any(), port);
    ins_service_.Init(addr, 3);
}

/*
 * TEST: Init
 * EXPECT: Initialize HttpEndpoint
 */
TEST_F(IndoorNavigationServiceFixture, Init_WillCallHttpEndpointInit)
{
    EXPECT_CALL(mock_lib_wrapper_, HttpEndpointInit(_, _)).Times(1);
    Pistache::Port    port(9080);
    Pistache::Address addr(Pistache::Ipv4::any(), port);
    ins_service_.Init(addr, 3);
}

/*
 * TEST: Start service
 * EXPECT: Sets Http endpoint handler
 * EXPECT: Starts serving http
 */
TEST_F(IndoorNavigationServiceFixture, Start_WillSetHandlerAndServeHttp)
{
    EXPECT_CALL(mock_lib_wrapper_, HttpEndpointSetHandler(_, _)).Times(1);
    EXPECT_CALL(mock_lib_wrapper_, HttpEndpointServe(_)).Times(1);

    Pistache::Port    port(9080);
    Pistache::Address addr(Pistache::Ipv4::any(), port);
    ins_service_.Init(addr, 3);
    ins_service_.Start();
}

/**
 * TEST: Shutdown service
 * EXPECT: Close database
 * EXPECT: Shutdown http endpoint
 */
TEST_F(IndoorNavigationServiceFixture, Shutdown_WillCloseDatabaseAndShutdownHttpEndPoint)
{
    EXPECT_CALL(mock_lib_wrapper_, HttpEndpointSetHandler(_, _)).Times(1);
    EXPECT_CALL(mock_lib_wrapper_, HttpEndpointServe(_)).Times(1);
    EXPECT_CALL(mock_lib_wrapper_, HttpEndpointShutdown(_)).Times(1);
    EXPECT_CALL(mock_data_store_, Close()).Times(1);

    Pistache::Port    port(9080);
    Pistache::Address addr(Pistache::Ipv4::any(), port);
    ins_service_.Init(addr, 3);
    ins_service_.Start();
    ins_service_.Shutdown();
}

} // namespace !ins_service