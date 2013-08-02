#ifndef _COMMON_
#define _COMMON_

#include <jni.h>
#include <android/log.h>

#define  LOGI(TAG,...)  __android_log_print(ANDROID_LOG_INFO,(TAG),__VA_ARGS__)
#define  LOGE(TAG,...)  __android_log_print(ANDROID_LOG_ERROR,(TAG),__VA_ARGS__)

#define USE_JNI_VERSION JNI_VERSION_1_6
extern JavaVM* g_VM;	//�}���`�X���b�h�����Ƃ��K�v�ɂȂ�B

jint getJNIEnv(JNIEnv** env);

#endif
