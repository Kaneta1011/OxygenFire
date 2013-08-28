#pragma once
#include "kMesh.h"
#include "defines.h"
#include "GraphicsLib\Class\tRenderState\RenderState.h"
#include "../kDevice/kDevice.h"

namespace klib
{
	class IMeshRenderDelegate
	{
	protected:
		const kMesh* m_Mesh;

	public:
		IMeshRenderDelegate():m_Mesh(NULL){}
		virtual ~IMeshRenderDelegate(){}
		virtual bool CreateBuffer(const kMesh* data){m_Mesh=data;}
		virtual void Render()=0;
		virtual void Render(const char* name)=0;
		virtual void Render(const math::Matrix& mat,kGraphicsPipline* shader)=0;
	};

}