#ifndef INS_SERVER_INCLUDE_LIB_WRAPPER_HPP
#define INS_SERVER_INCLUDE_LIB_WRAPPER_HPP

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

namespace ins_service
{

void HttpEndpointInit(const std::shared_ptr<Pistache::Http::Endpoint>& http_end_point, Pistache::Http::Endpoint::Options opts);

void HttpEndpointSetHandler(const std::shared_ptr<Pistache::Http::Endpoint>& http_end_point, Pistache::Rest::Router& router);

void HttpEndpointServe(const std::shared_ptr<Pistache::Http::Endpoint>& http_end_point);

void HttpEndpointShutdown(const std::shared_ptr<Pistache::Http::Endpoint>& http_end_point);
}

#endif // !INS_SERVER_INCLUDE_LIB_WRAPPER_HPP
