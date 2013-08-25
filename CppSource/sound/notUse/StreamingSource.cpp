#include "../StreamingSource.h"
#include "../waveLoader.h"
#include "../mlsound.h"

#include "../../utility/utility.h"
#include <stdlib.h>

#ifndef NULL
#define NULL 0
#endif

using namespace mlSound;

static const char* TAG = "StreamingSoruce";
unsigned int StreamingSoruce::initSumCount = 1;

StreamingSoruce::StreamingSoruce():
mIsEnable(false),
mID(0),
mChannelCount(0),
mSamplePerSec(0),
mBitPerSample(0),
mSize(0),
mpSrc(NULL)
{
}

StreamingSoruce::~StreamingSoruce()
{
	clear();
}

void StreamingSoruce::clear()
{
	this->mIsEnable = false;
	this->mChannelCount = this->mSamplePerSec = this->mBitPerSample = 0;
	this->mSize = 0;
	this->mID = 0;
	if( this->mpSrc ){ delete[] this->mpSrc; this->mpSrc = NULL; }
}

void StreamingSoruce::load(const char* fileName)
{
	clear();

	WaveLoader loader;
	if( loader.load(fileName) )
	{
		this->mSize = loader.getSize();
		this->mpSrc = new src_type[loader.getSize()];
		memcpy(this->mpSrc, loader.getData(), sizeof(src_type)*loader.getSize());
		this->mChannelCount = loader.getChannelCount();
		this->mSamplePerSec = loader.getSamplePerSecond();
		this->mBitPerSample = loader.getBitPerSample();
		this->mID = initSumCount;
		initSumCount++;

		this->mIsEnable = true;
	}
	else
	{
		LOGE(TAG,"Failure load... filename = \"%s\"", fileName);
	}
}

void StreamingSoruce::play()
{
	Sound::streaming();
}
