#pragma once
#include "types.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//プラットフォーム固有ヘッダ
#ifdef WIN32
#include <windows.h>
#include <process.h>
#include <crtdbg.h>// デバッグ版の場合、メモリリーク場所を特定するマクロ
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
		//		プラットフォーム固有定義
		//
		//*****************************************************************************
		//*****************************************************************************
		//		WIN32
		//*****************************************************************************
#ifdef WIN32
	//リーク特定可能new
	// メモリリーク発生時にデバッガに出力する内容をわかりやすくする
#if (defined(DEBUG) || defined(_DEBUG))
#define _CRTDBG_MAP_ALLOC                            // mallocによるメモリリーク検出でCPPファイル名と行数出力指定
#define NEW  new(_NORMAL_BLOCK, __FILE__, __LINE__)  // new によるメモリリーク検出でCPPファイル名と行数出力指定
#else
#define NEW  new
#endif
	//デバッグ出力
#ifndef dprintf
#ifdef _DEBUG
#define dprintf( ... ) \
	{ \
	char c[1024]; \
	sprintf_s(  c, __VA_ARGS__ ); \
	OutputDebugString( c ); \
}
#else
#define dprintf( ... ) // 空実装
#endif
#endif

	//エラー出力(WIN32ではdprintfと同じ)
#ifndef eprintf
#ifdef _DEBUG
#define eprintf( ... ) \
	{ \
	dprintf(__VA_ARGS__);\
}
#else
#define eprintf( ... ) // 空実装
#endif
#endif
		//*****************************************************************************
		//		Android
		//*****************************************************************************
#else
	//リーク特定可能new(Android版は通常new)
#define NEW new
	//デバッグ出力
#ifndef dprintf
#ifdef _DEBUG

//デバッグ出力タグ
#define  LOG_TAG    "libgl2jni"

#define dprintf( ... ) \
	{ \
	__android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);\
}
#else
#define dprintf( ... ) // 空実装
#endif
#endif

	//エラー出力
#ifndef eprintf
#ifdef _DEBUG
#define eprintf( ... ) \
	{ \
	__android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__);\
}
#else
#define eprintf( ... ) // 空実装
#endif
#endif

#endif

		//*****************************************************************************
		//
		//		プラットフォーム共通定義
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
	//マクロ展開の順番に考慮したトークン連結演算子
#ifndef M_CAT
#define M_CAT(a,b)	M_CAT_2(a,b)
#endif
#ifndef M_CAT_2
#define M_CAT_2(a,b)	M_CAT_3(a##b)
#endif
#ifndef M_CAT_3
#define M_CAT_3(x)	x
#endif

	//安全に解放する
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p!=NULL) { delete (p);     (p) = NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p!=NULL) { delete[] (p);   (p) = NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p!=NULL) { (p)->Release(); (p) = NULL; } }
#endif

	//単精度浮動小数点型の小さい値
#ifndef FEQ_EPS
#define  FEQ_EPS   1e-6 
#endif
	//単精度浮動小数点型のさらに小さい値
#ifndef FEQ_EPS2
#define  FEQ_EPS2   1e-12 
#endif

	//警告
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
#define WARNING( exp, ... ) // 空実装
#endif
#endif
	//動的アサーション(実行されれば必ず強制終了される)
#ifndef DYNAMIC_ASSERT
#ifdef _DEBUG    //DEBUGコンパイル時は条件をチェック.
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
#else           // RELESEコンパイル時は何もしない. 
#define DYNAMIC_ASSERT(exp,...)
#endif
#endif
	//静的アサーション
#ifndef STATIC_ASSERT
#ifdef _DEBUG    //DEBUGコンパイル時は条件をチェック.
#define STATIC_ASSERT(exp)   typedef char M_CAT(static_assert_,__LINE__)[ (x) ? 1 : -1 ]
#else
#define	STATIC_ASSERT(exp) 
#endif
#endif

	//範囲チェックWARNING
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
	//範囲チェックDYNAMIC_ASSERT
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