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
//	音利用のサンプルを一番下に書いておきます。(7/31 植田)
//
#include "sound\mlsound.h"
#include "utility\assetsLoader.h"
#include "input\Input.h"
#include "utility\debugMessageMng.h"//デバッグ用の文字列表示のためのヘッダー

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
//	JNIEnvが必要になったとき引数にないとき、下のコード参考にしてください。
//	主に、スレッドが走っているときにAssetsからファイルを読み込むときに必要になると思います。
//
//	#include "utility.h"
//
//	JNIEnv* env;
//	getJNIEnv(&env);//<- getJNIEnv()はutility.hで宣言している
//
//	参考HP　：　http://blog.nekobako.net/post/36/　解決方法という見出しの下あたりにコードあり。
//
//=======================================================================================================

//----------------------------------------------------------------------
//
//	JAVA側から呼び出される関数
//
//----------------------------------------------------------------------
/*
	グラフィック以外の初期化
*/
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_systemInit(JNIEnv * env, jobject obj, jobject asset, jint input_maxPoint)
{
	LOGI(TAG, "Execute systemInit.");
	
	AssetsLoader::sInit(env, asset);
	mlInput::init(input_maxPoint);

	LOGI(TAG, "Complete systemInit.");
}

//
//		API開始時またはPause状態から再び再開されるときに呼び出される
//		グラフィック関連はJava_jp_ac_ecc_oxygenfire_GL2JNILib_init関数で必ず行ってください	APIが落ちます
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onResume(JNIEnv * env, jobject obj)
{
	LOGI(TAG, "Execute onResume.");
	
	Sound::init();

	LOGI(TAG, "Complete onResume.");
}

//
//		グラフィックの初期化
//

JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject methods)
{
	LOGI(TAG, "Execute graphic init");
	//レンダーステート初期化
	RenderState::setScreenWidth(width);
	RenderState::setScreenHeight(height);
	RenderState::Setting_Viewport(.0f,.0f,width,height);
	RenderState::Setting_ViewMatrix(Vector3(20,20,20),Vector3(0,10,0),Vector3(0,1,0));
	RenderState::Setting_PerspectiveMatrix(K_PI/4,(float)width/(float)height,.1f,100.0f);

	rlib::r2DPipeline::init();

	dprintf("Screen Size Initialize w=%d h=%d",width,height);

	//glLineWidth(1.0f);

	//シーン作成
	testScene::_create();
	//シーン割り当て
	framework.sceneChange(testScene::_getInstancePtr());

	//framework.sceneChange(new rTestScene() );
	
	LOGI(TAG, "Complete graphic init");

}

JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_update(JNIEnv * env, jobject obj, jfloat dt)
{
//===========================================================================================
//	デバッグ用の文字列表示のサンプル
//	あと、dtの値は適当です(8/12植田　直しました。)
	DEBUG_MSG("dt=%.3f[ms]", dt);
	//mlInput::debugMseeage();
//===========================================================================================
	RenderState::Clear_Color(.5,.5,.5,1);
	RenderState::Clear_Buffer(CLEAR_BUFFER_COLOR);
	RenderState::Clear_Buffer(CLEAR_BUFFER_DEPTH);
	
	//シーン更新
	if(framework.sceneUpdate()){framework.sceneRender();}

	mlInput::update(dt);
	DEBUG_FLUSH_MSG();//ここでデバッグ用の文字列をTextViewに設定しているので、消さないで!!
}

//
//		Activityが非表示になったとき呼び出される(Pause状態と勝手に命名)
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onPause(JNIEnv * env, jobject obj)
{
	LOGI(TAG, "Execute onPause.");
	
	Sound::clear();
	//フレームワーククリア
	framework.sceneClear();
	//シーン破壊
	testScene::_destroy();

	rlib::r2DPipeline::clear();

	LOGI(TAG, "Complete onPause.");
}

//
//		APIが破棄されるときに呼び出される
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onDestory(JNIEnv * env, jobject obj)
{
	LOGI(TAG, "Execute onDestory.");
	
	mlInput::clear();

	LOGI(TAG, "Complete onDestory.");
}

JavaVM* g_VM;	//getJNIEnv関数を使うときとマルチスレッドを作るとき必要になる。
//
//	ライブラリがロードされたときに自動的に呼ばれる関数
//
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	LOGI(TAG, "Execute JNI_OnLoad.");
	g_VM = vm;

	return USE_JNI_VERSION;
}

//
//	ライブラリがアンロードされたときに自動的に呼ばれる関数
//
JNIEXPORT void JNI_UnLoad(JavaVM* vm, void* reserved)
{
	LOGI(TAG, "Execute JNI_UnLoad.");

}
