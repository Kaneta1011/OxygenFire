#ifndef _ML_SOUND_
#define _ML_SOUND_

#include <jni.h>

//
//	使用する音APIを決定している
//
#define USE_SOUND_LIB OPENSLES

#if USE_SOUND_LIB == OPENSLES
#include "OpenSLES.h"
#endif

//
//		音処理を使いたいならmlSound::Baseを利用してください。
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
		void volume(int No, float per/*per:0～1の間*/);

	private:
		bool mIsAsync;
		Device	mDevice;
		OutputMix mOutputMix;

		Player mPlayer[PLAYER_MAX];
	};
}

#endif