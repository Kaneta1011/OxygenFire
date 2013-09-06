#ifndef _ASSETS_LOADER_
#define _ASSETS_LOADER_

//=======================================================================================================
//
//	Assets����t�@�C����ǂݍ��ރN���X
//	�@�\�F	�}���`�X���b�h��ł����p�\
//		�E�p�X�w�肩��̃t�@�C���ǂݍ���
//		�E�e�N�X�`���̓ǂݍ���	//JNIEnv�N���X���K�v	���̃R�����g���Ă�ł�������
//
//****************************************************************************************
//	################################################################
//	#### �e�N�X�`����ǂݍ��ޏꍇ��JNIEnv�N���X���K�v�ɂȂ�܂� ####
//	################################################################
//		�e�N�X�`���̓ǂݍ��ݎ���Java���̊֐��𗘗p���Ă��邽�߁AJNIEnv�N���X���K�v�ɂȂ�܂�
//
//		JNIEnv�N���X�͑�́AJNI�֐��̈����ɂ��邩�Autility.h�Ő錾���Ă���JavaVM* g_VM�̃����o�֐�GetEnv()����Ƃ��Ă��܂�
//		JNIEnv�N���X���g���Ƃ��ɂȂ�����A���̃R�����g���ڂ�ʂ��Ă�������
//
//****************************************************************************************
//=======================================================================================================
//
//	�EJNIEnv���K�v�ɂȂ����Ƃ��������玝���Ă���Ȃ��Ƃ��A���̃R�[�h�Q�l�ɂ��Ă�������
//	��ɁA�X���b�h�������Ă���Ƃ���Assets����t�@�C����ǂݍ��ނƂ��ɕK�v�ɂȂ�Ǝv���܂�
//
//--------------------------------------------------------------------------------------------
//	#include "utility.h"
//
//	JNIEnv* env;
//	getJNIEnv(&env);//���| utility.h�Ő錾���Ă���
//
//--------------------------------------------------------------------------------------------
//
//	�Q�lHP�@�F�@http://blog.nekobako.net/post/36/�@�������@�Ƃ������o���̉�������ɃR�[�h����
//	���ƁA�c��N�́uAndroidNDK�v���O���~���O�K�C�h�v�Ƃ����{��p.57�`p.61���Q�l�ɂȂ�܂���
//
//=======================================================================================================
//****************************************************************************************************
//							�T���v��
//****************************************************************************************************
//----------------------------------------------------------------------------------------------------
//					�t�@�C���ǂݍ��݂̃T���v��
//----------------------------------------------------------------------------------------------------
//	char* data;
//	int size;
//	const char* fileName = "test.txt";
//	AssetsLoader::load(&data, &size, fileName);
//
//=======================================================================================================
//----------------------------------------------------------------------------------------------------
//					�e�N�X�`���ǂݍ��݂̃T���v��
//----------------------------------------------------------------------------------------------------
//	JNIEnv* env;		//�����܂���JavaVM����
//	GLuint gTexture;
//	const char* fileName = "testImage.png";
//	AssetsLoader::loadTexture(&gTexture, fileName, env);
//
//	//	�T���v���[�̐ݒ�͊e���ݒ肵�Ă�������
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
	Assets�ɂ���摜����e�N�X�`����ǂݍ��ފ֐�
	����glTexImage2D���Ă�ł��܂�
	�T���v���[�Ȃǂ̃e�N�X�`���̐ݒ�͗��p�����s���Ă�������
	*/
	static bool loadTexture(GLuint* outTexture, const char* fileName, JNIEnv* env, GLint format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);

	/*
	Bitmap�ɓf���o����
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