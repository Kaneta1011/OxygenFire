#include "WaveLoader.h"

#ifdef JNI
#include "AssetsLoader.h"
#include "Common.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <memory>

WaveLoader::WaveLoader()
{
}

WaveLoader::~WaveLoader()
{
	clear();
}

void WaveLoader::clear()
{
	this->mData.clear();
}

bool WaveLoader::load(const char* filePath)
{
	bool isOK = false;
	FILE* fp = fopen(filePath, "rb");
	if( fp )
	{
		int size=0;
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* buf = new char[size];
		fread( buf, 1, size, fp );
		
		init(buf, size);

		delete[] buf;
		fclose(fp);

		isOK = true;
	}
	return isOK;
}

#ifdef JNI
bool WaveLoader::load(JNIEnv * env, jobject assetsManager, jstring fileName)
{
	bool isOK;

	AssetsLoader loader;
	if( loader.load(env, assetsManager, fileName) )
	{

		isOK = true;
	}
	return isOK;
}
#endif


void WaveLoader::init(char* src, int filesize)
{
	if( src[0] == 'R' && src[1] == 'I' && src[2] == 'F' && src[3] == 'F' &&
		src[8] == 'W' && src[9] == 'A' && src[10] == 'V' && src[11] == 'E' )
	{
		src += 4;
		//unsigned size = *(unsigned*)src;
		src +=8;
		int i=12;

		while(i<filesize)
		{
			unsigned size = 0;
			memcpy( &size, &src[4], 4 );
			switch( checkTag(src) )
			{
			case TAG_FORMAT:
				memcpy( &this->mFormat, src, size+8 );
				break;
			case TAG_FACT:
				break;
			case TAG_DATA:
				this->mData.datas = new unsigned char[size];
				memcpy( &this->mData, src, 8 );
				memcpy( this->mData.datas, &src[8], size);
				break;
			case TAG_LIST:
				break;
			default:
				i = filesize;
				break;
			}

			src += size+8;
			i += size+8;
		}
	}
}

WaveLoader::TAG_TYPE WaveLoader::checkTag(char* src)
{
	char work[5];
	memcpy( work, src, 4 );
	work[4] = 0;

	if( strcmp(work, "fmt ") == 0 )
		return TAG_FORMAT;
	if( strcmp(work, "data") == 0 )
		return TAG_DATA;
	if( strcmp(work, "fact") == 0 )
		return TAG_FACT;
	if( strcmp(work, "LIST") == 0 )
		return TAG_LIST;

	return TAG_UNKNOWN;
}
