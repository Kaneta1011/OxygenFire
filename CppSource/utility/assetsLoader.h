#ifndef _ASSETS_LOADER_
#define _ASSETS_LOADER_

#include <jni.h>
#include <android\asset_manager.h>

//
//	基本Eclipseからres/assetsフォルダーに使うファイルを追加していくみたい
//	詳しくは調べて
//	使うときはJavaからJNIEnvクラスとAssetsManagerクラスを渡せばOK
//
class AssetsLoader
{
public:
	AssetsLoader();
	~AssetsLoader();

	void clear();
	bool load(JNIEnv * env, jobject assetsManager, jstring fileName);

public:
	int   size()const{return this->mSize;}
	const char* buffer()const{return this->mBuf;}

private:
	int		mSize;
	char*	mBuf;
};

#endif