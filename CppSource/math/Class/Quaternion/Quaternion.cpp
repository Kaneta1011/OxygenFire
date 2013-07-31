#include "..\..\kmathf.h"

namespace klib
{
	namespace math
	{
		//*****************************************************************************
		//		�֐�
		//*****************************************************************************
		float Quaternion::length() const { return ksqrtf( this->lengthSq() ); }
		const Quaternion* Quaternion::sLerp(const Quaternion& q,f32 t)
		{
			return QuaternionSlerp(this,*this,q,t);
		}
		//*****************************************************************************
		//		��������֘A
		//*****************************************************************************
		//�I�C���[�p����X����]�N�H�[�^�j�I���쐬
		const Quaternion* Quaternion::setRX(f32 rad)
		{
			return QuaternionRotationX(this,rad);
		}
		//�I�C���[�p����Y����]�N�H�[�^�j�I���쐬
		const Quaternion* Quaternion::setRY(f32 rad)
		{
			return QuaternionRotationY(this,rad);
		}
		//�I�C���[�p����Z����]�N�H�[�^�j�I���쐬
		const Quaternion* Quaternion::setRZ(f32 rad)
		{
			return QuaternionRotationZ(this,rad);
		}
		//�I�C���[�p����N�H�[�^�j�I�����쐬����(XYZ�I�[�_�[)
		const Quaternion* Quaternion::setRXYZ(f32 sx,f32 sy,f32 sz)
		{
			return QuaternionRotationXYZ(this,Vector3(sx,sy,sz));
		}
		//�I�C���[�p����N�H�[�^�j�I�����쐬����(ZXY�I�[�_�[)
		const Quaternion* Quaternion::setRZXY(f32 sx,f32 sy,f32 sz)
		{
			return QuaternionRotationZXY(this,Vector3(sx,sy,sz));
		}
		//�C�ӂ̎��Ɖ�]�p����N�H�[�^�j�I�����쐬����
		const Quaternion* Quaternion::setR( const Vector3 &axis, f32 rad ){
			return QuaternionAxisRotation(this,axis,rad);
		}
		//*****************************************************************************
		//		�����擾�֘A
		//*****************************************************************************
		//�N�H�[�^�j�I���̎��Ɖ�]�p�ɕ�������
		void Quaternion::decompose( Vector3* axis, f32* rad ) const {
			QuaternionDecompose(axis,rad,*this);
		}
		//�N�H�[�^�j�I�����p���x�N�g���ɕ�������
		void Quaternion::decompose( Vector3* right, Vector3* up,Vector3* front) const {
			QuaternionDecompose(right,up,front,*this);
		}
		//*****************************************************************************
		//		�ϊ��֘A
		//*****************************************************************************
		//�N�H�[�^�j�I���ϊ�
		const Matrix* Quaternion::toMatrix(Matrix* out)const
		{
			//�N�H�[�^�j�I���ɕϊ�
			return QuaternionToMatrix(out,*this);
		}
		//�I�C���[�p�ϊ�
		const Vector3* Quaternion::toEulerXYZ(Vector3* out)const
		{
			return QuaternionToEulerXYZ(out,*this);
		}
		//�I�C���[�p�ϊ�
		const Vector3* Quaternion::toEulerZXY(Vector3* out)const
		{
			return QuaternionToEulerZXY(out,*this);
		}

	};
};