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
		static bool createVertexShaderFromMemory(kShader* out,const char* buffer,s32 length)
		{
			out->m_ShaderID = glCreateShader(GL_VERTEX_SHADER);

			glShaderSource(out->m_ShaderID, 1, &buffer, &length);
			glCompileShader(out->m_ShaderID);
			return true;
		}
		/**
		* @brief �s�N�Z���V�F�[�_�[���V�F�[�_�[�����񂩂�쐬����
		* @param[out] out �쐬�����V�F�[�_�[
		* @param[in] buffer �V�F�[�_�[���L�q���ꂽ������
		*/
		static bool createPixelShaderFromMemory(kShader* out,const char* buffer,s32 length)
		{
			out->m_ShaderID = glCreateShader(GL_FRAGMENT_SHADER);

			glShaderSource(out->m_ShaderID, 1, &buffer, &length);
			glCompileShader(out->m_ShaderID);
			return true;
		}
		/**
		* @brief ���_���C�A�E�g���쐬����
		* @param[out] out �쐬�������C�A�E�g
		* @param[in] usePipline ���C�A�E�g���g�p����`��p�C�v���C��
		* @param[in] desc ���_��`
		* @param[in] descsize ��`��
		*/
		static bool createInputLayout(kInputLayout* out,const kGraphicsPipline* usePipline,const kInputElementDesc* desc,u32 descsize)
		{
			dprintf("createInputLayout\n");
			if(!out)return false;
			//���_�t�H�[�}�b�g�����`���m�ۂ���
			out->mp_Desc=new kInputLayoutDesc[descsize];
			out->m_TypeNum=descsize;
			out->m_VertexSize=0;
			for(int i=0;i<descsize;i++)
			{
				//�V�F�[�_���P�[�V�������擾����
				out->mp_Desc[i].m_Location=glGetAttribLocation(usePipline->m_Program,desc[i].SemanticName);
				//�t�H�[�}�b�g�����擾����
				out->mp_Desc[i].m_Types.dxgi_format=PixelToFormat(desc[i].Format,&out->mp_Desc[i].m_Types.component,&out->mp_Desc[i].m_Types.size,&out->mp_Desc[i].m_Types.normalize);
				dprintf("SemanticName=%s\n",desc[i].SemanticName);
				dprintf("AttribLocation=%u\n",out->mp_Desc[i].m_Location);
				dprintf("SemanticSize=%u\n",out->mp_Desc[i].m_Types.size);
				dprintf("SemanticOffset=%u\n",out->m_VertexSize);
				out->m_VertexSize+=out->mp_Desc[i].m_Types.size;
			}
			//bit�P�ʂ�byte�P�ʂɕϊ�
			out->m_VertexSize/=8;
			dprintf("InputLayoutSize=%ubyte\n",out->m_VertexSize);
			return true;
		}
		/**
		* @brief ���_�o�b�t�@�[���쐬����
		* @param[out] out �쐬�������_�o�b�t�@
		* @param[in] data �o�b�t�@�̏����l
		* @param[in] datasize �o�b�t�@�T�C�Y
		* @param[in] type �o�b�t�@�^�C�v
		*/
		static bool createVertexBuffer(kObjectBuffer*out,const void* data,u32 datasize,s32 type)
		{
			dprintf("createVertexBuffer\n");
			//���_�o�b�t�@�I�u�W�F�N�g�̍쐬
			glGenBuffers(1,&out->m_BO);
			glBindBuffer(GL_ARRAY_BUFFER,out->m_BO);
			glBufferData(GL_ARRAY_BUFFER,datasize,data,type);
			out->m_BufferSize=datasize;
			out->m_BufferType=GL_ARRAY_BUFFER;
			//�o�C���h����
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			return true;
		}
		/**
		* @brief �C���f�b�N�X�o�b�t�@�[���쐬����
		* @param[out] out �쐬�����C���f�b�N�X�o�b�t�@
		* @param[in] data �o�b�t�@�̏����l
		* @param[in] datasize �o�b�t�@�T�C�Y
		* @param[in] type �o�b�t�@�^�C�v
		*/
		static bool createIndexBuffer(kObjectBuffer* out,const void* data,u32 datasize,s32 type)
		{
			dprintf("createIndexBuffer\n");
			//�C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�̍쐬
			glGenBuffers(1, &out->m_BO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out->m_BO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, datasize,data, type);
			out->m_BufferSize=datasize;
			out->m_BufferType=GL_ELEMENT_ARRAY_BUFFER;
			//�@�o�C���h�������̂����ǂ�
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			return true;
		}
		/**
		* @brief �o�b�t�@�̓��e���X�V����
		* @param[in] in �X�V����o�b�t�@
		* @param[in] data �X�V����l
		* @param[in] datasize �X�V����T�C�Y
		*/
		static bool updateSubResource(const kObjectBuffer* in,const void* data,u32 datasize)
		{
			glBindBuffer( in->m_BufferType, in->m_BO );
			glBufferSubData(in->m_BufferType, 0, datasize, data);  
			return true;
		}
		/**
		* @brief ���_���C�A�E�g��ݒ肷��
		* @param[in] in ���_���C�A�E�g
		*/
		static bool IAsetInputLayout(const kInputLayout* in)
		{
			u32 offset=0;
			const kInputLayoutDesc* desc=in->mp_Desc;
			for(int i=0;i<in->m_TypeNum;i++)
			{
				//���i�K�ł̓��P�[�V������0�̂�
				glEnableVertexAttribArray(desc[i].m_Location);
				glVertexAttribPointer(desc[i].m_Location, desc[i].m_Types.component, desc[i].m_Types.dxgi_format, desc[i].m_Types.normalize, in->m_VertexSize, (GLvoid*)offset);
				//dprintf("Location %u\n",desc[i].m_Location);
				//dprintf("Component %u\n",desc[i].m_Types.component);
				//dprintf("VertexSize %u\n",in->m_VertexSize);
				//dprintf("Offset %u\n",offset);
				offset+=desc[i].m_Types.size;
			}
			return true;
		}
		/**
		* @brief ���_�o�b�t�@��ݒ肷��
		* @param[in] in ���_���C�A�E�g
		*/
		static bool IAsetVertexBuffer(const kObjectBuffer* in)
		{
			glBindBuffer( GL_ARRAY_BUFFER, in->m_BO );
			return true;
		}
		/**
		* @brief �C���f�b�N�X�o�b�t�@��ݒ肷��
		* @param[in] in ���_���C�A�E�g
		*/
		static bool IAsetIndexBuffer(const kObjectBuffer* in)
		{
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, in->m_BO );
			return true;
		}
	};
}