//	use .h
#include	"RenderState.h"
#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
//	use namespace
using namespace RenderLib;



/*
2013/7/31
エラーコードの設定は後で

*/

//----------------------------------------------------------------------
//	RenderState
//----------------------------------------------------------------------
void RenderState::Clear_Color(float R,float G,float B,float A)
{
	glClearColor(R, G, B, A);
	//checkGlError("glClearColor");
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
	//checkGlError("glClear");
}

void RenderState::Viewport(float X,float Y,float Width,float Height)
{
	glViewport(X, Y, Width, Height);
  //checkGlError("glViewport");
}