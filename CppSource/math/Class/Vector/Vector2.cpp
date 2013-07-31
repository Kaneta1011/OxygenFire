#include "..\..\kmathf.h"
namespace klib
{
	namespace math
	{
		//*****************************************************************************
		//		������Z�I�y���[�^
		//*****************************************************************************
		Vector2& Vector2::operator=(const Vector3& val){this->x=val.x;this->y=val.y;return *this;}
		Vector2& Vector2::operator=(const Vector4& val){this->x=val.x;this->y=val.y;return *this;}
		//*****************************************************************************
		//		�֐�
		//*****************************************************************************
		f32 Vector2::degree(const Vector2& vec)const{return Vector2Degree(*this,vec);}
		f32 Vector2::length()const{return Vector2Length(*this);}
		f32 Vector2::distanceSq(const Vector2& vec)const{return Vector2DistanceSq(*this,vec);}
		f32 Vector2::distance(const Vector2& vec)const{return Vector2Distance(*this,vec);}
		const Vector2* Vector2::normalize(){return Vector2Normalize(this,*this);}
		const Vector2* Vector2::lerp(const Vector3& from,const Vector3& dest,f32 t){return Vector2Lerp(this,from,dest,t);}
		const Vector2* Vector2::projection(const Vector3& from,const Vector3& dest){return Vector2Projection(this,from,dest);}
		//*****************************************************************************
		//		��������֘A
		//*****************************************************************************
		const Vector2* Vector2::setR( f32 rad ){return Vector2fromEuler(this,rad);}
		const Vector2* Vector2::setLength(f32 len){return Vector2SetLength(this,*this,len);}
		//*****************************************************************************
		//		�ϊ��֘A
		//*****************************************************************************
		f32 Vector2::toEuler()const{return Vector2toEuler(*this);}
	}
}