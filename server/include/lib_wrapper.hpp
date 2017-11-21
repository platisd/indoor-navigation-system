#ifndef INS_SERVER_INCLUDE_LIB_WRAPPER_HPP
#define INS_SERVER_INCLUDE_LIB_WRAPPER_HPP

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

using namespace Pistache::Http;

namespace ins_service
{

void HttpEndpointInit(std::shared_ptr<Endpoint> http_end_point, Pistache::Http::Endpoint::Options opts);

void HttpEndpointSetHandler(std::shared_ptr<Endpoint> http_end_point, Pistache::Rest::Router& router);

void HttpEndpointServe(std::shared_ptr<Endpoint> http_end_point);

void HttpEndpointShutdown(std::shared_ptr<Endpoint> http_end_point);
}

#endif // !INS_SERVER_INCLUDE_LIB_WRAPPER_HPP
