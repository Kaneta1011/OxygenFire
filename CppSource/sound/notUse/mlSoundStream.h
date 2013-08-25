#ifndef _ML_SOUND_STREAM_
#define _ML_SOUND_STREAM_

#include "OpenSLES.h"
#include "mlSoundDefine.h"
#include "../../utility/SinglyLinkedList.h"

namespace mlSound
{
	class StreamingSoruce;

	class StreamingPlayer
	{
	public:
		struct PlayInfo
		{
			mlSound::StreamingSoruce* src;
			long nowPos;
		};

	public:
		static void sCallback(SLAndroidSimpleBufferQueueItf bq, void *context);

	public:
		StreamingPlayer();
		~StreamingPlayer();

		void clear();
		void init(mlSound::Device& device, mlSound::OutputMix& outputMix, bool isAsync = false);

		void addList(StreamingSoruce* pSrc);

		void play();
		void stop();

	protected:
		void enqueue();

	private:
		long set16BitSource(short* outBuf, SinglyLinkedList<StreamingPlayer::PlayInfo>::Iterator &it);
		long set8BitSource(short* outBuf, SinglyLinkedList<StreamingPlayer::PlayInfo>::Iterator &it);

	private:
		PCMPlayer	mPCM;
		bool		mIsPlaying;
		int			mNextQueueIndex;
		SinglyLinkedList<PlayInfo> mPlayList;
	};
}

#endif