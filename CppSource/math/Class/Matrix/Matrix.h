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
		* @class 4x4行列クラス
		*/
		struct Matrix:public float4x4
		{
			/**
			* @brief コンストラクタ
			*/
			Matrix();
			inline operator f32*(){return (f32*)m;}
			inline operator const f32*()const{return (const f32*)m;}
			/**
			* @brief *演算子
			* @param[in] mat 掛ける行列
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
			* @brief *=演算子
			* @param[in] mat 掛ける行列
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
			* @brief 単位行列化
			* @return this
			*/
			const Matrix* identity();
			/**
			* @brief 逆行列化
			* @return this
			*/
			const Matrix* inverse();
			/**
			* @brief 正規化
			* 各軸成分を正規化する
			* @return this
			*/
			const Matrix* normalize();
			/**
			* @brief 行列転置
			* @return this
			*/
			inline const Matrix* transpose()
			{
				Matrix work=*this;
				for(int i=0;i<4;i++)for(int j=0;j<4;j++)this->m[j][i]=work.m[i][j];
				return this;
			}
			/**
			* @brief 行列式を解く
			* @return 行列式
			*/
			f32 determinant()const;
			//*****************************************************************************
			//		成分操作関連
			//*****************************************************************************
			/**
			* @brief 平行移動成分を代入する
			* @param[in] x x移動成分
			* @param[in] y y移動成分
			* @param[in] z z移動成分
			* @return this
			*/
			const Matrix* setT(f32 x,f32 y,f32 z);
			/**
			* @brief 平行移動成分を代入する
			* @param[in] pos 移動成分
			* @return this
			*/
			inline const Matrix* setT(const Vector3& pos){return this->setT(pos.x,pos.y,pos.z);}
			/**
			* @brief 回転成分を代入する(XYZオーダー)
			* @param[in] x x角成分
			* @param[in] y y角成分
			* @param[in] z z角成分
			* @return this
			*/
			const Matrix* setRXYZ(f32 x,f32 y,f32 z);
			/**
			* @brief 回転成分を代入する(XYZオーダー)
			* @param[in] angle オイラー角成分
			* @return this
			*/
			inline const Matrix* setRXYZ(const Vector3& angle){return this->setRXYZ(angle.x,angle.y,angle.z);}
			/**
			* @brief 回転成分を代入する(ZXYオーダー)
			* @param[in] x x角成分
			* @param[in] y y角成分
			* @param[in] z z角成分
			* @return this
			*/
			const Matrix* setRZXY(f32 x,f32 y,f32 z);
			/**
			* @brief 回転成分を代入する(ZXYオーダー)
			* @param[in] angle オイラー角成分
			* @return this
			*/
			inline const Matrix* setRZXY(const Vector3& angle){return this->setRZXY(angle.x,angle.y,angle.z);}
			/**
			* @brief 任意軸回転成分を代入する
			* @param[in] axis x角成分
			* @param[in] r 回転角成分
			* @return this
			*/
			const Matrix* setR(const Vector3& axis,f32 r);
			/**
			* @brief クォータニオン回転成分を代入する
			* @param[in] q クォータニオン
			* @return this
			*/
			const Matrix* setR(const Quaternion& q);
			/**
			* @brief X軸回転成分を代入する
			* @param[in] r 回転角
			* @return this
			*/
			const Matrix* setRX(f32 r);
			/**
			* @brief y軸回転成分を代入する
			* @param[in] r 回転角
			* @return this
			*/
			const Matrix* setRY(f32 r);
			/**
			* @brief z軸回転成分を代入する
			* @param[in] r 回転角
			* @return this
			*/
			const Matrix* setRZ(f32 r);
			/**
			* @brief 拡大成分を乗算する
			* @param[in] x x拡大成分
			* @param[in] y y拡大成分
			* @param[in] z z拡大成分
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
			* @brief 拡大成分を乗算する
			* @param[in] xyz 拡大成分
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
			* @brief 拡大成分を乗算する
			* @param[in] s 拡大成分
			* @return this
			*/
			inline const Matrix* mulS(const Vector3& s){return this->mulS(s.x,s.y,s.z);}
			//*****************************************************************************
			//		成分取得関連
			//*****************************************************************************
			/**
			* @brief 行成分を取得する
			* @param[in] index 取得する行のインデックス
			* @param[out] out 行成分
			* @return 出力結果と同じ
			*/
			inline const Vector4* getRow(s32 index,Vector4* out)const{out->x=this->m[index][0];out->y=this->m[index][1];out->z=this->m[index][2];out->w=this->m[index][3];return out;}
			/**
			* @brief 列成分を取得する
			* @param[in] index 取得する列のインデックス
			* @param[out] out 列成分
			* @return 出力結果と同じ
			*/
			inline const Vector4* getCol(s32 index,Vector4* out)const{out->x=this->m[0][index];out->y=this->m[1][index];out->z=this->m[2][index];out->w=this->m[3][index];return out;}
				/**
			* @brief 行成分を取得する
			* @param[in] index 取得する行のインデックス
			* @param[out] out 行成分
			* @return 出力結果と同じ
			*/
			inline const Vector3* getRow(s32 index,Vector3* out)const{out->x=this->m[index][0];out->y=this->m[index][1];out->z=this->m[index][2];;return out;}
			/**
			* @brief 列成分を取得する
			* @param[in] index 取得する列のインデックス
			* @param[out] out 列成分
			* @return 出力結果と同じ
			*/
			inline const Vector3* getCol(s32 index,Vector3* out)const{out->x=this->m[0][index];out->y=this->m[1][index];out->z=this->m[2][index];return out;}
			/**
			* @brief 平行移動成分を取得する
			* @param[out] out 移動成分
			* @return 出力結果と同じ
			*/
			inline const Vector3* getT(Vector3* out)const{out->x=this->_41;out->y=this->_42;out->z=this->_43;return out;}
			/**
			* @brief 行列の成分を分解する
			* @param[out] s 拡大成分
			* @param[out] q 回転クォータニオン成分
			* @param[out] p 移動成分
			*/
			void decompose(Vector3* s,Quaternion* q,Vector3* p)const;
			/**
			* @brief 行列の成分をベクトルに分解する
			* @param[out] x 行列のX軸成分
			* @param[out] y 行列のY軸成分
			* @param[out] z 行列のZ軸成分
			* @param[out] p 行列の位置成分
			*/
			void decompose(Vector3* x,Vector3* y,Vector3* z,Vector3* p)const;
			/**
			* @brief 回転成分のみを行列に分解する
			* 3x3要素を正規化して位置情報を0にしたものを出力
			* @param[out] m 回転行列
			* @return 分解の成功
			*/
			const Matrix* decomposeR(Matrix* m)const;
			/**
			* @brief 回転成分のみをクォータニオンに分解する
			* 3x3要素を正規化して位置情報を0にしたものを出力
			* @param[out] q 回転クォータニオン
			* @return 分解の成功
			*/
			const Quaternion* decomposeR(Quaternion* q)const;
			//*****************************************************************************
			//		変換関連
			//*****************************************************************************
			/**
			* @brief クォータニオンに変換する(スケーリングされていると意図した結果にならない)
			* @param[out] q 回転クォータニオン
			* @return 出力結果と同じ
			*/
			const Quaternion* toQuaternion(Quaternion* q)const;
			/**
			* @brief XYZオーダーオイラー角に変換する(スケーリングされていると意図した結果にならない)
			* @param[out] out オイラー角
			* @return 出力結果と同じ
			*/
			const Vector3* toEulerXYZ(Vector3* out)const;
			/**
			* @brief ZXYオーダーオイラー角に変換する(スケーリングされていると意図した結果にならない)
			* @param[out] out オイラー角
			* @return 出力結果と同じ
			*/
			const Vector3* toEulerZXY(Vector3* out)const;
		};
	};
};