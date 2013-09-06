#include "AssetsLoader.h"

#include <android\asset_manager_jni.h>

#include <assert.h>
#include <android\bitmap.h>

#include <stdlib.h>
#include <math.h>

#include "utility.h"
#include "defines.h"

#include "utility\Bitmap.h"

#include "GraphicsLib\Class\kGraphicsPipline\kGraphicsPipline.h"

using namespace rlib;


AAssetManager* AssetsLoader::sAssetMng = NULL;
jclass		   AssetsLoader::sJNICallMethod = NULL;
AssetsLoader::LoadTextureQueue AssetsLoader::m_LoadTextureQueue;
bool AssetsLoader::m_Loading=false;

klib::thread::kMutex AssetsLoader::m_Mutex;

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
	m_LoadTextureQueue.clear();
	m_Loading=false;
}

void AssetsLoader::sClear(JNIEnv* env)
{
	sAssetMng = NULL;
	if( sJNICallMethod != NULL ){ env->DeleteGlobalRef(sJNICallMethod); sJNICallMethod = NULL;}
	m_LoadTextureQueue.clear();
	m_Loading=false;
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
	else
	{
		LOGE(TAG, "miss load filename = %s", fileName);
	}
	*outSize = size;
	return isOK;
}

//bool AssetsLoader::loadTexture(GLuint* outTexture, const char* fileName, JNIEnv* env, GLint format, GLenum type)
//{
//	bool isOK = true;
//	Bitmap load;
//	isOK=load.load(fileName,format,type);
//	if(isOK)
//	{
//		load.genTexture(outTexture);
//	}
//	////java���̊֐����g��
//	//jmethodID loadImageId = env->GetStaticMethodID(sJNICallMethod, "loadImage", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
//
//	//if( loadImageId != NULL )
//	//{
//
//	////Java������Bitmap�N���X���擾
//	//	jstring name = env->NewStringUTF(fileName);
//
//	//	jobject bitmap = env->CallStaticObjectMethod(sJNICallMethod, loadImageId, name );
//
//	//	env->DeleteLocalRef(name);
//
//	////�ǂݍ���Bitmap����摜�T�C�Y�A�s�N�Z���������o��
//	//	AndroidBitmapInfo info;
//	//	int ret;
//	//	if( (ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0 )
//	//	{
//	//		LOGE(TAG, "Failure Get Bitmap Info...");
//	//		isOK = false;
//	//	}
//	//	if( isOK ){
//	//		LOGI(TAG, "width = %d | height = %d", info.width, info.height);
//	//		LOGI(TAG, "format = %d | stride = %d | flag = %d", info.format, info.stride, info.flags);
//
//	//	//Lock�������Ȃ���Bitmap����s�N�Z���������o���Ȃ�
//	//		char* pixels = 0;
//	//		if( (ret = AndroidBitmap_lockPixels(env, bitmap, (void**)&pixels) ) < 0 )
//	//		{
//	//			LOGE(TAG, "Failure Lock Bitmap...");
//	//			isOK = false;
//	//		}
//	//		if( isOK )
//	//		{
//
//	//		//�e�N�X�`��ID�̎擾
//	//			checkGlError(TAG, "unknwon");
//	//			glGenTextures(1, outTexture);
//	//			checkGlError(TAG, "glGenTextures");
//	//			glBindTexture(GL_TEXTURE_2D, *outTexture);
//	//			checkGlError(TAG, "glBindTexture");
//
//	//		//GPU�Ƀs�N�Z���f�[�^��]��
//	//			glTexImage2D( GL_TEXTURE_2D, 0,
//	//					format, info.width, info.height, 0,
//	//					format, type, pixels );
//	//			checkGlError(TAG, "glTexImage2D");
//
//	//		//Unlock��Y�ꂸ��
//	//			AndroidBitmap_unlockPixels(env, bitmap);
//	//		//����I��
//	//			isOK = true;
//	//		}
//	//	}
//	//}
//	//else
//	//{//if( loadImageId == NULL ) �G���[����
//	//	LOGE(TAG, "file:%s | line:%d | Failure get MethodID from JNICallMethod class...", __LINE__, __FILE__);
//	//	return false;
//	//}
//	return isOK;
//}

//�e�N�X�`���̒x���ǂݍ��݂��J�n����
void AssetsLoader::begin()
{
	m_Mutex.lock(1);
	dprintf("AssetsLoader begin");
	//�L���[�ɗ��܂��Ă���e�N�X�`�����폜����
	while(!m_LoadTextureQueue.isEmpty())
	{
		LoadTextureInfo* info;
		m_LoadTextureQueue.get(&info);
		delete info;
	}
	m_LoadTextureQueue.clear();
	m_Loading=true;
}
//�e�N�X�`���̒x���ǂݍ��݂��I������
void AssetsLoader::end()
{

	WARNING(m_Loading,"Not begin loading!!!");
	int sum=0;
	//�L���[�ɗ��܂��Ă���e�N�X�`�����쐬����
	while(!m_LoadTextureQueue.isEmpty())
	{
		LoadTextureInfo* info;
		m_LoadTextureQueue.get(&info);
		info->mp_Bitmap->genTexture(info->m_TexID);
		//�T���v���[�w��
		//���̕������w��ł���悤�ɂ���ׂ�
		glBindTexture(GL_TEXTURE_2D,*info->m_TexID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		delete info;
		
		sum++;
	}
	m_Loading=false;
	dprintf("AssetsLoader end Sum=%d",sum);
	m_Mutex.unLock(1);
}

bool AssetsLoader::loadTexture(GLuint* outTexture, const char* fileName, JNIEnv* env, GLint format, GLenum type)
{
	bool isOK = true;
	LoadTextureInfo* info=new LoadTextureInfo;
	info->m_TexID=outTexture;
	info->mp_Bitmap=new Bitmap;
	isOK=(info->mp_Bitmap->load(fileName,format,type));
	if(isOK)
	{
		m_LoadTextureQueue.put(info);
		//info->mp_Bitmap->genTexture(info->m_TexID);
	}
	//delete info;
	return isOK;
}

bool AssetsLoader::loadBitmap(rlib::Bitmap* outBitmap, const char* fileName, GLint format, GLenum type )
{
	outBitmap = new rlib::Bitmap();
	return outBitmap->load(fileName, format, type);
}
