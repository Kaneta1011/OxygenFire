#ifndef _ASSETS_LOADER_
#define _ASSETS_LOADER_

//=======================================================================================================
//
//	Assetsからファイルを読み込むクラス
//	機能：	マルチスレッド上でも利用可能
//		・パス指定からのファイル読み込み
//		・テクスチャの読み込み	//JNIEnvクラスが必要	下のコメントを呼んでください
//
//****************************************************************************************
//	################################################################
//	#### テクスチャを読み込む場合はJNIEnvクラスが必要になります ####
//	################################################################
//		テクスチャの読み込み時にJava側の関数を利用しているため、JNIEnvクラスが必要になります
//
//		JNIEnvクラスは大体、JNI関数の引数にあるか、utility.hで宣言しているJavaVM* g_VMのメンバ関数GetEnv()からとってきます
//		JNIEnvクラスを使うときになったら、下のコメントも目を通してください
//
//****************************************************************************************
//=======================================================================================================
//
//	・JNIEnvが必要になったとき引数から持ってこれないとき、下のコード参考にしてください
//	主に、スレッドが走っているときにAssetsからファイルを読み込むときに必要になると思います
//
//--------------------------------------------------------------------------------------------
//	#include "utility.h"
//
//	JNIEnv* env;
//	getJNIEnv(&env);//＜－ utility.hで宣言している
//
//--------------------------------------------------------------------------------------------
//
//	参考HP　：　http://blog.nekobako.net/post/36/　解決方法という見出しの下あたりにコードあり
//	あと、田代君の「AndroidNDKプログラミングガイド」という本のp.57～p.61が参考になりました
//
//=======================================================================================================
//****************************************************************************************************
//							サンプル
//****************************************************************************************************
//----------------------------------------------------------------------------------------------------
//					ファイル読み込みのサンプル
//----------------------------------------------------------------------------------------------------
//	char* data;
//	int size;
//	const char* fileName = "test.txt";
//	AssetsLoader::load(&data, &size, fileName);
//
//=======================================================================================================
//----------------------------------------------------------------------------------------------------
//					テクスチャ読み込みのサンプル
//----------------------------------------------------------------------------------------------------
//	JNIEnv* env;		//引数またはJavaVMから
//	GLuint gTexture;
//	const char* fileName = "testImage.png";
//	AssetsLoader::loadTexture(&gTexture, fileName, env);
//
//	//	サンプラーの設定は各自設定してください
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//=======================================================================================================

#include <jni.h>
#include <android\asset_manager.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "utility\Bitmap.h"
#include "templateLib\kQueue.h"
#include "thread\Class\kMutex\kMutex.h"

struct LoadTextureInfo
{
	GLuint* m_TexID;
	rlib::Bitmap* mp_Bitmap;
	LoadTextureInfo()
	{
		m_TexID=NULL;
		mp_Bitmap=NULL;
	}
	~LoadTextureInfo()
	{
		delete mp_Bitmap;
	}
};

class AssetsLoader
{
	friend class rlib::Bitmap;
	typedef klib::ktl::kQueue_Safe<LoadTextureInfo*> LoadTextureQueue;
public:
	static void sInit(JNIEnv* env, jobject asset);
	static void sClear(JNIEnv* env);

	static bool load(char** out, int* outSize, const char* fileName);

	static void begin();
	static void end();

	/*
	Assetsにある画像からテクスチャを読み込む関数
	中でglTexImage2Dを呼んでいます
	サンプラーなどのテクスチャの設定は利用側が行ってください
	*/
	static bool loadTexture(GLuint* outTexture, const char* fileName, JNIEnv* env, GLint format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);

	/*
	Bitmapに吐き出す版
	*/
	static bool loadBitmap(rlib::Bitmap* outBitmap, const char* fileName, GLint format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);


	static AAssetManager* getAssetManager(){return sAssetMng;}

protected:
	static AAssetManager* sAssetMng;
	static jclass		  sJNICallMethod;
	static LoadTextureQueue m_LoadTextureQueue;
	static bool m_Loading;
	static klib::thread::kMutex m_Mutex;
};

#endif