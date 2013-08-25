#pragma once
#include "IMeshRenderDelegate.h"

namespace klib
{
	class kMeshGLES20Render:public IMeshRenderDelegate
	{
	protected:
		kObjectBuffer* mp_VBO;
		kObjectBuffer* mp_IBO;
	public:
		kMeshGLES20Render(){}
		virtual ~kMeshGLES20Render()
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
			//頂点バッファオブジェクトの作成
			kDevice::createVertexBuffer(mp_VBO,m_MeshData->mp_Vertex,sizeof(kMeshVertex)*m_MeshData->m_Info.NumVertex,GL_DYNAMIC_DRAW);
			//インデックスバッファオブジェクトの作成
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
			//モデル行列作成(テスト用)
			//シェーダに行列転送
			shader->Send_Matrix(mat);
			//頂点バッファを更新する
			kDevice::updateSubResource(mp_VBO,((kMeshVertex*)m_MeshData->mp_Vertex),sizeof(kMeshVertex)*m_MeshData->m_Info.NumVertex);
			//頂点バッファをパイプラインにセットする
			kDevice::IAsetVertexBuffer(mp_VBO);
			//インデックスバッファをパイプラインにセットする
			kDevice::IAsetIndexBuffer(mp_IBO);
			//シェーダーをパイプラインにセットする
			shader->setPipline();
			//描画
			glDrawElements( GL_TRIANGLES, m_MeshData->m_Info.NumFace*3, GL_UNSIGNED_INT, NULL );

		}

	};
}