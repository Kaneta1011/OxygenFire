#pragma once
#include "..\..\..\types.h"
#include "..\..\..\defines.h"

namespace klib
{
	namespace math
	{
		struct Matrix;
		struct Quaternion;
		struct Vector2;
		struct Vector3;
		struct Vector4;
		struct Vector2:public	float2
		{
			inline Vector2(){this->x=this->y=0.0f;}
			inline Vector2(f32 x,f32 y){this->x=x;this->y=y;}
			inline Vector2(const Vector2& val){*this=val;}
			inline Vector2(const Vector3& val){*this=val;}
			inline Vector2(const Vector4& val){*this=val;}
			//*****************************************************************************
			//		���Z�I�y���[�^
			//*****************************************************************************
			inline Vector2 operator + () const { return Vector2(x,y); }
			inline Vector2 operator - () const { return Vector2(-x,-y); }
			//*****************************************************************************
			//		float���Z�I�y���[�^
			//*****************************************************************************
			inline Vector2 operator+(f32 val)const{return Vector2(this->x+val,this->y+val);}
			inline Vector2 operator-(f32 val)const{return Vector2(this->x-val,this->y-val);}
			inline Vector2 operator*(f32 val)const{return Vector2(this->x*val,this->y*val);}
			inline Vector2 operator/(f32 val)const{return Vector2(this->x/val,this->y/val);}
			inline Vector2& operator+=(f32 val){this->x+=val;this->y+=val;return *this;}
			inline Vector2& operator-=(f32 val){this->x-=val;this->y-=val;return *this;}
			inline Vector2& operator*=(f32 val){this->x*=val;this->y*=val;return *this;}
			inline Vector2& operator/=(f32 val){this->x/=val;this->y/=val;return *this;}
			//*****************************************************************************
			//		Vector2���Z�I�y���[�^
			//*****************************************************************************
			inline Vector2 operator+(const Vector2& val)const{return Vector2(this->x+val.x,this->y+val.y);}
			inline Vector2 operator-(const Vector2& val)const{return Vector2(this->x-val.x,this->y-val.y);}
			inline Vector2& operator+=(const Vector2& val){this->x+=val.x;this->y+=val.y;return *this;}
			inline Vector2& operator-=(const Vector2& val){this->x-=val.x;this->y-=val.y;return *this;}
			//*****************************************************************************
			//		������Z�I�y���[�^
			//*****************************************************************************
			inline Vector2& operator=(const Vector2& val){this->x=val.x;this->y=val.y;return *this;}
			inline Vector2& operator=(const Vector3& val);
			inline Vector2& operator=(const Vector4& val);
			//*****************************************************************************
			//		�֐�
			//*****************************************************************************
			/**
			* @brief �x�N�g���̓��ς����߂�
			* @param[in] vec �x�N�g��
			* @return ����
			*/
			inline f32 dot(const Vector2& vec)const{return this->x*vec.x + this->y*vec.y;}
			/**
			* @brief �x�N�g���̊O�ς����߂�
			* @param[in] vec �x�N�g��
			* @return �O��
			*/
			inline f32 cross(const Vector2& vec)const{return this->x*vec.y-vec.x*this->y;}
			/**
			* @brief �x�N�g���Ԃ̊p�x�����߂�
			* @param[in] vec �x�N�g��
			* @return �p�x
			*/
			f32 degree(const Vector2& vec)const;
			/**
			* @brief �x�N�g���̒����̓������߂�
			* @return �����̓��
			*/
			inline f32 lengthSq()const{return this->dot(*this);}
			/**
			* @brief �x�N�g���̒��������߂�
			* @return ����
			*/
			f32 length()const;
			/**
			* @brief �x�N�g���̋����̓������߂�
			* @return �����̓��
			*/
			f32 distanceSq(const Vector2& vec)const;
			/**
			* @brief �x�N�g���̋��������߂�
			* @return ����
			*/
			f32 distance(const Vector2& vec)const;
			/**
			* @brief �x�N�g���𐳋K��
			* @return ���g�̃|�C���^
			*/
			const Vector2* normalize();
			/**
			* @brief �x�N�g������`��Ԃ����g�Ɋi�[
			* @param[in] from ��ԃx�N�g��
			* @param[in] dest ��ԃx�N�g��
			* @param[in] t ��ԌW��
			* @return ���g�̃|�C���^
			*/
			const Vector2* lerp(const Vector3& from,const Vector3& dest,f32 t);
			/**
			* @brief �ˉe�x�N�g�������߂Ď��g�ɑ��
			* @param[in] from ��ԃx�N�g��
			* @param[in] dest ��ԃx�N�g��
			* @return ���g�̃|�C���^
			*/
			const Vector2* projection(const Vector3& from,const Vector3& dest);
			//*****************************************************************************
			//		��������֘A
			//*****************************************************************************
			/**
			* @brief �p�x����x�N�g�����쐬����
			* @param[in] rad �p�x
			* @return ���g�̃|�C���^
			*/
			const Vector2* setR(f32 rad);
			/**
			* @brief �x�N�g����C�ӂ̒����ɂ���
			* @param[in] len �C�ӂ̒���
			* @return ���g�̃|�C���^
			*/
			const Vector2* setLength(f32 len);
			//*****************************************************************************
			//		�ϊ��֘A
			//*****************************************************************************
			/**
			* @brief �x�N�g�����p�x�ɕϊ�����
			* @return �p�x
			*/
			f32 toEuler()const;
		};
		struct Vector3:public	float3
		{
			inline Vector3(){this->x=this->y=this->z=0.0f;}
			inline Vector3(f32 x,f32 y,f32 z){this->x=x;this->y=y;this->z=z;}
			inline Vector3(const Vector2& val){*this=val;this->z=0.0f;}
			inline Vector3(const Vector3& val){*this=val;}
			inline Vector3(const Vector4& val){*this=val;}
			inline Vector3(const Quaternion& val){*this=val;}
			//*****************************************************************************
			//		���Z�I�y���[�^
			//*****************************************************************************
			inline Vector3 operator + () const { return Vector3(x,y,z); }
			inline Vector3 operator - () const { return Vector3(-x,-y,-z); }
			//*****************************************************************************
			//		float���Z�I�y���[�^
			//*****************************************************************************
			inline Vector3 operator+(f32 val)const{return Vector3(this->x+val,this->y+val,this->z+val);}
			inline Vector3 operator-(f32 val)const{return Vector3(this->x-val,this->y-val,this->z-val);}
			inline Vector3 operator*(f32 val)const{return Vector3(this->x*val,this->y*val,this->z*val);}
			inline Vector3 operator/(f32 val)const{return Vector3(this->x/val,this->y/val,this->z/val);}
			inline Vector3& operator+=(f32 val){this->x+=val;this->y+=val;this->z+=val;return *this;}
			inline Vector3& operator-=(f32 val){this->x-=val;this->y-=val;this->z-=val;return *this;}
			inline Vector3& operator*=(f32 val){this->x*=val;this->y*=val;this->z*=val;return *this;}
			inline Vector3& operator/=(f32 val){this->x/=val;this->y/=val;this->z/=val;return *this;}
			//*****************************************************************************
			//		Vector2���Z�I�y���[�^
			//*****************************************************************************
			inline Vector3 operator+(const Vector2& val)const{return Vector3(this->x+val.x,this->y+val.y,this->z);}
			inline Vector3 operator-(const Vector2& val)const{return Vector3(this->x-val.x,this->y-val.y,this->z);}
			inline Vector3& operator+=(const Vector2& val){this->x+=val.x;this->y+=val.y;return *this;}
			inline Vector3& operator-=(const Vector2& val){this->x-=val.x;this->y-=val.y;return *this;}
			//*****************************************************************************
			//		Vector3���Z�I�y���[�^
			//*****************************************************************************
			inline Vector3 operator+(const Vector3& val)const{return Vector3(this->x+val.x,this->y+val.y,this->z+val.z);}
			inline Vector3 operator-(const Vector3& val)const{return Vector3(this->x-val.x,this->y-val.y,this->z-val.z);}
			inline Vector3& operator+=(const Vector3& val){this->x+=val.x;this->y+=val.y;this->z+=val.z;return *this;}
			inline Vector3& operator-=(const Vector3& val){this->x-=val.x;this->y-=val.y;this->z-=val.z;return *this;}
			//*****************************************************************************
			//		������Z�I�y���[�^
			//*****************************************************************************
			inline Vector3& operator=(const Vector2& val){this->x=val.x;this->y=val.y;return *this;}
			inline Vector3& operator=(const Vector3& val){this->x=val.x;this->y=val.y;this->z=val.z;return *this;}
			inline Vector3& operator=(const Vector4& val);
			inline Vector3& operator=(const Quaternion& val);
			//*****************************************************************************
			//		�֐�
			//*****************************************************************************
			/**
			* @brief �x�N�g�����m�̓���
			* @param[in] val ���ς����߂�x�N�g��
			* @return ���߂�����
			*/
			inline f32 dot(const Vector3& val)const { return this->x*val.x + this->y*val.y + this->z*val.z; }
			/**
			* @brief �x�N�g�����m�̊O�ς����߂�
			* @param[out] out ���߂��O��
			* @param[in] in �O�ς����߂�x�N�g��
			* @return �o�͌��ʂƓ���.
			*/
			const Vector3* cross(Vector3* out,const Vector3& in)const;
			/**
			* @brief �x�N�g���Ԃ̊p�x�����߂�
			* @param[in] in �p�x�����߂�x�N�g��
			* @return �x�N�g���Ԃ̊p�x
			*/
			f32 degree(const Vector3& in)const;
			/**
			* @brief �x�N�g���̒����̓������߂�
			* @return �x�N�g���̒����̓��
			*/
			inline f32 lengthSq()const{return this->dot(*this);}
			/**
			* @brief �x�N�g���̒��������߂�
			* @return �x�N�g���̒���
			*/
			f32 length()const;
			/**
			* @brief �x�N�g�����m�̋����̓������߂�
			* @return �x�N�g���̒����̓��
			*/
			f32 distanceSq(const Vector3& vec)const;
			/**
			* @brief �x�N�g�����m�̋��������߂�
			* @return �x�N�g���̒���
			*/
			f32 distance(const Vector3& vec)const;
			/**
			* @brief �x�N�g���𐳋K������
			* @return ���g�̃|�C���^
			*/
			const Vector3* normalize();
			/**
			* @brief �x�N�g�����m�̐��`��Ԃ����g�ɑ������
			* @param[in] from ��ԃx�N�g��
			* @param[in] dest ��ԃx�N�g��
			* @param[in] t ��ԌW��
			* @return ���߂��x�N�g��.
			*/
			const Vector3* lerp(const Vector3& from,const Vector3& dest,f32 t);
			/**
			* @brief �x�N�g�����m�̋��ʐ��`��Ԃ����g�ɑ������
			* @param[in] from ��ԃx�N�g��
			* @param[in] dest ��ԃx�N�g��
			* @param[in] t ��ԌW��
			* @return ���߂��x�N�g��.
			*/
			const Vector3* sLerp(const Vector3& from,const Vector3& dest,f32 t);
			/**
			* @brief �x�N�g�����N�H�[�^�j�I���ŉ�]�����Ď��g�ɑ������
			* @param[in] q �N�H�[�^�j�I��
			* @return ���g�̃|�C���^
			*/
			const Vector3* rotate(const Quaternion& q);
			/**
			* @brief �x�N�g����C�ӂ̎��Ɗp�x�ŉ�]�����Ď��g�ɑ������
			* @param[in] axis �C�ӂ̎�
			* @param[in] r ��]�p
			* @return ���g�̃|�C���^
			*/
			const Vector3* rotate(const Vector3& axis,f32 r);
			/**
			* @brief �x�N�g����(x,y,z,1)�Ƃ��čs��ϊ����Ď��g�ɑ������
			* @param[in] m �s��
			* @return ���g�̃|�C���^
			*/
			const Vector3* trans(const Matrix& m);
			/**
			* @brief �x�N�g����(x,y,z,0)�Ƃ��čs��ϊ����Ď��g�ɑ������
			* @param[in] m �s��
			* @return ���g�̃|�C���^
			*/
			const Vector3* trans3x3(const Matrix& m);
			/**
			* @brief ���˃x�N�g�����i�[����
			* @param[in] front �i�s�x�N�g��
			* @param[in] normal �@���x�N�g��
			* @return ���g�̃|�C���^
			*/
			const Vector3* reflect(const Vector3& front,const Vector3& normal);
			/**
			* @brief ���܃x�N�g�����i�[����
			* @param[in] front �i�s�x�N�g��
			* @param[in] normal �@���x�N�g��
			* @param[in] Eta ���ܗ�
			* @return ���g�̃|�C���^
			*/
			const Vector3* refract(const Vector3& front ,const Vector3& normal , f32 Eta);
			/**
			* @brief �ˉe�x�N�g�����i�[����
			* @param[in] from �ˉe����x�N�g��
			* @param[in] dest �ˉe�����x�N�g��
			* @return ���g�̃|�C���^
			*/
			const Vector3* projection(const Vector3& from,const Vector3& dest);

