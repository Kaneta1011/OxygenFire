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
	private:
	public:
		kMeshFireRender(){}
		~kMeshFireRender(){}
		bool CreateBuffer(const kMesh* data)
		{
			IMeshRenderDelegate::CreateBuffer(data);
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
			for(int i=0;i<10;i++)
			{
				s32 index=(s32)(math::kcube(drand48())*(m_MeshData->m_Info.NumVertex-177));
				//s32 index = lrand48()%(m_MeshData->m_Info.NumVertex-177);
				Vector3 birthPos=m_MeshData->mp_Vertex[index].m_Pos;
				birthPos.trans(mat);
				EffectLib::Particle_Singleton::getInstance()->Setting_Single(EffectLib::Particle::SINGLE_NORMAL,30,birthPos,Vector3(0,0,0),drand48()*0.15f+0.1f,COLOR(255,128,64,255),COLOR(255,128,64,255),COLOR(255,128,64,255));
			}
		}
	};
}