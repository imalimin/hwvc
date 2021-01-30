/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "FkGraphicContext.h"
#include "FkGraphicTexture.h"
#include "GLES2/gl2.h"

TEST(FkGraphicAllocatorTest, Context) {
    auto context = std::make_shared<FkGraphicContext>("Test");
    EXPECT_EQ(context->create(), FK_OK);
    context->destroy();
}

TEST(FkGraphicAllocatorTest, Alloc) {
    auto context = std::make_shared<FkGraphicContext>("Test");
    EXPECT_EQ(context->create(), FK_OK);
    EXPECT_EQ(context->makeCurrent(), FK_OK);

    auto allocator = std::make_shared<FkGraphicAllocator>();
    {
        FkTexDescription desc(GL_TEXTURE_2D);
        desc.fmt = FkColor::kFormat::RGBA;
        auto tex = allocator->alloc(desc);
        EXPECT_NE(tex, nullptr);
        tex->update(FkColor::kFormat::RGBA, 16, 16);
        EXPECT_NE(allocator->size(), 0);
    }
    EXPECT_EQ(allocator->size(), 0);
    EXPECT_NE(allocator->capacity(), 0);
    allocator->release();
    EXPECT_EQ(allocator->size(), 0);
    EXPECT_EQ(allocator->capacity(), 0);

    context->destroy();
    EXPECT_EQ(eglGetCurrentContext(), EGL_NO_CONTEXT);
}