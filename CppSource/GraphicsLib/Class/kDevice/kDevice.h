#pragma once
#include "defines.h"
#include "../kShader/kShader.h"
#include "../kObjectBuffer/kObjectBuffer.h"
#include "../kInputLayout/kInputLayout.h"
#include "../kGraphicsPipline/kGraphicsPipline.h"
#include "../kBlendState/kBlendState.h"
#include "../kRasterizerState/kRasterizerState.h"
#include "../kDepthStencilState/kDepthStencilState.h"

namespace klib
{
	/**
	* @class �f�o�C�X�N���X
	*/
	class kDevice
	{
	private:
		static const kInputLayout* m_IAInputLayout;
		static const kObjectBuffer* m_IAVertexBuffer;
		static const kObjectBuffer* m_IAIndexBuffer;

		static const kBlendState* m_OMBlendState;
		static const kDepthStencilState* m_OMDepthStencilState;
		static const kRasterizerState* m_RasterizerState;
	public:
		/**
		* @brief ���_�V�F�[�_�[���V�F�[�_�[�����񂩂�쐬����
		* @param[out] out �쐬�����V�F�[�_�[
		* @param[in] buffer �V�F�[�_�[���L�q���ꂽ������
		*/
		static bool createVertexShaderFromMemory(kShader* out,const char* buffer,s32 length);
		/**
		* @brief �s�N�Z���V�F�[�_�[���V�F�[�_�[�����񂩂�쐬����
		* @param[out] out �쐬�����V�F�[�_�[
		* @param[in] buffer �V�F�[�_�[���L�q���ꂽ������
		*/
		static bool createPixelShaderFromMemory(kShader* out,const char* buffer,s32 length);
		/**
		* @brief ���_���C�A�E�g���쐬����
		* @param[out] out �쐬�������C�A�E�g
		* @param[in] usePipline ���C�A�E�g���g�p����`��p�C�v���C��
		* @param[in] desc ���_��`
		* @param[in] descsize ��`��
		*/
		static bool createInputLayout(kInputLayout* out,const kGraphicsPipline* usePipline,const kInputElementDesc* desc,u32 descsize);
		/**
		* @brief ���_�o�b�t�@�[���쐬����
		* @param[out] out �쐬�������_�o�b�t�@
		* @param[in] data �o�b�t�@�̏����l
		* @param[in] datasize �o�b�t�@�T�C�Y
		* @param[in] type �o�b�t�@�^�C�v
		*/
		static bool createVertexBuffer(kObjectBuffer*out,const void* data,u32 datasize,s32 type);
		/**
		* @brief �C���f�b�N�X�o�b�t�@�[���쐬����
		* @param[out] out �쐬�����C���f�b�N�X�o�b�t�@
		* @param[in] data �o�b�t�@�̏����l
		* @param[in] datasize �o�b�t�@�T�C�Y
		* @param[in] type �o�b�t�@�^�C�v
		*/
		static bool createIndexBuffer(kObjectBuffer* out,const void* data,u32 datasize,s32 type);
		/**
		* @brief �u�����h�X�e�[�g���쐬����(Directx11��z�肵���֐�)
		* @param[out] out �쐬�����u�����h�X�e�[�g
		* @param[in] BlendStateType �u�����h�^�C�v�̔z��
		* @param[in] BlendStateTypeLength �z��̃T�C�Y
		*/
		static void createBlendState( kBlendState* out,const eBlendType* BlendStateType, u32 BlendStateTypeLength );
		/**
		* @brief �u�����h�X�e�[�g���쐬����(Directx11��z�肵���֐�)
		* @param[out] out �쐬�����u�����h�X�e�[�g
		* @param[in] BlendStateType �u�����h�^�C�v
		*/
		static void createBlendStateAll( kBlendState* out,const eBlendType BlendStateType);
		/**
		* @brief �f�v�X�X�e���V���X�e�[�g���쐬����
		* @param[out] out �쐬�����f�v�X�X�e���V���X�e�[�g
		* @param[in] enable �[�x�e�X�g���g�p���邩
		* @param[in] func �[�x�e�X�g��r�֐�
		*/
		static bool createDepthStencilState(kDepthStencilState* out,bool enable,eDepthFunc func);
		/**
		* @brief ���X�^���C�U�X�e�[�g���쐬����
		* @param[out] out �쐬�������X�^���C�U�X�e�[�g
		* @param[in] fill �|���S���`�惂�[�h
		* @param[in] cull �J�����O����
		* @param[in] front �|���S�����ʔ���
		*/
		static bool createRasterizerState(kRasterizerState* out,eFillMode fill,eCullMode cull,bool front);
		/**
		* @brief �o�b�t�@�̓��e���X�V����
		* @param[in] in �X�V����o�b�t�@
		* @param[in] data �X�V����l
		* @param[in] datasize �X�V����T�C�Y
		*/
		static bool updateSubResource(const kObjectBuffer* in,const void* data,u32 datasize);
		/**
		* @brief ���_���C�A�E�g��ݒ肷��
		* @param[in] in ���_���C�A�E�g
		*/
		static bool IAsetInputLayout(const kInputLayout* in);
		/**
		* @brief ���_�o�b�t�@��ݒ肷��
		* @param[in] in ���_���C�A�E�g
		*/
		static bool IAsetVertexBuffer(const kObjectBuffer* in);
		/**
		* @brief �C���f�b�N�X�o�b�t�@��ݒ肷��
		* @param[in] in ���_���C�A�E�g
		*/
		static bool IAsetIndexBuffer(const kObjectBuffer* in);
		/**
		* @brief �u�����h�X�e�[�g��ݒ肷��
		* @param[in] in �u�����h�X�e�[�g
		*/
		static bool OMsetBlendState(const kBlendState* in);
		/**
		* @brief �f�v�X�X�e���V���X�e�[�g��ݒ肷��
		* @param[in] in �f�v�X�X�e���V���X�e�[�g
		*/
		static bool OMsetDepthStencilState(const kDepthStencilState* in);
		/**
		* @brief ���X�^���C�U�X�e�[�g��ݒ肷��
		* @param[in] in ���X�^���C�U�X�e�[�g
		*/
		static bool RSsetState(const kRasterizerState* in);
		/**
		* @brief �ݒ肵�Ă���C���f�b�N�X�o�b�t�@���g�p���Đݒ肵�Ă��钸�_�o�b�t�@��`�悷��
		* @param[in] indexnum �C���f�b�N�X��
		*/
		static bool drawIndexed(u32 indexnum);

	};

}