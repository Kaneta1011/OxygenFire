#pragma once
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "GraphicsLib\Class\kMesh\IMeshRenderDelegate.h"
#include "EffectLib\Effect.h"
#include "StandardLib\SmartPointer.h"
#include "templateLib\kVector.h"

namespace klib
{
	class kMeshFireRender:public	IMeshRenderDelegate
	{
		typedef wp<EffectLib::EmitterSet> EmitterWP;
	private:
		kObjectBuffer* mp_VBO;
		kObjectBuffer** mp_IBO;
	public:
		kMeshFireRender(){}
		~kMeshFireRender(){}
		bool CreateBuffer(const kMesh* data)
		{
			IMeshRenderDelegate::CreateBuffer(data);

			const kMeshData* m_MeshData=m_Mesh->getMeshPtr();
			dprintf("%u",m_MeshData->m_Info.NumVertex);
			for(int i=0;i<m_MeshData->m_Info.NumVertex;i++)
			{
				eprintf("%d x=%f y=%f z=%f",i,m_MeshData->mp_Vertex[i].m_Pos.x,m_MeshData->mp_Vertex[i].m_Pos.y,m_MeshData->mp_Vertex[i].m_Pos.z);
			}

			
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
			for(int i=0;i<20;i++)
			{
				s32 index=(s32)(math::kcube(drand48())*(m_MeshData->m_Info.NumVertex-177));
				//s32 index = lrand48()%(m_MeshData->m_Info.NumVertex-177);
				Vector3 birthPos=m_MeshData->mp_Vertex[index].m_Pos;
				birthPos.trans(mat);
				EffectLib::Particle_Singleton::getInstance()->Setting_Single(EffectLib::Particle::SINGLE_NORMAL,30,birthPos,Vector3(0,0,0),drand48()*0.15f+0.1f,COLOR(255,128,64,255),COLOR(255,128,64,255),COLOR(255,128,64,255));
			}

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