#pragma once
#include "..\..\..\types.h"
#include "..\..\..\defines.h"
#include "..\Vector\Vector.h"

namespace klib
{
	namespace math
	{
		struct Matrix;
		struct Quaternion:public	float4
		{
			inline Quaternion(){this->x=this->y=this->z=0.0f;this->w=1.0f;}
			inline Quaternion(const Vector3& vec){this->x=vec.x;this->y=vec.y;this->z=vec.z;this->w=0.0f;}
			inline Quaternion(const Vector3& axis,f32 r){this->setR(axis,r);}
			inline Quaternion(f32 x,f32 y,f32 z,f32 w){this->x=x;this->y=y;this->z=z;this->w=w;}
			inline Quaternion(const Quaternion& q){this->x=q.x;this->y=q.y;this->z=q.z;this->w=q.w;}
			//*****************************************************************************
			//		���Z�I�y���[�^
			//*****************************************************************************
			inline Quaternion operator + () const { return Quaternion(x,y,z,w); }
			inline Quaternion operator - () const { return Quaternion(-x,-y,-z,-w); }
			//*****************************************************************************
			//		Quaternion���Z�I�y���[�^
			//*****************************************************************************
			inline Quaternion operator+(const Quaternion& val)const{return Quaternion(this->x+val.x,this->y+val.y,this->z+val.z,this->w+val.w);}
			inline Quaternion operator-(const Quaternion& val)const{return Quaternion(this->x-val.x,this->y-val.y,this->z-val.z,this->w-val.w);}
			inline Quaternion operator*( const Quaternion& val ) const {
				//return Quaternion(
				//	this->y * val.z - this->z * val.y + this->x * val.w + this->w * val.x,
				//	this->z * val.x - this->x * val.z + this->y * val.w + this->w * val.y,
				//	this->x * val.y - this->y * val.x + this->z * val.w + this->w * val.z,
				//	this->w * val.w - this->x * val.x - this->y * val.y - this->z * val.z
				//	);
				// 12 muls, 30 adds
				float E = (this->x + this->z)*(val.x +val.y);
				float F = (this->z- this->x)*(val.x - val.y);
				float G = (this->w + this->y)*(val.w - val.z);
				float H = (this->w - this->y)*(val.w + val.z);
				float A = F - E;
				float B = F + E;
				return Quaternion(
					(this->w + this->x)*(val.w + val.x) + (A - G - H) * 0.5f,
					(this->w- this->x)*(val.y+ val.z) + (B + G - H) * 0.5f,
					(this->y+ this->z)*(val.w- val.x) + (B - G + H) * 0.5f,
					(this->z - this->y)*(val.y - val.z) + (A + G + H) * 0.5f
					);
			}
			inline Quaternion& operator+=( const Quaternion& val ){ this->x+=val.x; this->y+=val.y; this->z+=val.z; this->w+=val.z; return *this;}
			inline Quaternion& operator-=( const Quaternion& val ){ this->x-=val.x; this->y-=val.y; this->z-=val.z; this->w-=val.z; return *this; }
			inline Quaternion& operator*=(const Quaternion& val){*this=(*this)*(val);return *this;}
			//*****************************************************************************
			//		float���Z�I�y���[�^
			//*****************************************************************************
			inline Quaternion operator+( f32 val )const{return Quaternion(this->x+val,this->y+val,this->z+val,this->w+val);}
			inline Quaternion operator-( f32 val )const{return Quaternion(this->x-val,this->y-val,this->z-val,this->w-val);}
			inline Quaternion operator*( f32 val )const{return Quaternion(this->x*val,this->y*val,this->z*val,this->w*val);}
			inline Quaternion operator/( f32 val )const{return Quaternion(this->x/val,this->y/val,this->z/val,this->w/val);}
			inline Quaternion& operator+=( f32 val ){ this->x+=val; this->y+=val; this->z+=val; this->w+=val; return *this; }
			inline Quaternion& operator-=( f32 val ){ this->x-=val; this->y-=val; this->z-=val; this->w-=val; return *this; }
			inline Quaternion& operator*=( f32 val ){ this->x*=val; this->y*=val; this->z*=val; this->w*=val; return *this; }
			inline Quaternion& operator/=( f32 val ){ this->x/=val; this->y/=val; this->z/=val; this->w/=val; return *this; }
			//*****************************************************************************
			//		������Z�I�y���[�^
			//*****************************************************************************
			inline Quaternion& operator=( const Vector3& val ){ this->x=val.x; this->y=val.y; this->z=val.z; this->w=0.0f; return *this; }
			inline Quaternion& operator=( const Quaternion& val ){ this->x=val.x; this->y=val.y; this->z=val.z; this->w=val.w; return *this; }
			inline Quaternion& operator=( f32 val ){ this->x=val; this->y=val; this->z=val; this->w=val; return *this; }
			//*****************************************************************************
			//		�֐�
			//*****************************************************************************
			/**
			* @brief �N�H�[�^�j�I���Ƃ̓���
			* @return ����
			*/
			inline f32 dot(const Quaternion& val) const { return this->x*val.x + this->y*val.y + this->z*val.z + this->w*val.w; }
			/**
			* @brief ������2��v�Z
			* @return ������2��
			*/
			inline f32 lengthSq() const { return this->dot(*this); }
			/**
			* @brief �����v�Z
			* @return ����
			*/
			f32 length() const;
			/**
			* @brief ���K��
			* @return �o�͌��ʂƓ���(���K���ł��Ȃ����NULL���A��)
			*/
			inline const Quaternion* normalize()
			{ 
				f32 len=this->length();
				if(len==0.0f)return NULL;
				*this /= len;
				return this;
			}
			/**
			* @brief �P�ʉ�
			* @return �o�͌��ʂƓ���
			*/
			inline const Quaternion* identity(){this->x=this->y=this->z=0.0f;this->w=1.0f;return this;}
			/**
			* @brief ������
			* @return �o�͌��ʂƓ���
			*/
			inline const Quaternion* conjugate(){ this->x=-this->x;this->y=-this->y;this->z=-this->z;return this;}
			/**
			* @brief �����N�H�[�^�j�I�����쐬
			* @return �����N�H�[�^�j�I��
			*/
			inline Quaternion createConjugate()const
			{
				Quaternion ret=*this;
				ret.x=-ret.x;ret.y=-ret.y;ret.z=-ret.z;ret.w=-ret.w;
				return ret;
			}
			/**
			* @brief �t�������Ď��g�ɑ��
			* @return �o�͌��ʂƓ���(�t�����ł��Ȃ����NULL���A��)
			*/
			inline const Quaternion* inverse()
			{
				this->conjugate();
				f32 lensq=this->lengthSq();
				*this/=lensq;
				return this;
			}
			/**
			* @brief �t���N�H�[�^�j�I�����쐬,�쐬�ł��Ȃ���΋����I������
			* @return �t���N�H�[�^�j�I��
			*/
			inline Quaternion createInverse()const
			{
				Quaternion ret=this->createConjugate();
				f32 lensq=this->lengthSq();
				ret/=lensq;
				return ret;
			}
			/**
			* @brief �N�H�[�^�j�I�����m�̍������ĐV�����N�H�[�^�j�I�����쐬����
			* ���g�̉�]��q�̉�]��������
			* @param[in] q ���������]
			* @return ��]���ʂ̃N�H�[�^�j�I��
			*/
			inline Quaternion comp(const Quaternion& q){return q*(*this);}
			/**
			* @brief �N�H�[�^�j�I�����N�H�[�^�j�I���ŉ�]
			* @param[in] q ��]�N�H�[�^�j�I��
			* @return ���g�̃|�C���^
			*/
			inline const Quaternion* rotate(const Quaternion& q){*this=q*(*this);return this;}
			/**
			* @brief �N�H�[�^�j�I�����ʐ��`��Ԃ��Ď��g�ɑ������
			* @param[in] q ��]�N�H�[�^�j�I��
			* @param[in] t �W��
			* @return ���g�̃|�C���^
			*/
			const Quaternion* sLerp(const Quaternion& q,f32 t);
			//*****************************************************************************
			//		��������֘A
			//*****************************************************************************
			/**
			* @brief �I�C���[�p����X����]�N�H�[�^�j�I���쐬
			* @param[in] rad x����]����
			* @return ���g�̃|�C���^
			*/
			const Quaternion* setRX(f32 rad);
			/**
			* @brief �I�C���[�p����Y����]�N�H�[�^�j�I���쐬
			* @param[in] rad y����]����
			* @return ���g�̃|�C���^
			*/
			const Quaternion* setRY(f32 rad);
			/**
			* @brief �I�C���[�p����Z����]�N�H�[�^�j�I���쐬
			* @param[in] rad z����]����
			* @return ���g�̃|�C���^
			*/
			const Quaternion* setRZ(f32 rad);
			/**
			* @brief �I�C���[�p����N�H�[�^�j�I�����쐬����(XYZ�I�[�_�[)
			* @param[in] x x�p����
			* @param[in] y y�p����
			* @param[in] z z�p����
			* @return ���g�̃|�C���^
			*/
			const Quaternion* setRXYZ(f32 sx,f32 sy,f32 sz);
			/**
			* @brief �I�C���[�p����N�H�[�^�j�I�����쐬����(XYZ�I�[�_�[)
			* @param[in] angle �I�C���[�p����
			* @return ���g�̃|�C���^
			*/
			inline const Quaternion* setRXYZ(const Vector3& angle){return this->setRXYZ(angle.x,angle.y,angle.z);}
			/**
			* @brief �I�C���[�p����N�H�[�^�j�I�����쐬����(ZXY�I�[�_�[)
			* @param[in] x x�p����
			* @param[in] y y�p����
			* @param[in] z z�p����
			* @return ���g�̃|�C���^
			*/
			const Quaternion* setRZXY(f32 sx,f32 sy,f32 sz);
			/**
			* @brief �I�C���[�p����N�H�[�^�j�I�����쐬����(ZXY�I�[�_�[)
			* @param[in] angle �I�C���[�p����
			* @return ���g�̃|�C���^
			*/
			inline const Quaternion* setRZXY(const Vector3& angle){return this->setRZXY(angle.x,angle.y,angle.z);}
			/**
			* @brief �C�ӂ̎��Ɖ�]�p����N�H�[�^�j�I�����쐬����
			* @param[in] axis �C�ӂ̎�
			* @param[in] rad ��]�p
			* @return ���g�̃|�C���^
			*/
			const Quaternion* setR( const Vector3 &axis, f32 rad );
			//*****************************************************************************
			//		�����擾�֘A
			//*****************************************************************************
			/**
			* @brief �N�H�[�^�j�I���̎��Ɖ�]�p�ɕ�������
			* @param[out] axis ��
			* @param[out] rad ��]�p
			*/
			void decompose( Vector3* axis, f32* rad ) const;
			/**
			* @brief �N�H�[�^�j�I�����p���x�N�g���ɕ�������
			* @param[out] right X���x�N�g��
			* @param[out] up Y���x�N�g��
			* @param[out] front Z���x�N�g��
			*/
			void decompose( Vector3* right, Vector3* up,Vector3* front) const;

			//*****************************************************************************
			//		�ϊ��֘A
			//*****************************************************************************
			/**
			* @brief �s��ɕϊ�����
			* @param[out] out ��]�s��
			* @return �o�͌��ʂƓ���
			*/
			const Matrix* toMatrix(Matrix* out)const;
			/**
			* @brief XYZ�I�[�_�[�I�C���[�p�ɕϊ�����
			* @param[out] out �I�C���[�p
			* @return �o�͌��ʂƓ���
			*/
			const Vector3* toEulerXYZ(Vector3* out)const;
			/**
			* @brief ZXY�I�[�_�[�I�C���[�p�ɕϊ�����(�X�P�[�����O����Ă���ƈӐ}�������ʂɂȂ�Ȃ�)
			* @param[out] out �I�C���[�p
			* @return �o�͌��ʂƓ���
			*/
			const Vector3* toEulerZXY(Vector3* out)const;
		};


	};
};