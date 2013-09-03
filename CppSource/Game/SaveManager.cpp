#include "SaveManager.h"

#include "utility\FolderPathManager.h"
#include "utility\utility.h"
#include <stdio.h>

using namespace rlib;

static const char* TAG = "SaveManager";

bool SaveManager::save()
{
	LOGI(TAG, "Execute save");

	char* path;
	//FolderPathManager::setLocalPath(&path, "save.txt");
	FolderPathManager::setSDCardPath(&path, "save.txt");

	FILE* fp = fopen(path, "wb");
	delete [] path;

	if( fp )
	{
		//fputs("save test.", fp);
		fprintf(fp, "yattaze!");

		fclose( fp );
		LOGI(TAG, "Complete save");
		return true;
	}
	else
	{
		LOGI(TAG, "Failure open save file ...");
		return false;
	}
}

bool SaveManager::load()
{
	LOGI(TAG, "Execute load");
	char* path;
	//FolderPathManager::setLocalPath(&path, "save.txt");
	FolderPathManager::setSDCardPath(&path, "save.txt");

	FILE* fp = fopen(path, "rb");
	delete [] path;

	if( fp )
	{
		char buf[128];
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		fread(buf, 1, size, fp);

		buf[size] = 0;
		LOGI(TAG, "len=%d | %s", size, buf);

		fclose( fp );
		LOGI(TAG, "Complete load");
		return true;
	}
	else
	{
		LOGI(TAG, "Failure open load file ...");
		return false;
	}

}
