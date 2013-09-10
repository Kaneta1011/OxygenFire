#ifndef _K_MATH_F_
#define _K_MATH_F_

#include <math.h>
#include "..\types.h"
#include "..\defines.h"
#include "Class\Matrix\Matrix.h"
#include "Class\Quaternion\Quaternion.h"
#include "Class\Vector\Vector.h"

namespace klib
{
	namespace math
	{

		//*****************************************************************************
		//
		//		���w�֘A
		//
		//*****************************************************************************
		//�~����
#define K_PI			(3.1415926535897932384626433832795f)
		//�~����*2
#define K_PI2			(6.283185307179586476925286766559f)
		//1���W�A�����b
#define K_RAD		(57.295779513082320876798154814105f)
		//1�x���b
#define K_DEG		(0.01745329251994329576923690768489f)
		/**
		* @brief ��Βl�擾�֐�
		* @param[int] _X �X�J���[
		*/
		inline f32 kfabsf(f32 _X){return fabsf(_X);}
		/**
		* @brief �T�C���֐�
		* @param[int] _X �X�J���[
		*/
		inline f32 ksinf(f32 _X){return sinf(_X);}
		/**
		* @brief �R�T�C���֐�
		* @param[int] _X �X�J���[
		*/
		inline f32 kcosf(f32 _X){return cosf(_X);}
		/**
		* @brief �^���W�F���g�֐�
		* @param[int] _X �X�J���[
		*/
		inline f32 ktanf(f32 _X){return tanf(_X);}
		/**
		* @brief �R�^���W�F���g�֐�
		* @param[int] _X �X�J���[
		*/
		inline f32 kcotf(f32 _X){return 1.0f/tanf(_X);}
		/**
		* @brief ��Βl�擾�֐�
		* @param[int] _X �X�J���[
		*/
		inline f32 kasinf(f32 _X){return asinf(_X);}
		/**
		* @brief ��Βl�擾�֐�
		* @param[int] _X �X�J���[
		*/
		inline f32 kacosf(f32 _X){return acosf(_X);}
		/**
		* @brief ��Βl�擾�֐�
		* @param[int] _X �X�J���[
		*/
		inline f32 katanf(f32 _X){return atanf(_X);}
		/**
		* @brief ��Βl�擾�֐�
		* @param[int] _X �X�J���[
		*/
		inline f32 katan2f(f32 _Y,f32 _X){return atan2f(_Y,_X);}
		/**
		* @brief ��Βl�擾�֐�
		* @param[int] _X �X�J���[
		*/
		inline f32 ksqrtf(f32 _X){return sqrtf(_X);}
		/**
		* @brief ���������_�̏�]�v�Z
		* @param[in] X �����鐔
		* @param[in] Y ���鐔
		*/
		template<typename T>
		inline T kmod( T X, T Y ){return ( X - int( X / Y ) * Y );}
		/**
		* @brief 32bit�����^v��b�`e�̊ԂŌJ��Ԃ�
		* @param[in] b �X�J���[
		* @param[in] e �X�J���[
		* @param[in] v �X�J���[
		* @return ���b�v�A���E���h���ꂽ�l
		*/
		inline s32 kwrapi(s32 b,s32 e,s32 v)
		{
			s32 dist=e-b;
			v-=b;
			v-=(v/dist)*dist;
			return v+(v>>31 & dist)+b;
		}
		/**
		* @brief 32bit���������_�^v��b�`e�̊ԂŌJ��Ԃ�
		* @param[in] b �X�J���[
		* @param[in] e �X�J���[
		* @param[in] v �X�J���[
		* @return ���b�v�A���E���h���ꂽ�l
		*/
		inline f32 kwrapf( f32 b, f32 e,f32 v )  
		{  
			f32 dist = e - b;  
			v -= b;  
			v -= s32( v / dist ) * dist;  
			s32 ofs = ( *(s32*)&v >> 31 ) & *(s32*)&dist;  
			return v + *(f32*)&ofs + b;  
		}
		/**
		* @brief 64bit�����^v��b�`e�̊ԂŌJ��Ԃ�
		* @param[in] b �X�J���[
		* @param[in] e �X�J���[
		* @param[in] v �X�J���[
		* @return ���b�v�A���E���h���ꂽ�l
		*/
		inline s64 kwrapl(s64 b,s64 e,s64 v)
		{
			s64 dist=e-b;
			v-=b;
			v-=(v/dist)*dist;
			return v+(v>>63 & dist)+b;
		}
		/**
		* @brief 64bit���������_�^v��b�`e�̊ԂŌJ��Ԃ�
		* @param[in] b �X�J���[
		* @param[in] e �X�J���[
		* @param[in] v �X�J���[
		* @return ���b�v�A���E���h���ꂽ�l
		*/
		inline f64 kwrap( f64 b, f64 e,f64 v )  
		{  
			f64 dist = e - b;  
			v -= b;  
			v -= s64( v / dist ) * dist;  
			s64 ofs = ( *(s64*)&v >> 63 ) & *(s64*)&dist;  
			return v + *(f64*)&ofs + b;  
		}
		/**
		* @brief V��Min�`Max�̊ԂŃN�����v
		* @param[in] Min �ŏ��l
		* @param[in] Max �ő�l
		* @param[in] V �l
		* @return �N�����v���ꂽ�l
		*/
		inline f32 kclampf(  f32 Min, f32 Max ,f32 V )  
		{  
			f32 d1 = Min - V;  
			f32 d2 = V - Max;  
			s32 r = ( *(s32*)&V ^ *(s32*)&Min ) & ( *(s32*)&d1 >> 31 );  
			r ^= *(s32*)&Min;  
			r  = ( ( r ^ *(s32*)&Max ) & ( *(s32*)&d2 >> 31 ) ) ^ *(s32*)&Max;  
			return *(f32*)&r;  
		} 
		/**
		* @brief V��Min�`Max�̊ԂŃN�����v
		* @param[in] Min �ŏ��l
		* @param[in] Max �ő�l
		* @param[in] V �l
		* @return �N�����v���ꂽ�l
		*/
		inline void kclampf(  f32 Min, f32 Max ,f32* V )  
		{  
			f32 d1 = Min - *V;  
			f32 d2 = *V - Max;  
			s32 r = ( *(s32*)V ^ *(s32*)&Min ) & ( *(s32*)&d1 >> 31 );  
			r ^= *(s32*)&Min;  
			r  = ( ( r ^ *(s32*)&Max ) & ( *(s32*)&d2 >> 31 ) ) ^ *(s32*)&Max;  
			*V = *(f32*)&r;  
		} 

