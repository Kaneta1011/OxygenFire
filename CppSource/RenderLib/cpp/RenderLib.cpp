//	use .h
#include	"RenderLib\\RenderState.h"
#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
//	use namespace
using namespace RenderLib;
using namespace klib::math;

//	use static data
Matrix RenderState::s_mView;
Matrix RenderState::s_mProjection;
int RenderState::s_ScreenWidth=0;
int RenderState::s_ScreenHeight=0;

//----------------------------------------------------------------------
//	RenderState
//----------------------------------------------------------------------
void RenderState::Clear_Color(float R,float G,float B,float A)
{
	glClearColor(R, G, B, A);
}

void RenderState::Clear_Buffer(eCLEAR_BUFFER_TYPE Type)
{

	switch(Type)
	{
	case CLEAR_BUFFER_COLOR:
		glClear(GL_COLOR_BUFFER_BIT);
		break;
	case CLEAR_BUFFER_DEPTH:
		glClear(GL_DEPTH_BUFFER_BIT);
		break;
	};
}

void RenderState::Setting_Viewport(float X,float Y,float Width,float Height)
{
	glViewport(X, Y, Width, Height);
}

void RenderState::Setting_ViewMatrix(
	const Vector3& Pos,
	const Vector3& TargetPos,
	const Vector3& UpPos )
{
	Vector3 zaxis;
	zaxis.x = TargetPos.x - Pos.x;
	zaxis.y = TargetPos.y - Pos.y;
	zaxis.z = TargetPos.z - Pos.z;
	Vector3 xaxis = Vector3( UpPos.y*zaxis.z - UpPos.z*zaxis.y, 
		UpPos.z*zaxis.x - UpPos.x*zaxis.z, UpPos.x*zaxis.y - UpPos.y*zaxis.x ); 
	Vector3 yaxis = Vector3( zaxis.y*xaxis.z - zaxis.z*xaxis.y, zaxis.z*xaxis.x - 
		zaxis.x*xaxis.z, zaxis.x*xaxis.y - zaxis.y*xaxis.x ); 
   
	xaxis.normalize();
	yaxis.normalize();
	zaxis.normalize();

	s_mView._11 = xaxis.x;
	s_mView._12 = xaxis.y;
	s_mView._13 = xaxis.z;
	s_mView._14 = 0;

	s_mView._21 = yaxis.x;
	s_mView._22 = yaxis.y;
	s_mView._23 = yaxis.z;
	s_mView._24 = 0;

	s_mView._31 = zaxis.x;
	s_mView._32 = zaxis.y;
	s_mView._33 = zaxis.z;
	s_mView._34 = 0;

	s_mView._41 = Pos.x;
	s_mView._42 = Pos.y;
	s_mView._43 = Pos.z;
	s_mView._44 = 1;
	s_mView.inverse();
}

void RenderState::Setting_PerspectiveMatrix(
	float FovY,float Aspect,float Near, float Far )
{
	float yScale = 1 / tanf(FovY/2);
	float xScale = yScale / Aspect;

	s_mProjection._11 = xScale;
	s_mProjection._12 = 0;
	s_mProjection._13 = 0;
	s_mProjection._14 = 0;

	s_mProjection._21 = 0;
	s_mProjection._22 = yScale;
	s_mProjection._23 = 0;
	s_mProjection._24 = 0;

	s_mProjection._31 = 0;
	s_mProjection._32 = 0;
	s_mProjection._33 = Far / (Far-Near);
	s_mProjection._34 = 1;

	s_mProjection._41 = 0;
	s_mProjection._42 = 0;
	s_mProjection._43 = (-Near*Far) / (Far-Near);
	s_mProjection._44 = 0;
}