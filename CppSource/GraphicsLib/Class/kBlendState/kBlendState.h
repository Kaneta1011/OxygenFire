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
			bool BlendEnable;		//���u�����h���s����
			s32 SrcBlend;				//�s�N�Z���V�F�[�_�[����̐F�̌W��
			s32 DstBlend;				//�t���[���o�b�t�@�̐F�̌W��
			s32 BlendOp;				//�u�����h�v�Z��
		};

		struct kBlendDesc {
			bool AlphaToCoverageEnable;
			//bool IndependentBlendEnable;
			kRenderTargetBlendDesc RenderTarget;
		}m_Desc;

	public:

	};

};