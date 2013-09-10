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
		math::Vector3 m_Angle;
	public:
		ICamera(const math::Vector3& pos,const math::Vector3& angle):m_Pos(pos),m_Angle(angle){}
		virtual ~ICamera(){}
		virtual void update()=0;
		virtual math::Vector3 getPos()const{return m_Pos;}
		virtual math::Vector3 getAngle()const{return m_Angle;}
		virtual void setFov(f32 fov){RenderLib::RenderState::Setting_PerspectiveMatrix(fov,(f32)RenderLib::RenderState::getScreenWidth()/(f32)RenderLib::RenderState::getScreenHeight(),0.1f,100.0f);}
	};

}