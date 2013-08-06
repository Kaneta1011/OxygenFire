/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
//	�����p�̃T���v������ԉ��ɏ����Ă����܂��B(7/31 �A�c)
//
#include "sound\mlsound.h"
#include "utility\assetsLoader.h"
#include "input\Input.h"

// OpenGL ES 2.0 code

#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include "../CppSource/math/kmathf.h"
#include	"ShaderLib\ShaderManager.h"
#include	"RenderLib\RenderState.h"
#include	"math\\kmathf.h"
#include	"RenderLib\\Object3D\\Mesh.h"
#include	<assert.h>

//	use namespace
using namespace ShaderLib;
using namespace RenderLib;
using namespace klib::math;

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


//=======================================================================================================
//
//	JNIEnv���K�v�ɂȂ����Ƃ������ɂȂ��Ƃ��A���̃R�[�h�Q�l�ɂ��Ă��������B
//	��ɁA�X���b�h�������Ă���Ƃ���Assets����t�@�C����ǂݍ��ނƂ��ɕK�v�ɂȂ�Ǝv���܂��B
//
//	#include "utility.h"
//
//	JNIEnv* env;
//	getJNIEnv(&env);//<- getJNIEnv()��utility.h�Ő錾���Ă���
//
//	�Q�lHP�@�F�@http://blog.nekobako.net/post/36/�@�������@�Ƃ������o���̉�������ɃR�[�h����B
//
//=======================================================================================================
JavaVM* g_VM;	//getJNIEnv�֐����g���Ƃ��ƃ}���`�X���b�h�����Ƃ��K�v�ɂȂ�B

//
//	���C�u���������[�h���ꂽ�Ƃ��Ɏ����I�ɌĂ΂��֐�
//
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	LOGI("Passage JNI_OnLoad.");
	g_VM = vm;

	return USE_JNI_VERSION;
}

//
//	���C�u�������A�����[�h���ꂽ�Ƃ��Ɏ����I�ɌĂ΂��֐�
//
JNIEXPORT void JNI_UnLoad(JavaVM* vm, void* reserved)
{
	LOGI("Passage JNI_UnLoad.");

}

//=======================================================================================================

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

Mesh* spMesh;


#define TEST_MAX 100
Vector3 testPos[TEST_MAX];

float getRandomNumberFloat( float Min, float Max )
{
	if( Min < 0 )
		Max += -Min;
	else
		Max -= Min;

	return ( float )rand() / ( float )RAND_MAX * Max + Min;
}

//----------------------------------------------------------------------
//	Initialize
//----------------------------------------------------------------------
bool Initialize() 
{
	ShaderManager::Init();



	//LOGI("%d\n%s",__LINE__,__FILE__);
	//assert(!""); 
	

	spMesh = new Mesh;
	spMesh->Init();
	spMesh->Create_Box(0.5f,0.5f,0.5f);
	spMesh->setPosition(0,0,0);
	spMesh->setAngle(0,0,0);
	spMesh->setScale(1,1,1);
	spMesh->Update();

	for(int n=0;n<TEST_MAX;n++)
	{
		testPos[n] = Vector3(getRandomNumberFloat(-10,10),
			getRandomNumberFloat(-10,10),getRandomNumberFloat(-10,10));
	}

  return true;
}

//----------------------------------------------------------------------
//	delete
//----------------------------------------------------------------------
void Delete()
{
	ShaderManager::Delete();

	delete spMesh; spMesh=NULL;
}

//----------------------------------------------------------------------
//	Update
//----------------------------------------------------------------------
bool Update()
{
	static Vector3 inputOffset(0,0,0);
	if( mlInput::key() != mlInput::FREE )
	{
		inputOffset.x = (mlInput::getX()/(float)RenderState::getScreenWidth()*2.f - 1.f) * 10.f;
		inputOffset.y = (mlInput::getY()/(float)RenderState::getScreenHeight()*2.f - 1.f) * 10.f;
	}
	//	Matrix�ݒ�
	RenderState::Setting_ViewMatrix(Vector3(20,20,20),Vector3(0,0,0) + inputOffset,Vector3(0,1,0));
	RenderState::Setting_PerspectiveMatrix((float)K_PI/4, 
		(float)RenderState::getScreenWidth()/(float)RenderState::getScreenHeight(),
		0.1f, 100.0f );

	//	angle�e�X�g
	static float workX = .0f;
	static float workY = .0f;
	static float workZ = .0f;
	workX += 0.01f;
	workY += 0.01f;
	workZ += 0.01f;

	spMesh->setAngle(Vector3(workX,workY,workZ));
	//spMesh->Update();

	return true;
}
//----------------------------------------------------------------------
//	Render
//----------------------------------------------------------------------
void Render() 
{
	glClearColor(0.3f,0.3f,0.3f,1.0f);
	RenderState::Clear_Buffer(CLEAR_BUFFER_COLOR);
	RenderState::Clear_Buffer(CLEAR_BUFFER_DEPTH);
	RenderState::Setting_Viewport(0,0,RenderState::getScreenWidth(),RenderState::getScreenHeight());


	for(int n=0;n<TEST_MAX;n++)
	{
		spMesh->setPosition(testPos[n]);
		spMesh->Update();
		spMesh->Render(ShaderManager::getSimple());
	}
}


