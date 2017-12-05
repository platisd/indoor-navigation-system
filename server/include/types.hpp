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
    explicit AccessPoint(std::string id)
        : mac_addr(id)
        , pos(Position{ 0, 0, 0 })
    {
    }

    AccessPoint(std::string id, Position p)
        : mac_addr(id)
        , pos(p)
    {
    }

    std::string mac_addr;
    Position    pos;

    bool operator==(const AccessPoint& rhs) const
    {
        return this->pos == rhs.pos && this->mac_addr == rhs.mac_addr;
    }
};

typedef std::pair<AccessPoint, int32_t>              AccessPointRssiPair;
typedef std::pair<AccessPoint, std::vector<int32_t>> AccessPointRssiListPair;

} // namespace ins_service

#endif // INS_SERVICE_INS_INCLUDE_TYPES_HPP