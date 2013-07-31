#pragma once

#include "..\..\..\types.h"
#include "..\..\..\defines.h"
#include "..\Vector\Vector.h"

namespace klib
{
	namespace math
	{
		struct Quaternion;
		/**
		* @class 4x4�s��N���X
		*/
		struct Matrix:public float4x4
		{
			/**
			* @brief �R���X�g���N�^
			*/
			Matrix();
			inline operator f32*(){return (f32*)m;}
			inline operator const f32*()const{return (const f32*)m;}
			/**
			* @brief *���Z�q
			* @param[in] mat �|����s��
			*/
			inline Matrix operator*(const Matrix& mat)const
			{
				Matrix ref;
				ref._11 = this->_11 * mat._11 + this->_12 * mat._21 + this->_13 * mat._31 + this->_14 * mat._41;
				ref._12 = this->_11 * mat._12 + this->_12 * mat._22 + this->_13 * mat._32 + this->_14 * mat._42;
				ref._13 = this->_11 * mat._13 + this->_12 * mat._23 + this->_13 * mat._33 + this->_14 * mat._43;
				ref._14 = this->_11 * mat._14 + this->_12 * mat._24 + this->_13 * mat._34 + this->_14 * mat._44;

				ref._21 = this->_21 * mat._11 + this->_22 * mat._21 + this->_23 * mat._31 + this->_24 * mat._41;
				ref._22 = this->_21 * mat._12 + this->_22 * mat._22 + this->_23 * mat._32 + this->_24 * mat._42;
				ref._23 = this->_21 * mat._13 + this->_22 * mat._23 + this->_23 * mat._33 + this->_24 * mat._43;
				ref._24 = this->_21 * mat._14 + this->_22 * mat._24 + this->_23 * mat._34 + this->_24 * mat._44;

				ref._31 = this->_31 * mat._11 + this->_32 * mat._21 + this->_33 * mat._31 + this->_34 * mat._41;
				ref._32 = this->_31 * mat._12 + this->_32 * mat._22 + this->_33 * mat._32 + this->_34 * mat._42;
				ref._33 = this->_31 * mat._13 + this->_32 * mat._23 + this->_33 * mat._33 + this->_34 * mat._43;
				ref._34 = this->_31 * mat._14 + this->_32 * mat._24 + this->_33 * mat._34 + this->_34 * mat._44;

				ref._41 = this->_41 * mat._11 + this->_42 * mat._21 + this->_43 * mat._31 + this->_44 * mat._41;
				ref._42 = this->_41 * mat._12 + this->_42 * mat._22 + this->_43 * mat._32 + this->_44 * mat._42;
				ref._43 = this->_41 * mat._13 + this->_42 * mat._23 + this->_43 * mat._33 + this->_44 * mat._43;
				ref._44 = this->_41 * mat._14 + this->_42 * mat._24 + this->_43 * mat._34 + this->_44 * mat._44;
				return ref;
			}
			/**
			* @brief *=���Z�q
			* @param[in] mat �|����s��
			*/
			inline Matrix& operator*=(const Matrix& mat)
			{
				Matrix work;
				work._11 = this->_11 * mat._11 + this->_12 * mat._21 + this->_13 * mat._31 + this->_14 * mat._41;
				work._12 = this->_11 * mat._12 + this->_12 * mat._22 + this->_13 * mat._32 + this->_14 * mat._42;
				work._13 = this->_11 * mat._13 + this->_12 * mat._23 + this->_13 * mat._33 + this->_14 * mat._43;
				work._14 = this->_11 * mat._14 + this->_12 * mat._24 + this->_13 * mat._34 + this->_14 * mat._44;

				work._21 = this->_21 * mat._11 + this->_22 * mat._21 + this->_23 * mat._31 + this->_24 * mat._41;
				work._22 = this->_21 * mat._12 + this->_22 * mat._22 + this->_23 * mat._32 + this->_24 * mat._42;
				work._23 = this->_21 * mat._13 + this->_22 * mat._23 + this->_23 * mat._33 + this->_24 * mat._43;
				work._24 = this->_21 * mat._14 + this->_22 * mat._24 + this->_23 * mat._34 + this->_24 * mat._44;

				work._31 = this->_31 * mat._11 + this->_32 * mat._21 + this->_33 * mat._31 + this->_34 * mat._41;
				work._32 = this->_31 * mat._12 + this->_32 * mat._22 + this->_33 * mat._32 + this->_34 * mat._42;
				work._33 = this->_31 * mat._13 + this->_32 * mat._23 + this->_33 * mat._33 + this->_34 * mat._43;
				work._34 = this->_31 * mat._14 + this->_32 * mat._24 + this->_33 * mat._34 + this->_34 * mat._44;

				work._41 = this->_41 * mat._11 + this->_42 * mat._21 + this->_43 * mat._31 + this->_44 * mat._41;
				work._42 = this->_41 * mat._12 + this->_42 * mat._22 + this->_43 * mat._32 + this->_44 * mat._42;
				work._43 = this->_41 * mat._13 + this->_42 * mat._23 + this->_43 * mat._33 + this->_44 * mat._43;
				work._44 = this->_41 * mat._14 + this->_42 * mat._24 + this->_43 * mat._34 + this->_44 * mat._44;

				*this=work;
				return *this;
			}
			/**
			* @brief �P�ʍs��
			* @return this
			*/
			const Matrix* identity();
			/**
			* @brief �t�s��
			* @return this
			*/
			const Matrix* inverse();
			/**
			* @brief ���K��
			* �e�������𐳋K������
			* @return this
			*/
			const Matrix* normalize();
			/**
			* @brief �s��]�u
			* @return this
			*/
			inline const Matrix* transpose()
			{
				Matrix work=*this;
				for(int i=0;i<4;i++)for(int j=0;j<4;j++)this->m[j][i]=work.m[i][j];
				return this;
			}
			/**
			* @brief �s�񎮂�����
			* @return �s��
			*/
			f32 determinant()const;
			//*****************************************************************************
			//		��������֘A
			//*****************************************************************************
			/**
			* @brief ���s�ړ�������������
			* @param[in] x x�ړ�����
			* @param[in] y y�ړ�����
			* @param[in] z z�ړ�����
			* @return this
			*/
			const Matrix* setT(f32 x,f32 y,f32 z);
			/**
			* @brief ���s�ړ�������������
			* @param[in] pos �ړ�����
			* @return this
			*/
			inline const Matrix* setT(const Vector3& pos){return this->setT(pos.x,pos.y,pos.z);}
			/**
			* @brief ��]������������(XYZ�I�[�_�[)
			* @param[in] x x�p����
			* @param[in] y y�p����
			* @param[in] z z�p����
			* @return this
			*/
			const Matrix* setRXYZ(f32 x,f32 y,f32 z);
			/**
			* @brief ��]������������(XYZ�I�[�_�[)
			* @param[in] angle �I�C���[�p����
			* @return this
			*/
			inline const Matrix* setRXYZ(const Vector3& angle){return this->setRXYZ(angle.x,angle.y,angle.z);}
			/**
			* @brief ��]������������(ZXY�I�[�_�[)
			* @param[in] x x�p����
			* @param[in] y y�p����
			* @param[in] z z�p����
			* @return this
			*/
			const Matrix* setRZXY(f32 x,f32 y,f32 z);
			/**
			* @brief ��]������������(ZXY�I�[�_�[)
			* @param[in] angle �I�C���[�p����
			* @return this
			*/
			inline const Matrix* setRZXY(const Vector3& angle){return this->setRZXY(angle.x,angle.y,angle.z);}
			/**
			* @brief �C�ӎ���]������������
			* @param[in] axis x�p����
			* @param[in] r ��]�p����
			* @return this
			*/
			const Matrix* setR(const Vector3& axis,f32 r);
			/**
			* @brief �N�H�[�^�j�I����]������������
			* @param[in] q �N�H�[�^�j�I��
			* @return this
			*/
			const Matrix* setR(const Quaternion& q);
			/**
			* @brief X����]������������
			* @param[in] r ��]�p
			* @return this
			*/
			const Matrix* setRX(f32 r);
			/**
			* @brief y����]������������
			* @param[in] r ��]�p
			* @return this
			*/
			const Matrix* setRY(f32 r);
			/**
			* @brief z����]������������
			* @param[in] r ��]�p
			* @return this
			*/
			const Matrix* setRZ(f32 r);
			/**
			* @brief �g�听������Z����
			* @param[in] x x�g�听��
			* @param[in] y y�g�听��
			* @param[in] z z�g�听��
			* @return this
			*/
			inline const Matrix* mulS(f32 x,f32 y,f32 z)
			{
				_11*=x;		_12*=x;		_13*=x;
				_21*=y;		_22*=y;		_23*=y;
				_31*=z;		_32*=z;		_33*=z;
				return this;
			}
			/**
			* @brief �g�听������Z����
			* @param[in] xyz �g�听��
			* @return this
			*/
			inline const Matrix* mulS(f32 xyz)
			{
				_11*=xyz;		_12*=xyz;		_13*=xyz;
				_21*=xyz;		_22*=xyz;		_23*=xyz;
				_31*=xyz;		_32*=xyz;		_33*=xyz;
				return this;
			}
			/**
			* @brief �g�听������Z����
			* @param[in] s �g�听��
			* @return this
			*/
			inline const Matrix* mulS(const Vector3& s){return this->mulS(s.x,s.y,s.z);}
			//*****************************************************************************
			//		�����擾�֘A
			//*****************************************************************************
			/**
			* @brief �s�������擾����
			* @param[in] index �擾����s�̃C���f�b�N�X
			* @param[out] out �s����
			* @return �o�͌��ʂƓ���
			*/
			inline const Vector4* getRow(s32 index,Vector4* out)const{out->x=this->m[index][0];out->y=this->m[index][1];out->z=this->m[index][2];out->w=this->m[index][3];return out;}
			/**
			* @brief �񐬕����擾����
			* @param[in] index �擾�����̃C���f�b�N�X
			* @param[out] out �񐬕�
			* @return �o�͌��ʂƓ���
			*/
			inline const Vector4* getCol(s32 index,Vector4* out)const{out->x=this->m[0][index];out->y=this->m[1][index];out->z=this->m[2][index];out->w=this->m[3][index];return out;}
				/**
			* @brief �s�������擾����
			* @param[in] index �擾����s�̃C���f�b�N�X
			* @param[out] out �s����
			* @return �o�͌��ʂƓ���
			*/
			inline const Vector3* getRow(s32 index,Vector3* out)const{out->x=this->m[index][0];out->y=this->m[index][1];out->z=this->m[index][2];;return out;}
			/**
			* @brief �񐬕����擾����
			* @param[in] index �擾�����̃C���f�b�N�X
			* @param[out] out �񐬕�
			* @return �o�͌��ʂƓ���
			*/
			inline const Vector3* getCol(s32 index,Vector3* out)const{out->x=this->m[0][index];out->y=this->m[1][index];out->z=this->m[2][index];return out;}
			/**
			* @brief ���s�ړ��������擾����
			* @param[out] out �ړ�����
			* @return �o�͌��ʂƓ���
			*/
			inline const Vector3* getT(Vector3* out)const{out->x=this->_41;out->y=this->_42;out->z=this->_43;return out;}
			/**
			* @brief �s��̐����𕪉�����
			* @param[out] s �g�听��
			* @param[out] q ��]�N�H�[�^�j�I������
			* @param[out] p �ړ�����
			*/
			void decompose(Vector3* s,Quaternion* q,Vector3* p)const;
			/**
			* @brief �s��̐������x�N�g���ɕ�������
			* @param[out] x �s���X������
			* @param[out] y �s���Y������
			* @param[out] z �s���Z������
			* @param[out] p �s��̈ʒu����
			*/
			void decompose(Vector3* x,Vector3* y,Vector3* z,Vector3* p)const;
			/**
			* @brief ��]�����݂̂��s��ɕ�������
			* 3x3�v�f�𐳋K�����Ĉʒu����0�ɂ������̂��o��
			* @param[out] m ��]�s��
			* @return �����̐���
			*/
			const Matrix* decomposeR(Matrix* m)const;
			/**
			* @brief ��]�����݂̂��N�H�[�^�j�I���ɕ�������
			* 3x3�v�f�𐳋K�����Ĉʒu����0�ɂ������̂��o��
			* @param[out] q ��]�N�H�[�^�j�I��
			* @return �����̐���
			*/
			const Quaternion* decomposeR(Quaternion* q)const;
			//*****************************************************************************
			//		�ϊ��֘A
			//*****************************************************************************
			/**
			* @brief �N�H�[�^�j�I���ɕϊ�����(�X�P�[�����O����Ă���ƈӐ}�������ʂɂȂ�Ȃ�)
			* @param[out] q ��]�N�H�[�^�j�I��
			* @return �o�͌��ʂƓ���
			*/
			const Quaternion* toQuaternion(Quaternion* q)const;
			/**
			* @brief XYZ�I�[�_�[�I�C���[�p�ɕϊ�����(�X�P�[�����O����Ă���ƈӐ}�������ʂɂȂ�Ȃ�)
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