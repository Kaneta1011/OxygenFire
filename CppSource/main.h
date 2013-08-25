#ifndef _MAIN_
#define _MAIN_

#include <jni.h>

//----------------------------------------------------------------------
//
//	JAVA側に渡す関数
//
//----------------------------------------------------------------------
extern "C" {
	//初期化
	//グラフィック以外の処理
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_systemInit(JNIEnv * env, jobject obj, jobject asset, jint input_maxPoint);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject methods);

	//更新
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_update(JNIEnv * env, jobject obj, jfloat dt);

	//
	//	Activityのライフサイクルにあわせてよびだされる関数
	//	Activityのライフサイクルの参考HP	http://www.javadrive.jp/android/activity/index2.html
	//
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onPause(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onResume(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onDestory(JNIEnv * env, jobject obj);

};

#define USE_JNI_VERSION JNI_VERSION_1_6
extern JavaVM* g_VM;	//マルチスレッドを作るとき必要になる。

#endif