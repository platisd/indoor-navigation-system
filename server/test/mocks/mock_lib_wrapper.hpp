#ifndef INS_SERVER_TEST_MOCKS_LIB_WRAPPER_HPP
#define INS_SERVER_TEST_MOCKS_LIB_WRAPPER_HPP

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lib_wrapper.hpp"

using namespace Pistache::Http;

namespace ins_service
{

class MockLibWrapper;

extern ::testing::NiceMock<MockLibWrapper>* g_mocked_lib_wrapper_;

class MockLibWrapper
{
public:
    MOCK_METHOD2(HttpEndpointInit, void(std::shared_ptr<Endpoint>, Endpoint::Options));

    MOCK_METHOD2(HttpEndpointSetHandler, void(std::shared_ptr<Endpoint>, Pistache::Rest::Router&));

    MOCK_METHOD1(HttpEndpointServe, void(std::shared_ptr<Endpoint>));

    MOCK_METHOD1(HttpEndpointShutdown, void(std::shared_ptr<Endpoint>));

    ~MockLibWrapper()
    {
        g_mocked_lib_wrapper_ = nullptr;
    }
};
}

#endif // !INS_SERVER_TEST_MOCKS_LIB_WRAPPER_HPP