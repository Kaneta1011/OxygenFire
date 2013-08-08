#include "OpenSLES.h"

#include <jni.h>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <android\asset_manager.h>
#include <android/asset_manager_jni.h>

using namespace mlSound;

#ifndef M
#define M(o) (*o)
#endif

#ifndef NULL
#define NULL 0
#endif

//===================================================================
//	SLVec3Dの数学関数
//===================================================================
inline void mult(SLVec3D* vec, int value)
{
	vec->x *= value;	vec->y *= value; vec->z *= value;
}

inline SLint32 dot(const SLVec3D& a, const SLVec3D& b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline SLVec3D cross(const SLVec3D& a, const SLVec3D& b)
{
	SLVec3D r;
	r.x = a.y*b.z - a.z*b.y;
	r.y = a.z*b.x - a.x*b.z;
	r.z = a.x*b.y - a.y*b.x;
	return r;
}

inline void normalize(SLVec3D* vec)
{
	float len = sqrt( dot(*vec, *vec) );
	vec->x /= len; vec->y /= len; vec->z /= len;
	mult(vec, 1000);
}

//===========================================================
//		単位変換
//===========================================================
float			Device::convertToRate(SLmillibel milliBel)
{
	float rate = 1.f - milliBel / SL_MILLIBEL_MIN;
	return rate;
}

/**
 * @brief 音量（0-1）をミリベルへ変換する
 */
SLmillibel	Device::convertToMilliBel(float rate)
{
  const float min_limit = 0.1f;
  const float vol = std::min(std::max(1.f - rate, 0.f), 1.f);
  SLint16 volume_level = SL_MILLIBEL_MIN;
  if (rate > min_limit)
  {
    const static float base2 = log(2);
    const float antilogarithm = log(1.f / vol);
    const float bell = base2 / antilogarithm;
    volume_level = static_cast<SLint16>(bell * -1000.f);
  }
  return volume_level;
}

//===========================================================
//
//	Device
//
//===========================================================
Device::Device():
	meObject(NULL),
	meEngine(NULL)
{
}

Device::~Device()
{
	clear();
}

void Device::init(SLboolean isAsync)
{
    SLresult result;

    // create engine
    result = slCreateEngine(&this->meObject, 0, NULL, 0, NULL, NULL);
	assert( ! isError(result, "Failure create Engine Object...") );
	// realize the engine
    result = (*this->meObject)->Realize(this->meObject, isAsync);
	assert( ! isError(result, "Failure realize Engine Object...") );

    // get the engine interface, which is needed in order to create other objects
    result = (*this->meObject)->GetInterface(this->meObject, SL_IID_ENGINE, &this->meEngine);
	assert( ! isError(result, "Failure get Engine Interface...") );
}

void Device::clear()
{
    // destroy engine object, and invalidate all associated interfaces
    if (this->meObject != NULL) {
        (*this->meObject)->Destroy(this->meObject);
        this->meObject = NULL;
        this->meEngine = NULL;
    }
}

//Create系
SLresult Device::createOutputMix(IObject* outMix, SLuint32 interfaceNum, const SLInterfaceID* pIDs, const SLboolean* pReqs, SLboolean isAsync)
{
	SLresult result = M(this->meEngine)->CreateOutputMix(this->meEngine, &outMix->getObject(), interfaceNum, pIDs, pReqs );
	assert( ! isError(result, "Failure create OutputMix...") );
	result = M(outMix->getObject())->Realize(outMix->getObject(), isAsync );
	assert( ! isError(result, "Failure realized OutputMix...") );
	return result;
}

SLresult Device::createAudioPlayer(IObject* outPlayer, SLDataSource* pSrc, SLDataSink* pSink, SLuint32 interfaceNum, const SLInterfaceID* pIDs, const SLboolean* pReqs, SLboolean isAsync)
{
	SLresult result = M(this->meEngine)->CreateAudioPlayer(this->meEngine, &outPlayer->getObject(), pSrc, pSink, interfaceNum, pIDs, pReqs );
	assert( ! isError(result, "Failure create AudioPlayer...") );
	result = M(outPlayer->getObject())->Realize(outPlayer->getObject(), isAsync);
	assert( ! isError(result, "Failure realized AudioPlayer...") );
	return result;
}

SLresult Device::createListener(IObject* outListener, SLuint32 interfaceNum, const SLInterfaceID* pIDs, const SLboolean* pReqs, SLboolean isAsync)
{
	SLresult result = M(this->meEngine)->CreateListener(this->meEngine, &outListener->getObject(), interfaceNum, pIDs, pReqs );
	assert( ! isError(result, "Failure create Listener...") );
	result = M(outListener->getObject())->Realize(outListener->getObject(), isAsync);
	assert( ! isError(result, "Failure realized Listener...") );
	return result;
}

SLresult Device::createGroup(IObject* outGroup, SLuint32 interfaceNum, const SLInterfaceID* pIDs, const SLboolean* pReqs, SLboolean isAsync)
{
	SLresult result = M(this->meEngine)->CreateListener(this->meEngine, &outGroup->getObject(), interfaceNum, pIDs, pReqs );
	assert( ! isError(result, "Failure create 3DGroup...") );
	result = M(outGroup->getObject())->Realize(outGroup->getObject(), isAsync);
	assert( ! isError(result, "Failure realized 3DGroup...") );
	return result;
}

//=====================================================
//
//			IObject
//
//=====================================================
IObject::~IObject()
{
	clear();
}

void IObject::clear()
{
	if( isOK() )
	{
		M(this->mObject)->Destroy(this->mObject);
		this->mObject = NULL;
	}
}

//=====================================================
//
//			OutputMix
//
//=====================================================
OutputMix::OutputMix():
	mOutputMix(NULL),
	mEnvReverb(NULL),
	mEqualizer(NULL),
	mVirtualizer(NULL)
{
	resetRequests();
}

OutputMix::~OutputMix()
{
	clear();
}

void OutputMix::clear()
{
	if( this->mObject != NULL )
	{
		IObject::clear();
		this->mOutputMix = NULL;
		this->mEnvReverb = NULL;
		this->mEqualizer = NULL;
		this->mVirtualizer = NULL;
	}
	resetRequests();

}

void OutputMix::resetRequests()
{
	req[0] = SL_BOOLEAN_TRUE;
	for( int i=1; i<INTERFACE_TYPE_NUM; i++ )
	{
		req[i] = SL_BOOLEAN_FALSE;
	}
}

SLboolean	OutputMix::init(Device& device, SLboolean isAsync)
{
	const SLInterfaceID ids[] = {
		SL_IID_OUTPUTMIX,
	};
	int num = sizeof(ids)/sizeof(ids[0]);

	SLresult result = device.createOutputMix(this, num, ids, this->req, isAsync);
	assert( !Device::isError(result, "Failure initialize mlSound::OutputMix...") ) ;
//各インターフェイスの設定
	assert( !Device::isError(M(this->mObject)->GetInterface(this->mObject, ids[0], &this->mOutputMix),		"Failure Get Interface OutputMix..") );
	return SL_BOOLEAN_TRUE;
}

//===========================================================
//
//		Playerクラス
//
//===========================================================
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

Player::Player():
mPlayer(NULL)
{
}

Player::~Player()
{
	clear();
}

void Player::clear()
{
	if( mObject != NULL )
	{
		setPlayState(STATE_PAUSE);
		IObject::clear();
		this->mPlayer = NULL;
		this->mSeek.clear();
		this->mVolume.clear();
	}
}

void	Player::resetRequest()
{
	for( int i=0; i<INTERFACE_TYPE_NUM; i++ )
	{
		this->mReq[i] = SL_BOOLEAN_FALSE;
	}
	//onInterface(TYPE_PLAY, SL_BOOLEAN_TRUE);
	//onInterface(TYPE_SEEK, SL_BOOLEAN_TRUE);
	//onInterface(TYPE_VOLUME, SL_BOOLEAN_TRUE);
	//onInterface(TYPE_EFFECT_SEND, SL_BOOLEAN_TRUE);
}

SLboolean Player::load(Device& soundBase, AAssetManager* pAssetMng, const char* filename, SLint32 assetMode, OutputMix& outputMix, bool isAsync)
{
	clear();
//pAssetMngからデータを読み込む
	int fd;
	off_t start, length;
	{
		AAsset* asset = AAssetManager_open(pAssetMng, filename, assetMode);
	// the asset might not be found
		if (NULL == asset) {
			LOGI("Failure open asset... : filename:\"%s\"", filename);
			return JNI_FALSE;
		}
	// open asset as file descriptor
		fd = AAsset_openFileDescriptor(asset, &start, &length);
		assert(0 <= fd);
		AAsset_close(asset);
	}
	{//ベースとなるオブジェクトを作成
		// configure audio source
		SLDataLocator_AndroidFD loc_fd = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
		SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
		SLDataSource audioSrc = {&loc_fd, &format_mime};
		// configure audio sink
		SLDataLocator_OutputMix loc_outmix = outputMix.getDataLocator();
		SLDataSink audioSnk = {&loc_outmix, NULL};
		// create audio player
		const SLInterfaceID ids[] = {
			SL_IID_PLAY,
			SL_IID_EFFECTSEND,
			SL_IID_SEEK,
			SL_IID_VOLUME,
		};
		int num = sizeof(ids)/sizeof(ids[0]);
		soundBase.createAudioPlayer(this, &audioSrc, &audioSnk, num, ids, mReq, isAsync );
	}
	{//各インターフェイスの取得
		assert( ! Device::isError(M(this->mObject)->GetInterface(this->mObject, SL_IID_PLAY, &this->mPlayer), "Failure get interface SLPlay...") );
		this->mSeek.init(this->mObject);
		this->mVolume.init(this->mObject);
		this->mEffectSend.init(this->mObject);

		this->stop();
		this->mSeek.setLoop(SL_BOOLEAN_FALSE);
	}
	LOGI("mlSound", "Complete Player::load.");
	return SL_BOOLEAN_TRUE;
}

//========================================
//		よく使う機能
//========================================
void		Player::play()
{
	setPlayState(STATE_PLAY);
}

void		Player::pause()
{
	setPlayState(STATE_PAUSE);
}

void		Player::stop()
{
	setPlayState(STATE_STOP);
}

void		Player::setLoop(SLboolean isLoop)
{
	this->mSeek.setLoop(isLoop);
}

void		Player::setVolume(float rate)
{
	this->mVolume.setVolumeLevel(rate);
}

//========================================
//		Playインターフェイス関連
//========================================
SLboolean		Player::setPlayState(PLAY_STATE state)
{
    if ( isOK()) {
        // set the player's state
		SLresult result = M(this->mPlayer)->SetPlayState(this->mPlayer, state );
		assert( !Device::isError(result, "Failure SetPlayState..."));
		return true;
    }
	return false;
}

SLboolean		Player::getPlayState(PLAY_STATE* pout)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mPlayer)->GetPlayState(this->mPlayer, (SLuint32*)pout);
		assert(!Device::isError(result, "Failure GetPlayState..."));
		if( result == SL_RESULT_SUCCESS )
		{
			is = SL_BOOLEAN_TRUE;			
		}
	}
	return is;
}

