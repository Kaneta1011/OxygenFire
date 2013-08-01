#ifndef _INNER_ML_SOUND_
#define _INNER_ML_SOUND_

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "../utility/utility.h"
#include <assert.h>
#include <sys/types.h>

class AAssetManager;

namespace mlSound
{
	class IObject;

	class Device
	{
	public:
		static bool isError(SLresult result, const char* errorMsg)
		{
			if( result != SL_RESULT_SUCCESS ){
				LOGE("mlSound",errorMsg);
				return true;
			}
			return false;
		}

		static float			convertToRate(SLmillibel milliBel);
		static SLmillibel	convertToMilliBel(float rate);
	public:
		Device();
		~Device();

		void init(SLboolean isAsync);
		void clear();

	public:
		//Create系
		SLresult createOutputMix(IObject* outMix, SLuint32 interfaceNum, const SLInterfaceID* pIDs, const SLboolean* pReqs, SLboolean isAsync);
		SLresult createAudioPlayer(IObject* outPlayer, SLDataSource* pSrc, SLDataSink* pSink, SLuint32 interfaceNum, const SLInterfaceID* pIDs, const SLboolean* pReqs, SLboolean isAsync);
		SLresult createListener(IObject* outListener, SLuint32 interfaceNum, const SLInterfaceID* pIDs, const SLboolean* pReqs, SLboolean isAsync);
		SLresult createGroup(IObject* outGroup, SLuint32 interfaceNum, const SLInterfaceID* pIDs, const SLboolean* pReqs, SLboolean isAsync);

	private:
	// engine interfaces
	//e = Engine
		SLObjectItf		meObject;
		SLEngineItf		meEngine;
	};

	class IObject
	{
	public:
		IObject():mObject(NULL){}
		virtual ~IObject();

		virtual void clear();

		bool	isOK(){return this->mObject != NULL; }
		SLObjectItf& getObject(){return this->mObject;}
	protected:
		SLObjectItf mObject;
	};

	class OutputMix : public IObject
	{
	public:
		enum INTERFACE_TYPE
		{
			TYPE_OUTPUTMIX,
			TYPE_ENVIRONMENTAL_REVERB,
			TYPE_EQUALIZER,
			TYPE_VIRTUALIZER,
			INTERFACE_TYPE_NUM
		};
	public:
		OutputMix();
		~OutputMix();

		void clear();
		void		onInterface(INTERFACE_TYPE type, SLboolean isON){
			this->req[type] = isON;
		}
		SLboolean	init(Device& device, SLboolean isAsync);

	public:
		SLDataLocator_OutputMix	getDataLocator()const{
			SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, this->mObject};
			return loc_outmix;
		}

		SLboolean	isOnInterface(INTERFACE_TYPE type)
		{
			return this->req[type];
		}

		SLObjectItf&											getObject(){return this->mObject; }
		SLOutputMixItf											getOutputMix(){return this->mOutputMix;}			
		SLEnvironmentalReverbItf						getEnvironmentalReverb(){return this->mEnvReverb;}			
		SLEqualizerItf&										getEqualizer	(){return this->mEqualizer;}									
		SLVirtualizerItf&										getVirtualizer(){return this->mVirtualizer;}

	private:
		void resetRequests();

