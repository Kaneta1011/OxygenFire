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

inline unsigned long xor128() {
  static unsigned long x=123456789, y=362436069, z=521288629, w=88675123;
  static unsigned long t;
	t=(x^(x<<11));
  x=y; y=z; z=w;
  return ( w=(w^(w>>19))^(t^(t>>8)) );
}

inline float tRand( float Min, float Max )
{
	if( Min < 0 )
		Max += -Min;
	else
		Max -= Min;

	static float rand;
	rand = xor128() / 4294967295.0f;

	return rand * Max + Min;
}


#endif