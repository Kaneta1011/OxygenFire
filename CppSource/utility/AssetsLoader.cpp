#include "AssetsLoader.h"

#include <android\asset_manager_jni.h>

#include <assert.h>

AssetsLoader::AssetsLoader():
mSize(0),
mBuf(NULL)
{
}

AssetsLoader::~AssetsLoader()
{
	clear();
}

void AssetsLoader::clear()
{
	if( this->mBuf ){ delete[] this->mBuf; this->mBuf = NULL; }
}

bool AssetsLoader::load(JNIEnv * env, jobject assetsManager, jstring fileName)
{
	clear();
	bool isOK = false;

	AAssetManager* mgr = AAssetManager_fromJava(env, assetsManager);
    assert(NULL != mgr && "AssetsLoader::load | Failure Get AssetsManager...");

	const char* filePath = env->GetStringUTFChars(fileName, NULL);
	AAsset* asset = AAssetManager_open(mgr, filePath, AASSET_MODE_UNKNOWN);
	env->ReleaseStringUTFChars(fileName, filePath);

	if( asset )
	{
		this->mSize = AAsset_getLength(asset);
		if( this->mSize > 0 )
		{
			this->mBuf = new char[this->mSize+1];
			AAsset_read(asset, this->mBuf, this->mSize);
			this->mBuf[this->mSize] = '\0';
			isOK = true;
		}

		AAsset_close(asset);
	}

	return isOK;
}

