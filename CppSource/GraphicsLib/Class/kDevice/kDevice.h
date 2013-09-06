#pragma once
#include "defines.h"
#include "../kShader/kShader.h"
#include "../kObjectBuffer/kObjectBuffer.h"
#include "../kInputLayout/kInputLayout.h"
#include "../kGraphicsPipline/kGraphicsPipline.h"
#include "../kBlendState/kBlendState.h"
#include "../kRasterizerState/kRasterizerState.h"
#include "../kDepthStencilState/kDepthStencilState.h"
#include "templateLib\kQueue.h"

namespace klib
{
	//�x���ǂݍ��݂̍ۂɎg�p����o�b�t�@�쐬���\����
	struct LoadBufferInfo
	{
		const void* m_Data;
		u32 m_DataSize;
		GLenum m_Type;
		kObjectBuffer* m_Buffer;
	};
	//�x���ǂݍ��݂̍ۂɎg�p����V�F�[�_�[�쐬���\����
	struct LoadShaderInfo
	{
		kTechnique* m_Technique;
		char* mp_VShader;
		char* mp_PShader;
		s32 m_VShaderLength;
		s32 m_PShaderLength;
		LoadShaderInfo(kTechnique* tec,const char* vbuf,s32 vlen,const char* pbuf,s32 plen)
		{
			m_Technique=tec;
			m_VShaderLength=vlen;
			mp_VShader=new char[vlen];
			CopyMemory(mp_VShader,vbuf,sizeof(char)*vlen);

			m_PShaderLength=plen;
			mp_PShader=new char[plen];
			CopyMemory(mp_PShader,pbuf,sizeof(char)*plen);
		}
		~LoadShaderInfo()
		{
			delete[] mp_VShader;
			delete[] mp_PShader;
		}
	};
	//�x���ǂݍ��݂̍ۂɎg�p����C���v�b�g���C�A�E�g�쐬���\����
	struct CreateInputLayoutInfo
	{
		const GLuint* m_Program;
		kInputLayout* m_InputLayout;
		const char* m_SemanticName[16];
	};

	
	/**
	* @class �f�o�C�X�N���X
	*/
	class kDevice
	{
		typedef ktl::kQueue_Safe<LoadBufferInfo*> LoadVertexBufferQueue;
		typedef ktl::kQueue_Safe<LoadBufferInfo*> LoadIndexBufferQueue;
		typedef ktl::kQueue_Safe<LoadShaderInfo*> LoadShaderQueue;
		typedef ktl::kQueue_Safe<CreateInputLayoutInfo*> CreateInputLayoutQueue;
	private:
		static thread::kMutex m_Mutex;
		//���_�o�b�t�@�̒x���ǂݍ��ݏ��L���[
		static LoadVertexBufferQueue m_LoadVertexBufferQueue;
		//�C���f�b�N�X�o�b�t�@�̒x���ǂݍ��ݏ��L���[
		static LoadIndexBufferQueue m_LoadIndexBufferQueue;
		//�V�F�[�_�[�̒x���ǂݍ��ݏ��L���[
		static LoadShaderQueue m_LoadShaderQueue;
		//�C���v�b�g���C�A�E�g�̒x���ǂݍ��ݏ��L���[
		static CreateInputLayoutQueue m_CreateInputLayoutQueue;

		static const kInputLayout* m_IAInputLayout;
		static const kObjectBuffer* m_IAVertexBuffer;
		static const kObjectBuffer* m_IAIndexBuffer;

		static const kBlendState* m_OMBlendState;
		static const kDepthStencilState* m_OMDepthStencilState;
		static const kRasterizerState* m_RasterizerState;
	public:
		static void begin();
		static void end();
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
		static bool createShaderMemory(kTechnique* out,const char* vertexBuffer,s32 vertexLength,const char* pixelBuffer,s32 pixelLength);
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
		static bool createDepthStencilState(kDepthStencilState* out,bool enable,bool writeenable,eDepthFunc func);
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