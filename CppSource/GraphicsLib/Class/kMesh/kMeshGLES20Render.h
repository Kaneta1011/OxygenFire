#pragma once
#include "IMeshRenderDelegate.h"

namespace klib
{
	class kMeshGLES20Render:public IMeshRenderDelegate
	{
	protected:
		kObjectBuffer* mp_VBO;
		kObjectBuffer** mp_IBO;
	public:
		kMeshGLES20Render(){}
		virtual ~kMeshGLES20Render()
		{
			SAFE_DELETE(mp_VBO);
			for(int i=0;i<m_Mesh->getMeshPtr()->m_Info.MaterialCount;i++)
			{
				SAFE_DELETE(mp_IBO[i]);
			}
			SAFE_DELETE_ARRAY(mp_IBO);
		}
		bool CreateBuffer(const kMesh* data)
		{
			IMeshRenderDelegate::CreateBuffer(data);
			const kMeshData* m_MeshData=data->getMeshPtr();
			
			mp_VBO = new kObjectBuffer();
			mp_IBO = new kObjectBuffer*[m_MeshData->m_Info.MaterialCount];

			//���_�o�b�t�@�I�u�W�F�N�g�̍쐬
			kDevice::createVertexBuffer(mp_VBO,m_MeshData->mp_Vertex,sizeof(kMeshVertex)*m_MeshData->m_Info.NumVertex,GL_DYNAMIC_DRAW);

			//�}�e���A�����̃C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�̍쐬
			for(int i=0;i<m_MeshData->m_Info.MaterialCount;i++)
			{
				dprintf("Material No.%2d",i);
				mp_IBO[i]=NULL;
				if(m_MeshData->m_Info.MaterialNumFace[i]==0)continue;

				mp_IBO[i]=new kObjectBuffer();

				kDevice::createIndexBuffer(mp_IBO[i],m_MeshData->m_Info.MaterialIndex[i],sizeof(u16)*m_MeshData->m_Info.MaterialNumFace[i]*3,GL_STATIC_DRAW);
			}
		}
		void Render()
		{
		}
		void Render(const char* name)
		{
		}
		void Render(const math::Matrix& mat,kGraphicsPipline* shader)
		{
			const kMeshData* m_MeshData=m_Mesh->getMeshPtr();
			//���f���s��쐬(�e�X�g�p)
			math::Matrix wvp = 
				mat * 
				RenderLib::RenderState::getViewMatrix() *
				RenderLib::RenderState::getProjectionMatrix();
			//�V�F�[�_�ɍs��]��
			shader->setShaderValue("WVP",wvp);
			//���_�o�b�t�@���X�V����
			kDevice::updateSubResource(mp_VBO,((kMeshVertex*)m_MeshData->mp_Vertex),sizeof(kMeshVertex)*m_MeshData->m_Info.NumVertex);

			for(int i=0;i<m_MeshData->m_Info.MaterialCount;i++)
			{
				if(!mp_IBO[i])continue;
				if(m_MeshData->m_Info.Diffuse[i])shader->setTexture("colorTex",0,m_MeshData->m_Info.Diffuse[i]);
				//���_�o�b�t�@���p�C�v���C���ɃZ�b�g����
				kDevice::IAsetVertexBuffer(mp_VBO);
				//�C���f�b�N�X�o�b�t�@���p�C�v���C���ɃZ�b�g����
				kDevice::IAsetIndexBuffer(mp_IBO[i]);
				//�V�F�[�_�[���p�C�v���C���ɃZ�b�g����
				shader->setPipline();
				//�`��
				kDevice::drawIndexed(m_MeshData->m_Info.MaterialNumFace[i]*3);
			}

		}

	};
}