/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMaterialComponent.h"

FkMaterialComponent::FkMaterialComponent() : FkGraphicComponent() {
    FK_MARK_SUPER

}

FkMaterialComponent::FkMaterialComponent(const FkMaterialComponent &o) : FkGraphicComponent(o) {
    FK_MARK_SUPER

}

FkMaterialComponent::~FkMaterialComponent() {

}
