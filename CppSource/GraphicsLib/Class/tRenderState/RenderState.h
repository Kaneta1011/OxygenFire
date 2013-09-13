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
	static void Setting_PolygonBathSides(bool Flag);//���ʕ`�悷�邩
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

	// �X�N���[�����W�����[���h���W�ɕϊ�
	static klib::math::Vector3* screenToWorld(klib::math::Vector3* out,
		int Sx,  // �X�N���[��X���W
		int Sy,  // �X�N���[��Y���W
		float fZ  // �ˉe��Ԃł�Z�l�i0�`1�j
		) {
			using namespace klib;
			using namespace math;
			// �e�s��̋t�s����Z�o
			Matrix InvView, InvPrj, VP, InvViewport;
			MatrixInverse(&InvView,RenderLib::RenderState::getViewMatrix());
			MatrixInverse(&InvPrj,RenderLib::RenderState::getProjectionMatrix());
			VP.identity();
			VP._11 = RenderLib::RenderState::getScreenWidth()/2.0f; VP._22 = -RenderLib::RenderState::getScreenHeight()/2.0f;
			VP._41 = RenderLib::RenderState::getScreenWidth()/2.0f; VP._42 = RenderLib::RenderState::getScreenHeight()/2.0f;
			MatrixInverse(&InvViewport,VP);

			// �t�ϊ�
			Matrix tmp = InvViewport * InvPrj * InvView;
			Vector3 work=Vector3((f32)Sx,(f32)Sy,fZ);
			work.trans(InvViewport);
			*out=Vector3((f32)Sx,(f32)Sy,fZ);
			//�ϊ���w�Ŏˉe���Ă���
			out->trans(tmp);

			return out;
	}

	static void worldToScreen(const klib::math::Vector3& wpos,int* sx,int* sy)
	{
		using namespace klib;
		using namespace math;
		// �e�s��̋t�s����Z�o
		Matrix WVP,VP;
		VP.identity();
		VP._11 = RenderLib::RenderState::getScreenWidth()/2.0f; VP._22 = -RenderLib::RenderState::getScreenHeight()/2.0f;
		VP._41 = RenderLib::RenderState::getScreenWidth()/2.0f; VP._42 = RenderLib::RenderState::getScreenHeight()/2.0f;

		WVP=RenderLib::RenderState::getViewMatrix()*RenderLib::RenderState::getProjectionMatrix();

		Vector3 ret=wpos;
		ret.trans(WVP);
		ret.trans(VP);

		*sx=ret.x;
		*sy=ret.y;
	}

	static klib::f32 scaleParse(klib::f32 scale,const klib::math::Vector3& pos)
	{
		using namespace klib;
		using namespace math;

		Vector3 viewpos=pos;
		viewpos.trans(RenderLib::RenderState::getViewMatrix());
		viewpos.x=0.0f;
		viewpos.y=scale*0.5f;

		viewpos.trans(RenderLib::RenderState::getProjectionMatrix());
		f32 ret=viewpos.y*(RenderLib::RenderState::getScreenWidth()/2.0f);
		//dprintf("%f",ret);
		return ret;
	}


#endif