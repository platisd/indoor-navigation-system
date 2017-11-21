#include "lib_wrapper.hpp"

namespace ins_service
{

void HttpEndpointInit(std::shared_ptr<Pistache::Http::Endpoint> http_end_point, Pistache::Http::Endpoint::Options opts)
{
    http_end_point->init(opts);
}

void HttpEndpointSetHandler(std::shared_ptr<Endpoint> http_end_point, Pistache::Rest::Router& router)
{
    http_end_point->setHandler(router.handler());
}

void HttpEndpointServe(std::shared_ptr<Endpoint> http_end_point)
{
    http_end_point->serve();
}

void HttpEndpointShutdown(std::shared_ptr<Endpoint> http_end_point)
{
    http_end_point->shutdown();
}
}