SLmillisecond	Player::getDurarion()
{
	SLmillisecond out = -1;
	if( isOK() )
	{
		SLresult result = M(this->mPlayer)->GetDuration(this->mPlayer, &out);
		assert( !Device::isError(result, "Failure GetDurartion..."));
	}
	return out;
}

//-----------------------------------------------------------
//		現在の再生位置の取得
//
SLmillisecond	Player::getPosition()
{
	SLmillisecond out = -1;
	if( isOK() )
	{
		SLresult result = M(this->mPlayer)->GetPosition(this->mPlayer, &out);
		assert( !Device::isError(result, "Failure GetPosition..."));
	}
	return out;
}

//------------------------------------------------------
//		Callback関数の設定
//
SLboolean		Player::registerCallback(slPlayCallback callback, void* pContext)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mPlayer)->RegisterCallback(this->mPlayer, callback, pContext);
		assert( !Device::isError(result, "Failure RegisterCallback..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;			
	}
	return is;
}
//--------------------------------------------------------
//		コールバックイベントのマスク関連
//
SLboolean		Player::setCallbackEventMask(SLuint32 mask)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mPlayer)->SetCallbackEventsMask(this->mPlayer, mask);
		assert( !Device::isError(result, "Failure SetCallbackEventMask..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean		Player::getCallbackEventMask(SLuint32* pout)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mPlayer)->GetCallbackEventsMask(this->mPlayer, pout);
		assert( !Device::isError(result, "Failure GetCallbackEventMask..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean		Player::setMarkerPosition(SLmillisecond millSec)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mPlayer)->SetMarkerPosition(this->mPlayer, millSec);
		assert( !Device::isError(result, "Failure SetMarkerPosition..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

//--------------------------------------------------------
//		マーカー関連
//
SLboolean		Player::getMarkerPosition(SLmillisecond* pout)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mPlayer)->GetMarkerPosition(this->mPlayer, pout);
		assert( !Device::isError(result, "Failure GetMarkerPosition..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean		Player::clearMarkerPosition()
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mPlayer)->ClearMarkerPosition(this->mPlayer);
		assert( !Device::isError(result, "Failure ClearMarkerPosition..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

//------------------------------------------------------------
//			再生位置を通知する期間
//
SLboolean		Player::setPositionUpdatePeriod(SLmillisecond millSec)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mPlayer)->SetPositionUpdatePeriod(this->mPlayer, millSec);
		assert( !Device::isError(result, "Failure SetPositionUpdatePeriod..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean		Player::getPositionUpdatePeriod(SLmillisecond* pout)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mPlayer)->GetPositionUpdatePeriod(this->mPlayer, pout);
		assert( !Device::isError(result, "Failure GetPositionUpdatePeriod..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

//======================================================================
//
//		Seek
//
//======================================================================
Seek::Seek():
mItf(NULL)
{
}

Seek::~Seek()
{
	clear();
}

void		Seek::clear()
{
	this->mItf = NULL;
}

SLboolean	Seek::init(SLObjectItf& object)
{
	clear();
	SLboolean is;
	assert( SL_BOOLEAN_TRUE == (is = !(SLboolean)Device::isError(M(object)->GetInterface(object, SL_IID_SEEK, &this->mItf), "Failure init Seek...") ) );
	return is;
}

SLboolean	Seek::setPosition(SLmillisecond posByMillSecond, SEEK_MODE mode )
{
	SLboolean is = SL_BOOLEAN_FALSE;
	// enable whole file looping
	if( isOK() )
	{
		SLresult result = M(this->mItf)->SetPosition(this->mItf, posByMillSecond, (SLuint32)mode);
		assert( ! Device::isError(result, "Failure SetLoop...") );
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean Seek::setLoop(SLboolean isLoop, SLmillisecond start, SLmillisecond end)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	// enable whole file looping
	if( isOK() )
	{
		SLresult result = M(this->mItf)->SetLoop(this->mItf, isLoop, start, end);
		assert( ! Device::isError(result, "Failure SetLoop...") );
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean	Seek::getLoop(SLboolean* outIsLoop, SLmillisecond* outStart, SLmillisecond* outEnd)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mItf)->GetLoop(this->mItf, outIsLoop, outStart, outEnd);
		assert( !Device::isError(result, "Failure getLoop..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

//======================================================================
//
//		Volume
//
//======================================================================
Volume::Volume():
mItf(NULL)
{
}

Volume::~Volume()
{
	clear();
}

void		Volume::clear()
{
	this->mItf = NULL;
}

SLboolean	Volume::init(SLObjectItf& object)
{
	clear();
	SLboolean is;
	assert( SL_BOOLEAN_TRUE == (is = !(SLboolean)Device::isError(M(object)->GetInterface(object, SL_IID_VOLUME, &this->mItf), "Failure init Volume...") ) );
	return is;
}

SLboolean	Volume::setVolumeLevel(float rate)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLmillibel milliBel = Device::convertToMilliBel(rate);
		SLresult result = M(this->mItf)->SetVolumeLevel(this->mItf,milliBel);
		assert( !Device::isError(result, "Failure GetVolumeLevel..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean	Volume::getVolumeLevel(float* out)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLmillibel milliBel;
		SLresult result = M(this->mItf)->GetVolumeLevel(this->mItf,&milliBel);
		assert( !Device::isError(result, "Failure GetVolumeLevel..."));
		if( result == SL_RESULT_SUCCESS )
		{
			is = SL_BOOLEAN_TRUE;
			*out = Device::convertToRate(milliBel);
		}
	}
	return is;
}

SLboolean	Volume::setMute(SLboolean isMute)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mItf)->SetMute(this->mItf,isMute);
		assert( !Device::isError(result, "Failure SetMute..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean	Volume::getMute(SLboolean *out)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mItf)->GetMute(this->mItf,out);
		assert( !Device::isError(result, "Failure GetMute..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean	Volume::EnableStereoPosition(SLboolean isEnable)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mItf)->EnableStereoPosition(this->mItf,isEnable);
		assert( !Device::isError(result, "Failure EnabledStereoPosition..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean	Volume::isEnableStereoPosition(SLboolean *out)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mItf)->IsEnabledStereoPosition(this->mItf,out);
		assert( !Device::isError(result, "Failure getEnableSteroPosition..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

//percentの値は-1000～1000の間
SLboolean	Volume::setStereoPosition(int percent)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mItf)->SetStereoPosition(this->mItf,percent);
		assert( !Device::isError(result, "Failure setStereoPosition..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean	Volume::getStereoPosition(int *out)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLpermille permille;
		SLresult result = M(this->mItf)->GetStereoPosition(this->mItf,&permille);
		assert( !Device::isError(result, "Failure getStereoPosition..."));
		if( result == SL_RESULT_SUCCESS ){
			is = SL_BOOLEAN_TRUE;
			*out = permille;
		}
	}
	return is;
}

//======================================================================
//
//		Seek
//
//======================================================================
EffectSend::EffectSend():
mItf(NULL)
{
}

EffectSend::~EffectSend()
{
	clear();
}

void		EffectSend::clear()
{
	this->mItf = NULL;
}

SLboolean	EffectSend::init(SLObjectItf& object)
{
	clear();
	SLboolean is;
	assert( SL_BOOLEAN_TRUE == (is = !(SLboolean)Device::isError(M(object)->GetInterface(object, SL_IID_EFFECTSEND, &this->mItf), "Failure init EffectSend...") ) );
	return is;
}

SLboolean EffectSend::enableEffectSend(const void* effectItf, SLboolean enable, float rate)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLmillibel millibel = Device::convertToMilliBel(rate);
		SLresult result = M(this->mItf)->EnableEffectSend(this->mItf,effectItf, enable, millibel);
		assert( !Device::isError(result, "Failure enableEffectSend..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean EffectSend::isEnableEffectSend(const void* effectItf, SLboolean* outIsEnable)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mItf)->IsEnabled(this->mItf,effectItf, outIsEnable);
		assert( !Device::isError(result, "Failure isEnableEffectSend..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean EffectSend::setDirectLevel(float rate)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLmillibel millibel = Device::convertToMilliBel(rate);
		SLresult result = M(this->mItf)->SetDirectLevel(this->mItf,millibel);
		assert( !Device::isError(result, "Failure setDirectLevel..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean EffectSend::getDirectLevel(float* pout)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLmillibel millibel;
		SLresult result = M(this->mItf)->GetDirectLevel(this->mItf,&millibel);
		assert( !Device::isError(result, "Failure getDirectLevel..."));
		if( result == SL_RESULT_SUCCESS )
		{
			is = SL_BOOLEAN_TRUE;
			*pout = Device::convertToRate(millibel);
		}
	}
	return is;
}

SLboolean EffectSend::setSendLevel(const void* effectItf, float rate)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLmillibel millibel = Device::convertToMilliBel(rate);
		SLresult result = M(this->mItf)->SetSendLevel(this->mItf,effectItf,millibel);
		assert(!Device::isError(result, "Failure setSendLevel..."));
		if( result == SL_RESULT_SUCCESS ){
			is = SL_BOOLEAN_TRUE;
		}
	}
	return is;
}

SLboolean EffectSend::getSendLevel(const void* effectItf, float *pout)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLmillibel millibel;
		SLresult result = M(this->mItf)->GetSendLevel(this->mItf,effectItf,&millibel);
		assert( !Device::isError(result, "Failure getSendLevel...") );
		if( result == SL_RESULT_SUCCESS ){
			is = SL_BOOLEAN_TRUE;
			*pout = Device::convertToRate(millibel);
		}
	}
	return is;
}

//===========================================================
//
//		PCMPlayerクラス
//
//===========================================================
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

PCMPlayer::PCMPlayer():
mPlayer(NULL),
mMaxQueue(0)
{
}

PCMPlayer::~PCMPlayer()
{
	clear();
}

void PCMPlayer::clear()
{
	if( mObject != NULL )
	{
		setPlayState(STATE_PAUSE);
		M(this->mQueue)->Clear(this->mQueue);
		M(this->mObject)->Destroy(this->mObject);
		this->mObject = NULL;
		this->mPlayer = NULL;
		this->mVolume.clear();
		this->mMaxQueue = 0;
	}
}

void	PCMPlayer::resetRequest()
{
	for( int i=0; i<INTERFACE_TYPE_NUM; i++ )
	{
		this->mReq[i] = SL_BOOLEAN_TRUE;
	}
}

//=====================================
//		基本コールバック関数
//=====================================
// this callback handler is called every time a buffer finishes playing
void PCMPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
//とりあえず空のコールバック関数にしておく
	LOGI("mlSound", "passage buffer queue callback.");
}

SLboolean PCMPlayer::init(Device& soundBase, OutputMix& outputMix, CHANNEL_TYPE numChannel, int maxQueue, bool isAsync)
{
	clear();
	{//ベースとなるオブジェクトを作成
		// configure audio source
		this->mMaxQueue = maxQueue;
		SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {
			SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
			this->mMaxQueue
		};

		SLDataFormat_PCM pcmFormat;
		if( numChannel == CHANNEL_MONO ){
			pcmFormat.numChannels = 1;
			pcmFormat.channelMask = SL_SPEAKER_FRONT_CENTER;
		}
		else if( numChannel == CHANNEL_STEREO )
		{
			pcmFormat.numChannels = 2;
			pcmFormat.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
		}

		pcmFormat.formatType = SL_DATAFORMAT_PCM;
		pcmFormat.samplesPerSec = SL_SAMPLINGRATE_8;
		pcmFormat.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
		pcmFormat.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
		pcmFormat.endianness = SL_BYTEORDER_LITTLEENDIAN;

		SLDataSource audioSrc = {&loc_bufq, &pcmFormat};

		// configure audio sink
		SLDataLocator_OutputMix loc_outmix = outputMix.getDataLocator();
		SLDataSink audioSnk = {&loc_outmix, NULL};

		// create audio player
		const SLInterfaceID ids[] = {
			SL_IID_BUFFERQUEUE,
			SL_IID_EFFECTSEND,
			SL_IID_VOLUME,
		};
		int num = sizeof(ids)/sizeof(ids[0]);
		soundBase.createAudioPlayer(this, &audioSrc, &audioSnk, num, ids, mReq, isAsync );
	}
	{//各インターフェイスの取得
		assert( ! Device::isError(M(this->mObject)->GetInterface(this->mObject, SL_IID_PLAY, &this->mPlayer), "Failure get interface SLPlay at PCMPlayer...") );
		assert( ! Device::isError(M(this->mObject)->GetInterface(this->mObject, SL_IID_BUFFERQUEUE, &this->mQueue), "Failure get interface SLBufferQueue at PCMPlayer...") );
		this->mVolume.init(this->mObject);
		this->mEffectSend.init(this->mObject);
		//this->mGroup.init(this->mObject);

		M(this->mQueue)->RegisterCallback(this->mQueue, PCMPlayerCallback, NULL);
		play();
	}
	LOGI("mlSound", "Complete PCMPlayer::init.");
	return SL_BOOLEAN_TRUE;
}

//========================================
//		よく使う機能
//========================================
void		PCMPlayer::enqueue(const void* buffer, SLuint32 size)
{
	if( isOK() )
	{
		if( getQueueCount() < this->mMaxQueue )
		{
			SLresult result = M(this->mQueue)->Enqueue(this->mQueue, buffer, size);
			assert( !Device::isError(result, "Failure enqueue..."));
		}
		else
		{
			LOGI("mlSound", "Failure enqueue because over queue max count. | max count = %d", this->mMaxQueue);
		}
	}
}

void		PCMPlayer::play()
{
	setPlayState(STATE_PLAY);
}

void		PCMPlayer::pause()
{
	setPlayState(STATE_PAUSE);
}

void		PCMPlayer::stop()
{
	setPlayState(STATE_STOP);
}

void		PCMPlayer::setVolume(float rate)
{
	this->mVolume.setVolumeLevel(rate);
}

//========================================
//		Playインターフェイス関連
//========================================
SLboolean		PCMPlayer::setPlayState(PLAY_STATE state)
{
    if ( isOK()) {
        // set the player's state
		SLresult result = M(this->mPlayer)->SetPlayState(this->mPlayer, state );
		assert( !Device::isError(result, "Failure SetPlayState..."));
		return true;
    }
	return false;
}

SLboolean		PCMPlayer::getPlayState(PLAY_STATE* pout)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mPlayer)->GetPlayState(this->mPlayer, (SLuint32*)pout);
		assert(!Device::isError(result, "Failure GetPlayState..."));
		if( result == SL_RESULT_SUCCESS )
		{
			is = SL_BOOLEAN_TRUE;			
		}
	}
	return is;
}
//--------------------------------------------------------
//		コールバックイベントのマスク関連
//
SLboolean		PCMPlayer::setCallbackEventMask(SLuint32 mask)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mPlayer)->SetCallbackEventsMask(this->mPlayer, mask);
		assert( !Device::isError(result, "Failure SetCallbackEventMask..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean		PCMPlayer::getCallbackEventMask(SLuint32* pout)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLresult result = M(this->mPlayer)->GetCallbackEventsMask(this->mPlayer, pout);
		assert( !Device::isError(result, "Failure GetCallbackEventMask..."));
		if( result == SL_RESULT_SUCCESS )
			is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLint32				PCMPlayer::getQueueCount()
{
	if( isOK() )
	{
		SLAndroidSimpleBufferQueueState state;
		SLresult result = M(this->mQueue)->GetState(this->mQueue, &state);
		assert( !Device::isError(result, "Failure getQueueCount..."));
		return state.count;
	}
	return -1;
}

//===========================================================
//
//		Locationクラス
//
//===========================================================
Location::Location():
mItf(NULL)
{

}

Location::~Location()
{
	clear();
}

void Location::clear()
{
	this->mItf = NULL;
}

SLboolean Location::init(SLObjectItf& object)
{
	clear();
	SLboolean is;
	assert( SL_BOOLEAN_TRUE == (is = !(SLboolean)Device::isError(M(object)->GetInterface(object, SL_IID_3DLOCATION, &this->mItf), "Failure init 3DLocation...") ) );
	return is;
}

SLboolean Location::setPos(float x, float y, float z)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{
		SLVec3D pos={x*1000,y*1000,z*1000};//ミリメートル単位
		M(this->mItf)->SetLocationCartesian(this->mItf, &pos );
		is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean Location::setOrientation(float frontX, float frontY, float frontZ)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{//ミリメートル単位
		SLVec3D front={frontX*1000,frontY*1000,frontZ*1000};
		SLVec3D up={0,999,1};
		SLVec3D right = cross(up, front);
		SLVec3D UP = cross( front, right );

		normalize(&front);
		normalize(&UP);
		M(this->mItf)->SetOrientationVectors(this->mItf, &front, &UP );
		is = SL_BOOLEAN_TRUE;
	}
	return is;
}

SLboolean Location::setOrientation(float frontX, float frontY, float frontZ, float upX, float upY, float upZ)
{
	SLboolean is = SL_BOOLEAN_FALSE;
	if( isOK() )
	{//ミリメートル単位
		SLVec3D front={frontX*1000,frontY*1000,frontZ*1000};
		SLVec3D up={upX*1000,upY*1000,upZ*1000};
		normalize(&front);
		normalize(&up);
		M(this->mItf)->SetOrientationVectors(this->mItf, &front, &up);
		is = SL_BOOLEAN_TRUE;
	}
	return is;
}

//===========================================================
//
//		Listenerクラス
//
//===========================================================
Listener::Listener()
{
}

Listener::~Listener()
{
	clear();
}

void Listener::clear()
{
	if( isOK() )
	{
		M(this->mObject)->Destroy(this->mObject);
		this->mLocate.clear();
	}
}

SLboolean Listener::init(Device& device, SLboolean isAsync)
{
	SLInterfaceID ids[]=
	{
		SL_IID_3DLOCATION
	};
	SLboolean reqs[]={
		SL_BOOLEAN_TRUE,
	};
	SLint32 num = sizeof(ids)/sizeof(ids[0]);
	assert( !Device::isError(device.createListener(this, num, ids, reqs, isAsync), "Failure create Listener... in Listener::init()") );
	this->mLocate.init(this->mObject);
	return SL_BOOLEAN_TRUE;
}

//============================================
//	よく使うやつ
//============================================
SLboolean Listener::setPos(float x, float y, float z)
{
	return this->mLocate.setPos(x,y,z);
}

SLboolean Listener::setOrientation(float frontX, float frontY, float frontZ)
{
	return this->mLocate.setOrientation(frontX,frontY,frontZ);
}

SLboolean Listener::setOrientation(float frontX, float frontY, float frontZ, float upX, float upY, float upZ)
{
	return this->mLocate.setOrientation(frontX,frontY,frontZ, upX, upY, upZ);
}
