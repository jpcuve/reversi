//
// Created by jpcuv on 01-07-24.
//

#ifndef CYCLONE_SRC_MAIN_H_
#define CYCLONE_SRC_MAIN_H_

#include "pch.h"
#include <ostream>

void ThrowIfFailed(HRESULT result);

void ThrowIfNull(void *result);

void ThrowIfFalse(bool result);

#endif //CYCLONE_SRC_MAIN_H_
