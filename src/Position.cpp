//
// Created by jpcuv on 20-12-23.
//

#include "Position.h"

std::ostream &operator<<(std::ostream &os, const Position& that) {
    os << "(" << that.x << ", " << that.y << ")";
    return os;
}
