#ifndef _MAIN_
#define _MAIN_

#include <jni.h>

//----------------------------------------------------------------------
//
//	JAVA���ɓn���֐�
//
//----------------------------------------------------------------------
extern "C" {
	//������
	//�O���t�B�b�N�ȊO�̏���
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_systemInit(JNIEnv * env, jobject obj, jobject asset, jint input_maxPoint);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject methods);

	//�X�V
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_update(JNIEnv * env, jobject obj, jfloat dt);

	//
	//	Activity�̃��C�t�T�C�N���ɂ��킹�Ă�т������֐�
	//	Activity�̃��C�t�T�C�N���̎Q�lHP	http://www.javadrive.jp/android/activity/index2.html
	//
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onPause(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onResume(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onDestory(JNIEnv * env, jobject obj);

};

#define USE_JNI_VERSION JNI_VERSION_1_6
extern JavaVM* g_VM;	//�}���`�X���b�h�����Ƃ��K�v�ɂȂ�B

#endif