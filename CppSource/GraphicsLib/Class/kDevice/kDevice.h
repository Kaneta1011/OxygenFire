#pragma once
#include "defines.h"
#include "../kShader/kShader.h"
#include "../kObjectBuffer/kObjectBuffer.h"
#include "../kInputLayout/kInputLayout.h"
#include "../kGraphicsPipline/kGraphicsPipline.h"

namespace klib
{
	/**
	* @class �f�o�C�X�N���X
	*/
	class kDevice
	{
	private:
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
	};
}