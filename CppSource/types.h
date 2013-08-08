#pragma once
//#include <tchar.h>
namespace klib
{
	///@brief	符号付整数1バイト型
	typedef signed char					s8;
	///@brief	符号付整数2バイト型
	typedef signed short				s16;
	///@brief	符号付整数4バイト型
	typedef signed int					s32;
	///@brief	符号付整数8バイト型
	typedef signed long long			s64;

	///@brief	符号無し整数1バイト型
	typedef unsigned char				u8;
	///@brief	符号無し整数2バイト型
	typedef unsigned short			u16;
	///@brief	符号無し整数4バイト型
	typedef unsigned long				u32;
	///@brief	符号無し整数8バイト型
	typedef unsigned long long		u64;

	//@brief	浮動小数点数4バイト
	typedef float							f32;
	//@brief	倍精度浮動小数点数8バイト
	typedef double							f64;

	///@brief	符号無し整数1バイト型
	typedef u8								byte;

	///@brief unicode、マルチバイト文字型
	//typedef TCHAR						tchar;

	struct float2
	{
		union {
			f32 m[2];
			struct {
				f32  x, y;
			};
			struct {
				f32  u, v;
			};
		};

		inline f32& operator[](s32 pIndex){ return m[pIndex]; }
		inline const f32& operator[](s32 pIndex) const { return m[pIndex]; }
		inline bool operator==(const float2& pVector) const { return ((m[0] == pVector.m[0]) && (m[1] == pVector.m[1])); }
		inline bool operator!=(const float2& pVector) const { return !operator==( pVector ); }
		//inline operator f32*(){return m;}
		//inline operator const f32*()const{return (const f32*)m;}
	};
	struct float3
	{
		union {
			f32 m[3];
			struct {
				f32  x, y, z;
			};
		};

		inline f32& operator[](s32 pIndex){ return m[pIndex]; }
		inline const f32& operator[](s32 pIndex) const { return m[pIndex]; }
		//inline float3& operator=(const float2& val){m[0]=val.m[0];m[1]=val.m[1];}
		inline bool operator==(const float3& pVector) const { return ((m[0] == pVector.m[0]) && (m[1] == pVector.m[1]) && (m[2] == pVector.m[2])); }
		inline bool operator!=(const float3& pVector) const { return !operator==(pVector); }
		//inline operator f32*(){return m;}
		//inline operator const f32*()const{return (const f32*)m;}
	};
	struct float4
	{
		union {
			f32 m[4];
			struct {
				f32  x, y, z, w;
			};
		};

		inline f32& operator[](s32 pIndex){ return m[pIndex]; }
		inline const f32& operator[](s32 pIndex) const { return m[pIndex]; }
		//inline float4& operator=(const float2& val){m[0]=val.m[0];m[1]=val.m[1];}
		//inline float4& operator=(const float3& val){m[0]=val.m[0];m[1]=val.m[1];m[2]=val.m[2];}
		inline bool operator==(const float4& pVector) const { return ((m[0] == pVector.m[0]) && (m[1] == pVector.m[1]) && (m[2] == pVector.m[2]) && (m[3] == pVector.m[3])); }
		inline bool operator!=(const float4& pVector) const { return !operator==( pVector ); }
		//inline operator f32*(){return m;}
		//inline operator const f32*()const{return (const f32*)m;}
	};
	struct float4x4
	{
		union {
			float4	m[4];
			f32		m16[16];
			struct {
				f32  _11, _12, _13, _14;
				f32  _21, _22, _23, _24;
				f32  _31, _32, _33, _34;
				f32  _41, _42, _43, _44;
			};
		};
		inline float4& operator[](s32 pIndex){ return m[pIndex]; }
		inline const float4& operator[](s32 pIndex) const { return m[pIndex]; }
		inline bool operator==(const float4x4& pVector) const { return ((m[0] == pVector.m[0]) && (m[1] == pVector.m[1]) && (m[2] == pVector.m[2]) && (m[3] == pVector.m[3])); }
		inline bool operator!=(const float4x4& pVector) const { return !operator==( pVector ); }
		//inline operator float4*(){return m;}
		//inline operator const float4*()const{return (const float4*)m;}
		//inline operator f32*(){return (f32*)m;}
		//inline operator const f32*()const{return (const f32*)m;}
	};
};