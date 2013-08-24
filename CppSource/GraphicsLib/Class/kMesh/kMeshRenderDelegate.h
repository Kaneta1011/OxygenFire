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