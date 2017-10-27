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

struct Position
{
    double x;
    double y;
    double z;
};

} // namespace ins_service

#endif // INS_SERVICE_INS_INCLUDE_TYPES_HPP