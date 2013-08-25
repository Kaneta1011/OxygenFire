#ifndef _SOUND_STREAMING_SOURCE_
#define _SOUND_STREAMING_SOURCE_

namespace mlSound
{
	class StreamingSoruce
	{
	public:
		typedef unsigned long src_size_type;
		typedef unsigned char src_type;
	public:
		StreamingSoruce();
		~StreamingSoruce();

		void clear();
		/*
		Waveファイルしか読み込めない
		*/
		void load(const char* fileName);

		void play();

	public://ゲッター・セッター
		bool			enable()const{return this->mIsEnable;}
		const src_type* src()const{return this->mpSrc;}
		src_size_type	size()const{return this->mSize;}
		unsigned int	getBitPerSample()const{return this->mBitPerSample;}
		unsigned int	getChannelCount()const{return this->mChannelCount;}
		unsigned int	getSamplePerSecond()const{return this->mSamplePerSec;}

	private:
		bool			mIsEnable;
		unsigned int	mID;
		unsigned int	mChannelCount;
		unsigned int	mSamplePerSec;
		unsigned int	mBitPerSample;
		src_size_type	mSize;
		src_type*		mpSrc;

	private:
		static unsigned int initSumCount;

	};

}

#endif