			//*****************************************************************************
			//		��������֘A
			//*****************************************************************************
			/**
			* @brief �x�N�g�����m�̊O�ς����߂��㎩�g�ɑ������
			* @param[in] in �O�ς����߂�x�N�g��
			* @return ���g�̃|�C���^
			*/
			const Vector3* setCross(const Vector3& in);
			/**
			* @brief �p�ƕ��ʊp����x�N�g�������߂�
			* @param[in] pitch �s�b�`�p(X��)
			* @param[in] yaw ���[�p(Y��)
			* @return ���߂��x�N�g��.
			*/
			const Vector3* setR( f32 pitch, f32 yaw );
			/**
			* @brief �x�N�g����C�ӂ̒����ɂ���
			* @param[in] len �C�ӂ̒���
			* @return ���g�̃|�C���^
			*/
			const Vector3* setLength(f32 len);
			//*****************************************************************************
			//		�ϊ��֘A
			//*****************************************************************************
			/**
			* @brief �x�N�g�����p�ƕ��ʊp�ɕϊ�����
			* @param[out] out ���߂��p�x
			* @return ���߂��p�x.
			*/
			const Vector3* toEuler( Vector3* out )const;
		};
		struct Vector4:public	float4
		{
			inline Vector4(){this->x=this->y=this->z=this->w=0.0f;}
			inline Vector4(f32 x,f32 y,f32 z,f32 w){this->x=x;this->y=y;this->z=z;this->w=w;}
			inline Vector4(const Vector2& val){*this=val;this->z=this->w=0.0f;}
			inline Vector4(const Vector3& val){*this=val;this->w=0.0f;}
			inline Vector4(const Vector4& val){*this=val;}
			inline Vector4(const Quaternion& val){*this=val;}
			//*****************************************************************************
			//		���Z�I�y���[�^
			//*****************************************************************************
			inline Vector4 operator + () const { return Vector4(x,y,z,w); }
			inline Vector4 operator - () const { return Vector4(-x,-y,-z,-w); }
			//*****************************************************************************
			//		float���Z�I�y���[�^
			//*****************************************************************************
			inline Vector4 operator+(f32 val)const{return Vector4(this->x+val,this->y+val,this->z+val,this->w+val);}
			inline Vector4 operator-(f32 val)const{return Vector4(this->x-val,this->y-val,this->z-val,this->w-val);}
			inline Vector4 operator*(f32 val)const{return Vector4(this->x*val,this->y*val,this->z*val,this->w*val);}
			inline Vector4 operator/(f32 val)const{return Vector4(this->x/val,this->y/val,this->z/val,this->w/val);}
			inline Vector4& operator+=(f32 val){this->x+=val;this->y+=val;this->z+=val;this->w+=val;return *this;}
			inline Vector4& operator-=(f32 val){this->x-=val;this->y-=val;this->z-=val;this->w-=val;return *this;}
			inline Vector4& operator*=(f32 val){this->x*=val;this->y*=val;this->z*=val;this->w*=val;return *this;}
			inline Vector4& operator/=(f32 val){this->x/=val;this->y/=val;this->z/=val;this->w/=val;return *this;}
			//*****************************************************************************
			//		Vector2���Z�I�y���[�^
			//*****************************************************************************
			inline Vector4 operator+(const Vector2& val)const{return Vector4(this->x+val.x,this->y+val.y,this->z,this->w);}
			inline Vector4 operator-(const Vector2& val)const{return Vector4(this->x-val.x,this->y-val.y,this->z,this->w);}
			inline Vector4& operator+=(const Vector2& val){this->x+=val.x;this->y+=val.y;return *this;}
			inline Vector4& operator-=(const Vector2& val){this->x-=val.x;this->y-=val.y;return *this;}
			//*****************************************************************************
			//		Vector3���Z�I�y���[�^
			//*****************************************************************************
			inline Vector4 operator+(const Vector3& val)const{return Vector4(this->x+val.x,this->y+val.y,this->z+val.z,this->w);}
			inline Vector4 operator-(const Vector3& val)const{return Vector4(this->x-val.x,this->y-val.y,this->z-val.z,this->w);}
			inline Vector4& operator+=(const Vector3& val){this->x+=val.x;this->y+=val.y;this->z+=val.z;return *this;}
			inline Vector4& operator-=(const Vector3& val){this->x-=val.x;this->y-=val.y;this->z-=val.z;return *this;}
			//*****************************************************************************
			//		Vector4���Z�I�y���[�^
			//*****************************************************************************
			inline Vector4 operator+(const Vector4& val)const{return Vector4(this->x+val.x,this->y+val.y,this->z+val.z,this->w+val.w);}
			inline Vector4 operator-(const Vector4& val)const{return Vector4(this->x-val.x,this->y-val.y,this->z-val.z,this->w-val.w);}
			inline Vector4& operator+=(const Vector4& val){this->x+=val.x;this->y+=val.y;this->z+=val.z;this->w+=val.w;return *this;}
			inline Vector4& operator-=(const Vector4& val){this->x-=val.x;this->y-=val.y;this->z-=val.z;this->w-=val.w;return *this;}
			//*****************************************************************************
			//		Matrix���Z�I�y���[�^
			//*****************************************************************************
			//*****************************************************************************
			//		������Z�I�y���[�^
			//*****************************************************************************
			inline Vector4& operator=(const Vector2& val){this->x=val.x;this->y=val.y;return *this;}
			inline Vector4& operator=(const Vector3& val){this->x=val.x;this->y=val.y;this->z=val.z;return *this;}
			inline Vector4& operator=(const Vector4& val){this->x=val.x;this->y=val.y;this->z=val.z;this->w=val.w;return *this;}
			inline Vector4& operator=(const Quaternion& val);
			//*****************************************************************************
			//		�֐�
			//*****************************************************************************
			/**
			* @brief �x�N�g���̓��ς����߂�
			* @return ����
			*/
			inline f32 dot(const Vector4& val)const { return this->x*val.x + this->y*val.y + this->z*val.z + this->w*val.w; }
			/**
			* @brief �x�N�g���̒����̓������߂�
			* @return �����̓��
			*/
			inline f32 lengthSq()const{return this->dot(*this);}
			/**
			* @brief �x�N�g���̒��������߂�
			* @return ����
			*/
			f32 length()const;
			/**
			* @brief �x�N�g���𐳋K������
			* @return ���g�̃|�C���^
			*/
			const Vector4* normalize();
			/**
			* @brief �x�N�g������`��Ԃ���
			* @param[in] from ��ԃx�N�g��
			* @param[in] dest ��ԃx�N�g��
			* @param[in] t ��ԌW��
			* @return �o�͌��ʂƓ���
			*/
			const Vector4* lerp(const Vector4& from,const Vector4& dest,f32 t);
			/**
			* @brief �x�N�g�����s��ϊ�����
			* @param[in] m �s��
			* @return ���g�̃|�C���^
			*/
			const Vector4* trans(const Matrix& m);
		};
	};
};