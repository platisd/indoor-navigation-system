#include "lib_wrapper.hpp"

namespace ins_service
{

void HttpEndpointInit(const std::shared_ptr<Pistache::Http::Endpoint>& http_end_point, Pistache::Http::Endpoint::Options opts)
{
    http_end_point->init(opts);
}

void HttpEndpointSetHandler(const std::shared_ptr<Pistache::Http::Endpoint>& http_end_point, Pistache::Rest::Router& router)
{
    http_end_point->setHandler(router.handler());
}

void HttpEndpointServe(const std::shared_ptr<Pistache::Http::Endpoint>& http_end_point)
{
    http_end_point->serveThreaded();
}

void HttpEndpointShutdown(const std::shared_ptr<Pistache::Http::Endpoint>& http_end_point)
{
    http_end_point->shutdown();
}
}  // namespace ins_service