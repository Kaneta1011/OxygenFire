#include "..\..\kmathf.h"
namespace klib
{
	namespace math
	{
		//*****************************************************************************
		//		代入演算オペレータ
		//*****************************************************************************
		Vector4& Vector4::operator=(const Quaternion& val){this->x=val.x;this->y=val.y;this->z=val.z;this->w=val.w;return *this;}
		//*****************************************************************************
		//		関数
		//*****************************************************************************
		f32 Vector4::length()const{return Vector4Length(*this);}
		const Vector4* Vector4::normalize(){return Vector4Normalize(this,*this);}
		const Vector4* Vector4::lerp(const Vector4& from,const Vector4& dest,f32 t){return Vector4Lerp(this,from,dest,t);}
		const Vector4* Vector4::trans(const Matrix& m){return MatrixVec4Transform(this,*this,m);}
	}
}