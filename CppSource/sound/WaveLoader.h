#ifndef _WAVE_LOADER_
#define _WAVE_LOADER_

#define JNI 
#ifdef JNI
#include <jni.h>
#include <android\asset_manager.h>
#endif

class WaveLoader
{
public:
	struct Header
	{
		char riffHeader[4];
		long fileSize;
		char waveHeader[4];

	};

	struct Format
	{
		char fmtChanck[4];
		long size;
		short formatID;
		unsigned short channels;
		unsigned long samplesPerSecond;
		unsigned long avgBytesPerSec;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
	};

	struct Data
	{
		char chanck[4];
		long size;
		unsigned char *datas;

		Data():datas(0){}
		~Data(){clear();}
		void clear(){if(datas){delete[] datas; datas=0;}}
	};

public:
	WaveLoader();
	~WaveLoader();

	void clear();
	bool load(const char* filePath);
#ifdef JNI
	bool load(JNIEnv * env, jobject assetsManager, jstring fileName);
#endif
	
private:
	enum TAG_TYPE{
		TAG_FORMAT,
		TAG_FACT,
		TAG_DATA,
		TAG_LIST,
		TAG_UNKNOWN,
	};
private:
	void init(char* src, int size);

	TAG_TYPE checkTag(char* src);

private:
	Format	mFormat;
	Data	mData;
};

#endif