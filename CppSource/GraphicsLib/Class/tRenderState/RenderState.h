#ifndef __teRenderState_H__
#define __teRenderState_H__
#include	"math\\kmathf.h"

namespace RenderLib{

using namespace klib::math;

enum eCLEAR_BUFFER_TYPE
{
	CLEAR_BUFFER_COLOR,
	CLEAR_BUFFER_DEPTH,
};

//----------------------------------------------------------------------
//	RenderState
//----------------------------------------------------------------------
class RenderState
{
public:
	//	Clear
	static void Clear_Color(float R,float G,float B,float A);
	static void Clear_Buffer(eCLEAR_BUFFER_TYPE Type);
	//	Setting
	static void Setting_Viewport(float X,float Y,float Widh,float Height);
	static void Setting_ViewMatrix(const Vector3& Pos,const Vector3& TargetPos,const Vector3& UpPos);
	static void Setting_PerspectiveMatrix(float FovY,float Aspect,float Near, float Far);
	static void Setting_PolygonBathSides(bool Flag);//—¼–Ê•`‰æ‚·‚é‚©
	//	set
	static void setScreenWidth(int Value){s_ScreenWidth=Value;}
	static void setScreenHeight(int Value){s_ScreenHeight=Value;}
	//	get
	static Matrix getViewMatrix(){return s_mView;}
	static Matrix getProjectionMatrix(){return s_mProjection;}
	static int getScreenWidth(){return s_ScreenWidth;}
	static int getScreenHeight(){return s_ScreenHeight;}
private:
	//	Data
	static Matrix		s_mView;
	static Matrix		s_mProjection;
	static int	s_ScreenWidth;
	static int	s_ScreenHeight;
};

}//RenderState



#endif