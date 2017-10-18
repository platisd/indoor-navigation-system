//
// Created by samueli on 2017-10-02.
//

#ifndef INS_SERVER_INS_SERVICE_HPP
#define INS_SERVER_INS_SERVICE_HPP

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <spdlog/spdlog.h>
#include <memory>
#include <mutex>

#include "data_store.hpp"
#include "localization.hpp"
#include "types.hpp"

namespace ins_service {

class IndoorNavigationService {
   public:
    explicit IndoorNavigationService(Pistache::Address addr)
        : http_end_point_(std::make_shared<Pistache::Http::Endpoint>(addr)),
          data_store_(nullptr),
          localization_(nullptr),
          console_(spdlog::stdout_logger_mt("INSService")) {}

    int Init(int thread_count = 2);

    void Start();

    void Shutdown();

   private:
    void SetupRoutes();

    void SetReceivedSignalStrengths(const Pistache::Rest::Request &request,
                                    Pistache::Http::ResponseWriter response);

    void ResetDeviceLocation(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    void GetDevicePosition(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    void GetEmployeePosition(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    void Auth(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    void HandleReady(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    void PrintCookies(const Pistache::Rest::Request &request);

    std::shared_ptr<DataStore> data_store_;
    std::shared_ptr<Localization> localization_;
    std::shared_ptr<Pistache::Http::Endpoint> http_end_point_;
    Pistache::Rest::Router router_;
    std::shared_ptr<spdlog::logger> console_;
};

}  // namespace ins_service

#endif  // INS_SERVER_INS_SERVICE_HPP
