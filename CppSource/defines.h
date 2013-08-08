#pragma once
#include "types.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//�v���b�g�t�H�[���ŗL�w�b�_
#ifdef WIN32
#include <windows.h>
#include <process.h>
#include <crtdbg.h>// �f�o�b�O�ł̏ꍇ�A���������[�N�ꏊ����肷��}�N��
#else
#include <jni.h>
#include <pthread.h>
#include <android\log.h>
#include <unistd.h>
#endif

namespace klib
{
		//*****************************************************************************
		//
		//		�v���b�g�t�H�[���ŗL��`
		//
		//*****************************************************************************
		//*****************************************************************************
		//		WIN32
		//*****************************************************************************
#ifdef WIN32
	//���[�N����\new
	// ���������[�N�������Ƀf�o�b�K�ɏo�͂�����e���킩��₷������
#if (defined(DEBUG) || defined(_DEBUG))
#define _CRTDBG_MAP_ALLOC                            // malloc�ɂ�郁�������[�N���o��CPP�t�@�C�����ƍs���o�͎w��
#define NEW  new(_NORMAL_BLOCK, __FILE__, __LINE__)  // new �ɂ�郁�������[�N���o��CPP�t�@�C�����ƍs���o�͎w��
#else
#define NEW  new
#endif
	//�f�o�b�O�o��
#ifndef dprintf
#ifdef _DEBUG
#define dprintf( ... ) \
	{ \
	char c[1024]; \
	sprintf_s(  c, __VA_ARGS__ ); \
	OutputDebugString( c ); \
}
#else
#define dprintf( ... ) // �����
#endif
#endif

	//�G���[�o��(WIN32�ł�dprintf�Ɠ���)
#ifndef eprintf
#ifdef _DEBUG
#define eprintf( ... ) \
	{ \
	dprintf(__VA_ARGS__);\
}
#else
#define eprintf( ... ) // �����
#endif
#endif
		//*****************************************************************************
		//		Android
		//*****************************************************************************
#else
	//���[�N����\new(Android�ł͒ʏ�new)
#define NEW new
	//�f�o�b�O�o��
#ifndef dprintf
#ifdef _DEBUG

//�f�o�b�O�o�̓^�O
#define  LOG_TAG    "libgl2jni"

#define dprintf( ... ) \
	{ \
	__android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);\
}
#else
#define dprintf( ... ) // �����
#endif
#endif

	//�G���[�o��
#ifndef eprintf
#ifdef _DEBUG
#define eprintf( ... ) \
	{ \
	__android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__);\
}
#else
#define eprintf( ... ) // �����
#endif
#endif

#endif

		//*****************************************************************************
		//
		//		�v���b�g�t�H�[�����ʒ�`
		//
		//*****************************************************************************
#ifndef EqualMemory
#define EqualMemory(Destination,Source,Length) (!memcmp((Destination),(Source),(Length)))
#endif
#ifndef MoveMemory
#define MoveMemory(Destination,Source,Length) memmove((Destination),(Source),(Length))
#endif
#ifndef CopyMemory
#define CopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#endif
#ifndef FillMemory
#define FillMemory(Destination,Length,Fill) memset((Destination),(Fill),(Length))
#endif
#ifndef ZeroMemory
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif
	//�}�N���W�J�̏��Ԃɍl�������g�[�N���A�����Z�q
#ifndef M_CAT
#define M_CAT(a,b)	M_CAT_2(a,b)
#endif
#ifndef M_CAT_2
#define M_CAT_2(a,b)	M_CAT_3(a##b)
#endif
#ifndef M_CAT_3
#define M_CAT_3(x)	x
#endif

	//���S�ɉ������
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p!=NULL) { delete (p);     (p) = NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p!=NULL) { delete[] (p);   (p) = NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p!=NULL) { (p)->Release(); (p) = NULL; } }
#endif

	//�P���x���������_�^�̏������l
#ifndef FEQ_EPS
#define  FEQ_EPS   1e-6 
#endif
	//�P���x���������_�^�̂���ɏ������l
#ifndef FEQ_EPS2
#define  FEQ_EPS2   1e-12 
#endif

	//�x��
#ifndef WARNING
#ifdef _DEBUG
#define WARNING( exp, ... )   do {                                           \
	if (!(exp)) {                                             \
	dprintf(("/*************************************WARNING!!!!!!!!!!*************************************/\n"));\
	dprintf(("%s(%d)\n"),__FILE__,__LINE__);\
	dprintf(__VA_ARGS__);\
	dprintf(("\n"));\
	dprintf(("/*******************************************************************************************/\n"));\
	}\
} while (0)
#else
#define WARNING( exp, ... ) // �����
#endif
#endif
	//���I�A�T�[�V����(���s�����ΕK�������I�������)
#ifndef DYNAMIC_ASSERT
#ifdef _DEBUG    //DEBUG�R���p�C�����͏������`�F�b�N.
#define	DYNAMIC_ASSERT(exp,...)   do {                                           \
	if (!(exp)) {                                             \
	eprintf(("/*************************************ASSERT!!!!!!!!!!!*************************************/\n"));\
	eprintf(("%s(%d)\n"),__FILE__,__LINE__);\
	eprintf(__VA_ARGS__);\
	eprintf("\n");\
	eprintf(("/*******************************************************************************************/\n"));\
	assert(exp);											\
	exit(1); /*abort();*/                               \
	}                                                       \
} while (0)
#else           // RELESE�R���p�C�����͉������Ȃ�. 
#define DYNAMIC_ASSERT(exp,...)
#endif
#endif
	//�ÓI�A�T�[�V����
#ifndef STATIC_ASSERT
#ifdef _DEBUG    //DEBUG�R���p�C�����͏������`�F�b�N.
#define STATIC_ASSERT(exp)   typedef char M_CAT(static_assert_,__LINE__)[ (x) ? 1 : -1 ]
#else
#define	STATIC_ASSERT(exp) 
#endif
#endif

	//�͈̓`�F�b�NWARNING
#ifndef WARNING_LIMIT
#ifdef _DEBUG
#define WARNING_LIMIT(val,mi,ma,...)\
	do{\
	WARNING((mi)<=(val)&&(val)<=(ma),__VA_ARGS__);\
	}while(0)
#else
#define WARNING_LIMIT(val,mi,ma,...)
#endif
#endif
	//�͈̓`�F�b�NDYNAMIC_ASSERT
#ifndef DYNAMIC_ASSERT_LIMIT
#ifdef _DEBUG
#define DYNAMIC_ASSERT_LIMIT(val,mi,ma,...)\
	do{\
	DYNAMIC_ASSERT((mi)<=(val)&&(val)<=(ma),__VA_ARGS__);\
	}while(0)
#else
#define DYNAMIC_ASSERT_LIMIT(val,mi,ma,...)
#endif
#endif
}