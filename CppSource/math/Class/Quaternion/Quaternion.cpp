#include "..\..\kmathf.h"

namespace klib
{
	namespace math
	{
		//*****************************************************************************
		//		関数
		//*****************************************************************************
		float Quaternion::length() const { return ksqrtf( this->lengthSq() ); }
		const Quaternion* Quaternion::sLerp(const Quaternion& q,f32 t)
		{
			return QuaternionSlerp(this,*this,q,t);
		}
		//*****************************************************************************
		//		成分操作関連
		//*****************************************************************************
		//オイラー角からX軸回転クォータニオン作成
		const Quaternion* Quaternion::setRX(f32 rad)
		{
			return QuaternionRotationX(this,rad);
		}
		//オイラー角からY軸回転クォータニオン作成
		const Quaternion* Quaternion::setRY(f32 rad)
		{
			return QuaternionRotationY(this,rad);
		}
		//オイラー角からZ軸回転クォータニオン作成
		const Quaternion* Quaternion::setRZ(f32 rad)
		{
			return QuaternionRotationZ(this,rad);
		}
		//オイラー角からクォータニオンを作成する(XYZオーダー)
		const Quaternion* Quaternion::setRXYZ(f32 sx,f32 sy,f32 sz)
		{
			return QuaternionRotationXYZ(this,Vector3(sx,sy,sz));
		}
		//オイラー角からクォータニオンを作成する(ZXYオーダー)
		const Quaternion* Quaternion::setRZXY(f32 sx,f32 sy,f32 sz)
		{
			return QuaternionRotationZXY(this,Vector3(sx,sy,sz));
		}
		//任意の軸と回転角からクォータニオンを作成する
		const Quaternion* Quaternion::setR( const Vector3 &axis, f32 rad ){
			return QuaternionAxisRotation(this,axis,rad);
		}
		//*****************************************************************************
		//		成分取得関連
		//*****************************************************************************
		//クォータニオンの軸と回転角に分解する
		void Quaternion::decompose( Vector3* axis, f32* rad ) const {
			QuaternionDecompose(axis,rad,*this);
		}
		//クォータニオンを姿勢ベクトルに分解する
		void Quaternion::decompose( Vector3* right, Vector3* up,Vector3* front) const {
			QuaternionDecompose(right,up,front,*this);
		}
		//*****************************************************************************
		//		変換関連
		//*****************************************************************************
		//クォータニオン変換
		const Matrix* Quaternion::toMatrix(Matrix* out)const
		{
			//クォータニオンに変換
			return QuaternionToMatrix(out,*this);
		}
		//オイラー角変換
		const Vector3* Quaternion::toEulerXYZ(Vector3* out)const
		{
			return QuaternionToEulerXYZ(out,*this);
		}
		//オイラー角変換
		const Vector3* Quaternion::toEulerZXY(Vector3* out)const
		{
			return QuaternionToEulerZXY(out,*this);
		}

	};
};