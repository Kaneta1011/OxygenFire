#pragma once
#include "defines.h"
namespace klib
{
	enum eDepthFunc
	{
		eNEVER = 0,
		eLESS = 1,
		eEQUAL = 2,
		eLESS_EQUAL = 3,
		eGREATER = 4,
		eNOT_EQUAL = 5,
		eGREATER_EQUAL = 6,
		eALWAYS = 7,
	};

	static s32 DEPTHFUNC_LIST[] =
	{
		GL_NEVER,
		GL_LESS,
		GL_EQUAL,
		GL_LEQUAL,
		GL_GREATER,
		GL_NOTEQUAL,
		GL_GEQUAL,
		GL_ALWAYS
	};

	inline s32 getDepthFunc(eDepthFunc mode)
	{
		return DEPTHFUNC_LIST[mode];
	}



	class kDepthStencilState
	{
		friend class kDevice;
	private:
		struct kDepthStencilDesc {
			bool DepthEnable;	//深度テストを行うか
			//D3D11_DEPTH_WRITE_MASK DepthWriteMask;//未使用
			s32 DepthFunc;		//深度テストの際の比較関数
			//bool StencilEnable;
			//u8 StencilReadMask;
			//u8 StencilWriteMask;
			//D3D11_DEPTH_STENCILOP_DESC FrontFace;//未使用
			//D3D11_DEPTH_STENCILOP_DESC BackFace;//未使用
		}m_Desc;
	public:
	};

};