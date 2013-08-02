#include "utility.h"

jint getJNIEnv(JNIEnv** env)
{
	return g_VM->GetEnv((void**)env, USE_JNI_VERSION);
}