		/**
		* @brief 0�`1�̊ԂɃN�����v
		* @param[in] _X �X�J���[
		* @return �N�����v���ꂽ�l
		*/
		inline f32 ksaturatef( const f32& _X )
		{
			long l = *(long*)&_X;
			l &= ( (long)( l ^ 0x80000000 ) >> 31 );
			l -= 0x3F800000;
			l &= l >> 31;
			l += 0x3F800000;
			return *(f32*)&l;
		}
		/**
		* @brief 0�`1�̊ԂɃN�����v
		* @param[input] _X �X�J���[
		*/
		inline void ksaturatef( f32* _X )
		{
			long l = *(long*)&_X;
			l &= ( (long)( l ^ 0x80000000 ) >> 31 );
			l -= 0x3F800000;
			l &= l >> 31;
			l += 0x3F800000;
			*_X=*(f32*)&l;
		}
		template<typename T>
		inline T kquadin(T t)
		{
			return t*t;
		}
		template<typename T>
		inline T kquadout(T t)
		{
			return t*(2.0f - t);
		}
		template<typename T>
		inline T kcube(T t)
		{
			return  t * t * ( 3.0f - 2.0f * t );   
		}
		/**
		* @brief min�`max�̊Ԃ���`���
		* @param[int] _X �X�J���[
		*/
		template<typename T,typename U>
		inline T klerp(T min, T max, U t)
		{
			return min + (max - min) * t;
		}
		template<typename T,typename U>
		inline T kbezier(T p0,T p1,T p2,T p3,U t)
		{
			f32 minusT=1.0f-t;
			f32 minusT2=minusT*minusT;
			f32 t2=t*t;
			return p0 * (minusT2) * (minusT)+ p1 * (3.0f * (minusT2) * t )+ p2* (3.0f * (minusT) * t2) + p3 * (t2* t);
		}
		/**
		* @brief ���W�A�����p�x�ɕϊ����܂�
		* @param[int] rad ���W�A���p
		* @return �p�x
		*/
		inline f32 krtodf(f32 rad){return rad*K_RAD;}
		/**
		* @brief �p�x�����W�A���ɕϊ����܂�
		* @param[int] deg �p�x
		* @return ���W�A��
		*/
		inline f32 kdtorf(f32 deg){return deg*K_DEG;}
		//*****************************************************************************
		//
		//		Vector2�֘A
		//
		//*****************************************************************************
		//*****************************************************************************
		//		�ϊ��֘A
		//*****************************************************************************
		inline f32 Vector2toEuler(const Vector2& vec)
		{
			return kacosf(vec.x);
		}
		//*****************************************************************************
		//		�֐��֘A
		//*****************************************************************************
		inline f32 Vector2Dot(const Vector2& from,const Vector2& dest){return from.x*dest.x + from.y*dest.y;}
		inline f32 Vector2Cross(const Vector2& from,const Vector2& dest){return from.x*dest.y-dest.x*from.y;}
		inline f32 Vector2LengthSq(const Vector2& vec){return Vector2Dot(vec,vec);}
		inline f32 Vector2Length(const Vector2& vec){return ksqrtf(Vector2LengthSq(vec));}
		/**
		* @brief �x�N�g����C�ӂ̒����ŕ�������
		* @param[out] out ���ʃx�N�g��
		* @param[in] in �x�N�g��
		* @param[in] len �C�ӂ̒���
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector2* Vector2SetLength(Vector2* out,const Vector2& in,f32 len)
		{
			*out=(in/Vector2Length(in))*len;
			return out;
		}
		inline f32 Vector2DistanceSq(const Vector2& from,const Vector2& dest){return Vector2LengthSq(dest-from);}
		inline f32 Vector2Distance(const Vector2& from,const Vector2& dest){return Vector2Length(dest-from);}
		inline const Vector2* Vector2Normalize(Vector2* out,const Vector2& vec)
		{
			f32 len=Vector2Length(vec);
			*out=vec/len;
			return out;
		}
		/**
		* @brief �x�N�g�����m�����p�x�����߂�
		* @param[in] from �p�x�����߂邱����̃x�N�g��.
		* @param[in] dest �p�x�����߂鑊��x�N�g��.
		* @return �p�x.
		*/
		inline f32 Vector2Degree(const Vector2& from,const Vector2& dest)
		{
			float cos_Theta = Vector2Dot( from,dest ) / Vector2Length(from) * Vector2Length(dest);
			kclampf( -1.0f, 1.0f ,&cos_Theta);
			float angle = kacosf( cos_Theta );
			return angle;
		}
		/**
		* @brief �x�N�g�����m����`��Ԃ���
		* @param[out] out ��Ԃ��ꂽ�x�N�g��
		* @param[in] from ��Ԃ��邱����̃x�N�g��
		* @param[in] dest �⊮���鑊��̃x�N�g��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector2* Vector2Lerp(Vector2* out,const Vector2& from,const Vector2& dest,f32 t)
		{
			out->x=klerp(from.x,dest.x,t);
			out->y=klerp(from.y,dest.y,t);
			return out;
		}
		inline f32 Vector2ProjectionLength(const Vector2& from,const Vector2& dest){return Vector2Dot(from,dest)/Vector2Length(dest);}
		inline const Vector2* Vector2Projection(Vector2* out,const Vector2& from,const Vector2& dest)
		{
			*out = dest*(Vector2Dot(from,dest)/Vector2LengthSq(dest));
			return out;
		}
		//*****************************************************************************
		//		�x�N�g���쐬�֘A
		//*****************************************************************************
		inline const Vector2* Vector2fromEuler(Vector2* out,f32 r)
		{
			out->x=kcosf(r);
			out->y=ksinf(r);
			return out;
		}
		//*****************************************************************************
		//
		//		Vector3�֘A
		//
		//*****************************************************************************
		//*****************************************************************************
		//		�ϊ��֘A
		//*****************************************************************************
		/**
		* @brief �x�N�g������p(pitch�p)�ƕ��ʊp(yaw�p)�����߂�(�X�P�[�����O����Ă���ƈӐ}�������ʂɂȂ�Ȃ�)
		* @param[out] out ���߂��p�x
		* @param[in] in �ϊ�����x�N�g��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* Vector3toEuler( Vector3* out , const Vector3& in )
		{
			float len;
			// �x�N�g���̒��������߂�
			len = ksqrtf( in.x*in.x + in.y*in.y + in.z*in.z );
			// �x�N�g���𐳋K������
			float X = in.x / len;
			float Y = in.y / len;
			float Z = in.z / len;
			// �s�b�`�̌v�Z���s��
			out->x=kasinf(-Y);
			// ���[�̌v�Z���s��
			out->y=katan2f( X, Z );
			return out;
		}
		//*****************************************************************************
		//		�֐��֘A
		//*****************************************************************************
		/**
		* @brief �x�N�g���̊O�ς����߂�
		* @param[out] out �x�N�g���̊O��
		* @param[in] from �O�ς����߂邱����̃x�N�g��
		* @param[in] dest �O�ς����߂鑊��̃x�N�g��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* Vector3Cross( Vector3* out, const Vector3& from, const Vector3& dest )
		{
			Vector3 work=from;
			out->x = work.y*dest.z - work.z*dest.y;
			out->y = work.z*dest.x - work.x*dest.z;
			out->z = work.x*dest.y - work.y*dest.x;
			return out;
		}
		/**
		* @brief �x�N�g���̓��ς����߂�
		* @param[in] from ���ς����߂邱����̃x�N�g��
		* @param[in] dest ���ς����߂鑊��̃x�N�g��
		* @return �x�N�g���̓���
		*/
		inline f32 Vector3Dot(const Vector3& from,const Vector3& dest)
		{
			return from.x*dest.x + from.y*dest.y + from.z*dest.z;
		}
		/**
		* @brief �x�N�g���̒����̓������߂�
		* @param[in] val ���������߂�x�N�g��
		* @return �x�N�g���̒����̓��
		*/
		inline f32 Vector3LengthSq(const Vector3& val)
		{
			return Vector3Dot(val,val);
		}
		/**
		* @brief �x�N�g���̒��������߂�
		* @param[in] val ���������߂�x�N�g��
		* @return �x�N�g���̒���
		*/
		inline f32 Vector3Length(const Vector3& val)
		{
			return ksqrtf(Vector3LengthSq(val));
		}
		/**
		* @brief �x�N�g����C�ӂ̒����ŕ�������
		* @param[out] out ���ʃx�N�g��
		* @param[in] in �x�N�g��
		* @param[in] len �C�ӂ̒���
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* Vector3SetLength(Vector3* out,const Vector3& in,f32 len)
		{
			*out=(in/Vector3Length(in))*len;
			return out;
		}
		/**
		* @brief �x�N�g���̋����̓������߂�
		* @param[in] from ���������߂�x�N�g��
		* @param[in] dest ���������߂�x�N�g��
		* @return �x�N�g���̋����̓��
		*/
		inline f32 Vector3DistanceSq(const Vector3& from,const Vector3& dest)
		{
			return Vector3LengthSq(dest-from);
		}
		/**
		* @brief �x�N�g���̋��������߂�
		* @param[in] from ���������߂�x�N�g��
		* @param[in] dest ���������߂�x�N�g��
		* @return �x�N�g���̋���
		*/
		inline f32 Vector3Distance(const Vector3& from,const Vector3& dest)
		{
			return Vector3Length(dest-from);
		}
		/**
		* @brief �x�N�g���̒��������߂�
		* @param[in] val ���������߂�x�N�g��
		* @return �x�N�g���̒���
		*/
		inline const Vector3* Vector3Normalize(Vector3* out,const Vector3& val)
		{
			f32 len=Vector3Length(val);
			if(len != 0 ){
			*out=val/len;
			}
			return out;
		}
		/**
		* @brief �x�N�g�����m�����p�x�����߂�
		* @param[in] from �p�x�����߂邱����̃x�N�g��.
		* @param[in] dest �p�x�����߂鑊��x�N�g��.
		* @return �p�x.
		*/
		inline f32 Vector3Degree(const Vector3& from,const Vector3& dest)
		{
			float cos_Theta = Vector3Dot( from,dest ) / Vector3Length(from) * Vector3Length(dest);
			kclampf( -1.0f, 1.0f ,&cos_Theta);
			float angle = kacosf( cos_Theta );
			return angle;
		}
		/**
		* @brief �x�N�g�����m����`��Ԃ���
		* @param[out] out ��Ԃ��ꂽ�x�N�g��
		* @param[in] from ��Ԃ��邱����̃x�N�g��
		* @param[in] dest �⊮���鑊��̃x�N�g��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* Vector3Lerp(Vector3* out,const Vector3& from,const Vector3& dest,f32 t)
		{
			out->x=klerp(from.x,dest.x,t);
			out->y=klerp(from.y,dest.y,t);
			out->z=klerp(from.z,dest.z,t);
			return out;
		}
		/**
		* @brief �x�N�g�����m�����ʐ��`��Ԃ���
		* @param[out] out ��Ԃ��ꂽ�x�N�g��
		* @param[in] from ��Ԃ��邱����̃x�N�g��
		* @param[in] dest �⊮���鑊��̃x�N�g��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* Vector3Slerp(Vector3* out,const Vector3& from,const Vector3& dest,f32 t)
		{
			// 2�x�N�g���Ԃ̊p�x�i�s�p���j
			f32 angle = kacosf( Vector3Dot( from, dest ) );
			// sin��
			f32 SinTh = ksinf( angle );
			// ��ԌW��
			f32 Ps = ksinf( angle * ( 1.0f - t ) );
			f32 Pe = ksinf( angle * t );
			*out = ( from  * Ps + dest * Pe ) / SinTh;
			// �ꉞ���K�����ċ��ʐ��`��Ԃ�
			return Vector3Normalize(out,*out);
		}

		/**
		* ���˃x�N�g�������߂� �D
		* @param[out] out ���˃x�N�g��
		* @param[in] front �i�s�x�N�g��
		* @param [in]  normal �@���x�N�g�� �D
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* Vector3Reflect( Vector3* out,const Vector3& front ,const Vector3& normal )
		{
			Vector3 v( -front );
			Vector3 t( normal * ( Vector3Dot(v,normal) * 2.0f ) );
			*out = t + front;
			return out;
		}

		/**
		* ���܃x�N�g�������߂�D
		* @param[out] out ���܃x�N�g��
		* @param[in] front �i�s�x�N�g��
		* @param [in]  normal �@���x�N�g�� �D
		* @param[in] Eta ���ܗ�
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* Vector3Refract( Vector3* out,const Vector3& front ,const Vector3& normal , f32 Eta )
		{
			Vector3 T1( -front );
			float cosI  = Vector3Dot(T1,normal);
			float cosT2 = 1.0f - Eta * Eta * ( 1.0f - cosI * cosI );
			if( cosT2 <= 0.0f ){
				out->x = 0.0f;
				out->y = 0.0f;
			} else {
				T1 = front * Eta;
				Vector3 T2( normal * ( Eta * cosI - ksqrtf( kfabsf( cosT2 ) ) ) );
				*out = T1 + T2;
			}
			return out;
		}
		/**
		* @brief �ˉe�x�N�g���̒��������߂�
		* @param[in] from �ˉe����x�N�g��
		* @param[in] dest �ˉe�����x�N�g��
		* @return �ˉe�x�N�g���̒���
		*/
		inline f32 Vector3ProjectionLength(const Vector3& from,const Vector3& dest){return Vector3Dot(from,dest)/Vector3Length(dest);}
		/**
		* @brief �ˉe�x�N�g�������߂�
		* @param[out] out �ˉe�x�N�g��
		* @param[in] from �ˉe����x�N�g��
		* @param[in] dest �ˉe�����x�N�g��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* Vector3Projection(Vector3* out,const Vector3& from,const Vector3& dest)
		{
			*out = dest*(Vector3Dot(from,dest)/Vector3LengthSq(dest));
			return out;
		}
		//*****************************************************************************
		//		�x�N�g���쐬�֘A
		//*****************************************************************************
		/**
		* @brief �p(pitch�p)�ƕ��ʊp(yaw�p)����x�N�g�������߂�
		* @param[out] out ���߂��x�N�g��
		* @param[in] pitch �s�b�`�p(X��)
		* @param[in] yaw ���[�p(Y��)
		* @return ���߂��x�N�g��.
		*/
		inline const Vector3* Vector3fromPitchYaw( Vector3* out , f32 pitch , f32 yaw)
		{
			out->x = kcosf( pitch ) * ksinf( yaw );
			out->y = -ksinf( pitch );
			out->z = kcosf( pitch ) * kcosf( yaw );
			return out;
		}
		//*****************************************************************************
		//
		//		Vector4�֘A
		//
		//*****************************************************************************
		//*****************************************************************************
		//		�֐��֘A
		//*****************************************************************************
		/**
		* @brief �x�N�g���̓��ς����߂�
		* @param[in] from ���ς����߂邱����̃x�N�g��
		* @param[in] dest ���ς����߂鑊��̃x�N�g��
		* @return �x�N�g���̓���
		*/
		inline f32 Vector4Dot(const Vector4& from,const Vector4& dest){ return from.x*dest.x + from.y*dest.y + from.z*dest.z + from.w*dest.w; }
		/**
		* @brief �x�N�g���̒����̓������߂�
		* @param[in] vec �����̓������߂�x�N�g��
		* @return �x�N�g���̒����̓��
		*/
		inline f32 Vector4LengthSq(const Vector4& vec){return Vector4Dot(vec,vec);}
		/**
		* @brief �x�N�g���̒��������߂�
		* @param[in] vec ���������߂�x�N�g��
		* @return �x�N�g���̒���
		*/
		inline f32 Vector4Length(const Vector4& vec){return ksqrtf(Vector4LengthSq(vec));}
		/**
		* @brief �x�N�g���𐳋K������
		* @param[out] out ���K�������x�N�g��
		* @param[in] vec ���K������x�N�g��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector4* Vector4Normalize(Vector4* out,const Vector4& vec)
		{
			f32 len=Vector4Length(vec);
			*out=vec/len;
			return out;
		}
		/**
		* @brief �x�N�g�����m����`��Ԃ���
		* @param[out] out ��Ԃ��ꂽ�x�N�g��
		* @param[in] from ��Ԃ��邱����̃x�N�g��
		* @param[in] dest �⊮���鑊��̃x�N�g��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector4* Vector4Lerp(Vector4* out,const Vector4& from,const Vector4& dest,f32 t)
		{
			out->x=klerp(from.x,dest.x,t);
			out->y=klerp(from.y,dest.y,t);
			out->z=klerp(from.z,dest.z,t);
			out->w=klerp(from.w,dest.w,t);
			return out;
		}
		//*****************************************************************************
		//
		//		�s��֘A
		//
		//*****************************************************************************
		//*****************************************************************************
		//		�ϊ��֘A
		//*****************************************************************************
		/**
		* @brief �s����N�H�[�^�j�I���ɕϊ�����(�X�P�[�����O����Ă���ƈӐ}�������ʂɂȂ�Ȃ�)
		* @param[out] q ��]�N�H�[�^�j�I��
		* @param[in] mat ��]�s��
		* @return �o�͌��ʂƓ���
		*/
		inline const Quaternion* MatrixToQuaternion(Quaternion* q,const Matrix& mat)
		{
			// �ő听��������
			f32 elem[ 4 ]; // 0:x, 1:y, 2:z, 3:w
			elem[ 0 ] = mat._11 - mat._22 - mat._33 + 1.0f;
			elem[ 1 ] = -mat._11 + mat._22 - mat._33 + 1.0f;
			elem[ 2 ] = -mat._11 - mat._22 + mat._33 + 1.0f;
			elem[ 3 ] = mat._11 + mat._22 + mat._33 + 1.0f;

			unsigned biggestIndex = 0;
			for ( int i = 1; i < 4; i++ ) {
				if ( elem[i] > elem[biggestIndex] )
					biggestIndex = i;
			}

			// �ő�v�f�̒l���Z�o
			f32 v = ksqrtf( elem[biggestIndex] ) * 0.5f;
			(*q)[biggestIndex]= v;
			f32 mult = 0.25f / v;

			switch ( biggestIndex ) {
			case 0: // x
				(*q)[1] = (mat._12 + mat._21) * mult;
				(*q)[2] = (mat._31 + mat._13) * mult;
				(*q)[3] = (mat._23 - mat._32) * mult;
				break;
			case 1: // y
				(*q)[0] = (mat._12 + mat._21) * mult;
				(*q)[2] = (mat._23 + mat._32) * mult;
				(*q)[3] = (mat._31 - mat._13) * mult;
				break;
			case 2: // z
				(*q)[0] = (mat._31 + mat._13) * mult;
				(*q)[1] = (mat._23 + mat._32) * mult;
				(*q)[3] = (mat._12 - mat._21) * mult;
				break;
			case 3: // w
				(*q)[0] = (mat._23 - mat._32) * mult;
				(*q)[1] = (mat._31 - mat._13) * mult;
				(*q)[2] = (mat._12 - mat._21) * mult;
				break;
			}
			return q;
		}
		/**
		* @brief �s�񂩂�I�C���[�p�ɕϊ�(XYZ�I�[�_�[�s���p)(�X�P�[�����O����Ă���ƈӐ}�������ʂɂȂ�Ȃ�)
		* @param[out] out �I�C���[�p
		* @param[in] mat ��]�s��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* MatrixToEulerXYZ(Vector3* out,const Matrix& mat)
		{
			out->y = kasinf(-mat._13);
			f32 C = kcosf(out->y);

			//cos(��)��0�łȂ����
			if (kfabsf(C) > FEQ_EPS) {
				out->x=(mat._33<0)?K_PI-kasinf(mat._23/C):kasinf(mat._23/C);
				out->z=katan2f(mat._12,mat._11);
			}
			//cos(��)��0�Ȃ��
			else
			{
				out->x=0.0f;
				out->z=katan2f(-mat._21,mat._22);
			}

			return out;
		}
		/**
		* @brief �s�񂩂�I�C���[�p�ɕϊ�(ZXY�I�[�_�[�s���p)(�X�P�[�����O����Ă���ƈӐ}�������ʂɂȂ�Ȃ�)
		* @param[out] out �I�C���[�p
		* @param[in] mat ��]�s��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* MatrixToEulerZXY(Vector3* out,const Matrix& mat)
		{
			out->x = kasinf(-mat._32);
			f32 C = kcosf(out->x);

			//cos(��)��0�łȂ����
			if (kfabsf(C) > FEQ_EPS) {
				out->z=(mat._22<0)?K_PI-kasinf(mat._12/C):kasinf(mat._12/C);
				out->y=katan2f(mat._31,mat._33);
			}
			//cos(��)��0�Ȃ��
			else
			{
				out->z=0.0f;
				out->y=katan2f(-mat._13,mat._11);
			}

			return out;
		}
		//*****************************************************************************
		//		�֐��֘A
		//*****************************************************************************
		/**
		* @brief 4x4�s��̍s�񎮂̌v�Z
		* @param[in] mat ���̍s��
		* @return �s�񎮂̒l
		*/
		inline f32 MatrixDet(const Matrix& mat)
		{
			return mat._11*mat._22*mat._33*mat._44+mat._11*mat._23*mat._34*mat._42+mat._11*mat._24*mat._32*mat._43
				+mat._12*mat._21*mat._34*mat._43+mat._12*mat._23*mat._31*mat._44+mat._12*mat._24*mat._33*mat._41
				+mat._13*mat._21*mat._32*mat._44+mat._13*mat._22*mat._34*mat._41+mat._13*mat._24*mat._31*mat._42
				+mat._14*mat._21*mat._33*mat._42+mat._14*mat._22*mat._31*mat._43+mat._14*mat._23*mat._32*mat._41
				-mat._11*mat._22*mat._34*mat._43-mat._11*mat._23*mat._32*mat._44-mat._11*mat._24*mat._33*mat._42
				-mat._12*mat._21*mat._33*mat._44-mat._12*mat._23*mat._34*mat._41-mat._12*mat._24*mat._31*mat._43
				-mat._13*mat._21*mat._34*mat._42-mat._13*mat._22*mat._31*mat._44-mat._13*mat._24*mat._32*mat._41
				-mat._14*mat._21*mat._32*mat._43-mat._14*mat._22*mat._33*mat._41-mat._14*mat._23*mat._31*mat._42;
		}
		/**
		* @brief �x�N�g����(x,y,z,1)�Ƃ��čs��ϊ�
		* in��out������ł����S
		* @param[out] out �ϊ���̃x�N�g��
		* @param[in] i �ϊ��O�̃x�N�g��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector4* MatrixVec3Transform(Vector4* out,const Vector3& in,const Matrix& mat)
		{
			Vector4 work;
			//�ϊ�
			work.x=in.x*mat._11+in.y*mat._21+in.z*mat._31+mat._41;
			work.y=in.x*mat._12+in.y*mat._22+in.z*mat._32+mat._42;
			work.z=in.x*mat._13+in.y*mat._23+in.z*mat._33+mat._43;
			work.w=in.x*mat._14+in.y*mat._24+in.z*mat._34+mat._44;
			*out=work;
			return out;
		}
		/**
		* @brief �x�N�g����(x,y,z,1)�Ƃ��čs��ϊ��A�ϊ���w�Ɏˉe����
		* in��out������ł����S
		* @param[out] out �ϊ���̃x�N�g��
		* @param[in] i �ϊ��O�̃x�N�g��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* MatrixVec3TransformCoord(Vector3* out,const Vector3& in,const Matrix& mat)
		{
			Vector3 work;
			//�ϊ�
			work.x=in.x*mat._11+in.y*mat._21+in.z*mat._31+mat._41;
			work.y=in.x*mat._12+in.y*mat._22+in.z*mat._32+mat._42;
			work.z=in.x*mat._13+in.y*mat._23+in.z*mat._33+mat._43;
			float w=in.x*mat._14+in.y*mat._24+in.z*mat._34+mat._44;
			//�ˉe
			*out=work/w;
			return out;
		}
		/**
		* @brief �x�N�g����(x,y,z,0)�Ƃ��čs��ϊ�
		* in��out������ł����S
		* @param[out] out �ϊ���̃x�N�g��
		* @param[in] i �ϊ��O�̃x�N�g��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* MatrixVec3TransformNormal(Vector3* out,const Vector3& in,const Matrix& mat)
		{
			Vector3 work;
			//�ϊ�
			work.x=in.x*mat._11+in.y*mat._21+in.z*mat._31;
			work.y=in.x*mat._12+in.y*mat._22+in.z*mat._32;
			work.z=in.x*mat._13+in.y*mat._23+in.z*mat._33;
			//�ˉe
			*out=work;
			return out;
		}
		/**
		* @brief �x�N�g�����s��ϊ�
		* in��out������ł����S
		* @param[out] out �ϊ���̃x�N�g��
		* @param[in] i �ϊ��O�̃x�N�g��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector4* MatrixVec4Transform(Vector4* out,const Vector4& in,const Matrix& mat)
		{
			Vector4 work;
			//�ϊ�
			work.x=in.x*mat._11+in.y*mat._21+in.z*mat._31+in.w*mat._41;
			work.y=in.x*mat._12+in.y*mat._22+in.z*mat._32+in.w*mat._42;
			work.z=in.x*mat._13+in.y*mat._23+in.z*mat._33+in.w*mat._43;
			work.w=in.x*mat._14+in.y*mat._24+in.z*mat._34+in.w*mat._44;
			*out=work;
			return out;
		}
		//*****************************************************************************
		//		�����擾�֘A
		//*****************************************************************************
		/**
		* @brief �s��̊g�听�����擾����
		* @param[out] out �g�听��
		* @param[in] in �s��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* MatrixGetScale(Vector3* out,const Matrix& in)
		{
			//X�����K��(�X�P�[������)
			out->x=ksqrtf(in._11*in._11 + in._12*in._12 + in._13*in._13);
			//Y�����K��(�X�P�[������)
			out->y=ksqrtf(in._21*in._21 + in._22*in._22 + in._23*in._23);
			//Z�����K��(�X�P�[������)
			out->z=ksqrtf(in._31*in._31 + in._32*in._32 + in._33*in._33);
			return out;
		}
		/**
		* @brief �s��̊e���x�N�g�����擾
		* @param[out] right X���x�N�g��
		* @param[out] up Y��
		* @param[out] front Z���x�N�g��
		* @param[in] in �s��
		*/
		inline void MatrixGetAxis(Vector3* right,Vector3* up,Vector3* front,const Matrix& in)
		{
			right->x=in._11;	right->y=in._12;	right->z=in._13;
			up->x=in._21;		up->y=in._22;		up->z=in._23;
			front->x=in._31;	front->y=in._32;	front->z=in._33;
		}
		/**
		* @brief �s��̕��s�ړ��������擾����
		* @param[out] out �g�听��
		* @param[in] in �s��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* MatrixGetTranslation(Vector3* out,const Matrix& in)
		{
			out->x=in._41;	out->y=in._42;	out->z=in._43;
			return out;
		}
		/**
		* @brief �s��𕪉��A�����ł��Ȃ���΃G���[
		* @param[out] s �g�听��
		* @param[out] q ��]�N�H�[�^�j�I��
		* @param[out] p ���s�ړ�����
		* @param[in] in �s��
		*/
		inline void MatrixDecompose(Vector3* s,Quaternion* q,Vector3* p,const Matrix& in)
		{
			Matrix work=in;
			MatrixGetScale(s,work);
			work._11/=s->x;	work._12/=s->x;	work._13/=s->x;
			work._21/=s->y;	work._22/=s->y;	work._23/=s->y;
			work._31/=s->z;	work._32/=s->z;	work._33/=s->z;
			MatrixToQuaternion(q,work);
			MatrixGetTranslation(p,in);
		}
		/**
		* @brief �s����x�N�g���ɕ���
		* @param[out] x X���x�N�g��
		* @param[out] y Y���x�N�g��
		* @param[out] z Z���x�N�g��
		* @param[out] p ���s�ړ�����
		* @param[in] in �s��
		*/
		inline void MatrixDecompose(Vector3* x,Vector3* y,Vector3* z,Vector3* p,const Matrix& in)
		{
			MatrixGetAxis(x,y,z,in);
			MatrixGetTranslation(p,in);
		}
		/**
		* @brief �s��̉�]�����𐳋K�������s����쐬����(�X�P�[�����O����Ă��Ă��������l���A�邱�Ƃ��ۏႳ��Ă���)
		* @param[out] out ���K�����ꂽ��]�s��
		* @param[in] in �s��
		* @return �o�͌��ʂƓ���
		*/
		inline const Matrix* MatrixDecomposeR(Matrix* out,const Matrix& in)
		{
			Vector3 s;
			MatrixGetScale(&s,in);
			out->_11=in._11/s.x;	out->_12=in._12/s.x;	out->_13=in._13/s.x;	out->_14=0.0f;
			out->_21=in._21/s.y;	out->_22=in._22/s.y;	out->_23=in._23/s.y;	out->_24=0.0f;
			out->_31=in._31/s.z;	out->_32=in._32/s.z;	out->_33=in._33/s.z;	out->_34=0.0f;
			out->_41=0.0f;				out->_42=0.0f;				out->_43=0.0f;				out->_44=1.0f;
			return out;
		}
		/**
		* @brief �s�����]�N�H�[�^�j�I���ɕ�������(�X�P�[�����O����Ă��Ă��������l���A�邱�Ƃ��ۏႳ��Ă���)
		* @param[out] out ��]�N�H�[�^�j�I��
		* @param[in] in �s��
		* @return �o�͌��ʂƓ���
		*/
		inline const Quaternion* MatrixDecomposeR(Quaternion* out,const Matrix& in)
		{
			Matrix work;
			//��]�������s��ɕ���
			MatrixDecomposeR(&work,in);
			//�N�H�[�^�j�I���ɕϊ�
			return MatrixToQuaternion(out,work);
		}
		//*****************************************************************************
		//		�s��쐬�֘A
		//*****************************************************************************
		/**
		* @brief 4x4�s��̒P�ʍs��
		* @param[inout] mat �s��
		*/
		inline void MatrixIdentity(Matrix* mat)
		{
			mat->m[0][0]=1.0f;		mat->m[0][1]=0.0f;		mat->m[0][2]=0.0f;		mat->m[0][3]=0.0f;
			mat->m[1][0]=0.0f;		mat->m[1][1]=1.0f;		mat->m[1][2]=0.0f;		mat->m[1][3]=0.0f;		
			mat->m[2][0]=0.0f;		mat->m[2][1]=0.0f;		mat->m[2][2]=1.0f;		mat->m[2][3]=0.0f;		
			mat->m[3][0]=0.0f;		mat->m[3][1]=0.0f;		mat->m[3][2]=0.0f;		mat->m[3][3]=1.0f;		
		}
		/**
		* @brief 4x4�s��̋t�s������߂�A�t�s�񂪑��݂��Ȃ���΃G���[
		* @param[out] out �t�s��
		* @param[out] m ���̍s��
		* @return �t�s��̑���
		*/
		inline void MatrixInverse(Matrix* out,const Matrix& m)
		{
			Matrix mat=m;
			f32 det = MatrixDet(mat);

			f32 inv_det = 1.0f/det;

			out->_11  = inv_det*(mat._22*mat._33*mat._44+mat._23*mat._34*mat._42+mat._24*mat._32*mat._43-mat._22*mat._34*mat._43-mat._23*mat._32*mat._44-mat._24*mat._33*mat._42);
			out->_12= inv_det*(mat._12*mat._34*mat._43+mat._13*mat._32*mat._44+mat._14*mat._33*mat._42-mat._12*mat._33*mat._44-mat._13*mat._34*mat._42-mat._14*mat._32*mat._43);
			out->_13= inv_det*(mat._12*mat._23*mat._44+mat._13*mat._24*mat._42+mat._14*mat._22*mat._43-mat._12*mat._24*mat._43-mat._13*mat._22*mat._44-mat._14*mat._23*mat._42);
			out->_14= inv_det*(mat._12*mat._24*mat._33+mat._13*mat._22*mat._34+mat._14*mat._23*mat._32-mat._12*mat._23*mat._34-mat._13*mat._24*mat._32-mat._14*mat._22*mat._33);

			out->_21= inv_det*(mat._21*mat._34*mat._43+mat._23*mat._31*mat._44+mat._24*mat._33*mat._41-mat._21*mat._33*mat._44-mat._23*mat._34*mat._41-mat._24*mat._31*mat._43);
			out->_22= inv_det*(mat._11*mat._33*mat._44+mat._13*mat._34*mat._41+mat._14*mat._31*mat._43-mat._11*mat._34*mat._43-mat._13*mat._31*mat._44-mat._14*mat._33*mat._41);
			out->_23= inv_det*(mat._11*mat._24*mat._43+mat._13*mat._21*mat._44+mat._14*mat._23*mat._41-mat._11*mat._23*mat._44-mat._13*mat._24*mat._41-mat._14*mat._21*mat._43);
			out->_24= inv_det*(mat._11*mat._23*mat._34+mat._13*mat._24*mat._31+mat._14*mat._21*mat._33-mat._11*mat._24*mat._33-mat._13*mat._21*mat._34-mat._14*mat._23*mat._31);

			out->_31= inv_det*(mat._21*mat._32*mat._44+mat._22*mat._34*mat._41+mat._24*mat._31*mat._42-mat._21*mat._34*mat._42-mat._22*mat._31*mat._44-mat._24*mat._32*mat._41);
			out->_32= inv_det*(mat._11*mat._34*mat._42+mat._12*mat._31*mat._44+mat._14*mat._32*mat._41-mat._11*mat._32*mat._44-mat._12*mat._34*mat._41-mat._14*mat._31*mat._42);
			out->_33 = inv_det*(mat._11*mat._22*mat._44+mat._12*mat._24*mat._41+mat._14*mat._21*mat._42-mat._11*mat._24*mat._42-mat._12*mat._21*mat._44-mat._14*mat._22*mat._41);
			out->_34 = inv_det*(mat._11*mat._24*mat._32+mat._12*mat._21*mat._34+mat._14*mat._22*mat._31-mat._11*mat._22*mat._34-mat._12*mat._24*mat._31-mat._14*mat._21*mat._32);

			out->_41 = inv_det*(mat._21*mat._33*mat._42+mat._22*mat._31*mat._43+mat._23*mat._32*mat._41-mat._21*mat._32*mat._43-mat._22*mat._33*mat._41-mat._23*mat._31*mat._42);
			out->_42 = inv_det*(mat._11*mat._32*mat._43+mat._12*mat._33*mat._41+mat._13*mat._31*mat._42-mat._11*mat._33*mat._42-mat._12*mat._31*mat._43-mat._13*mat._32*mat._41);
			out->_43 = inv_det*(mat._11*mat._23*mat._42+mat._12*mat._21*mat._43+mat._13*mat._22*mat._41-mat._11*mat._22*mat._43-mat._12*mat._23*mat._41-mat._13*mat._21*mat._42);
			out->_44 = inv_det*(mat._11*mat._22*mat._33+mat._12*mat._23*mat._31+mat._13*mat._21*mat._32-mat._11*mat._23*mat._32-mat._12*mat._21*mat._33-mat._13*mat._22*mat._31);

		}
		/**
		* @brief �s��̉�]�����𐳋K������A���K���ł��Ȃ���΃G���[
		* @param[inout] m �s��
		* @return �o�͌��ʂƓ���
		*/
		inline const Matrix* MatrixNormalize(Matrix* m)
		{
			Vector3 s;
			MatrixGetScale(&s,*m);
			m->_11=m->_11/s.x;	m->_12=m->_12/s.x;	m->_13=m->_13/s.x;	m->_14=0.0f;
			m->_21=m->_21/s.y;	m->_22=m->_22/s.y;	m->_23=m->_23/s.y;	m->_24=0.0f;
			m->_31=m->_31/s.z;	m->_32=m->_32/s.z;	m->_33=m->_33/s.z;	m->_34=0.0f;
			return m;
		}
		/**
		* @brief �]�u�s��쐬
		* @param[out] out �]�u��s��
		* @param[in] in �s��
		* @return �o�͌��ʂƓ���
		*/
		inline const Matrix* MatrixTranspose(Matrix* out,const Matrix& in)
		{
			Matrix work=in;
			for(int i=0;i<4;i++)for(int j=0;j<4;j++)out->m[j][i]=work.m[i][j];
			return out;
		}

