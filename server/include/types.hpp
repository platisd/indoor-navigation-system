//
// Created by samueli on 2017-10-10.
//

#ifndef INS_SERVICE_INS_INCLUDE_TYPES_HPP
#define INS_SERVICE_INS_INCLUDE_TYPES_HPP

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

} // namespace ins_service

#endif // INS_SERVICE_INS_INCLUDE_TYPES_HPP