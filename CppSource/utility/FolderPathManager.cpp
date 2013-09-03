#include "FolderPathManager.h"

#include "utility\utility.h"

#include <string>

static const char* TAG = "FolderPathManager";

int  FolderPathManager::sLocalPathLength = 0;
int	 FolderPathManager::sSDCardPathLength = 0;
char* FolderPathManager::sLocalPath = NULL;
char* FolderPathManager::sSDCardPath = NULL;

void FolderPathManager::init(JNIEnv *env, jstring localPath, jstring sdCardPath)
{
	LOGI(TAG, "Execute FolderPathManager init");
	if( !sLocalPath ){
		sLocalPathLength = env->GetStringUTFLength(localPath)+1;
		sLocalPath = new char[sLocalPathLength];
		const char* org = env->GetStringUTFChars( localPath, NULL );
		strcpy( sLocalPath, org );
		env->ReleaseStringUTFChars(localPath, org);
		LOGI(TAG, "localPath = %s | len = %d", sLocalPath, sLocalPathLength);
	}

	if( !sSDCardPath ){
		sSDCardPathLength = env->GetStringUTFLength(sdCardPath)+1;
		sSDCardPath = new char[sSDCardPathLength];
		const char* org = env->GetStringUTFChars(sdCardPath, NULL );
		strcpy( sSDCardPath, org );
		env->ReleaseStringUTFChars(sdCardPath, org);
		LOGI(TAG, "sdCardPath = %s | len = %d", sSDCardPath, sSDCardPathLength);
	}
	LOGI(TAG, "Complete FolderPathManager init");
}

void FolderPathManager::clear()
{
	LOGI(TAG, "Execute FolderPathManager clear");
	if( sLocalPath ){ delete[] sLocalPath; sLocalPath = NULL; sLocalPathLength = 0; }
	if( sSDCardPath ){ delete[] sSDCardPath; sSDCardPath = NULL; sSDCardPathLength = 0; }
	LOGI(TAG, "Complete FolderPathManager clear");
}


void FolderPathManager::setLocalPath(char** out, const char* fileName)
{
	int len = strlen(fileName)+1;

	*out = new char[len+sLocalPathLength];
	sprintf(*out, "%s/%s", sLocalPath, fileName);
	LOGI(TAG, "setLocalPath path=%s | old=%s", *out, fileName);
}

void FolderPathManager::setSDCardPath(char**out, const char* fileName)
{
	int len = strlen(fileName)+1;

	*out = new char[len+sSDCardPathLength];
	sprintf(*out, "%s/%s", sSDCardPath, fileName);
	LOGI(TAG, "setSDCard path=%s | old=%s", *out, fileName);
}

extern "C"{
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_initFolderPath(JNIEnv * env,  jobject obj, jstring localPath, jstring sdCardPath);

}

JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_initFolderPath(JNIEnv * env, jobject obj, jstring localPath, jstring sdCardPath)
{
	FolderPathManager::init(env, localPath, sdCardPath);
}

