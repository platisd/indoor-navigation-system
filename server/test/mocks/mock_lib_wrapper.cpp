#include "mock_lib_wrapper.hpp"

namespace ins_service
{
::testing::NiceMock<MockLibWrapper>* g_mocked_lib_wrapper_;

void HttpEndpointInit(std::shared_ptr<Endpoint> http_end_point, Endpoint::Options opts)
{
    EXPECT_TRUE(g_mocked_lib_wrapper_ != nullptr);
    return g_mocked_lib_wrapper_->HttpEndpointInit(http_end_point, opts);
}

void HttpEndpointSetHandler(std::shared_ptr<Endpoint> http_end_point, Pistache::Rest::Router& router)
{
    EXPECT_TRUE(g_mocked_lib_wrapper_ != nullptr);
    return g_mocked_lib_wrapper_->HttpEndpointSetHandler(http_end_point, router);
}

void HttpEndpointServe(std::shared_ptr<Endpoint> http_end_point)
{
    EXPECT_TRUE(g_mocked_lib_wrapper_ != nullptr);
    return g_mocked_lib_wrapper_->HttpEndpointServe(http_end_point);
}

void HttpEndpointShutdown(std::shared_ptr<Endpoint> http_end_point)
{
    EXPECT_TRUE(g_mocked_lib_wrapper_ != nullptr);
    return g_mocked_lib_wrapper_->HttpEndpointShutdown(http_end_point);
}

}