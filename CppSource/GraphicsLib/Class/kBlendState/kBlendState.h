#pragma once

#include "defines.h"

namespace klib
{

	enum eBlendType
	{
		k_BLEND_NONE = 0,
		k_BLEND_ADD = 1,
	};

	class kBlendState
	{
		friend class kDevice;
	private:
		struct kRenderTargetBlendDesc {
			bool BlendEnable;		//αブレンドを行うか
			s32 SrcBlend;				//ピクセルシェーダーからの色の係数
			s32 DstBlend;				//フレームバッファの色の係数
			s32 BlendOp;				//ブレンド計算式
		};

		struct kBlendDesc {
			bool AlphaToCoverageEnable;
			//bool IndependentBlendEnable;
			kRenderTargetBlendDesc RenderTarget;
		}m_Desc;

	public:

	};

};