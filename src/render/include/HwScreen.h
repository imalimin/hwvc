//
// Created by mingyi.li on 2018/12/25.
//

#ifndef HARDWAREVIDEOCODEC_SCREEN_H
#define HARDWAREVIDEOCODEC_SCREEN_H

#include "Unit.h"
#include "Egl.h"
#include "BaseDrawer.h"
#include "NativeWindow.h"

class HwScreen : public Unit {
public:
    HwScreen(string alias);

    virtual ~HwScreen();

    /// 该函数会接收到一个pipeline实例即将销毁的消息
    /// 请在此释放跟opengl相关的资源，或其它
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | none         | none      | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool onDestroy(Message *msg) override;

    /// 该函数会接收到一个pipeline实例初始化的消息
    /// 请在此准备该Unit所需的资源
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | NativeWindow | none      | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool eventPrepare(Message *msg);

    /// 更新Window，用于App从后台切到前台，eglSurface需要
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | NativeWindow | none      | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool eventUpdateWindow(Message *msg);

    /// 渲染一张纹理到屏幕
    /// format:
    /// +----------------------------------+
    /// | msg::obj | msg::arg1 | msg::arg2 |
    /// +----------------------------------+
    /// | Size     | tex id    | none      |
    /// +----------------------------------+
    /// \param msg
    /// \return
    bool eventDraw(Message *msg);

private:
    Egl *egl = nullptr;
    BaseDrawer *drawer = nullptr;
    int width = 0;
    int height = 0;

    void initWindow(NativeWindow *nw);

    void draw(GLuint texture);

    void setScaleType(int dw, int dh);
};


#endif //HARDWAREVIDEOCODEC_SCREEN_H
