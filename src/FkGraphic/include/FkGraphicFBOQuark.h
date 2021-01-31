/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICFBOQUARK_H
#define FK_GRAPHIC_FKGRAPHICFBOQUARK_H

#include "FkQuark.h"
#include "FkGraphicFrameObject.h"

class FkGraphicFBOQuark FK_EXTEND FkQuark {
public:
    FkGraphicFBOQuark();

    FkGraphicFBOQuark(const FkGraphicFBOQuark &o) = delete;

    virtual ~FkGraphicFBOQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onDrawLayer(std::shared_ptr<FkProtocol> p);

private:
    std::shared_ptr<FkGraphicFBOAllocator> allocator = nullptr;
};

#endif //FK_GRAPHIC_FKGRAPHICFBOQUARK_H