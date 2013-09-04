#pragma once
#include "defines.h"
#include "../kShader/kShader.h"
#include "../kInputLayout/kInputLayout.h"
#include "math\kmathf.h"
#include "../kBlendState/kBlendState.h"
#include "../kRasterizerState/kRasterizerState.h"
#include "../kDepthStencilState/kDepthStencilState.h"
//#include "../r2DObj/r2DObj.h"
namespace rlib
{
	class r2DObj;
	class Texture;
}
namespace klib
{
	class kTechnique
	{
	protected:
		GLuint	m_Program;
		kShader* mp_VertexShader;
		kShader* mp_PixelShader;

		kBlendState* mp_BlendState;
		kDepthStencilState* mp_DepthStencilState;
		kRasterizerState* mp_RasterizerState;
	private:
	public:
		kTechnique();
		~kTechnique();
		/**
		* @brief ���_�V�F�[�_�[��ǂݍ���
		* @param[in] filename �t�@�C����
		*/
		bool createVertexShader(const char* filename);
		/**
		* @brief �s�N�Z���V�F�[�_�[��ǂݍ���
		* @param[in] filename �t�@�C����
		*/
		bool createPixelShader(const char* filename);
		/**
		* @brief �u�����h�X�e�[�g���쐬����
		* @param[in] BlendStateType �u�����h�^�C�v
		*/
		bool createBlendState(const eBlendType BlendStateType);
		/**
		* @brief �f�v�X�X�e���V���X�e�[�g���쐬����
		* @param[in] enable �[�x�e�X�g���g�p���邩
		* @param[in] func �[�x�e�X�g��r�֐�
		*/
		bool createDepthStencilState(bool enable,bool writeenable,eDepthFunc func);
		/**
		* @brief ���X�^���C�U�X�e�[�g���쐬����
		* @param[in] fill �|���S���`�惂�[�h
		* @param[in] cull �J�����O����
		* @param[in] front �|���S�����ʔ���
		*/
		bool createRasterizerState(eFillMode fill,eCullMode cull,bool front);

		bool complete();

		bool setTechnique();

		bool bindAttribLocation(s32 index,const char* name)
		{
			glBindAttribLocation(m_Program,index,name);
			return true;
		}

		void setShaderValue(const char* name,f32 val);
		void setShaderValue(const char* name,const f32* val,u32 len);
		void setShaderValue(const char* name,s32 val);
		void setShaderValue(const char* name,const s32* val,u32 len);
		void setShaderValue(const char* name,const math::Vector2& val);
		void setShaderValue(const char* name,const math::Vector2* val,u32 len);
		void setShaderValue(const char* name,const math::Vector3& val);
		void setShaderValue(const char* name,const math::Vector3* val,u32 len);
		void setShaderValue(const char* name,const math::Vector4& val);
		void setShaderValue(const char* name,const math::Vector4* val,u32 len);
		void setShaderValue(const char* name,const math::Matrix& val);
		void setShaderValue(const char* name,const math::Matrix* val,u32 len);

		void setTexture(const char* name,u32 index,const rlib::r2DObj* tex);
		void setTexture(const char* name,u32 index,const rlib::Texture* tex);
	};
	class kGraphicsPipline:public kTechnique
	{
		friend class kDevice;
	private:
		
		kInputLayout* mp_InputLayout;
		

	public:
		kGraphicsPipline();
		~kGraphicsPipline();

		/**
		* @brief ���_��`���w�肵�ăp�C�v���C��������������
		* @param[in] desc ���_��`
		* @param[in] descsize ��`��
		*/
		bool complete(const kInputElementDesc* desc,u32 descsize);
		/**
		* @brief �p�C�v���C�����w�肷��
		*/
		bool setPipline();

	};
}