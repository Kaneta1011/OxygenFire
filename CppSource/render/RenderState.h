#ifndef __RenderState_H__
#define __RenderState_H__

namespace RenderLib{


enum eCLEAR_BUFFER_TYPE
{
	CLEAR_BUFFER_COLOR,
	CLEAR_BUFFER_DEPTH,
};

//----------------------------------------------------------------------
//	RenderState
//	@tashiro memo 2013/7/31
//	�`��֌W�̐ݒ�͂����ɂ܂Ƃ߂Ă���,�Ƃ肠����static method��
//----------------------------------------------------------------------
class RenderState
{
public:
	//	Clear
	static void Clear_Color(float R,float G,float B,float A);
	static void Clear_Buffer(eCLEAR_BUFFER_TYPE Type);
	static void Viewport(float X,float Y,float Widh,float Height);
};

}//RenderState



#endif