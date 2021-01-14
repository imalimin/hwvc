/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkCalculatePrt.h"

FkCalculatePrt::FkCalculatePrt() : FkProtocol() {
    FK_MARK_SUPER
}

FkCalculatePrt::FkCalculatePrt(const FkCalculatePrt &o) : FkProtocol(o), number(o.number) {

}

FkCalculatePrt::~FkCalculatePrt() {

}