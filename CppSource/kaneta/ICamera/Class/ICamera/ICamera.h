#pragma once
#include "GraphicsLib\Class\tRenderState\RenderState.h"
#include "math\kmathf.h"
#include "input\Input.h"

namespace klib
{
	//ƒJƒƒ‰Šî’êƒNƒ‰ƒX
	class ICamera
	{
	protected:
		math::Vector3 m_Pos;
	public:
		ICamera():m_Pos(math::Vector3(.0f,.0f,.0f)){}
		virtual ~ICamera(){}
		virtual void update()=0;
		virtual math::Vector3 getPos()const{return m_Pos;}
		virtual void setFov(f32 fov){RenderLib::RenderState::Setting_PerspectiveMatrix(fov,(f32)RenderLib::RenderState::getScreenWidth()/(f32)RenderLib::RenderState::getScreenHeight(),0.1f,100.0f);}
	};

}