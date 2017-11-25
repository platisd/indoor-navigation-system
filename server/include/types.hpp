//
// Created by samueli on 2017-10-10.
//

#ifndef INS_SERVICE_INS_INCLUDE_TYPES_HPP
#define INS_SERVICE_INS_INCLUDE_TYPES_HPP

#include <string>
#include <utility>
#include <vector>

namespace ins_service
{

#define LOGGER_NAME "INS-Server"

typedef std::pair<std::string, int32_t>              MacRssi;
typedef std::pair<std::string, std::vector<int32_t>> MacRssiList;

enum QueryT
{
    DEVICE,
    EMPLOYEE
};

class Position
{
public:
    double x;
    double y;
    double z;

    bool operator==(const Position& rhs) const
    {
        return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
    }
};

class AccessPoint
{
public:
    std::string mac_addr;
    Position    pos;

    bool operator==(const AccessPoint& rhs) const
    {
        return this->pos == rhs.pos && this->mac_addr == rhs.mac_addr;
    }
};

} // namespace ins_service

#endif // INS_SERVICE_INS_INCLUDE_TYPES_HPP