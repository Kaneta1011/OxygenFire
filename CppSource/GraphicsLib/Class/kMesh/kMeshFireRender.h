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
		//ktl::kVector<EmitterWP> m_VertexEmitter;
		wp<EffectLib::EmitterSet> *m_VertexEmitter;
	public:
		kMeshFireRender(){}
		~kMeshFireRender(){}
		bool CreateBuffer(const kMesh* data)
		{
			IMeshRenderDelegate::CreateBuffer(data);

			const kMeshData* m_MeshData=m_Mesh->getMeshPtr();
			m_VertexEmitter=new wp<EffectLib::EmitterSet>[m_MeshData->m_Info.NumVertex];
			dprintf("%u",m_MeshData->m_Info.NumVertex);
			for(int i=0;i<m_MeshData->m_Info.NumVertex;i++)
			{
				
				//dprintf("x=%f y=%f z=%f",m_MeshData->mp_Vertex[i].m_Pos.x,m_MeshData->mp_Vertex[i].m_Pos.y,m_MeshData->mp_Vertex[i].m_Pos.z);
				if((i%60)!=0)continue;
				m_VertexEmitter[i] = EffectLib::EffectManager_Singleton::getInstance()->Create(EffectLib::FIRE_CHARGE,m_MeshData->mp_Vertex[i].m_Pos*0.01f,1.0f);
				if(m_VertexEmitter[i].IsExist())m_VertexEmitter[i]->Loop();
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
			//const kMeshData* m_MeshData=m_Mesh->getMeshPtr();
			//for(int i=0;i<m_MeshData->m_Info.NumVertex;i++)
			//{
			//	m_VertexEmitter[i]->Update();
			//}
		}

	};
}