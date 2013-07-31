#include "..\..\kmathf.h"

namespace klib
{
	namespace math
	{
		//コンストラクタ
		Matrix::Matrix()
		{
			ZeroMemory(m,sizeof(f32)*16);
		}
		//単位行列化
		const Matrix* Matrix::identity(){MatrixIdentity(this);return this;}
		//逆行列化
		const Matrix* Matrix::inverse(){MatrixInverse(this,*this);return this;}
		//正規化
		const Matrix* Matrix::normalize(){return MatrixNormalize(this);}
		//行列式取得
		f32 Matrix::determinant()const{return MatrixDet(*this);}
		//移動成分を代入する
		const Matrix* Matrix::setT(f32 x,f32 y,f32 z)
		{
			this->m[3].x=x;
			this->m[3].y=y;
			this->m[3].z=z;
			this->m[3].w=1.0f;
			return this;
		}
		//yaw pitch roll回転成分代入(XYZオーダー)
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
		//yaw pitch roll回転成分代入(ZXYオーダー)
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
		//任意軸回転代入
		const Matrix* Matrix::setR(const Vector3& axis,f32 r)
		{
			f32 s=ksinf(r);
			f32 c=kcosf(r);

			this->_11=(axis.x*axis.x)*(1.0f-c)+c;					this->_12=(axis.x*axis.y)*(1.0f-c)+axis.z*s;		this->_13=(axis.z*axis.x)*(1.0f-c)-axis.y*s;		this->_14=0.0f;
			this->_21=(axis.x*axis.y)*(1.0f-c)-axis.z*s;		this->_22=(axis.y*axis.y)*(1.0f-c)+c;					this->_23=(axis.y*axis.z)*(1.0f-c)+axis.x*s;		this->_24=0.0f;
			this->_31=(axis.z*axis.x)*(1.0f-c)+axis.y*s;		this->_32=(axis.y*axis.z)*(1.0f-c)-axis.x*s;		this->_33=(axis.z*axis.z)*(1.0f-c)+c;					this->_34=0.0f;
			return this;
		}
		//クォータニオンから回転代入
		const Matrix* Matrix::setR(const Quaternion& q)
		{
			return QuaternionToMatrix(this,q);
		}
		//X軸回転代入
		const Matrix* Matrix::setRX(f32 r)
		{
			f32 s=ksinf(r);
			f32 c=kcosf(r);

			this->_11=1.0f;	this->_12=0.0f;		this->_13=0.0f;		this->_14=0.0f;
			this->_21=0.0f;	this->_22=c;			this->_23=s;			this->_24=0.0f;
			this->_31=0.0f;	this->_32=-s;			this->_33=c;			this->_34=0.0f;
			return this;
		}
		//Y軸回転代入
		const Matrix* Matrix::setRY(f32 r)
		{
			f32 s=ksinf(r);
			f32 c=kcosf(r);

			this->_11=c;			this->_12=0.0f;	this->_13=-s;			this->_14=0.0f;
			this->_21=0.0f;		this->_22=1.0f;	this->_23=0;			this->_24=0.0f;
			this->_31=s;			this->_32=0.0f;	this->_33=c;			this->_34=0.0f;
			return this;
		}
		//Z軸回転代入
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
		//		成分取得関連
		//*****************************************************************************
		//行列をスケール・回転・位置に分解
		void Matrix::decompose(Vector3* s,Quaternion* q,Vector3* p)const
		{
			MatrixDecompose(s,q,p,*this);
		}
		//行列をベクトルに分解
		void Matrix::decompose(Vector3* x,Vector3* y,Vector3* z,Vector3* p)const
		{
			MatrixDecompose(x,y,z,p,*this);
		}
		//回転成分を行列に分解
		const Matrix* Matrix::decomposeR(Matrix* m)const
		{
			return MatrixDecomposeR(m,*this);
		}
		//回転成分をクォータニオンに分解
		const Quaternion* Matrix::decomposeR(Quaternion* q)const
		{
			return MatrixDecomposeR(q,*this);
		}
		//*****************************************************************************
		//		変換関連
		//*****************************************************************************
		//クォータニオン変換
		const Quaternion* Matrix::toQuaternion(Quaternion* q)const
		{
			//クォータニオンに変換
			return MatrixToQuaternion(q,*this);
		}
		//オイラー角変換
		const Vector3* Matrix::toEulerXYZ(Vector3* out)const
		{
			return MatrixToEulerXYZ(out,*this);
		}
		//オイラー角変換
		const Vector3* Matrix::toEulerZXY(Vector3* out)const
		{
			return MatrixToEulerZXY(out,*this);
		}

	};
};