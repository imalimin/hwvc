/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "platform/android/AlJavaNativeHelper.h"
#include "AlDisplayRecorder.h"
#include "HwAndroidWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

static JMethodDescription midOnNativeMessage = {
        "com/lmy/hwvcnative/processor/AlDisplayRecorder",
        "onNativeMessage", "(II)V"};
static JMethodDescription midOnRecordProgressDesc = {
        "com/lmy/hwvcnative/processor/AlDisplayRecorder",
        "onRecordProgress", "(J)V"};
static JMethodDescription midOnNativePrepared = {
        "com/lmy/hwvcnative/processor/AlDisplayRecorder",
        "onNativePrepared", "(I)V"};

static AlDisplayRecorder *getHandler(jlong handler) {
    return reinterpret_cast<AlDisplayRecorder *>(handler);
}

static void bindListener(jlong handler) {
    getHandler(handler)->setRecordListener([handler](int64_t timeInUs) {
        jobject jObject = nullptr;
        JNIEnv *pEnv = nullptr;
        jmethodID mid = nullptr;
        if (AlJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
            AlJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
            AlJavaNativeHelper::getInstance()->findMethod(handler,
                                                          midOnRecordProgressDesc,
                                                          &mid)) {
            pEnv->CallVoidMethod(jObject, mid, static_cast<jlong>(timeInUs));
        }
    });
    getHandler(handler)->setOnNativeReadyListener([handler](int32_t oesTex) {
        jobject jObject = nullptr;
        JNIEnv *pEnv = nullptr;
        jmethodID mid = nullptr;
        if (AlJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
            AlJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
            AlJavaNativeHelper::getInstance()->findMethod(handler,
                                                          midOnNativePrepared,
                                                          &mid)) {
            pEnv->CallVoidMethod(jObject, mid, static_cast<jint>(oesTex));
        }
    });

}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_AlDisplayRecorder_create
        (JNIEnv *env, jobject thiz) {
    auto *p = new AlDisplayRecorder();
    p->post([] {
        AlJavaNativeHelper::getInstance()->attachThread();
    });
    jlong handler = reinterpret_cast<jlong>(p);
    AlJavaNativeHelper::getInstance()->registerAnObject(env, handler, thiz);
    bindListener(handler);
    return handler;
}
JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlDisplayRecorder_postEvent
        (JNIEnv *env, jobject thiz, jlong handler, jint what) {
    if (handler) {
        int w = what;
        getHandler(handler)->runOnCameraContext([handler, w]() {
            jobject jObject = nullptr;
            JNIEnv *pEnv = nullptr;
            jmethodID mid = nullptr;
            if (AlJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
                AlJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
                AlJavaNativeHelper::getInstance()->findMethod(handler,
                                                              midOnNativeMessage,
                                                              &mid)) {
                switch (w) {
                    case 4: {
                        pEnv->CallVoidMethod(jObject, mid, w, 0);
                        break;
                    }
                    case 2: {
                        pEnv->CallVoidMethod(jObject, mid, w, 0);
                        break;
                    }
                    default:
                        pEnv->CallVoidMethod(jObject, mid, w, 0);
                }
                pEnv->ExceptionCheck();
            }
        });
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlDisplayRecorder_setOutputFilePath
        (JNIEnv *env, jobject thiz, jlong handler, jstring filePath) {
    if (handler) {
        const char *pPilePath = env->GetStringUTFChars(filePath, JNI_FALSE);
        std::string pathStr(pPilePath);
        env->ReleaseStringUTFChars(filePath, pPilePath);
        getHandler(handler)->setOutputFilePath(pathStr);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlDisplayRecorder_setFormat
        (JNIEnv *env, jobject thiz, jlong handler, jint width, jint height, jint sampleFormat,
         jint channels, jint sampleRate) {
    if (handler) {
        getHandler(handler)->setFormat(width, height, HwSampleFormat(
                static_cast<HwFrameFormat>(sampleFormat), channels, sampleRate));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlDisplayRecorder_showAt
        (JNIEnv *env, jobject thiz, jlong handler, jobject surface) {
    if (handler) {
        getHandler(handler)->updateWindow(new HwAndroidWindow(env, surface));
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlDisplayRecorder_start
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->start();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlDisplayRecorder_pause
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->pause();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlDisplayRecorder_release
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        auto *p = getHandler(handler);
        p->post([] {
            AlLogI("Java_AlDisplayRecorder", "release");
            AlJavaNativeHelper::getInstance()->detachThread();
        });
        p->release();
        delete p;
    }
    AlJavaNativeHelper::getInstance()->unregisterAnObject(env, handler);
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlDisplayRecorder_invalidate
        (JNIEnv *env, jobject thiz, jlong handler, jfloatArray matrix, jlong tsInNs, jint cw,
         jint ch) {
    if (handler) {
        if (matrix) {
            jfloat *pMatrix = env->GetFloatArrayElements(matrix, JNI_FALSE);
            AlMatrix m = AlMatrix::fromArray(pMatrix);
            env->ReleaseFloatArrayElements(matrix, pMatrix, 0);
            getHandler(handler)->setCameraSize(cw, ch);
            getHandler(handler)->invalidate(&m, tsInNs);
        }
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlDisplayRecorder_cropOutputSize
        (JNIEnv *env, jobject thiz, jlong handler, jfloat left, jfloat top,
         jfloat right, jfloat bottom) {
    if (handler) {
        getHandler(handler)->cropOutputSize(left, top, right, bottom);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlDisplayRecorder_setBitrate
        (JNIEnv *env, jobject thiz, jlong handler, jint bitrate) {
    if (handler) {
        getHandler(handler)->setBitrate(bitrate);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlDisplayRecorder_setProfile
        (JNIEnv *env, jobject thiz, jlong handler, jstring profile) {
    if (handler) {
        const char *str = env->GetStringUTFChars(profile, JNI_FALSE);
        getHandler(handler)->setProfile(std::string(str));
        env->ReleaseStringUTFChars(profile, str);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlDisplayRecorder_setPreset
        (JNIEnv *env, jobject thiz, jlong handler, jstring profile) {
    if (handler) {
        const char *str = env->GetStringUTFChars(profile, JNI_FALSE);
        getHandler(handler)->setPreset(std::string(str));
        env->ReleaseStringUTFChars(profile, str);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlDisplayRecorder_setMaxSize
        (JNIEnv *env, jobject thiz, jlong handler, jint width, jint height) {
    if (handler) {
        getHandler(handler)->setMaxSize(width, height);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AlDisplayRecorder_setEnableHardware
        (JNIEnv *env, jobject thiz, jlong handler, jboolean enable) {
    if (handler) {
        getHandler(handler)->setEnableHardware(JNI_TRUE == enable);
    }
}

#ifdef __cplusplus
}
#endif
