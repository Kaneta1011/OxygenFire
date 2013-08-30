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

			//頂点バッファオブジェクトの作成
			kDevice::createVertexBuffer(mp_VBO,m_MeshData->mp_Vertex,sizeof(kMeshVertex)*m_MeshData->m_Info.NumVertex,GL_DYNAMIC_DRAW);

			//マテリアル毎のインデックスバッファオブジェクトの作成
			for(int i=0;i<m_MeshData->m_Info.MaterialCount;i++)
			{
				dprintf("Material No.%2d",i);
				mp_IBO[i]=NULL;
				if(m_MeshData->m_Info.MaterialNumFace[i]==0)continue;

				mp_IBO[i]=new kObjectBuffer();
				u16* work=new u16[m_MeshData->m_Info.MaterialNumFace[i]*3];
				dprintf("Index Num %u",m_MeshData->m_Info.MaterialNumFace[i]*3);
				for(int j=0;j<m_MeshData->m_Info.MaterialNumFace[i]*3;j++)
				{
					work[j]=(u16)m_MeshData->m_Info.MaterialIndex[i][j];
				}
				kDevice::createIndexBuffer(mp_IBO[i],work,sizeof(u16)*m_MeshData->m_Info.MaterialNumFace[i]*3,GL_STATIC_DRAW);
				delete[] work;
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
			//モデル行列作成(テスト用)
			math::Matrix wvp = 
				mat * 
				RenderLib::RenderState::getViewMatrix() *
				RenderLib::RenderState::getProjectionMatrix();
			//シェーダに行列転送
			shader->setShaderValue("WVP",wvp);
			//頂点バッファを更新する
			kDevice::updateSubResource(mp_VBO,((kMeshVertex*)m_MeshData->mp_Vertex),sizeof(kMeshVertex)*m_MeshData->m_Info.NumVertex);

			for(int i=0;i<m_MeshData->m_Info.MaterialCount;i++)
			{
				if(!mp_IBO[i])continue;
				shader->setTexture("colorTex",0,m_MeshData->m_Info.Diffuse[i]);
				//頂点バッファをパイプラインにセットする
				kDevice::IAsetVertexBuffer(mp_VBO);
				//インデックスバッファをパイプラインにセットする
				kDevice::IAsetIndexBuffer(mp_IBO[i]);
				//シェーダーをパイプラインにセットする
				shader->setPipline();
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE,GL_ONE);

	//glEnable(GL_TEXTURE_2D);
				//描画
				kDevice::drawIndexed(m_MeshData->m_Info.MaterialNumFace[i]*3);
			}

		}

	};
}