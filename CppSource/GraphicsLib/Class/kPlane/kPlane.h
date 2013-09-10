#pragma once
#include "defines.h"
#include "math\kmathf.h"
//ëOï˚êÈåæ
namespace rlib
{
	class r2DObj;
}
namespace klib
{
	class kObjectBuffer;
	class kGraphicsPipline;
}
namespace klib
{
	struct planeVertex
	{
		math::Vector3 m_Pos;
		math::Vector2 m_Tex;
	};
	class kPlane
	{
	private:
		static kObjectBuffer* m_VBO;
		static kObjectBuffer* m_IBO;
	public:
		static void init();
		static void release();

		static void render(kGraphicsPipline* pipline,rlib::r2DObj* tex,f32 angle,f32 sx,f32 sy,const math::Vector3& pos,f32 offsetx,f32 offsety,f32 width,f32 height);
		static void render(kGraphicsPipline* pipline,rlib::r2DObj* tex,f32 angle,f32 sx,f32 sy,const math::Vector3& pos,const math::Vector3& norm,f32 offsetx,f32 offsety,f32 width,f32 height);
		static void render(kGraphicsPipline* pipline,rlib::r2DObj* tex,f32 sx,f32 sy,const math::Vector3& eye,const math::Vector3& start,const math::Vector3& end,f32 offsetx,f32 offsety,f32 width,f32 height);
	};
}