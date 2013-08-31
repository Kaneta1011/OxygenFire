#ifndef _ML_SOUND_
#define _ML_SOUND_

#include <jni.h>

//---------------------------------------------------------------------------
//		�����p�̃T���v��(8/7 static�֐��֕ύX)
//{
//	Sound::init();	//������
//	Sound::clear();	//�j��
//
//	{//�t�@�C���̓ǂݍ��݂ƍ폜
//		Sound::add(
//			0,	//�ǉ�����ԍ�/*0�`(mlSound::Base::PLAYER_MAX-1)�̊�*/,
//			env,//JNIEnv�N���X�BJNI�̈����ɕK�����Ă�͂��B
//			"sound/bgm1.mp3",	//assets�̃p�X�B��ł�assets�t�H���_�[��sound�t�H���_�[�̒��ɂ���bgm1.mp3��ǂݍ���
//		);
//		Sound::del(0/*�g�p����ԍ�*/);//�폜
//	}
//	{//�Đ��֘A�̃R�[�h
//		Sound::play(  No/*�g�p����ԍ�*/, true/*���[�v�t���O*/);
//		Sound::pause( No/*�g�p����ԍ�*/);//�ꎞ��~
//		Sound::stop(  No/*�g�p����ԍ�*/);//�Đ��I��
//		Sound::volume(No/*�g�p����ԍ�*/, volume/*0�`1�͈̔�*/);
//	}
//}
//---------------------------------------------------------------------------

//
//	�g�p���鉹API�����肵�Ă���
//
#define USE_SOUND_LIB OPENSLES

#if USE_SOUND_LIB == OPENSLES
#include "notUse/OpenSLES.h"
#endif

#include "notUse\mlSoundStream.h"

class Sound
{
public:
	static const int PLAYER_MAX = 20;
public:


	static void init(bool isAsync = false);
	static void clear();

	static bool add(int No, const char* fileName);
	static bool add(int No, JNIEnv* env, jstring fileName);
	static void del(int No);

	static void play(int No, bool isLoop);
	static void stop(int No);
	static void pause(int No);
	static void volume(int No, float per/*per:0�`1�̊�*/);

	static mlSound::StreamingPlayer& streaming(){return mStreaming; }

private:
	static bool mIsAsync;
	static mlSound::Device	mDevice;
	static mlSound::OutputMix mOutputMix;

	static mlSound::Player mPlayer[PLAYER_MAX];

	static mlSound::StreamingPlayer mStreaming;

private:
	Sound(){}
	Sound(Sound&){}
	void operator=(Sound&){}
	~Sound(){}

};

#endif