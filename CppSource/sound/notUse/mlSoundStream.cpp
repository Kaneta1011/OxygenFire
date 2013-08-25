#include "mlSoundStream.h"

#include "../mlsound.h"
#include "../StreamingSource.h"

#include <stdlib.h>

using namespace mlSound;

static const char* TAG = "mlSound::StreamingPlayer";
#define QUEUE_COUNT 2
#define BUFFER_SIZE 8000	//8kHz
#define BUFFER_SUM BUFFER_SIZE*QUEUE_COUNT
static short sBuf[BUFFER_SUM];

StreamingPlayer::StreamingPlayer():
	mNextQueueIndex(0),
	mIsPlaying(false)
{
	memset(sBuf, 0, sizeof(short)*BUFFER_SUM);
	for( int i=0; i<BUFFER_SUM; i++ )
	{
		sBuf[i] = 32768 - ((i % 100) * 660);
	}
}

StreamingPlayer::~StreamingPlayer()
{
	clear();
}

void StreamingPlayer::clear()
{
	mPCM.clear();
}

void StreamingPlayer::init(mlSound::Device& device, mlSound::OutputMix& outputMix, bool isAsync)
{
	clear();
	if( !mPCM.init( device, outputMix, PCMPlayer::CHANNEL_MONO, StreamingPlayer::sCallback, this,
		SL_SAMPLINGRATE_22_05, SL_PCMSAMPLEFORMAT_FIXED_16, QUEUE_COUNT, isAsync) )
	{
		LOGE(TAG,"Failure initialize");
		exit(1);
	}
	LOGI(TAG,"Successed init");
}


void StreamingPlayer::addList(StreamingSoruce* pSrc)
{
	
	if( pSrc )
	{
		PlayInfo data;
		data.nowPos = 0;
		data.src = pSrc;
		this->mPlayList.pushFront(data);
		data.nowPos = 1000;
		this->mPlayList.pushFront(data);
		LOGI(TAG,"add list");
	}
	else
	{
		LOGE(TAG, "argument is null");
	}

	if( !this->mIsPlaying )
	{
		enqueue();
	}
}

void StreamingPlayer::enqueue()
{
	if( this->mPlayList.size() == 0 ){
		this->mIsPlaying = false;
		return ;
	}

	this->mIsPlaying = true;
//使用するメモリ領域の初期化を行う
	short* buf = &sBuf[this->mNextQueueIndex*BUFFER_SIZE];
	memset(buf, 0, sizeof(short)*BUFFER_SIZE);
	SinglyLinkedList<StreamingPlayer::PlayInfo>::Iterator it = this->mPlayList.begin();
	int i=0;
	while(!it.isEnd())
	{
		LOGI(TAG,"i=%d | src pos=%d | size=%u | bitPerSample=%u | samplingRate=%u", i++, it->nowPos, it->src->size(), it->src->getBitPerSample(), it->src->getSamplePerSecond());

		long size=0;
		switch( it->src->getBitPerSample() ){
		case mlSound::BIT_16:	size = set16BitSource(buf, it);		break;
		case mlSound::BIT_8:	size = set8BitSource(buf, it);		break;
		default:
			LOGE(TAG,"invalid bit per sample source!!");
		}

		it->nowPos += size;
		if( size < BUFFER_SIZE )
		{
			this->mPlayList.erase(it);
			LOGI(TAG,"finish source data");
		}
		else
		{
			it++;
		}
	}
//エンキュー実行
	LOGI(TAG,"nextQueueIndex=%d | srcNum=%u", this->mNextQueueIndex, this->mPlayList.size());
	this->mPCM.enqueue(buf, BUFFER_SIZE);

//後処理
	this->mNextQueueIndex = (this->mNextQueueIndex+1) % QUEUE_COUNT;
}

long StreamingPlayer::set16BitSource(short* outBuf, SinglyLinkedList<StreamingPlayer::PlayInfo>::Iterator &it)
{
	const short* pSrc = (short*)it->src->src();
	StreamingSoruce::src_size_type size = BUFFER_SIZE;
	StreamingSoruce::src_size_type srcSize = it->src->size()/2;
	if( size+it->nowPos > srcSize ){size = srcSize - it->nowPos;}
	for( int i=0; i<size; i++ ){
		outBuf[i] += pSrc[i+it->nowPos];
	}
	return size;
}

long StreamingPlayer::set8BitSource(short* outBuf, SinglyLinkedList<StreamingPlayer::PlayInfo>::Iterator &it)
{
	const mlSound::StreamingSoruce::src_type* pSrc = it->src->src();
	StreamingSoruce::src_size_type size = BUFFER_SIZE;
	StreamingSoruce::src_size_type srcSize = it->src->size();
	if( size+it->nowPos > srcSize ){size = srcSize - it->nowPos;}

	for( int i=0; i<size; i++ ){
	//byte型の値をshort型の値の範囲に変換する
		short set = pSrc[i+it->nowPos];
		if( set >= 0 )	set = (set-128)*258;
		else			set = (set-128)*256;
		outBuf[i] += set;
	}
	return size;
}


void StreamingPlayer::sCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
	StreamingPlayer* self = (StreamingPlayer*)context;
	LOGI(TAG, "passage buffer queue callback.");
	self->enqueue();
}

void StreamingPlayer::play()
{
	mPCM.play();
}

void StreamingPlayer::stop()
{
	mPCM.stop();
}