		/**
		* @brief X����]�s��쐬
		* @param[out] out ��]�s��
		* @param[in] r X��]�p
		* @return �o�͌��ʂƓ���
		*/
		inline const Matrix* MatrixRotationX(Matrix* out,f32 r)
		{
			f32 s=ksinf(r);
			f32 c=kcosf(r);

			out->_11=1.0f;	out->_12=0.0f;	out->_13=0.0f;	out->_14=0.0f;
			out->_21=0.0f;	out->_22=c;		out->_23=s;		out->_24=0.0f;
			out->_31=0.0f;	out->_32=-s;		out->_33=c;		out->_34=0.0f;
			out->_41=0.0f;	out->_42=0.0f;	out->_43=0.0f;	out->_44=1.0f;

			return out;
		}
		/**
		* @brief Y����]�s��쐬
		* @param[out] out ��]�s��
		* @param[in] r Y��]�p
		* @return �o�͌��ʂƓ���
		*/
		inline const Matrix* MatrixRotationY(Matrix* out,f32 r)
		{
			f32 s=ksinf(r);
			f32 c=kcosf(r);

			out->_11=c;			out->_12=0.0f;	out->_13=-s;			out->_14=0.0f;
			out->_21=0.0f;		out->_22=1.0f;	out->_23=0;			out->_24=0.0f;
			out->_31=s;			out->_32=0.0f;	out->_33=c;			out->_34=0.0f;
			out->_41=0.0f;		out->_42=0.0f;	out->_43=0.0f;		out->_44=1.0f;

			return out;
		}
		/**
		* @brief Z����]�s��쐬
		* @param[out] out ��]�s��
		* @param[in] r Z��]�p
		* @return �o�͌��ʂƓ���
		*/
		inline const Matrix* MatrixRotationZ(Matrix* out,f32 r)
		{
			f32 s=ksinf(r);
			f32 c=kcosf(r);

			out->_11=c;			out->_12=s;			out->_13=0.0f;	out->_14=0.0f;
			out->_21=-s;			out->_22=c;			out->_23=0.0f;	out->_24=0.0f;
			out->_31=0.0f;		out->_32=0.0f;		out->_33=1.0f;	out->_34=0.0f;
			out->_41=0.0f;		out->_42=0.0f;		out->_43=0.0f;	out->_44=1.0f;

			return out;
		}
		/**
		* @brief XYZ����]�s��쐬(XYZ�I�[�_�[)
		* @param[out] out ��]�s��
		* @param[in] a ��]�p
		* @return �o�͌��ʂƓ���
		*/
		inline const Matrix* MatrixRotationXYZ( Matrix* out,const Vector3& a)
		{
			f32	sx, sy, sz, cx, cy, cz;

			sx = ksinf(a.x);	sy = ksinf(a.y);	sz = ksinf(a.z);
			cx = kcosf(a.x);	cy = kcosf(a.y);	cz = kcosf(a.z);

			out->_11=( cy*cz );							out->_12=( cy*sz );							out->_13=( -sy );			out->_14=(0.0f);
			out->_21=( sx*sy*cz+(-cx*sz) );		out->_22=( sx*sy*sz+cx*cz );			out->_23=( sx*cy );		out->_24=(0.0f);
			out->_31=( cx*sy*cz+sx*sz );			out->_32=( cx*sy*sz+(-sx*cz) );		out->_33=( cx*cy );		out->_34=(0.0f);
			out->_41=(0.0f);								out->_42=(0.0f);								out->_43=(0.0f);			out->_44=(1.0f);

			return out;
		}

