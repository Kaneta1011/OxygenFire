#ifndef _COMMON_
#define _COMMON_

#include <jni.h>
#include <android/log.h>

#define  LOGI(TAG,...)  __android_log_print(ANDROID_LOG_INFO,(TAG),__VA_ARGS__)
#define  LOGE(TAG,...)  __android_log_print(ANDROID_LOG_ERROR,(TAG),__VA_ARGS__)

jint getJNIEnv(JNIEnv** env);

#endif
