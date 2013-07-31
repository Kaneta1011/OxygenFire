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
			//		演算オペレータ
			//*****************************************************************************
			inline Quaternion operator + () const { return Quaternion(x,y,z,w); }
			inline Quaternion operator - () const { return Quaternion(-x,-y,-z,-w); }
			//*****************************************************************************
			//		Quaternion演算オペレータ
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
			//		float演算オペレータ
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
			//		代入演算オペレータ
			//*****************************************************************************
			inline Quaternion& operator=( const Vector3& val ){ this->x=val.x; this->y=val.y; this->z=val.z; this->w=0.0f; return *this; }
			inline Quaternion& operator=( const Quaternion& val ){ this->x=val.x; this->y=val.y; this->z=val.z; this->w=val.w; return *this; }
			inline Quaternion& operator=( f32 val ){ this->x=val; this->y=val; this->z=val; this->w=val; return *this; }
			//*****************************************************************************
			//		関数
			//*****************************************************************************
			/**
			* @brief クォータニオンとの内積
			* @return 内積
			*/
			inline f32 dot(const Quaternion& val) const { return this->x*val.x + this->y*val.y + this->z*val.z + this->w*val.w; }
			/**
			* @brief 長さの2乗計算
			* @return 長さの2乗
			*/
			inline f32 lengthSq() const { return this->dot(*this); }
			/**
			* @brief 長さ計算
			* @return 長さ
			*/
			f32 length() const;
			/**
			* @brief 正規化
			* @return 出力結果と同じ(正規化できなければNULLが帰る)
			*/
			inline const Quaternion* normalize()
			{ 
				f32 len=this->length();
				if(len==0.0f)return NULL;
				*this /= len;
				return this;
			}
			/**
			* @brief 単位化
			* @return 出力結果と同じ
			*/
			inline const Quaternion* identity(){this->x=this->y=this->z=0.0f;this->w=1.0f;return this;}
			/**
			* @brief 共役化
			* @return 出力結果と同じ
			*/
			inline const Quaternion* conjugate(){ this->x=-this->x;this->y=-this->y;this->z=-this->z;return this;}
			/**
			* @brief 共役クォータニオンを作成
			* @return 共役クォータニオン
			*/
			inline Quaternion createConjugate()const
			{
				Quaternion ret=*this;
				ret.x=-ret.x;ret.y=-ret.y;ret.z=-ret.z;ret.w=-ret.w;
				return ret;
			}
			/**
			* @brief 逆数化して自身に代入
			* @return 出力結果と同じ(逆数化できなければNULLが帰る)
			*/
			inline const Quaternion* inverse()
			{
				this->conjugate();
				f32 lensq=this->lengthSq();
				*this/=lensq;
				return this;
			}
			/**
			* @brief 逆数クォータニオンを作成,作成できなければ強制終了する
			* @return 逆数クォータニオン
			*/
			inline Quaternion createInverse()const
			{
				Quaternion ret=this->createConjugate();
				f32 lensq=this->lengthSq();
				ret/=lensq;
				return ret;
			}
			/**
			* @brief クォータニオン同士の合成して新しいクォータニオンを作成する
			* 自身の回転にqの回転をさせる
			* @param[in] q 合成する回転
			* @return 回転結果のクォータニオン
			*/
			inline Quaternion comp(const Quaternion& q){return q*(*this);}
			/**
			* @brief クォータニオンをクォータニオンで回転
			* @param[in] q 回転クォータニオン
			* @return 自身のポインタ
			*/
			inline const Quaternion* rotate(const Quaternion& q){*this=q*(*this);return this;}
			/**
			* @brief クォータニオン球面線形補間して自身に代入する
			* @param[in] q 回転クォータニオン
			* @param[in] t 係数
			* @return 自身のポインタ
			*/
			const Quaternion* sLerp(const Quaternion& q,f32 t);
			//*****************************************************************************
			//		成分操作関連
			//*****************************************************************************
			/**
			* @brief オイラー角からX軸回転クォータニオン作成
			* @param[in] rad x軸回転成分
			* @return 自身のポインタ
			*/
			const Quaternion* setRX(f32 rad);
			/**
			* @brief オイラー角からY軸回転クォータニオン作成
			* @param[in] rad y軸回転成分
			* @return 自身のポインタ
			*/
			const Quaternion* setRY(f32 rad);
			/**
			* @brief オイラー角からZ軸回転クォータニオン作成
			* @param[in] rad z軸回転成分
			* @return 自身のポインタ
			*/
			const Quaternion* setRZ(f32 rad);
			/**
			* @brief オイラー角からクォータニオンを作成する(XYZオーダー)
			* @param[in] x x角成分
			* @param[in] y y角成分
			* @param[in] z z角成分
			* @return 自身のポインタ
			*/
			const Quaternion* setRXYZ(f32 sx,f32 sy,f32 sz);
			/**
			* @brief オイラー角からクォータニオンを作成する(XYZオーダー)
			* @param[in] angle オイラー角成分
			* @return 自身のポインタ
			*/
			inline const Quaternion* setRXYZ(const Vector3& angle){return this->setRXYZ(angle.x,angle.y,angle.z);}
			/**
			* @brief オイラー角からクォータニオンを作成する(ZXYオーダー)
			* @param[in] x x角成分
			* @param[in] y y角成分
			* @param[in] z z角成分
			* @return 自身のポインタ
			*/
			const Quaternion* setRZXY(f32 sx,f32 sy,f32 sz);
			/**
			* @brief オイラー角からクォータニオンを作成する(ZXYオーダー)
			* @param[in] angle オイラー角成分
			* @return 自身のポインタ
			*/
			inline const Quaternion* setRZXY(const Vector3& angle){return this->setRZXY(angle.x,angle.y,angle.z);}
			/**
			* @brief 任意の軸と回転角からクォータニオンを作成する
			* @param[in] axis 任意の軸
			* @param[in] rad 回転角
			* @return 自身のポインタ
			*/
			const Quaternion* setR( const Vector3 &axis, f32 rad );
			//*****************************************************************************
			//		成分取得関連
			//*****************************************************************************
			/**
			* @brief クォータニオンの軸と回転角に分解する
			* @param[out] axis 軸
			* @param[out] rad 回転角
			*/
			void decompose( Vector3* axis, f32* rad ) const;
			/**
			* @brief クォータニオンを姿勢ベクトルに分解する
			* @param[out] right X軸ベクトル
			* @param[out] up Y軸ベクトル
			* @param[out] front Z軸ベクトル
			*/
			void decompose( Vector3* right, Vector3* up,Vector3* front) const;

			//*****************************************************************************
			//		変換関連
			//*****************************************************************************
			/**
			* @brief 行列に変換する
			* @param[out] out 回転行列
			* @return 出力結果と同じ
			*/
			const Matrix* toMatrix(Matrix* out)const;
			/**
			* @brief XYZオーダーオイラー角に変換する
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