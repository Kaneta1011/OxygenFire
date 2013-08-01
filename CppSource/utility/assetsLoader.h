#ifndef _ASSETS_LOADER_
#define _ASSETS_LOADER_

#include <jni.h>
#include <android\asset_manager.h>

//
//	��{Eclipse����res/assets�t�H���_�[�Ɏg���t�@�C����ǉ����Ă����݂���
//	�ڂ����͒��ׂ�
//	�g���Ƃ���Java����JNIEnv�N���X��AssetsManager�N���X��n����OK
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