#pragma once
#include "GraphicsLib\Class\tRenderState\RenderState.h"
namespace klib
{
	class kCamera
	{
	protected:
	public:
		kCamera(){}
		virtual ~kCamera(){}
		virtual void update()=0;
	};
}