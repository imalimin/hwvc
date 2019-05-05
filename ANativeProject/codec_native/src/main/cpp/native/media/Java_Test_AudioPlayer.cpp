//
// Created by mingyi.li on 2019/5/5.
//


#include <jni.h>
#include <log.h>
#include "../include/AudioPlayer.h"
#include "../include/EventPipeline.h"

#ifdef __cplusplus
extern "C" {
#endif

static AudioPlayer *player = nullptr;
static FILE *file = nullptr;
static EventPipeline *pipeline = nullptr;

static void loopTest() {
    if (!file) {
        return;
    }
    pipeline->queueEvent([] {
        uint8_t data[8192];
        int ret = fread(data, 1, 8192, file);
        if (ret > 0) {
            player->write(data, 8192);
            loopTest();
        }
    });
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_media_AudioPlayerTest_start
        (JNIEnv *env, jobject thiz) {
    pipeline = new EventPipeline("AudioPlayerTest");
    file = fopen("/sdcard/2.pcm", "rb");
    if (file) {
        pipeline->queueEvent([] {
            Logcat::e("HWVC", "AudioPlayerTest_start");
            player = new AudioPlayer(2, 48000, SL_PCMSAMPLEFORMAT_FIXED_32, 1024);
            player->start();
            loopTest();
        });
    }
}
JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_media_AudioPlayerTest_stop
        (JNIEnv *env, jobject thiz) {
    pipeline->queueEvent([] {
        if (file) {
            player->stop();
            fclose(file);
            file = nullptr;
        }
    });
    delete pipeline;
    pipeline = nullptr;
}

#ifdef __cplusplus
}
#endif