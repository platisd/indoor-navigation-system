//
// Created by samueli on 2017-10-10.
//

#ifndef INS_SERVICE_INS_TYPES_HPP
#define INS_SERVICE_INS_TYPES_HPP

namespace ins_service {

enum QueryT { DEVICE, EMPLOYEE };

struct Position {
    double x;
    double y;
    double z;
};

}  // namespace ins_service

#endif  // INS_SERVICE_INS_TYPES_HPP