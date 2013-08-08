#ifndef _ML_SOUND_
#define _ML_SOUND_

#include <jni.h>

//
//	égópÇ∑ÇÈâπAPIÇåàíËÇµÇƒÇ¢ÇÈ
//
#define USE_SOUND_LIB OPENSLES

#if USE_SOUND_LIB == OPENSLES
#include "notUse/OpenSLES.h"
#endif

class Sound
{
public:
	static const int PLAYER_MAX = 20;
public:


	static void init(bool isAsync = false);
	static void clear();

	static bool add(int No, JNIEnv* env, jstring fileName);
	static void del(int No);

	static void play(int No, bool isLoop);
	static void stop(int No);
	static void pause(int No);
	static void volume(int No, float per/*per:0Å`1ÇÃä‘*/);

private:
	static bool mIsAsync;
	static mlSound::Device	mDevice;
	static mlSound::OutputMix mOutputMix;

	static mlSound::Player mPlayer[PLAYER_MAX];

private:
	Sound(){}
	Sound(Sound&){}
	void operator=(Sound&){}
	~Sound(){}

};

#endif