#include "../mlSound.h"

#include <android\asset_manager.h>
#include <android/asset_manager_jni.h>

#include "../../utility/assetsLoader.h"
#include "../../utility/utility.h"

using namespace mlSound;

#if USE_SOUND_LIB == OPENSLES
//
//		OpenSLES使用バージョンSound
//

static char* TAG = "mlSound";
bool					Sound::mIsAsync = false;
mlSound::Device			Sound::mDevice;
mlSound::OutputMix		Sound::mOutputMix;
mlSound::Player			Sound::mPlayer[Sound::PLAYER_MAX];

void Sound::init(bool isAsync)
{
	clear();
	mIsAsync = isAsync;
	mDevice.init(isAsync);
	mOutputMix.init(mDevice, isAsync);
	LOGI(TAG,"Complete init");
}

void Sound::clear()
{
	for( int i=0; i<PLAYER_MAX; i++ )
	{
		mPlayer[i].clear();
	}
	mOutputMix.clear();
	mDevice.clear();
	LOGI(TAG,"Complete init");
}

bool Sound::add(int No, JNIEnv* env, jstring fileName)
{
	bool isOK = true;
	assert( 0 <= No && No < PLAYER_MAX );

	Player& player = mPlayer[No];

    // use asset manager to open asset by filename
	AAssetManager* mgr = AssetsLoader::getAssetManager();
    assert(NULL != mgr);

    // convert Java string to UTF-8
    const char* name = (env)->GetStringUTFChars(fileName, NULL);
    assert(NULL != name);
	isOK = (bool)player.load(mDevice, mgr, name, AASSET_MODE_UNKNOWN, mOutputMix, mIsAsync);

	// release the Java string and UTF-8
    (env)->ReleaseStringUTFChars(fileName, name);

	return isOK;
}

void Sound::del(int No)
{
	assert( 0 <= No && No < PLAYER_MAX );
	mPlayer[No].clear();
}


void Sound::play(int No, bool isLoop)
{
	assert( 0 <= No && No < PLAYER_MAX );
	if( isLoop )
	{
		mPlayer[No].setLoop(isLoop);
	}
	mPlayer[No].play();
}

void Sound::stop(int No)
{
	assert( 0 <= No && No < PLAYER_MAX );
	mPlayer[No].stop();
}

void Sound::pause(int No)
{
	assert( 0 <= No && No < PLAYER_MAX );
	mPlayer[No].pause();
}

void Sound::volume(int No, float per/*per:0～1の間*/)
{
	assert( 0 <= No && No < PLAYER_MAX );
	mPlayer[No].setVolume(per);
}


#endif