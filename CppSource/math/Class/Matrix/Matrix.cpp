#include "..\..\kmathf.h"

namespace klib
{
	namespace math
	{
		//�R���X�g���N�^
		Matrix::Matrix()
		{
			ZeroMemory(m,sizeof(f32)*16);
		}
		//�P�ʍs��
		const Matrix* Matrix::identity(){MatrixIdentity(this);return this;}
		//�t�s��
		const Matrix* Matrix::inverse(){MatrixInverse(this,*this);return this;}
		//���K��
		const Matrix* Matrix::normalize(){return MatrixNormalize(this);}
		//�s�񎮎擾
		f32 Matrix::determinant()const{return MatrixDet(*this);}
		//�ړ�������������
		const Matrix* Matrix::setT(f32 x,f32 y,f32 z)
		{
			this->m[3].x=x;
			this->m[3].y=y;
			this->m[3].z=z;
			this->m[3].w=1.0f;
			return this;
		}
		//yaw pitch roll��]�������(XYZ�I�[�_�[)
		const Matrix* Matrix::setRXYZ(f32 x,f32 y,f32 z)
		{
			f32	sx, sy, sz, cx, cy, cz;

			sx = ksinf(x);	sy = ksinf(y);	sz = ksinf(z);
			cx = kcosf(x);	cy = kcosf(y);	cz = kcosf(z);

			this->_11=( cy*cz );							this->_12=( cy*sz );							this->_13=( -sy );			this->_14=(0.0f);
			this->_21=( sx*sy*cz+(-cx*sz) );		this->_22=( sx*sy*sz+cx*cz );			this->_23=( sx*cy );		this->_24=(0.0f);
			this->_31=( cx*sy*cz+sx*sz );			this->_32=( cx*sy*sz+(-sx*cz) );		this->_33=( cx*cy );		this->_34=(0.0f);
			return this;
		}
		//yaw pitch roll��]�������(ZXY�I�[�_�[)
		const Matrix* Matrix::setRZXY(f32 x,f32 y,f32 z)
		{
			f32	sx, sy, sz, cx, cy, cz;

			sx = ksinf(x);	sy = ksinf(y);	sz = ksinf(z);
			cx = kcosf(x);	cy = kcosf(y);	cz = kcosf(z);

			this->_11=( cz*cy + sz*sx*sy );		this->	_12=( sz*cx );		this->_13=(-cz*sy + sz*sx*cy );	this->_14=(0.0f);
			this->_21=(-sz*cy + cz*sx*sy );	this->_22=( cz*cx );		this->_23=( sz*sy + cz*sx*cy );		this->_24=(0.0f);
			this->_31=( cx*sy );						this->_32=(-sx );			this->_33=( cx*cy );						this->_34=(0.0f);
			return this;
		}
		//�C�ӎ���]���
		const Matrix* Matrix::setR(const Vector3& axis,f32 r)
		{
			f32 s=ksinf(r);
			f32 c=kcosf(r);

			this->_11=(axis.x*axis.x)*(1.0f-c)+c;					this->_12=(axis.x*axis.y)*(1.0f-c)+axis.z*s;		this->_13=(axis.z*axis.x)*(1.0f-c)-axis.y*s;		this->_14=0.0f;
			this->_21=(axis.x*axis.y)*(1.0f-c)-axis.z*s;		this->_22=(axis.y*axis.y)*(1.0f-c)+c;					this->_23=(axis.y*axis.z)*(1.0f-c)+axis.x*s;		this->_24=0.0f;
			this->_31=(axis.z*axis.x)*(1.0f-c)+axis.y*s;		this->_32=(axis.y*axis.z)*(1.0f-c)-axis.x*s;		this->_33=(axis.z*axis.z)*(1.0f-c)+c;					this->_34=0.0f;
			return this;
		}
		//�N�H�[�^�j�I�������]���
		const Matrix* Matrix::setR(const Quaternion& q)
		{
			return QuaternionToMatrix(this,q);
		}
		//X����]���
		const Matrix* Matrix::setRX(f32 r)
		{
			f32 s=ksinf(r);
			f32 c=kcosf(r);

			this->_11=1.0f;	this->_12=0.0f;		this->_13=0.0f;		this->_14=0.0f;
			this->_21=0.0f;	this->_22=c;			this->_23=s;			this->_24=0.0f;
			this->_31=0.0f;	this->_32=-s;			this->_33=c;			this->_34=0.0f;
			return this;
		}
		//Y����]���
		const Matrix* Matrix::setRY(f32 r)
		{
			f32 s=ksinf(r);
			f32 c=kcosf(r);

			this->_11=c;			this->_12=0.0f;	this->_13=-s;			this->_14=0.0f;
			this->_21=0.0f;		this->_22=1.0f;	this->_23=0;			this->_24=0.0f;
			this->_31=s;			this->_32=0.0f;	this->_33=c;			this->_34=0.0f;
			return this;
		}
		//Z����]���
		const Matrix* Matrix::setRZ(f32 r)
		{
			f32 s=ksinf(r);
			f32 c=kcosf(r);

			this->_11=c;			this->_12=s;			this->_13=0.0f;	this->_14=0.0f;
			this->_21=-s;			this->_22=c;			this->_23=0.0f;	this->_24=0.0f;
			this->_31=0.0f;		this->_32=0.0f;		this->_33=1.0f;	this->_34=0.0f;
			return this;
		}


		//*****************************************************************************
		//		�����擾�֘A
		//*****************************************************************************
		//�s����X�P�[���E��]�E�ʒu�ɕ���
		void Matrix::decompose(Vector3* s,Quaternion* q,Vector3* p)const
		{
			MatrixDecompose(s,q,p,*this);
		}
		//�s����x�N�g���ɕ���
		void Matrix::decompose(Vector3* x,Vector3* y,Vector3* z,Vector3* p)const
		{
			MatrixDecompose(x,y,z,p,*this);
		}
		//��]�������s��ɕ���
		const Matrix* Matrix::decomposeR(Matrix* m)const
		{
			return MatrixDecomposeR(m,*this);
		}
		//��]�������N�H�[�^�j�I���ɕ���
		const Quaternion* Matrix::decomposeR(Quaternion* q)const
		{
			return MatrixDecomposeR(q,*this);
		}
		//*****************************************************************************
		//		�ϊ��֘A
		//*****************************************************************************
		//�N�H�[�^�j�I���ϊ�
		const Quaternion* Matrix::toQuaternion(Quaternion* q)const
		{
			//�N�H�[�^�j�I���ɕϊ�
			return MatrixToQuaternion(q,*this);
		}
		//�I�C���[�p�ϊ�
		const Vector3* Matrix::toEulerXYZ(Vector3* out)const
		{
			return MatrixToEulerXYZ(out,*this);
		}
		//�I�C���[�p�ϊ�
		const Vector3* Matrix::toEulerZXY(Vector3* out)const
		{
			return MatrixToEulerZXY(out,*this);
		}

	};
};