#pragma once
#include "defines.h"
namespace klib
{
	enum eFillMode
	{
		eWIRE = 0,
		eSOLID = 1,
	};
	static s32 FILLMODE_LIST[]=
	{
		GL_LINES,
		GL_TRIANGLES
	};
	inline s32 getFillMode(eFillMode mode)
	{
		return FILLMODE_LIST[mode];
	}
	enum eCullMode
	{
		eNONE = 0,
		eFRONT = 1,
		eBACK = 2,
	};
	static s32 CULLMODE_LIST[]=
	{
		0,
		GL_FRONT,
		GL_BACK
	};
	inline s32 getCullMode(eCullMode mode)
	{
		return CULLMODE_LIST[mode];
	}


	class kRasterizerState
	{
		friend class kDevice;
	private:
		struct kRasterizerDesc {
			s32 FillMode;							//ポリゴン描画モード
			s32 CullMode;							//カリング向き
			bool FrontCounterClockwise;	//
			//int DepthBias;
			//float DepthBiasClamp;
			//float SlopeScaledDepthBias;
			//bool DepthClipEnable;
			//bool ScissorEnable;
			//bool MultisampleEnable;
			//bool AntialiasedLineEnable;
		}m_Desc;
	public:
	};
};