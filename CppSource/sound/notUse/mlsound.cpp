#include "../mlSound.h"

#include <android\asset_manager.h>
#include <android/asset_manager_jni.h>

using namespace mlSound;

#if USE_SOUND_LIB == OPENSLES
//
//		OpenSLES使用バージョンBase
//
Base::Base():
mIsAsync(false)
{
}

Base::~Base()
{
	clear();
}

void Base::init(bool isAsync)
{
	clear();
	this->mIsAsync = isAsync;
	this->mDevice.init(isAsync);
	this->mOutputMix.init(this->mDevice, isAsync);
}

void Base::clear()
{
	for( int i=0; i<PLAYER_MAX; i++ )
	{
		this->mPlayer[i].clear();
	}
}

bool Base::add(int No, JNIEnv* env, jobject assetsManager, jstring fileName, bool isStream)
{
	bool isOK = true;
	assert( 0 <= No && No < PLAYER_MAX );

	Player& player = this->mPlayer[No];

    // use asset manager to open asset by filename
	AAssetManager* mgr = AAssetManager_fromJava(env, assetsManager);
    assert(NULL != mgr);

    // convert Java string to UTF-8
    const char* name = (env)->GetStringUTFChars(fileName, NULL);
    assert(NULL != name);
	isOK = (bool)player.load(this->mDevice, mgr, name, AASSET_MODE_UNKNOWN, this->mOutputMix, this->mIsAsync);

	// release the Java string and UTF-8
    (env)->ReleaseStringUTFChars(fileName, name);

	return isOK;
}

void Base::del(int No)
{
	assert( 0 <= No && No < PLAYER_MAX );
	this->mPlayer[No].clear();
}


void Base::play(int No, bool isLoop)
{
	assert( 0 <= No && No < PLAYER_MAX );
	if( isLoop )
	{
		this->mPlayer[No].setLoop(isLoop);
	}
	this->mPlayer[No].play();
}

void Base::stop(int No)
{
	assert( 0 <= No && No < PLAYER_MAX );
	this->mPlayer[No].stop();
}

void Base::pause(int No)
{
	assert( 0 <= No && No < PLAYER_MAX );
	this->mPlayer[No].pause();
}

void Base::volume(int No, float per/*per:0～1の間*/)
{
	assert( 0 <= No && No < PLAYER_MAX );
	this->mPlayer[No].setVolume(per);
}


#endif