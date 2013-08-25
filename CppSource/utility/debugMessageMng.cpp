#include "debugMessageMng.h"

#include "utility.h"
#include <stdlib.h>

#ifndef NULL
#define NULL 0
#endif

#ifdef _DEBUG

jstring NewStringMS932(JNIEnv *env, const char *sjis);
const char* GetStringMS932Chars(JNIEnv *env, jstring strj, int *pLen);
void ReleaseStringMS932Chars(JNIEnv *env, jstring strj, const char *sjis);

jclass stringClass = NULL;

char	DebugMessageMng::sBuffer[BUFFER_SIZE]={'\0'};
jobject DebugMessageMng::debugInst = NULL;
jclass	DebugMessageMng::debugClass = NULL;
jmethodID DebugMessageMng::idAddMsg = NULL;
jmethodID DebugMessageMng::idFlush = NULL;

static const char* TAG = "DebugMessageMng";
void DebugMessageMng::init(JNIEnv* env, jobject activity)
{
	LOGI(TAG, "Passage debugInit");

	debugInst = env->NewGlobalRef(activity);

	jclass clazz = env->FindClass("java/lang/String");
	stringClass = (jclass)env->NewGlobalRef(clazz);

	clazz = env->GetObjectClass(debugInst);
	idAddMsg = env->GetMethodID(clazz, "addMsg", "(Ljava/lang/String;)V");
	idFlush = env->GetMethodID(clazz, "flush", "()V");

	LOGI(TAG, "Complete debugInit");
}

void DebugMessageMng::clear(JNIEnv* env)
{
	LOGI(TAG,"Passage debugDelete");
	if( debugClass != NULL ){ env->DeleteGlobalRef(debugClass); debugClass = NULL;}
	if( debugInst != NULL ){ env->DeleteGlobalRef(debugInst); debugInst = NULL;}
	if( stringClass != NULL ){ env->DeleteGlobalRef(stringClass); stringClass = NULL;}
	LOGI(TAG,"Complete debugDelete");
}

void DebugMessageMng::addMsg(const char* msg)
{
	if( debugInst == NULL ) return;

	JNIEnv* env;
	getJNIEnv(&env);
	//java���̊֐����g��
	if( idAddMsg )
	{
		jstring buf = NewStringMS932(env,msg);
		env->CallVoidMethod( debugInst, idAddMsg, buf);
		env->DeleteLocalRef(buf);
	}
	else
	{
		LOGE(TAG, "Failure addMsg");
	}
}

void DebugMessageMng::flush()
{
	if( debugInst == NULL ) return;

	JNIEnv* env;
	getJNIEnv(&env);
	//java���̊֐����g��
	if( idFlush )
	{
		env->CallVoidMethod( debugInst, idFlush );
	}
	else
	{
		LOGE(TAG, "Failure flush");
	}
}

/*
	SJIS�����񂩂�A�V����String�𐶐�����B
	2006-11-09 ���[�J���Q�Ƃ̏I����ǉ�
*/
jstring NewStringMS932(JNIEnv *env, const char *sjis)
{
	if (sjis==NULL) return NULL;

	jthrowable ej = env->ExceptionOccurred();
	if (ej!=NULL) env->ExceptionClear(); //�������̗�O���N���A

	int len = 0;
	jbyteArray arrj = NULL;
	jstring encj    = NULL;
	jmethodID mj    = NULL;
	jstring strj    = NULL;

	len = strlen(sjis);
	arrj = env->NewByteArray(len);
	if (arrj==NULL) goto END;
	env->SetByteArrayRegion(arrj, 0, len, (jbyte*)sjis);

	encj = env->NewStringUTF("MS932");
	if (encj==NULL) goto END;

	mj = env->GetMethodID(stringClass, "<init>", "([BLjava/lang/String;)V");
	if (mj==NULL) goto END;

	strj = (jstring)env->NewObject(stringClass, mj, arrj, encj);
	if (strj==NULL) goto END;

	if (ej!=NULL) env->Throw(ej); //�������Ă�����O��߂�
END:
	if( ej )	env->DeleteLocalRef(ej);
	if( encj )	env->DeleteLocalRef(encj);
	if( arrj )  env->DeleteLocalRef(arrj);

	return strj;
}

/*
	SJIS�̕������Ԃ��B
	���̕�����̓k���^�[�~�l�[�g����Ă���('\0'�ŏI����Ă���)�B
	�g�p��́A�擾�����������ReleaseStringMS932Chars�ŉ�����邱�ƁB
	2006-11-09 ���[�J���Q�Ƃ̏I����ǉ�
*/
const char* GetStringMS932Chars(JNIEnv *env, jstring strj, int *pLen)
{
	if (strj==NULL) return NULL;

	jthrowable ej = env->ExceptionOccurred();
	if (ej!=NULL) env->ExceptionClear(); //�������̗�O���N���A

	int len = 0;
	char *sjis = NULL;
	jbyteArray arrj = NULL;
	jmethodID  mj   = NULL;

	jstring encj = env->NewStringUTF("MS932");
	if (encj==NULL) goto END;

	stringClass = env->GetObjectClass(strj);
	if (stringClass==NULL) goto END;

	mj = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
	if (mj==NULL) goto END;
	arrj = (jbyteArray)env->CallObjectMethod(strj, mj, encj);
	if (arrj==NULL) goto END;

	len = env->GetArrayLength(arrj);
	if (len<0) goto END;
	sjis = (char*)malloc(len + 1);
	if (sjis==NULL){
		//env->FatalError("malloc");
		goto END;
	}
	env->GetByteArrayRegion(arrj, 0, len, (jbyte*)sjis);
	sjis[len] = '\0';
	if (pLen!=NULL) *pLen = len;

	if (ej!=NULL) env->Throw(ej); //�������Ă�����O��߂�
END:
	env->DeleteLocalRef(ej);
	env->DeleteLocalRef(encj);
	env->DeleteLocalRef(arrj);

	return sjis;
}

/*
	GetStringMS932Chars�ɂ���Ď擾������������������B
*/
void ReleaseStringMS932Chars(JNIEnv *env, jstring strj, const char *sjis)
{
	free((void*)sjis);
}

#endif

extern "C" {
	//
	//	�f�o�b�O�p
	//
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_debugInit(JNIEnv * env, jobject obj, jobject activity);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_debugDelete(JNIEnv * env, jobject obj);
};

//
//	�f�o�b�O�p
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_debugInit(JNIEnv * env, jobject obj, jobject activity)
{
	DEBUG_MSG_INIT(env,activity);
}

JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_debugDelete(JNIEnv * env, jobject obj)
{
	DEBUG_MSG_CLEAR(env);
}