	private:
		SLboolean req[INTERFACE_TYPE_NUM];
		SLOutputMixItf										mOutputMix;
		SLEnvironmentalReverbItf					mEnvReverb;
		SLEqualizerItf										mEqualizer;
		SLVirtualizerItf										mVirtualizer;
	};

	class Seek
	{
	public:
		//シーカー関連
		enum SEEK_MODE
		{
			SEEK_FAST				= SL_SEEKMODE_FAST,	//速さ優先
			SEEK_ACCURATE		= SL_SEEKMODE_ACCURATE,	//精密モード
		};
	public:
		Seek();
		~Seek();

		void		clear();
		SLboolean	init(SLObjectItf& object);

		SLboolean	setPosition(SLmillisecond posByMillSecond, SEEK_MODE mode);
		SLboolean	setLoop(SLboolean isLoop, SLmillisecond start=0, SLmillisecond end=SL_TIME_UNKNOWN);
		SLboolean	getLoop(SLboolean* outIsLoop, SLmillisecond* outStart, SLmillisecond* outEnd);

	public:
		SLSeekItf	getInterface(){return this->mItf;}
		bool isOK(){return this->mItf!=NULL;}

	private:
		SLSeekItf		mItf;
	};

	class Volume
	{
	public:
		Volume();
		~Volume();

		void		clear();
		SLboolean	init(SLObjectItf& object);

		bool isOK()const{return this->mItf != NULL; }

		//Volumeインターフェース関連
		SLboolean	setVolumeLevel(float rate);
		SLboolean	getVolumeLevel(float* outRate);
		SLboolean	setMute(SLboolean isMute);
		SLboolean	getMute(SLboolean *out);
		SLboolean	EnableStereoPosition(SLboolean isEnable);
		SLboolean	isEnableStereoPosition(SLboolean *out);
		//percentの値は-1000～1000の間
		SLboolean	setStereoPosition(int percent);
		SLboolean	getStereoPosition(int *out);

	public:
		SLVolumeItf	getInterface(){return this->mItf;}

	private:
		SLVolumeItf mItf;
	};

	class EffectSend
	{
	public:
		EffectSend();
		~EffectSend();

		void clear();
		SLboolean init(SLObjectItf& object);

		SLboolean enableEffectSend(const void* effectItf, SLboolean enable, float rate);
		SLboolean isEnableEffectSend(const void* effectItf, SLboolean* outIsEnable);
		SLboolean setDirectLevel(float rate);
		SLboolean getDirectLevel(float* pout);
		SLboolean setSendLevel(const void* effectItf, float rate);
		SLboolean getSendLevel(const void* effectItf, float *pout);

		bool isOK(){return this->mItf != NULL;}

	private:
		SLEffectSendItf	mItf;
	};

	class Player : public IObject
	{
	public:
		enum PLAY_STATE
		{
			STATE_PLAY		= SL_PLAYSTATE_PLAYING,
			STATE_PAUSE		= SL_PLAYSTATE_PAUSED,
			STATE_STOP		= SL_PLAYSTATE_STOPPED,
		};
		//コールバック関数が呼び出されるタイミングを決めるもの
		enum PLAY_EVENT_MASK
		{
			CALL_EVENT_END					= SL_PLAYEVENT_HEADATEND,
			CALL_EVENT_HEAD_AT_MARKER		= SL_PLAYEVENT_HEADATMARKER,
			CALL_EVENT_HEAD_AT_NEW_POS		= SL_PLAYEVENT_HEADATNEWPOS,
			CALL_EVENT_HEAD_BEGIN_MOVING	= SL_PLAYEVENT_HEADMOVING,
			CALL_EVENT_HEAD_STALLED			= SL_PLAYEVENT_HEADSTALLED,
		};

		enum INTERFACE_TYPE
		{
			TYPE_PLAY,
			TYPE_EFFECT_SEND,
			TYPE_SEEK,
			TYPE_VOLUME,
			INTERFACE_TYPE_NUM
		};
	public:
		Player();
		~Player();

		void clear();
		SLboolean load(Device& soundDevice, AAssetManager* pAssetMng, const char* filename, SLint32 assetMode, OutputMix& outputMix, bool isAsync);

	//インターフェイス関連
		void				onInterface(INTERFACE_TYPE type, SLboolean isON){ this->mReq[type] = isON; }
		SLboolean	isOnInterface(INTERFACE_TYPE type){	return this->mReq[type]; }

	public:
		void				play();
		void				pause();
		void				stop();
		void				setLoop(SLboolean isLoop);
		void				setVolume(float rate);

	public:
	//Playインターフェイス関連
		SLboolean		setPlayState(PLAY_STATE state);
		SLboolean		getPlayState(PLAY_STATE* pout);
		SLmillisecond	getDurarion();
		SLmillisecond	getPosition();
		//コールバック関数
		SLboolean		registerCallback(slPlayCallback callback, void* pContext);
		//コールバック関数の呼び出しタイミングの設定
		//「set」の際、引数にはmlSound::Player::PLAY_EVENT_MASK列挙型を使用してください。
		//「get」のときはご自由に
		SLboolean		setCallbackEventMask(SLuint32 mask);
		SLboolean		getCallbackEventMask(SLuint32* pout);
		SLboolean		setMarkerPosition(SLmillisecond millSec);
		SLboolean		getMarkerPosition(SLmillisecond* pout);
		SLboolean		clearMarkerPosition();
		//OepnSLのPlayerは一定の間隔でコールバックイベント以外にコールバック関数を呼び出しているみたい。
		//下のはその一定間隔を操作する関数
		SLboolean		setPositionUpdatePeriod(SLmillisecond millSec);
		SLboolean		getPositionUpdatePeriod(SLmillisecond* pout);

	public:
		Seek&	getSeek(){return this->mSeek;}
		Volume&	getVolume(){return this->mVolume;}
		EffectSend& getEffectSend(){return this->mEffectSend;}

	protected:
		void	resetRequest();

	protected:
		SLboolean	mReq[INTERFACE_TYPE_NUM];
		SLPlayItf		mPlayer;

		Seek				mSeek;
		Volume			mVolume;
		EffectSend	mEffectSend;
	};

	class PCMPlayer : public IObject
	{
	public:
		enum PLAY_STATE
		{
			STATE_PLAY			= SL_PLAYSTATE_PLAYING,
			STATE_PAUSE		= SL_PLAYSTATE_PAUSED,
			STATE_STOP		= SL_PLAYSTATE_STOPPED,
		};
		//コールバック関数が呼び出されるタイミングを決めるもの
		enum PLAY_EVENT_MASK
		{
			CALL_EVENT_END								= SL_PLAYEVENT_HEADATEND,
			CALL_EVENT_HEAD_AT_MARKER		= SL_PLAYEVENT_HEADATMARKER,
			CALL_EVENT_HEAD_AT_NEW_POS	= SL_PLAYEVENT_HEADATNEWPOS,
			CALL_EVENT_HEAD_BEGIN_MOVING	= SL_PLAYEVENT_HEADMOVING,
			CALL_EVENT_HEAD_STALLED			= SL_PLAYEVENT_HEADSTALLED,
		};

		enum CHANNEL_TYPE{
			CHANNEL_MONO,
			CHANNEL_STEREO,
		};

		enum INTERFACE_TYPE
		{
			TYPE_BUFFER_QUEUE,
			TYPE_EFFECT_SEND,
			TYPE_VOLUME,
			INTERFACE_TYPE_NUM
		};
	public:
		PCMPlayer();
		~PCMPlayer();

		void clear();
		SLboolean init(Device& soundDevice, OutputMix& outputMix, CHANNEL_TYPE numChannel, int maxQueue=4, bool isAsync=false);

	//インターフェイス関連
		void		onInterface(INTERFACE_TYPE type, SLboolean isON){ this->mReq[type] = isON; }
		SLboolean	isOnInterface(INTERFACE_TYPE type){	return this->mReq[type]; }

	public:
		void				enqueue(const void* buffer, SLuint32 size);
		void				play();
		void				pause();
		void				stop();
		void				setVolume(float rate);

	public:
	//Playインターフェイス関連
		SLboolean		setPlayState(PLAY_STATE state);
		SLboolean		getPlayState(PLAY_STATE* pout);
		//コールバック関数の呼び出しタイミングの設定
		//「set」の際、引数にはmlSound::Player::PLAY_EVENT_MASK列挙型を使用してください。
		//「get」のときはご自由に
		SLboolean		setCallbackEventMask(SLuint32 mask);
		SLboolean		getCallbackEventMask(SLuint32* pout);

		SLint32				getQueueCount();

	public:
		Volume&		getVolume(){return this->mVolume;}
		EffectSend& getEffectSend(){return this->mEffectSend;}

	protected:
		void	resetRequest();

	protected:
		SLboolean		mReq[INTERFACE_TYPE_NUM];
		SLPlayItf		mPlayer;

		SLAndroidSimpleBufferQueueItf mQueue;

		Volume			mVolume;
		EffectSend		mEffectSend;

		int				mMaxQueue;
	};

	class Location
	{
	public:
		Location();
		~Location();

		void clear();
		SLboolean init(SLObjectItf& object);

		bool isOK(){return this->mItf != NULL;}
		
		SLboolean setPos(float x, float y, float z);
		SLboolean setOrientation(float frontX, float frontY, float frontZ);
		SLboolean setOrientation(float frontX, float frontY, float frontZ, float upX, float upY, float upZ);

	private:
		SL3DLocationItf	mItf;

	};

	class Listener : public IObject
	{
	public:
		Listener();
		~Listener();

		void clear();
		SLboolean init(Device& device, SLboolean isAsync);

	public:
		//よく使うやつ
		SLboolean setPos(float x, float y, float z);
		SLboolean setOrientation(float frontX, float frontY, float frontZ);
		SLboolean setOrientation(float frontX, float frontY, float frontZ, float upX, float upY, float upZ);

	private:
		Location	mLocate;
	};

}
#endif