//
// Created by samueli on 2017-10-02.
//

#include "ins_service.hpp"
#include <sstream>

namespace ins_service
{

int IndoorNavigationService::Init(int thread_count)
{
    console_->debug("+ IndoorNavigationService::Init");

    data_store_ = std::make_shared<DataStore>();
    data_store_->Init("../ins.db");

    localization_ = std::make_shared<Localization>();

    auto opts
        = Pistache::Http::Endpoint::options().threads(thread_count).flags(Pistache::Tcp::Options::InstallSignalHandler);
    http_end_point_->init(opts);
    SetupRoutes();

    console_->debug("- IndoorNavigationService::Init");
    return 0;
}

void IndoorNavigationService::Start()
{
    console_->debug("+ IndoorNavigationService::Start");

    http_end_point_->setHandler(router_.handler());
    console_->info("Indoor Navigation Service now running ...");
    http_end_point_->serve();

    console_->debug("- IndoorNavigationService::Start");
}

void IndoorNavigationService::Shutdown()
{
    console_->debug("+ IndoorNavigationService::Shutdown");

    console_->info("Indoor Navigation Service is shutting down ...");
    http_end_point_->shutdown();
    data_store_->Close();

    console_->debug("- IndoorNavigationService::Shutdown");
}

void IndoorNavigationService::SetupRoutes()
{
    console_->debug("+ IndoorNavigationService::SetupRoutes");

    Pistache::Rest::Routes::Post(
        router_,
        "/set_rssi/:device_id/:mac_addr1/:rssi1/:mac_addr2?/:rssi2?/:mac_addr3?/:rssi3?/:mac_addr4?/:rssi4?/"
        ":mac_addr5?/:rssi5?/:mac_addr6?/:rssi6?/:mac_addr7?/:rssi7?/:mac_addr8?/:rssi8?/:mac_addr9?/:rssi9?/"
        ":mac_addr10?/:rssi10?",
        Pistache::Rest::Routes::bind(&IndoorNavigationService::SetReceivedSignalStrengths, this));

    Pistache::Rest::Routes::Post(router_,
                                 "/resolve_pos/:device_id",
                                 Pistache::Rest::Routes::bind(&IndoorNavigationService::ResolveDevicePosition, this));

    Pistache::Rest::Routes::Post(router_,
                                 "/reset_pos/:device_id",
                                 Pistache::Rest::Routes::bind(&IndoorNavigationService::ResetDevicePosition, this));

    Pistache::Rest::Routes::Get(router_,
                                "/get_device_pos/:device_id",
                                Pistache::Rest::Routes::bind(&IndoorNavigationService::GetDevicePosition, this));

    Pistache::Rest::Routes::Get(router_,
                                "/get_employee_pos/:employee_id",
                                Pistache::Rest::Routes::bind(&IndoorNavigationService::GetEmployeePosition, this));

    Pistache::Rest::Routes::Get(
        router_, "/ready", Pistache::Rest::Routes::bind(&IndoorNavigationService::HandleReady, this));

    Pistache::Rest::Routes::Get(router_, "/auth", Pistache::Rest::Routes::bind(&IndoorNavigationService::Auth, this));

    console_->debug("- IndoorNavigationService::SetupRoutes");
}

void IndoorNavigationService::SetReceivedSignalStrengths(const Pistache::Rest::Request& request,
                                                         Pistache::Http::ResponseWriter response)
{
    console_->debug("+ IndoorNavigationService::SetReceivedSignalStrengths");

    std::string device_id = request.param(":device_id").as<std::string>();

    std::vector<std::string> mac_address_list = { request.param(":mac_addr1").as<std::string>() };
    std::vector<double>      rssi_list        = { request.param(":rssi1").as<double>() };

    for (int i = 2; i <= 10; ++i)
    {
        std::stringstream mac_addr_key, rssi_key;
        mac_addr_key << ":mac_addr" << i;
        rssi_key << ":rssi" << i;
        if (request.hasParam(mac_addr_key.str()) && request.hasParam(rssi_key.str()))
        {
            mac_address_list.push_back(request.param(mac_addr_key.str()).as<std::string>());
            rssi_list.push_back(request.param(rssi_key.str()).as<double>());
        }
        else
        {
            break;
        }
    }

    // @TODO Keep class aware of created device tables to avoid always calling this function.
    if (!data_store_->CreateDeviceTable(device_id))
    {
        response.send(Pistache::Http::Code::Internal_Server_Error, "{result:error}");
        return;
    }
    if (!data_store_->InsertRSSIReadings(device_id, mac_address_list, rssi_list))
    {
        response.send(Pistache::Http::Code::Internal_Server_Error, "{result:error}");
        return;
    }

    console_->debug("- IndoorNavigationService::SetReceivedSignalStrengths");

    response.send(Pistache::Http::Code::Ok, "{result:success}");
}

void IndoorNavigationService::ResolveDevicePosition(const Pistache::Rest::Request& request,
                                                    Pistache::Http::ResponseWriter response)
{
    console_->debug("+ IndoorNavigationService::ResolveDevicePosition");

    std::string device_id = request.param(":device_id").as<std::string>();

    Position pos = localization_->ProcessRSSIDataSet(device_id);
    if (!data_store_->UpdateDeviceLocation(device_id, pos))
    {
        response.send(Pistache::Http::Code::Internal_Server_Error, "{result:error}");
        return;
    }

    response.send(Pistache::Http::Code::Ok, "{result:success}");

    console_->debug("- IndoorNavigationService::ResolveDevicePosition");
}
void IndoorNavigationService::ResetDevicePosition(const Pistache::Rest::Request& request,
                                                  Pistache::Http::ResponseWriter response)
{
    console_->debug("+ IndoorNavigationService::ResetDeviceLocation");

    std::string device_id = request.param(":device_id").as<std::string>();
    if (!data_store_->ClearDeviceTable(device_id))
    {
        response.send(Pistache::Http::Code::Internal_Server_Error, "{result:error}");
    }
    response.send(Pistache::Http::Code::Ok, "{result:success}");

    console_->debug("- IndoorNavigationService::ResetDeviceLocation");
}

void IndoorNavigationService::GetDevicePosition(const Pistache::Rest::Request& request,
                                                Pistache::Http::ResponseWriter response)
{
    console_->debug("+ IndoorNavigationService::GetDevicePosition");

    std::string device_id = request.param(":device_id").as<std::string>();

    Position pos;
    if (data_store_->GetPosition(device_id, QueryT::DEVICE, pos))
    {
        response.send(Pistache::Http::Code::Ok,
                      "device_id:" + device_id + " {pos_x:" + std::to_string(pos.x) + ",pos_y:" + std::to_string(pos.y)
                          + ",pos_z:"
                          + std::to_string(pos.z)
                          + "}");
        console_->info("X-{:03.3}, Y-{:03.3}, Z-{:03.3}, ", pos.x, pos.y, pos.z);
    }
    else
    {
        response.send(Pistache::Http::Code::Internal_Server_Error, "{error: device_id not found}");
    }

    console_->debug("- IndoorNavigationService::GetDevicePosition");
}

void IndoorNavigationService::GetEmployeePosition(const Pistache::Rest::Request& request,
                                                  Pistache::Http::ResponseWriter response)
{
    console_->debug("+ IndoorNavigationService::GetEmployeePosition");

    std::string employee_id = request.param(":employee_id").as<std::string>();

    Position pos;
    if (data_store_->GetPosition(employee_id, QueryT::EMPLOYEE, pos))
    {
        response.send(Pistache::Http::Code::Ok,
                      "employee_id:" + employee_id + " {pos_x:" + std::to_string(pos.x) + ",pos_y:"
                          + std::to_string(pos.y)
                          + ",pos_z:"
                          + std::to_string(pos.z)
                          + "}");
        console_->info("X-{:03.3}, Y-{:03.3}, Z-{:03.3}, ", pos.x, pos.y, pos.z);
    }
    else
    {
        response.send(Pistache::Http::Code::Internal_Server_Error, "{error: employee_id not found}");
    }

    console_->debug("- IndoorNavigationService::GetEmployeePosition");
}

void IndoorNavigationService::HandleReady(const Pistache::Rest::Request& request,
                                          Pistache::Http::ResponseWriter response)
{
    console_->debug("+ IndoorNavigationService::HandleReady");
    response.send(Pistache::Http::Code::Ok, "1");
    console_->debug("- IndoorNavigationService::HandleReady");
}

void IndoorNavigationService::Auth(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
    console_->debug("+ IndoorNavigationService::Auth");
    // TODO
    PrintCookies(request);
    response.cookies().add(Pistache::Http::Cookie("lang", "en-US"));
    response.send(Pistache::Http::Code::Ok);
    console_->debug("- IndoorNavigationService::Auth");
}

void IndoorNavigationService::PrintCookies(const Pistache::Rest::Request& request)
{
    console_->debug("- IndoorNavigationService::PrintCookies");
    auto cookies = request.cookies();
    std::cout << "Cookies: [" << std::endl;
    const std::string indent(4, ' ');
    for (const auto& c : cookies)
    {
        std::cout << indent << c.name << " = " << c.value << std::endl;
    }
    std::cout << "]" << std::endl;
    console_->debug("- IndoorNavigationService::PrintCookies");
}

} // namespace ins_service
