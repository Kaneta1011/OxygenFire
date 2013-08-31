#ifndef __teEffectItem_H__
#define __teEffectItem_H__
#include	"math\\kmathf.h"
typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
using namespace klib::math;

struct COLOR
{
	//	Data
	union {
		DWORD value;
		struct { BYTE alpha,blue, green, red; };
	};
	//	BasicSet 
	COLOR(){}
	COLOR(BYTE r,BYTE g,BYTE b, BYTE a){red=r;green=g;blue=b;alpha=a;}
	COLOR(DWORD Value){ value=Value; }
	//	Method
	Vector4 Value_0from1();
};


#endif