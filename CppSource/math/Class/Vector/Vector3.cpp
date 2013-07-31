#include "..\..\kmathf.h"
namespace klib
{
	namespace math
	{
		//*****************************************************************************
		//		代入演算オペレータ
		//*****************************************************************************
		Vector3& Vector3::operator=(const Vector4& val){this->x=val.x;this->y=val.y;this->z=val.z;return *this;}
		Vector3& Vector3::operator=(const Quaternion& val){this->x=val.x;this->y=val.y;this->z=val.z;return *this;}
		//*****************************************************************************
		//		関数
		//*****************************************************************************
		const Vector3* Vector3::cross(Vector3* out,const Vector3& in) const {return Vector3Cross(out,*this,in);}
		f32 Vector3::degree(const Vector3& in) const {return Vector3Degree(*this,in);}
		f32 Vector3::length()const{return Vector3Length(*this);}
		f32 Vector3::distanceSq(const Vector3& vec)const{return Vector3DistanceSq(*this,vec);}
		f32 Vector3::distance(const Vector3& vec)const{return Vector3Distance(*this,vec);}
		const Vector3* Vector3::normalize(){return Vector3Normalize(this,*this);}
		const Vector3* Vector3::lerp(const Vector3& from,const Vector3& dest,f32 t) {return Vector3Lerp(this,from,dest,t);}
		const Vector3* Vector3::sLerp(const Vector3& from,const Vector3& dest,f32 t) {return Vector3Slerp(this,from,dest,t);}
		const Vector3* Vector3::rotate(const Quaternion& q){return QuaternionVec3Rotation(this,q);}
		const Vector3* Vector3::rotate(const Vector3& axis,f32 r)
		{
			Quaternion rot(axis,r);
			return QuaternionVec3Rotation(this,rot);
		}
		const Vector3* Vector3::trans(const Matrix& m){return MatrixVec3TransformCoord(this,*this,m);}
		const Vector3* Vector3::trans3x3(const Matrix& m){return MatrixVec3TransformNormal(this,*this,m);}
		const Vector3* Vector3::reflect(const Vector3& front,const Vector3& normal){return Vector3Reflect(this,front,normal);}
		const Vector3* Vector3::refract(const Vector3& front,const Vector3& normal,f32 Eta){return Vector3Refract(this,front,normal,Eta);}
		const Vector3* Vector3::projection(const Vector3& from,const Vector3& dest){return Vector3Projection(this,from,dest);}
		//*****************************************************************************
		//		成分操作関連
		//*****************************************************************************
		const Vector3* Vector3::setCross(const Vector3& in){return Vector3Cross(this,*this,in);}
		const Vector3* Vector3::setR( f32 pitch, f32 yaw ){return Vector3fromPitchYaw(this,pitch,yaw);}
		const Vector3* Vector3::setLength(f32 len){return Vector3SetLength(this,*this,len);}
		//*****************************************************************************
		//		変換関連
		//*****************************************************************************
		const Vector3* Vector3::toEuler( Vector3* out )const{return Vector3toEuler(out,*this);}
	}
}