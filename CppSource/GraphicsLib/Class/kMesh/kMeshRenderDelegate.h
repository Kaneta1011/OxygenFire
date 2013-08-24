#pragma once
#include "kMesh.h"
#include "defines.h"
#include "ShaderLib\ShaderManager.h"
#include "../kDevice/kDevice.h"

namespace klib
{
	struct MeshData
	{
		math::Vector3 m_Pos;
		math::Vector4 m_Color;
	};
	class IMeshRenderDelegate
	{
	protected:
		const kMesh* m_Mesh;

	public:
		IMeshRenderDelegate():m_Mesh(NULL){}
		virtual ~IMeshRenderDelegate(){}
		virtual void Initialize(const kMesh* data)
		{
			m_Mesh=data;
		}
		virtual bool CreateBuffer(const kMesh* data){m_Mesh=data;}
		virtual void Render()=0;
		virtual void Render(const char* name)=0;
		virtual void Render(const math::Matrix& mat,kGraphicsPipline* shader)=0;
	};
	class kMeshGLES20Render2:public IMeshRenderDelegate
	{
	protected:
		kObjectBuffer* mp_VBO;
		kObjectBuffer* mp_IBO;
	public:
		kMeshGLES20Render2(){}
		virtual ~kMeshGLES20Render2()
		{
			delete mp_VBO;
			delete mp_IBO;
		}
		bool CreateBuffer(const kMesh* data)
		{
			mp_VBO = new kObjectBuffer();
			mp_IBO = new kObjectBuffer();
			IMeshRenderDelegate::CreateBuffer(data);
			const kMeshData* m_MeshData=m_Mesh->getMeshPtr();
			//���_�o�b�t�@�I�u�W�F�N�g�̍쐬
			kDevice::createVertexBuffer(mp_VBO,m_MeshData->mp_Vertex,sizeof(kMeshVertex)*m_MeshData->m_Info.NumVertex,GL_DYNAMIC_DRAW);
			//�C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�̍쐬
			kDevice::createVertexBuffer(mp_IBO,m_MeshData->m_Info.Index,sizeof(u32)*m_MeshData->m_Info.NumFace*3,GL_STATIC_DRAW);

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
			//�V�F�[�_�ɍs��]��
			shader->Send_Matrix(mat);
			//���_�o�b�t�@���X�V����
			kDevice::updateSubResource(mp_VBO,((kMeshVertex*)m_MeshData->mp_Vertex),sizeof(kMeshVertex)*m_MeshData->m_Info.NumVertex);
			//���_�o�b�t�@���p�C�v���C���ɃZ�b�g����
			kDevice::IAsetVertexBuffer(mp_VBO);
			//�C���f�b�N�X�o�b�t�@���p�C�v���C���ɃZ�b�g����
			kDevice::IAsetIndexBuffer(mp_IBO);
			//�V�F�[�_�[���p�C�v���C���ɃZ�b�g����
			shader->setPipline();
			//�`��
			glDrawElements( GL_TRIANGLES, m_MeshData->m_Info.NumFace*3, GL_UNSIGNED_INT, NULL );

		}

	};
}