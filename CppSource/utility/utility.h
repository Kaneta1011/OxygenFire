#ifndef _COMMON_
#define _COMMON_

#include <jni.h>
#include <android/log.h>

#define  LOGI(TAG,...)  __android_log_print(ANDROID_LOG_INFO,(TAG),__VA_ARGS__)
#define  LOGE(TAG,...)  __android_log_print(ANDROID_LOG_ERROR,(TAG),__VA_ARGS__)

#define printGLString(TAG, name, s) {\
    const char *v = (const char *) glGetString(s);\
    LOGI(TAG, "GL %s = %s\n", name, v);

#define checkGlError(TAG, op) \
    for (GLint error = glGetError(); error; error = glGetError()) {\
        LOGI(TAG, "after %s() glError (0x%x)\n", op, error);\
    }

jint getJNIEnv(JNIEnv** env);

#endif
