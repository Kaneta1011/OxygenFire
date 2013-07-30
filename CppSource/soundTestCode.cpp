/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/* This is a JNI example where we use native methods to play sounds
 * using OpenSL ES. See the corresponding Java source file located at:
 *
 *   src/com/example/nativeaudio/OpenAL/OpenAL.java
 */

#include <assert.h>
#include <jni.h>
#include <string.h>

// for __android_log_print(ANDROID_LOG_INFO, "YourApp", "formatted message");
// #include <android/log.h>

// for native audio
#include <SLES/OpenSLES.h>
#include "SLES/OpenSLES_Android.h"

// for native asset manager
#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "mlSound.h"

// pre-recorded sound clips, both are 8 kHz mono 16-bit signed little endian

static const char hello[] =
#include "hello_clip.h"
;

static const char android[] =
#include "android_clip.h"
;

mlSound::Device g_soundBase;
mlSound::OutputMix	g_outputMix;
mlSound::PCMPlayer g_bufferQuere;//未完
mlSound::Player		g_player;

//テスト用の音源
// synthesized sawtooth clip
#define SAWTOOTH_FRAMES 8000
static short sawtoothBuffer[SAWTOOTH_FRAMES];
// synthesize a mono sawtooth wave and place it into a buffer (called automatically on load)
__attribute__((constructor)) static void onDlOpen(void)
{
    unsigned i;
    for (i = 0; i < SAWTOOTH_FRAMES; ++i) {
        sawtoothBuffer[i] = 32768 - ((i % 100) * 660);
    }
}

extern "C" {
    JNIEXPORT void			JNICALL Java_com_example_opensl_OpenSL_initOpenSL(JNIEnv* env, jobject obj);
    JNIEXPORT jint			JNICALL Java_com_example_opensl_OpenSL_createAudioPlayer(JNIEnv* env, jobject obj, jobject assetManager, jstring filename);
    JNIEXPORT jboolean		JNICALL Java_com_example_opensl_OpenSL_setPlayingAudioPlayer(JNIEnv* env, jobject obj, jint id, jboolean isPlaying);
	JNIEXPORT void			JNICALL Java_com_example_opensl_OpenSL_restartAudioPlayer(JNIEnv* env, jobject obj);
	JNIEXPORT void			JNICALL Java_com_example_opensl_OpenSL_setLoop(JNIEnv* env, jobject obj, jboolean isLoop);
	JNIEXPORT void			JNICALL Java_com_example_opensl_OpenSL_setVolume(JNIEnv* env, jobject obj,  jfloat volumeRate);
	JNIEXPORT void			JNICALL Java_com_example_opensl_OpenSL_testBufferQueue(int bufNum);
	JNIEXPORT void			JNICALL Java_com_example_opensl_OpenSL_clearOpenSL(JNIEnv* env, jobject obj);

};

JNIEXPORT void JNICALL Java_com_example_opensl_OpenSL_initOpenSL(JNIEnv* env, jobject obj)
{
	g_soundBase.init(false);
	g_outputMix.init(g_soundBase, SL_BOOLEAN_FALSE);
	g_bufferQuere.init(g_soundBase, g_outputMix, mlSound::PCMPlayer::CHANNEL_MONO );

}

AAssetManager* getAssetManager(JNIEnv* env, jobject assetManager )
{
	AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    assert(NULL != mgr);
	return mgr;
}

JNIEXPORT jint	   JNICALL Java_com_example_opensl_OpenSL_createAudioPlayer(JNIEnv* env, jobject obj, jobject assetManager, jstring filename)
{
	// use asset manager to open asset by filename
	AAssetManager* mgr = getAssetManager(env, assetManager);

	// convert Java string to UTF-8
	const char*utf8 = (env)->GetStringUTFChars(filename, NULL);
	assert(NULL != utf8);
	SLboolean isOK = g_player.load(g_soundBase, mgr, utf8, AASSET_MODE_UNKNOWN, g_outputMix, false);
 
	AAsset* as = AAssetManager_open(mgr, utf8, AASSET_MODE_UNKNOWN);
	off_t size = AAsset_getLength(as);

	// release the Java string and UTF-8
	(env)->ReleaseStringUTFChars(filename, utf8);

	LOGI("読み込み完了");

	return 0;
}

JNIEXPORT jboolean JNICALL Java_com_example_opensl_OpenSL_setPlayingAudioPlayer(JNIEnv* env, jobject obj, jint id, jboolean isPlaying)
{
	if( g_player.isOK() )
	{
		g_player.setPlayState( isPlaying ? g_player.STATE_PLAY : g_player.STATE_PAUSE );
		return JNI_TRUE;
	}
	else
	{
		LOGE("再生する音楽ファイルがありません。ファイルを読み込んでから再生してください。");
		return JNI_FALSE;
	}
}

JNIEXPORT void	JNICALL Java_com_example_opensl_OpenSL_restartAudioPlayer(JNIEnv* env, jobject obj)
{
	if( g_player.isOK() )
	{
		g_player.getSeek().setPosition(0, mlSound::Seek::SEEK_FAST);
	}
}

JNIEXPORT void	JNICALL Java_com_example_opensl_OpenSL_setLoop(JNIEnv* env, jobject obj, jboolean isLoop)
{
	if( g_player.isOK() )
	{
		g_player.getSeek().setLoop( isLoop );
	}
}

JNIEXPORT void	JNICALL Java_com_example_opensl_OpenSL_setVolume(JNIEnv* env, jobject obj,  jfloat volumeRate)
{
	if( g_player.isOK() )
	{
		g_player.getVolume().setVolumeLevel(volumeRate);
	}
}

JNIEXPORT void	JNICALL Java_com_example_opensl_OpenSL_testBufferQueue(int bufNum)
{
	g_bufferQuere.enqueue(sawtoothBuffer, sizeof(sawtoothBuffer));
	g_bufferQuere.enqueue(hello, sizeof(hello));
}

JNIEXPORT void			JNICALL Java_com_example_opensl_OpenSL_clearOpenSL(JNIEnv* env, jobject obj)
{
//クリアー順序はこの順
	g_bufferQuere.clear();
	g_player.clear();
	g_outputMix.clear();
	g_soundBase.clear();
}

