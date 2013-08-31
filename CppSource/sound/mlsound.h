#ifndef _ML_SOUND_
#define _ML_SOUND_

#include <jni.h>

//---------------------------------------------------------------------------
//		音利用のサンプル(8/7 static関数へ変更)
//{
//	Sound::init();	//初期化
//	Sound::clear();	//破棄
//
//	{//ファイルの読み込みと削除
//		Sound::add(
//			0,	//追加する番号/*0～(mlSound::Base::PLAYER_MAX-1)の間*/,
//			env,//JNIEnvクラス。JNIの引数に必ずついてるはず。
//			"sound/bgm1.mp3",	//assetsのパス。例ではassetsフォルダーのsoundフォルダーの中にあるbgm1.mp3を読み込む
//		);
//		Sound::del(0/*使用する番号*/);//削除
//	}
//	{//再生関連のコード
//		Sound::play(  No/*使用する番号*/, true/*ループフラグ*/);
//		Sound::pause( No/*使用する番号*/);//一時停止
//		Sound::stop(  No/*使用する番号*/);//再生終了
//		Sound::volume(No/*使用する番号*/, volume/*0～1の範囲*/);
//	}
//}
//---------------------------------------------------------------------------

//
//	使用する音APIを決定している
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
	static void volume(int No, float per/*per:0～1の間*/);

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