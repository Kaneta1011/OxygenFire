#pragma once
#include "types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace klib
{
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

}