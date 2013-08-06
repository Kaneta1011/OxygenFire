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
JavaVM* g_VM;	//getJNIEnv関数を使うときとマルチスレッドを作るとき必要になる。

//
//	ライブラリがロードされたときに自動的に呼ばれる関数
//
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	LOGI("Passage JNI_OnLoad.");
	g_VM = vm;

	return USE_JNI_VERSION;
}

//
//	ライブラリがアンロードされたときに自動的に呼ばれる関数
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
	//	Matrix設定
	RenderState::Setting_ViewMatrix(Vector3(20,20,20),Vector3(0,0,0) + inputOffset,Vector3(0,1,0));
	RenderState::Setting_PerspectiveMatrix((float)K_PI/4, 
		(float)RenderState::getScreenWidth()/(float)RenderState::getScreenHeight(),
		0.1f, 100.0f );

	//	angleテスト
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
//	JAVA側に渡す関数
//
//----------------------------------------------------------------------
extern "C" {
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject methods);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_update(JNIEnv * env, jobject obj, jfloat dt);

	//グラフィック以外の処理
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_systemInit(JNIEnv * env, jobject obj, jobject asset, jint input_maxPoint);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_sendTouchEvent(JNIEnv * env, jobject obj, jint count, jintArray arrayID, jfloatArray pointsX, jfloatArray pointsY, jfloatArray arrayPressure, jint id, jint con);

	//
	//	Activityのライフサイクルにあわせてよびだされる関数
	//	Activityのライフサイクルの参考HP	http://www.javadrive.jp/android/activity/index2.html
	//
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onPause(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onResume(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onDestory(JNIEnv * env, jobject obj);

};

//
//		グラフィックの初期化
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
	グラフィック以外の初期化
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
//		タッチイベントの受信
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_sendTouchEvent(JNIEnv * env, jobject obj, jint count, jintArray arrayID, jfloatArray pointsX, jfloatArray pointsY, jfloatArray arrayPressure, jint id, jint con)
{
	mlInput::update(env, count, arrayID, pointsX, pointsY, arrayPressure, id, con );
}

//
//		Oxygenfire_Activity非表示になったとき呼び出される
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onPause(JNIEnv * env, jobject obj)
{
	LOGI("Passage onPause.");
	

	LOGI("Complete onPause.");
}

//
//		APIがPause状態から再び再開されるときに呼び出される
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onResume(JNIEnv * env, jobject obj)
{
	LOGI("Passage onResume.");
	

	LOGI("Complete onResume.");
}

//
//		APIが破棄されるときに呼び出される
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onDestory(JNIEnv * env, jobject obj)
{
	LOGI("Passage onDestory.");
	
	mlInput::clear();

	LOGI("Complete onDestory.");
}

//---------------------------------------------------------------------------
//		音利用のサンプル
//{
//	mlSound::Base g_base;
//	g_base.init();	//初期化
//	g_base.clear();	//破棄
//
//	{//ファイルの読み込みと削除
//		g_base.add(
//			0,	//追加する番号/*0～(mlSound::Base::PLAYER_MAX-1)の間*/,
//			env,//JNIEnvクラス。JNIの引数に必ずついてるはず。
//			assetsManager,	//JavaからAssetsManagerクラスを持ってくる。
//			"sound/bgm1.mp3",	//assetsのパス。例ではassetsフォルダーのsoundフォルダーの中にあるbgm1.mp3を読み込む
//		);
//		g_base.del(0/*使用する番号*/);//削除
//	}
//	{//再生関連のコード
//		g_base.play(  /*使用する番号*/, true/*ループフラグ*/);
//		g_base.pause( /*使用する番号*/);//一時停止
//		g_base.stop(  /*使用する番号*/);//再生終了
//		g_base.volume(/*使用する番号*/, volume/*0～1の範囲*/);
//	}
//}
//---------------------------------------------------------------------------
