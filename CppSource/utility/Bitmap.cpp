#include "Bitmap.h"

#include "utility\utility.h"
#include "utility\assetsLoader.h"

#include <assert.h>
#include <android\bitmap.h>
#include <stdlib.h>

using namespace rlib;

static const char* TAG = "Bitmap";

Bitmap::Bitmap():
	mWidth(0),
	mHeight(0),
	mFormat(0),
	mType(0),
	mBitmap(0)
{
}

Bitmap::~Bitmap()
{
	clear();
}

void Bitmap::clear()
{
	JNIEnv* env;
	getJNIEnv(&env);

	if( this->mBitmap ){ env->DeleteGlobalRef(this->mBitmap); this->mBitmap = 0; }
	mWidth = mHeight = 0;
}

bool Bitmap::load(const char* fileName, GLint format, GLenum type)
{
	JNIEnv* env;
	getJNIEnv(&env);

	bool isOK = true;
	//java側の関数を使う
	jmethodID loadImageId = env->GetStaticMethodID(AssetsLoader::sJNICallMethod, "loadImage", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
	if( loadImageId != NULL )
	{
	//Java側からBitmapクラスを取得
		jstring name = env->NewStringUTF(fileName);
		jobject bitmap = env->CallStaticObjectMethod(AssetsLoader::sJNICallMethod, loadImageId, name );
		this->mBitmap = env->NewGlobalRef(bitmap);
		env->DeleteLocalRef(name);
	//読み込んだBitmapから画像サイズ、ピクセル情報を取り出す
		AndroidBitmapInfo info;
		int ret;
		if( (ret = AndroidBitmap_getInfo(env, this->mBitmap, &info)) < 0 )
		{
			LOGE(TAG, "Failure Get Bitmap Info...");
			isOK = false;
		}
		else
		{
			this->mWidth = info.width;
			this->mHeight = info.height;
			this->mFormat = format;
			this->mType = type;
			LOGI(TAG, "width = %d | height = %d", info.width, info.height);
			LOGI(TAG, "format = %d | stride = %d | flag = %d", info.format, info.stride, info.flags);
		}
	}
	else
	{//if( loadImageId == NULL ) エラー処理
		LOGE(TAG, "file:%s | line:%d | Failure get MethodID from JNICallMethod class...", __LINE__, __FILE__);
		return false;
	}
	return isOK;

}

void Bitmap::genTexture(GLuint* outTexture)
{
	glGenTextures(1, outTexture);
	checkGlError(TAG, "glGenTextures");
	glBindTexture(GL_TEXTURE_2D, *outTexture);
	checkGlError(TAG, "glBindTexture");

	char* pixels = lock();
//GPUにピクセルデータを転送
	glTexImage2D( GL_TEXTURE_2D, 0,
		this->mFormat, this->mWidth, this->mHeight, 0,
			this->mFormat, this->mType, pixels );
	checkGlError(TAG, "glTexImage2D");

	unlock();

}

char* Bitmap::lock()
{
	JNIEnv* env;
	getJNIEnv(&env);

	char* pixels = 0;
	if( (AndroidBitmap_lockPixels(env, this->mBitmap, (void**)&pixels) ) < 0 )
	{
		LOGE(TAG, "Failure Lock Bitmap...");
		abort();
	}
	return pixels;
}

void Bitmap::unlock()
{
	JNIEnv* env;
	getJNIEnv(&env);
	AndroidBitmap_unlockPixels(env, this->mBitmap);
}