//----------------------------------------------------------------------
//
//	JAVA���ɓn���֐�
//
//----------------------------------------------------------------------
extern "C" {
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject methods);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_update(JNIEnv * env, jobject obj, jfloat dt);

	//�O���t�B�b�N�ȊO�̏���
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_systemInit(JNIEnv * env, jobject obj, jobject asset, jint input_maxPoint);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_sendTouchEvent(JNIEnv * env, jobject obj, jint count, jintArray arrayID, jfloatArray pointsX, jfloatArray pointsY, jfloatArray arrayPressure, jint id, jint con);

	//
	//	Activity�̃��C�t�T�C�N���ɂ��킹�Ă�т������֐�
	//	Activity�̃��C�t�T�C�N���̎Q�lHP	http://www.javadrive.jp/android/activity/index2.html
	//
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onPause(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onResume(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onDestory(JNIEnv * env, jobject obj);

};

//
//		�O���t�B�b�N�̏�����
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject methods)
{
	RenderState::setScreenWidth(width);
	RenderState::setScreenHeight(height);
	RenderState::Setting_PolygonBathSides(false);
	 
	Initialize();
}
 
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_update(JNIEnv * env, jobject obj, jfloat dt)
{
	if(Update()){Render();}
}

/*
	�O���t�B�b�N�ȊO�̏�����
*/
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_systemInit(JNIEnv * env, jobject obj, jobject asset, jint input_maxPoint)
{
	LOGI("Passage systemInit.");
	
	AssetsLoader::sInit(env, asset);
	mlInput::init(input_maxPoint);

	LOGI("Complete systemInit.");
}

#include <android\input.h>

//
//		�^�b�`�C�x���g�̎�M
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_sendTouchEvent(JNIEnv * env, jobject obj, jint count, jintArray arrayID, jfloatArray pointsX, jfloatArray pointsY, jfloatArray arrayPressure, jint id, jint con)
{
	mlInput::update(env, count, arrayID, pointsX, pointsY, arrayPressure, id, con );
}

//
//		Oxygenfire_Activity��\���ɂȂ����Ƃ��Ăяo�����
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onPause(JNIEnv * env, jobject obj)
{
	LOGI("Passage onPause.");
	

	LOGI("Complete onPause.");
}

//
//		API��Pause��Ԃ���ĂэĊJ�����Ƃ��ɌĂяo�����
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onResume(JNIEnv * env, jobject obj)
{
	LOGI("Passage onResume.");
	

	LOGI("Complete onResume.");
}

//
//		API���j�������Ƃ��ɌĂяo�����
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onDestory(JNIEnv * env, jobject obj)
{
	LOGI("Passage onDestory.");
	
	mlInput::clear();

	LOGI("Complete onDestory.");
}

//---------------------------------------------------------------------------
//		�����p�̃T���v��
//{
//	mlSound::Base g_base;
//	g_base.init();	//������
//	g_base.clear();	//�j��
//
//	{//�t�@�C���̓ǂݍ��݂ƍ폜
//		g_base.add(
//			0,	//�ǉ�����ԍ�/*0�`(mlSound::Base::PLAYER_MAX-1)�̊�*/,
//			env,//JNIEnv�N���X�BJNI�̈����ɕK�����Ă�͂��B
//			assetsManager,	//Java����AssetsManager�N���X�������Ă���B
//			"sound/bgm1.mp3",	//assets�̃p�X�B��ł�assets�t�H���_�[��sound�t�H���_�[�̒��ɂ���bgm1.mp3��ǂݍ���
//		);
//		g_base.del(0/*�g�p����ԍ�*/);//�폜
//	}
//	{//�Đ��֘A�̃R�[�h
//		g_base.play(  /*�g�p����ԍ�*/, true/*���[�v�t���O*/);
//		g_base.pause( /*�g�p����ԍ�*/);//�ꎞ��~
//		g_base.stop(  /*�g�p����ԍ�*/);//�Đ��I��
//		g_base.volume(/*�g�p����ԍ�*/, volume/*0�`1�͈̔�*/);
//	}
//}
//---------------------------------------------------------------------------
