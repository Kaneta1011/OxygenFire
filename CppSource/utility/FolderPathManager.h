#ifndef _FOLDER_PATH_MANAGER_
#define _FOLDER_PATH_MANAGER_

#ifndef NULL
#define NULL 0
#endif

#include "jni.h"
#include "StandardLib\SmartPointer.h"

class FolderPathManager
{
public:
	static void init(JNIEnv *env, jstring localPath, jstring sdCardPath);
	static void clear();

	static void setLocalPath(char** out, const char* fileName);
	static void setSDCardPath(char**out, const char* fileName);

	static void makeDirectory(const char* makeFolderName, const char* path);

private:
	static int   sLocalPathLength;
	static int	 sSDCardPathLength;
	static char* sLocalPath;
	static char* sSDCardPath;

};

#endif