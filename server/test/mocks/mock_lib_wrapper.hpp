#ifndef INS_SERVER_TEST_MOCKS_LIB_WRAPPER_HPP
#define INS_SERVER_TEST_MOCKS_LIB_WRAPPER_HPP

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lib_wrapper.hpp"

namespace ins_service
{

class MockLibWrapper;

extern ::testing::NiceMock<MockLibWrapper>* g_mocked_lib_wrapper_;

class MockLibWrapper
{
public:
    MOCK_METHOD2(HttpEndpointInit, void(std::shared_ptr<Pistache::Http::Endpoint>, Pistache::Http::Endpoint::Options));

    MOCK_METHOD2(HttpEndpointSetHandler, void(std::shared_ptr<Pistache::Http::Endpoint>, Pistache::Rest::Router&));

    MOCK_METHOD1(HttpEndpointServe, void(std::shared_ptr<Pistache::Http::Endpoint>));

    MOCK_METHOD1(HttpEndpointShutdown, void(std::shared_ptr<Pistache::Http::Endpoint>));

    ~MockLibWrapper()
    {
        g_mocked_lib_wrapper_ = nullptr;
    }
};
}

#endif // !INS_SERVER_TEST_MOCKS_LIB_WRAPPER_HPP