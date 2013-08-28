#include "AssetsLoader.h"

#include <android\asset_manager_jni.h>

#include <assert.h>
#include <android\bitmap.h>

#include <stdlib.h>
#include <math.h>

#include "utility.h"


AAssetManager* AssetsLoader::sAssetMng = NULL;
jclass		   AssetsLoader::sJNICallMethod = NULL;

static const char* TAG = "AssetsLoader";

void AssetsLoader::sInit(JNIEnv* env, jobject asset)
{
	sAssetMng = AAssetManager_fromJava(env, asset);

	jclass clazz = env->FindClass("jp/ac/ecc/oxygenfire/JNICallMethod");
	if( clazz == NULL ){
		LOGE(TAG, "file:%s | line:%d | Failure search JNICallMethod class...", __LINE__, __FILE__);
		abort();
	}
	AssetsLoader::sJNICallMethod = (jclass)env->NewGlobalRef(clazz);
}

void AssetsLoader::sClear(JNIEnv* env)
{
	sAssetMng = NULL;
	if( sJNICallMethod != NULL ){ env->DeleteGlobalRef(sJNICallMethod); sJNICallMethod = NULL;}
}

bool AssetsLoader::load(char** out, int* outSize, const char* fileName)
{
	assert(NULL != sAssetMng && "AssetsLoader::load | Failure Get AssetsManager...");
	bool isOK = false;
	int size = 0;
	AAsset* asset = AAssetManager_open(sAssetMng, fileName, AASSET_MODE_UNKNOWN);
	if( asset )
	{
		size = AAsset_getLength(asset);
		if( size > 0 )
		{
			*out = new char[size+1];
			AAsset_read(asset, *out, size);
			(*out)[size] = '\0';
			//LOGI(TAG, "Complete load \"%s\" | size = %d ", fileName, size);
			isOK = true;
		}
		AAsset_close(asset);
	}
	*outSize = size;
	return isOK;
}

bool AssetsLoader::loadTexture(GLuint* outTexture, const char* fileName, JNIEnv* env, GLint format, GLenum type)
{
	bool isOK = false;

	//java側の関数を使う
	jmethodID loadImageId = env->GetStaticMethodID(sJNICallMethod, "loadImage", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
	if( loadImageId != NULL )
	{
	//テクスチャIDの取得
		glGenTextures(1, outTexture);
		glBindTexture(GL_TEXTURE_2D, *outTexture);

	//Java側からBitmapクラスを取得
		jstring name = env->NewStringUTF(fileName);
		jobject bitmap = env->CallStaticObjectMethod(sJNICallMethod, loadImageId, name );
		env->DeleteLocalRef(name);
	//読み込んだBitmapから画像サイズ、ピクセル情報を取り出す
		AndroidBitmapInfo info;
		int ret;
		if( (ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0 )
		{
			LOGE(TAG, "Failure Get Bitmap Info...");
			abort();
		}
		LOGI(TAG, "width = %d | height = %d", info.width, info.height);

	//LockをかけないとBitmapからピクセル情報を取り出せない
		char* pixels = 0;
		if( (ret = AndroidBitmap_lockPixels(env, bitmap, (void**)&pixels) ) < 0 )
		{
			LOGE(TAG, "Failure Lock Bitmap...");
			abort();
		}
	//GPUにピクセルデータを転送
		glTexImage2D( GL_TEXTURE_2D, 0,
				format, info.width, info.height, 0,
				format, type, pixels );
		checkGlError(TAG, "glTexImage2D");
	//Unlockを忘れずに
		AndroidBitmap_unlockPixels(env, bitmap);
	//正常終了
		isOK = true;
	}
	else
	{//if( loadImageId == NULL ) エラー処理
		LOGE(TAG, "file:%s | line:%d | Failure get MethodID from JNICallMethod class...", __LINE__, __FILE__);
		abort();
	}
	return isOK;
}