		/**
		* @brief XYZ����]�s��쐬(ZXY�I�[�_�[)
		* @param[out] out ��]�s��
		* @param[in] a ��]�p
		* @return �o�͌��ʂƓ���
		*/
		inline const Matrix* MatrixRotationZXY( Matrix* out,const Vector3& a)
		{
			f32	sx, sy, sz, cx, cy, cz;

			sx = ksinf(a.x);	sy = ksinf(a.y);	sz = ksinf(a.z);
			cx = kcosf(a.x);	cy = kcosf(a.y);	cz = kcosf(a.z);

			out->_11=( cz*cy + sz*sx*sy );	out->_12=( sz*cx );		out->_13=(-cz*sy + sz*sx*cy );	out->_14=(0.0f);
			out->_21=(-sz*cy + cz*sx*sy );	out->_22=( cz*cx );		out->_23=( sz*sy + cz*sx*cy );	out->_24=(0.0f);
			out->_31=( cx*sy );						out->_32=(-sx );			out->_33=( cx*cy );						out->_34=(0.0f);
			out->_41=(0.0f);							out->_42=(0.0f);			out->_43=(0.0f);							out->_44=(1.0f);

			return out;
		}

		/**
		* @brief �C�ӎ���]�s��쐬
		* @param[out] out ��]�s��
		* @param[in] axis ��]��
		* @param[in] r ��]�p
		* @return �o�͌��ʂƓ���
		*/
		inline const Matrix* MatrixAxisRotation(Matrix* out,const Vector3& axis,f32 r)
		{
			f32 s=ksinf(r);
			f32 c=kcosf(r);

			out->_11=(axis.x*axis.x)*(1.0f-c)+c;					out->_12=(axis.x*axis.y)*(1.0f-c)+axis.z*s;		out->_13=(axis.z*axis.x)*(1.0f-c)-axis.y*s;		out->_14=0.0f;
			out->_21=(axis.x*axis.y)*(1.0f-c)-axis.z*s;		out->_22=(axis.y*axis.y)*(1.0f-c)+c;					out->_23=(axis.y*axis.z)*(1.0f-c)+axis.x*s;		out->_24=0.0f;
			out->_31=(axis.z*axis.x)*(1.0f-c)+axis.y*s;		out->_32=(axis.y*axis.z)*(1.0f-c)-axis.x*s;		out->_33=(axis.z*axis.z)*(1.0f-c)+c;					out->_34=0.0f;
			out->_41=0.0f;														out->_42=0.0f;														out->_43=0.0f;														out->_44=1.0f;

			return out;
		}
		inline Matrix* MatrixFrontVec( Matrix* out, const Vector3& Pos, const Vector3& Front, const Vector3& Up )
		{
			Vector3 xaxis = Vector3( Up.y*Front.z - Up.z*Front.y, Up.z*Front.x - Up.x*Front.z, Up.x*Front.y - Up.y*Front.x ); 
			Vector3 yaxis = Vector3( Front.y*xaxis.z - Front.z*xaxis.y, Front.z*xaxis.x - Front.x*xaxis.z, Front.x*xaxis.y - Front.y*xaxis.x ); 

			xaxis.normalize();
			yaxis.normalize();

			out->_11 = xaxis.x;																		out->_12 = xaxis.y;																		out->_13 = xaxis.z;																		out->_14 = 0;
			out->_21 = yaxis.x;																		out->_22 = yaxis.y;																		out->_23 = yaxis.z;																		out->_24 = 0;
			out->_31 = Front.x;																		out->_32 = Front.y;																		out->_33 = Front.z;																		out->_34 = 0;
			out->_41 = Pos.x;																			out->_42 = Pos.y;																			out->_43 = Pos.z;																			out->_44 = 1;
			return out;
		}
		/**
		* @brief ���ړ_�������p���s����쐬����
		* @param[out] out �p���s��
		* @param[in] Pos �ʒu
		* @param[in] At ���ړ_
		* @param[in] Up ����x�N�g��(���ړ_�ւ̃x�N�g�����D�悳���̂Œ���)
		* @return �o�͌��ʂƓ���
		*/
		inline Matrix* MatrixLookAt( Matrix* out, const Vector3& Pos, const Vector3& At, const Vector3& Up )
		{
			Vector3 zaxis;
			zaxis.x = At.x - Pos.x;
			zaxis.y = At.y - Pos.y;
			zaxis.z = At.z - Pos.z;
			Vector3 xaxis = Vector3( Up.y*zaxis.z - Up.z*zaxis.y, Up.z*zaxis.x - Up.x*zaxis.z, Up.x*zaxis.y - Up.y*zaxis.x ); 
			Vector3 yaxis = Vector3( zaxis.y*xaxis.z - zaxis.z*xaxis.y, zaxis.z*xaxis.x - zaxis.x*xaxis.z, zaxis.x*xaxis.y - zaxis.y*xaxis.x ); 

			xaxis.normalize();
			yaxis.normalize();
			zaxis.normalize();

			out->_11 = xaxis.x;																		out->_12 = xaxis.y;																		out->_13 = xaxis.z;																		out->_14 = 0;
			out->_21 = yaxis.x;																		out->_22 = yaxis.y;																		out->_23 = yaxis.z;																		out->_24 = 0;
			out->_31 = zaxis.x;																		out->_32 = zaxis.y;																		out->_33 = zaxis.z;																		out->_34 = 0;
			out->_41 = Pos.x;																			out->_42 = Pos.y;																			out->_43 = Pos.z;																			out->_44 = 1;
			return out;
		}
		/**
		* @brief ������W�n�r���[�ϊ��s����쐬����
		* @param[out] out �r���[�ϊ��s��
		* @param[in] Eye �J�����ʒu
		* @param[in] At ���ړ_
		* @param[in] Up ����x�N�g��(���ړ_�ւ̃x�N�g�����D�悳���̂Œ���)
		* @return �o�͌��ʂƓ���
		*/
		inline Matrix* MatrixLookAtViewLH( Matrix* out, const Vector3& Eye, const Vector3& At, const Vector3& Up )
		{
			Vector3 zaxis;
			zaxis.x = At.x - Eye.x;
			zaxis.y = At.y - Eye.y;
			zaxis.z = At.z - Eye.z;
			Vector3 xaxis = Vector3( Up.y*zaxis.z - Up.z*zaxis.y, Up.z*zaxis.x - Up.x*zaxis.z, Up.x*zaxis.y - Up.y*zaxis.x ); 
			Vector3 yaxis = Vector3( zaxis.y*xaxis.z - zaxis.z*xaxis.y, zaxis.z*xaxis.x - zaxis.x*xaxis.z, zaxis.x*xaxis.y - zaxis.y*xaxis.x ); 

			xaxis.normalize();
			yaxis.normalize();
			zaxis.normalize();

			out->_11 = xaxis.x;																		out->_12 = yaxis.x;																		out->_13 = zaxis.x;																		out->_14 = 0;
			out->_21 = xaxis.y;																		out->_22 = yaxis.y;																		out->_23 = zaxis.y;																		out->_24 = 0;
			out->_31 = xaxis.z;																		out->_32 = yaxis.z;																		out->_33 = zaxis.z;																		out->_34 = 0;
			out->_41 = -(xaxis.x*Eye.x + xaxis.y*Eye.y + xaxis.z*Eye.z);	out->_42 = -(yaxis.x*Eye.x + yaxis.y*Eye.y + yaxis.z*Eye.z);	out->_43 = -(zaxis.x*Eye.x + zaxis.y*Eye.y + zaxis.z*Eye.z);	out->_44 = 1;
			return out;
		}
		/**
		* @brief �p�[�X���e�ϊ��s����쐬����
		* @param[out] out �p�[�X���e�ϊ��s��
		* @param[in] fovY ����p
		* @param[in] aspect �X�N���[���̔䗦(Width/Height)
		* @param[in] zn �ŋߋ���
		* @param[in] zf �ŉ�����
		* @return �o�͌��ʂƓ���
		*/
		inline Matrix* MatrixPerspectiveLH( Matrix* out, f32 fovY, f32 aspect, f32 zn, f32 zf )
		{
			f32 yScale = kcotf(fovY/2.0f);
			f32 xScale = yScale / aspect;

			out->_11 = xScale;		out->_12 = 0.0f;		out->_13 = 0.0f;							out->_14 = 0.0f;
			out->_21 = 0.0f;			out->_22 = yScale;	out->_23 = 0.0f;							out->_24 = 0.0f;
			out->_31 = 0.0f;			out->_32 = 0.0f;		out->_33 = zf / (zf-zn);					out->_34 = 1.0f;
			out->_41 = 0.0f;			out->_42 = 0.0f;		out->_43 = (-zn*zf) / (zf-zn);		out->_44 = 0.0f;
			return out;
		}
		/**
		* @brief �p�[�X���e�ϊ��s����쐬����
		* @param[out] out �p�[�X���e�ϊ��s��
		* @param[in] w �X�N���[����
		* @param[in] h �X�N���[������
		* @param[in] zn �ŋߋ���
		* @param[in] zf �ŉ�����
		* @return �o�͌��ʂƓ���
		*/
		inline Matrix* MatrixOlthoLH( Matrix* out, f32 w, f32 h, f32 zn, f32 zf )
		{
			out->_11 = 2.0f / w;		out->_12 = 0.0f;			out->_13 = 0.0f;					out->_14 = 0.0f;
			out->_21 = 0.0f;			out->_22 = 2.0f / h;		out->_23 = 0.0f;					out->_24 = 0.0f;
			out->_31 = 0.0f;			out->_32 = 0.0f;			out->_33 = 1.0f / (zf-zn);		out->_34 = 0.0f;
			out->_41 = 0.0f;			out->_42 = 0.0f;			out->_43 = zn / (zn-zf);		out->_44 = 1.0f;
			return out;
		}
		//*****************************************************************************
		//
		//		�N�H�[�^�j�I���֘A
		//
		//*****************************************************************************
		//*****************************************************************************
		//		�ϊ��֘A
		//*****************************************************************************
		/**
		* @brief �N�H�[�^�j�I�����s��ɕϊ�����
		* @param[out] out ��]�s��
		* @param[in] q �N�H�[�^�j�I��
		* @return �o�͌��ʂƓ���
		*/
		inline const Matrix* QuaternionToMatrix(Matrix* out,const Quaternion& q)
		{
			f32 s = ksqrtf( q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w );
			s = 2.0f / (s * s);
			f32 vx = q.x * s;
			f32 vy = q.y * s;
			f32 vz = q.z * s;
			f32 wx = vx * q.w;
			f32 wy = vy * q.w;
			f32 wz = vz * q.w;
			f32 sx = q.x * vx;
			f32 sy = q.y * vy;
			f32 sz =q. z * vz;
			f32 cx = q.y * vz;
			f32 cy = q.z * vx;
			f32 cz = q.x * vy;

			out->_11 = 1.0f - sy - sz;
			out->_12 = cz + wz;
			out->_13 = cy - wy;
			out->_14 = 0.0f;
			out->_21 = cz - wz;
			out->_22 = 1.0f - sx - sz;
			out->_23 = cx + wx;
			out->_24 = 0.0f;
			out->_31 = cy + wy;
			out->_32 = cx - wx;
			out->_33 = 1.0f - sx - sy;
			out->_34 = 0.0f;
			out->_41 = 0.0f;
			out->_42 = 0.0f;
			out->_43 = 0.0f;
			out->_44 = 1.0f;

			return out;
		}
		/**
		* @brief �N�H�[�^�j�I������I�C���[�p�ɕϊ�(XYZ�I�[�_�[�s���p)
		* @param[out] out �I�C���[�p
		* @param[in] q �N�H�[�^�j�I��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* QuaternionToEulerXYZ(Vector3* out,const Quaternion& q)
		{
			Matrix work;
			QuaternionToMatrix(&work,q);
			return MatrixToEulerXYZ(out,work);
		}
		/**
		* @brief �N�H�[�^�j�I������I�C���[�p�ɕϊ�(ZXY�I�[�_�[�s���p)
		* @param[out] out �I�C���[�p
		* @param[in] q �N�H�[�^�j�I��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* QuaternionToEulerZXY(Vector3* out,const Quaternion& q)
		{
			Matrix work;
			QuaternionToMatrix(&work,q);
			return MatrixToEulerZXY(out,work);
		}
		//*****************************************************************************
		//		�֐��֘A
		//*****************************************************************************
		/**
		* @brief �N�H�[�^�j�I���̓���
		* @param[in] q1 �N�H�[�^�j�I��
		* @param[in] q2 �N�H�[�^�j�I��
		* @return ����
		*/
		inline f32 QuaternionDot(const Quaternion& q1,const Quaternion& q2)
		{
			return q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
		}
		/**
		* @brief �N�H�[�^�j�I���̒����̓����擾
		* @param[in] q �N�H�[�^�j�I��
		* @return �����̓��
		*/
		inline f32 QuaternionLengthSq(const Quaternion& q)
		{
			return QuaternionDot(q,q);
		}
		/**
		* @brief �N�H�[�^�j�I���̒������擾
		* @param[in] q �N�H�[�^�j�I��
		* @return ����
		*/
		inline f32 QuaternionLength(const Quaternion& q)
		{
			return ksqrtf(QuaternionLengthSq(q));
		}
		/**
		* @brief �N�H�[�^�j�I�����m�̍���
		* q1�̉�]�̌�ɁAq2�̉�]���������ʂ��v�Z
		* @param[in] q1 �ŏ��̉�]�N�H�[�^�j�I��
		* @param[in] q2 ���̉�]�N�H�[�^�j�I��
		* @return ��]���ʂ̃N�H�[�^�j�I��
		*/
		inline Quaternion QuaternionComp(const Quaternion& q1,const Quaternion& q2){return q2*q1;}
		/**
		* @brief �N�H�[�^�j�I���̋��ʐ��`���
		* @param[out] out ��Ԍ�̃N�H�[�^�j�I��
		* @param[in] q1 ���N�H�[�^�j�I��
		* @param[in] q2 ���N�H�[�^�j�I��
		* @return �o�͌��ʂƓ���
		*/
		inline const Quaternion* QuaternionSlerp(Quaternion* out,const Quaternion& q1,const Quaternion& q2,f32 t)
		{
			//	����
			float dot = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
			float ss = 1.0f - (dot * dot);
			if(ss <= 0.0f) {*out=q1;return out;}

			Quaternion Target = q2;
			if( dot < 0.0f )
			{
				Target = -Target;
				dot = -dot;
			}
			//	���ϒl�␳
			if(dot > 1.0f ) dot = 1.0f;

			//	��Ԋ����v�Z
			float ph = kacosf(dot);
			float s = 1.0f / ksinf(ph);
			float s1 = ksinf(ph * (1.0f-t)) * s;
			float s2 = ksinf(ph *  t      ) * s;
			*out = q1*s1 + Target*s2;
			//	���
			return out;
		}
		/*
		* @brief �x�N�g�����N�H�[�^�j�I���ŉ�]������
		* @param[out] out ��]��x�N�g��
		* @param[in] in �N�H�[�^�j�I��
		* @return �o�͌��ʂƓ���
		*/
		inline const Vector3* QuaternionVec3Rotation(Vector3* out,const Quaternion& q)
		{
			*out=Vector3(q*Quaternion(*out)*q.createConjugate());
			return out;
		}
		//*****************************************************************************
		//		�����擾�֘A
		//*****************************************************************************
		/**
		* @brief �N�H�[�^�j�I���̎��Ɖ�]�p�ɕ�������
		* @param[out] axis ��
		* @param[out] rad ��]�p
		* @param[in] �N�H�[�^�j�I��
		*/
		inline void QuaternionDecompose( Vector3* axis, f32* rad,const Quaternion& q ){
			*rad = kacosf( q.w );
			axis->x=q.x/ksinf(*rad);
			axis->y=q.y/ksinf(*rad);
			axis->z=q.z/ksinf(*rad);
			*rad *= 2.0f;
		}
		/**
		* @brief �N�H�[�^�j�I�����p���x�N�g���ɕ�������
		* �s��ɕϊ����Ď擾������͐��x�͈���������
		* @param[out] right X���x�N�g��
		* @param[out] up Y���x�N�g��
		* @param[out] front Z���x�N�g��
		* @param[in] �N�H�[�^�j�I��
		*/
		inline void QuaternionDecompose( Vector3* right, Vector3* up,Vector3* front,const Quaternion& q){
			f32 x2 = q.x + q.x;
			f32 y2 = q.y + q.y;
			f32 z2 = q.z + q.z;
			f32 xx = q.x * x2;f32 xy = q.x  * y2; f32 xz = q.x * z2;
			f32 yy = q.y * y2; f32 yz = q.y * z2;f32 zz = q.z * z2;
			f32 wx = q.w * x2; f32 wy = q.w * y2; f32 wz = q.w * z2;
			right->x=1.0f-(yy+zz);
			right->y=xy+wz;
			right->z=xz-wy;
			up->x=xy-wz;
			up->y=1.0f-(xx+zz);
			up->z=yz+wx;
			front->x=xz+wy;
			front->y=yz-wx;
			front->z=1.0f-(xx+yy);
		}
		//*****************************************************************************
		//		�s��쐬�֘A
		//*****************************************************************************
		/**
		* @brief �N�H�[�^�j�I����P�ʉ�
		* @param[out] q �P�ʉ�����N�H�[�^�j�I��
		* @return �o�͌��ʂƓ���
		*/
		inline const Quaternion* QuaternionIdentity(Quaternion* q)
		{
			q->x=0.0f;
			q->y=0.0f;
			q->z=0.0f;
			q->w=1.0f;
			return q;
		}
		/**
		* @brief �N�H�[�^�j�I���������N�H�[�^�j�I���ŕԂ�(-x,-y,-z,w)
		* @param[out] out ���������ꂽ�N�H�[�^�j�I��
		* @param[in] in �N�H�[�^�j�I��
		* @return �o�͌��ʂƓ���
		*/
		inline const Quaternion* QuaternionConjugate(Quaternion* out,const Quaternion& in)
		{
			out->x=-in.x;
			out->y=-in.y;
			out->z=-in.z;
			out->w=in.w;
			return out;
		}
		/**
		* @brief �N�H�[�^�j�I���𐳋K��
		* @param[out] out ���K�����ꂽ�N�H�[�^�j�I��
		* @param[in] in ���K������N�H�[�^�j�I��
		* @return �o�͌��ʂƓ���(���K���ł��Ȃ����NULL���A��)
		*/
		inline const Quaternion* QuaternionNormalize(Quaternion* out,const Quaternion& in)
		{
			f32 len=QuaternionLength(in);
			*out=in/len;
			return out;
		}
		/**
		* @brief �N�H�[�^�j�I�����t����
		* @param[out] out �t�������ꂽ�N�H�[�^�j�I��
		* @param[in] in �t��������N�H�[�^�j�I��
		* @return �o�͌��ʂƓ���
		*/
		inline const Quaternion* QuaternionInverse(Quaternion* out,const Quaternion& in)
		{
			*out=(*QuaternionConjugate(out,in))/QuaternionLengthSq(in);
		}
		/**
		* @brief X����]�N�H�[�^�j�I�����쐬
		* @param[out] out ��]�N�H�[�^�j�I��
		* @param[in] rad ��]�p
		* @return �o�͌��ʂƓ���
		*/
		inline const Quaternion* QuaternionRotationX(Quaternion* out,float rad)
		{
			f32 r=rad*0.5f;
			out->x=ksinf(r);
			out->y=0.0f;
			out->z=0.0f;
			out->w=kcosf(r);
			return out;
		}
		/**
		* @brief Y����]�N�H�[�^�j�I�����쐬
		* @param[out] out ��]�N�H�[�^�j�I��
		* @param[in] rad ��]�p
		* @return �o�͌��ʂƓ���
		*/
		inline const Quaternion* QuaternionRotationY(Quaternion* out,float rad)
		{
			f32 r=rad*0.5f;
			out->x=0.0f;
			out->y=ksinf(r);
			out->z=0.0f;
			out->w=kcosf(r);
			return out;
		}
		/**
		* @brief Z����]�N�H�[�^�j�I�����쐬
		* @param[out] out ��]�N�H�[�^�j�I��
		* @param[in] rad ��]�p
		* @return �o�͌��ʂƓ���
		*/
		inline const Quaternion* QuaternionRotationZ(Quaternion* out,float rad)
		{
			f32 r=rad*0.5f;
			out->x=0.0f;
			out->y=0.0f;
			out->z=ksinf(r);
			out->w=kcosf(r);
			return out;
		}
		/**
		* @brief XYZ��]�N�H�[�^�j�I�����쐬
		* @param[out] out ��]�N�H�[�^�j�I��
		* @param[in] sx x��]�p
		* @param[in] sy y��]�p
		* @param[in] sz z��]�p
		* @return �o�͌��ʂƓ���
		*/
		inline const Quaternion* QuaternionRotationXYZ(Quaternion* out,const Vector3& angle)
		{
			f32 ssx=ksinf(angle.x*.5f);
			f32 ssy=ksinf(angle.y*.5f);
			f32 ssz=ksinf(angle.z*.5f);
			f32 csx=kcosf(angle.x*.5f);
			f32 csy=kcosf(angle.y*.5f);
			f32 csz=kcosf(angle.z*.5f);

			out->w = csz*csy*csx + ssz*ssy*ssx;
			out->x = csz*csy*ssx - ssz*ssy*csx;
			out->y = csz*ssy*csx + ssz*csy*ssx;
			out->z = ssz*csy*csx - csz*ssy*ssx;

			return out;
		}
		/**
		* @brief ZXY��]�N�H�[�^�j�I�����쐬
		* @param[out] out ��]�N�H�[�^�j�I��
		* @param[in] sx x��]�p
		* @param[in] sy y��]�p
		* @param[in] sz z��]�p
		* @return �o�͌��ʂƓ���
		*/
		inline const Quaternion* QuaternionRotationZXY(Quaternion* out,const Vector3& angle)
		{
			f32 ssx=ksinf(angle.x*.5f);
			f32 ssy=ksinf(angle.y*.5f);
			f32 ssz=ksinf(angle.z*.5f);
			f32 csx=kcosf(angle.x*.5f);
			f32 csy=kcosf(angle.y*.5f);
			f32 csz=kcosf(angle.z*.5f);

			out->w = csy*csx*csz + ssy*csx*ssz;
			out->x = ssy*csx*ssz + csy*ssx*csz;
			out->y = -csy*ssx*ssz + ssy*csx*csz;
			out->z = -ssy*ssx*csz + csy*csx*ssz;

			return out;
		}
		/**
		* @brief �C�ӂ̎��Ɖ�]�p����N�H�[�^�j�I�����쐬����
		* @param[out] out �N�H�[�^�j�I��
		* @param[in] axis �C�ӂ̎�
		* @param[in] rad ��]�p
		*/
		inline const Quaternion* QuaternionAxisRotation(Quaternion* out, const Vector3 &axis, f32 rad ){
			f32 d = rad * 0.5f;
			out->x=axis.x*ksinf(d);
			out->y=axis.y*ksinf(d);
			out->z=axis.z*ksinf(d);
			out->w  = kcosf( d );
			return out;
		}
	};
};

#endif