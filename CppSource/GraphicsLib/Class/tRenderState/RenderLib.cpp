//	use .h
#include	"RenderState.h"
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

	MatrixLookAtViewLH(&s_mView,Pos,TargetPos,UpPos);

}

void RenderState::Setting_PerspectiveMatrix(
	float FovY,float Aspect,float Near, float Far )
{
	MatrixPerspectiveLH(&s_mProjection,FovY,Aspect,Near,Far);
}

void RenderState::Setting_PolygonBathSides(bool Flag)
{
	if( true == Flag )
	{
		glDisable(GL_CULL_FACE);
	}else{
		
		glEnable(GL_CULL_FACE); 
		glCullFace(GL_FRONT);
	}
}