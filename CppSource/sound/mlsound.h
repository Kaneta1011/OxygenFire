#ifndef _ML_SOUND_
#define _ML_SOUND_

#include <jni.h>

//
//	�g�p���鉹API�����肵�Ă���
//
#define USE_SOUND_LIB OPENSLES

#if USE_SOUND_LIB == OPENSLES
#include "OpenSLES.h"
#endif

//
//		���������g�������Ȃ�mlSound::Base�𗘗p���Ă��������B
//
namespace mlSound
{
	class Base
	{
	public:
		static const int PLAYER_MAX = 20;
	public:
		Base();
		~Base();

		void init(bool isAsync = false);
		void clear();

		bool add(int No, JNIEnv* env, jobject assetsManager, jstring fileName, bool isStream=false);
		void del(int No);

		void play(int No, bool isLoop);
		void stop(int No);
		void pause(int No);
		void volume(int No, float per/*per:0�`1�̊�*/);

	private:
		bool mIsAsync;
		Device	mDevice;
		OutputMix mOutputMix;

		Player mPlayer[PLAYER_MAX];
	};
}

#endif