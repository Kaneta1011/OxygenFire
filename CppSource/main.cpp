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
#include "utility\debugMessageMng.h"//�f�o�b�O�p�̕�����\���̂��߂̃w�b�_�[

#include "main.h"

// OpenGL ES 2.0 code

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include "../CppSource/math/kmathf.h"
#include	"GraphicsLib\Class\tRenderState\RenderState.h"
#include	"math\\kmathf.h"
#include	<assert.h>

#include "FrameWork\Class\kFrameWork\kFrameWork.h"
#include "GraphicsLib\Class\r2DObj\r2DRenderer.h"

#include "testScene.h"
#include "Ueda\rTestScene.h"

//	use namespace
using namespace RenderLib;
using namespace klib;
using namespace math;

static const char* TAG = "main.cpp";
klib::kFrameWork	framework;

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

//----------------------------------------------------------------------
//
//	JAVA������Ăяo�����֐�
//
//----------------------------------------------------------------------
/*
	�O���t�B�b�N�ȊO�̏�����
*/
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_systemInit(JNIEnv * env, jobject obj, jobject asset, jint input_maxPoint)
{
	LOGI(TAG, "Execute systemInit.");
	
	AssetsLoader::sInit(env, asset);
	mlInput::init(input_maxPoint);

	LOGI(TAG, "Complete systemInit.");
}

//
//		API�J�n���܂���Pause��Ԃ���ĂэĊJ�����Ƃ��ɌĂяo�����
//		�O���t�B�b�N�֘A��Java_jp_ac_ecc_oxygenfire_GL2JNILib_init�֐��ŕK���s���Ă�������	API�������܂�
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onResume(JNIEnv * env, jobject obj)
{
	LOGI(TAG, "Execute onResume.");
	
	Sound::init();

	LOGI(TAG, "Complete onResume.");
}

//
//		�O���t�B�b�N�̏�����
//

JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject methods)
{
	LOGI(TAG, "Execute graphic init");
	//�����_�[�X�e�[�g������
	RenderState::setScreenWidth(width);
	RenderState::setScreenHeight(height);
	RenderState::Setting_Viewport(.0f,.0f,width,height);
	RenderState::Setting_ViewMatrix(Vector3(20,20,20),Vector3(0,10,0),Vector3(0,1,0));
	RenderState::Setting_PerspectiveMatrix(K_PI/4,(float)width/(float)height,.1f,100.0f);

	rlib::r2DPipeline::init();

	dprintf("Screen Size Initialize w=%d h=%d",width,height);

	//glLineWidth(1.0f);

	//�V�[���쐬
	testScene::_create();
	//�V�[�����蓖��
	framework.sceneChange(testScene::_getInstancePtr());

	//framework.sceneChange(new rTestScene() );
	
	LOGI(TAG, "Complete graphic init");

}

JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_update(JNIEnv * env, jobject obj, jfloat dt)
{
//===========================================================================================
//	�f�o�b�O�p�̕�����\���̃T���v��
//	���ƁAdt�̒l�͓K���ł�(8/12�A�c�@�����܂����B)
	DEBUG_MSG("dt=%.3f[ms]", dt);
	//mlInput::debugMseeage();
//===========================================================================================
	RenderState::Clear_Color(.5,.5,.5,1);
	RenderState::Clear_Buffer(CLEAR_BUFFER_COLOR);
	RenderState::Clear_Buffer(CLEAR_BUFFER_DEPTH);
	
	//�V�[���X�V
	if(framework.sceneUpdate()){framework.sceneRender();}

	mlInput::update(dt);
	DEBUG_FLUSH_MSG();//�����Ńf�o�b�O�p�̕������TextView�ɐݒ肵�Ă���̂ŁA�����Ȃ���!!
}

//
//		Activity����\���ɂȂ����Ƃ��Ăяo�����(Pause��ԂƏ���ɖ���)
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onPause(JNIEnv * env, jobject obj)
{
	LOGI(TAG, "Execute onPause.");
	
	Sound::clear();
	//�t���[�����[�N�N���A
	framework.sceneClear();
	//�V�[���j��
	testScene::_destroy();

	rlib::r2DPipeline::clear();

	LOGI(TAG, "Complete onPause.");
}

//
//		API���j�������Ƃ��ɌĂяo�����
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onDestory(JNIEnv * env, jobject obj)
{
	LOGI(TAG, "Execute onDestory.");
	
	mlInput::clear();

	LOGI(TAG, "Complete onDestory.");
}

JavaVM* g_VM;	//getJNIEnv�֐����g���Ƃ��ƃ}���`�X���b�h�����Ƃ��K�v�ɂȂ�B
//
//	���C�u���������[�h���ꂽ�Ƃ��Ɏ����I�ɌĂ΂��֐�
//
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	LOGI(TAG, "Execute JNI_OnLoad.");
	g_VM = vm;

	return USE_JNI_VERSION;
}

//
//	���C�u�������A�����[�h���ꂽ�Ƃ��Ɏ����I�ɌĂ΂��֐�
//
JNIEXPORT void JNI_UnLoad(JavaVM* vm, void* reserved)
{
	LOGI(TAG, "Execute JNI_UnLoad.");

}
