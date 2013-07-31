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
			//		演算オペレータ
			//*****************************************************************************
			inline Vector2 operator + () const { return Vector2(x,y); }
			inline Vector2 operator - () const { return Vector2(-x,-y); }
			//*****************************************************************************
			//		float演算オペレータ
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
			//		Vector2演算オペレータ
			//*****************************************************************************
			inline Vector2 operator+(const Vector2& val)const{return Vector2(this->x+val.x,this->y+val.y);}
			inline Vector2 operator-(const Vector2& val)const{return Vector2(this->x-val.x,this->y-val.y);}
			inline Vector2& operator+=(const Vector2& val){this->x+=val.x;this->y+=val.y;return *this;}
			inline Vector2& operator-=(const Vector2& val){this->x-=val.x;this->y-=val.y;return *this;}
			//*****************************************************************************
			//		代入演算オペレータ
			//*****************************************************************************
			inline Vector2& operator=(const Vector2& val){this->x=val.x;this->y=val.y;return *this;}
			inline Vector2& operator=(const Vector3& val);
			inline Vector2& operator=(const Vector4& val);
			//*****************************************************************************
			//		関数
			//*****************************************************************************
			/**
			* @brief ベクトルの内積を求める
			* @param[in] vec ベクトル
			* @return 内積
			*/
			inline f32 dot(const Vector2& vec)const{return this->x*vec.x + this->y*vec.y;}
			/**
			* @brief ベクトルの外積を求める
			* @param[in] vec ベクトル
			* @return 外積
			*/
			inline f32 cross(const Vector2& vec)const{return this->x*vec.y-vec.x*this->y;}
			/**
			* @brief ベクトル間の角度を求める
			* @param[in] vec ベクトル
			* @return 角度
			*/
			f32 degree(const Vector2& vec)const;
			/**
			* @brief ベクトルの長さの二乗を求める
			* @return 長さの二乗
			*/
			inline f32 lengthSq()const{return this->dot(*this);}
			/**
			* @brief ベクトルの長さを求める
			* @return 長さ
			*/
			f32 length()const;
			/**
			* @brief ベクトルの距離の二乗を求める
			* @return 距離の二乗
			*/
			f32 distanceSq(const Vector2& vec)const;
			/**
			* @brief ベクトルの距離を求める
			* @return 距離
			*/
			f32 distance(const Vector2& vec)const;
			/**
			* @brief ベクトルを正規化
			* @return 自身のポインタ
			*/
			const Vector2* normalize();
			/**
			* @brief ベクトルを線形補間し自身に格納
			* @param[in] from 補間ベクトル
			* @param[in] dest 補間ベクトル
			* @param[in] t 補間係数
			* @return 自身のポインタ
			*/
			const Vector2* lerp(const Vector3& from,const Vector3& dest,f32 t);
			/**
			* @brief 射影ベクトルを求めて自身に代入
			* @param[in] from 補間ベクトル
			* @param[in] dest 補間ベクトル
			* @return 自身のポインタ
			*/
			const Vector2* projection(const Vector3& from,const Vector3& dest);
			//*****************************************************************************
			//		成分操作関連
			//*****************************************************************************
			/**
			* @brief 角度からベクトルを作成する
			* @param[in] rad 角度
			* @return 自身のポインタ
			*/
			const Vector2* setR(f32 rad);
			/**
			* @brief ベクトルを任意の長さにする
			* @param[in] len 任意の長さ
			* @return 自身のポインタ
			*/
			const Vector2* setLength(f32 len);
			//*****************************************************************************
			//		変換関連
			//*****************************************************************************
			/**
			* @brief ベクトルを角度に変換する
			* @return 角度
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
			//		演算オペレータ
			//*****************************************************************************
			inline Vector3 operator + () const { return Vector3(x,y,z); }
			inline Vector3 operator - () const { return Vector3(-x,-y,-z); }
			//*****************************************************************************
			//		float演算オペレータ
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
			//		Vector2演算オペレータ
			//*****************************************************************************
			inline Vector3 operator+(const Vector2& val)const{return Vector3(this->x+val.x,this->y+val.y,this->z);}
			inline Vector3 operator-(const Vector2& val)const{return Vector3(this->x-val.x,this->y-val.y,this->z);}
			inline Vector3& operator+=(const Vector2& val){this->x+=val.x;this->y+=val.y;return *this;}
			inline Vector3& operator-=(const Vector2& val){this->x-=val.x;this->y-=val.y;return *this;}
			//*****************************************************************************
			//		Vector3演算オペレータ
			//*****************************************************************************
			inline Vector3 operator+(const Vector3& val)const{return Vector3(this->x+val.x,this->y+val.y,this->z+val.z);}
			inline Vector3 operator-(const Vector3& val)const{return Vector3(this->x-val.x,this->y-val.y,this->z-val.z);}
			inline Vector3& operator+=(const Vector3& val){this->x+=val.x;this->y+=val.y;this->z+=val.z;return *this;}
			inline Vector3& operator-=(const Vector3& val){this->x-=val.x;this->y-=val.y;this->z-=val.z;return *this;}
			//*****************************************************************************
			//		代入演算オペレータ
			//*****************************************************************************
			inline Vector3& operator=(const Vector2& val){this->x=val.x;this->y=val.y;return *this;}
			inline Vector3& operator=(const Vector3& val){this->x=val.x;this->y=val.y;this->z=val.z;return *this;}
			inline Vector3& operator=(const Vector4& val);
			inline Vector3& operator=(const Quaternion& val);
			//*****************************************************************************
			//		関数
			//*****************************************************************************
			/**
			* @brief ベクトル同士の内積
			* @param[in] val 内積を求めるベクトル
			* @return 求めた内積
			*/
			inline f32 dot(const Vector3& val)const { return this->x*val.x + this->y*val.y + this->z*val.z; }
			/**
			* @brief ベクトル同士の外積を求める
			* @param[out] out 求めた外積
			* @param[in] in 外積を求めるベクトル
			* @return 出力結果と同じ.
			*/
			const Vector3* cross(Vector3* out,const Vector3& in)const;
			/**
			* @brief ベクトル間の角度を求める
			* @param[in] in 角度を求めるベクトル
			* @return ベクトル間の角度
			*/
			f32 degree(const Vector3& in)const;
			/**
			* @brief ベクトルの長さの二乗を求める
			* @return ベクトルの長さの二乗
			*/
			inline f32 lengthSq()const{return this->dot(*this);}
			/**
			* @brief ベクトルの長さを求める
			* @return ベクトルの長さ
			*/
			f32 length()const;
			/**
			* @brief ベクトル同士の距離の二乗を求める
			* @return ベクトルの長さの二乗
			*/
			f32 distanceSq(const Vector3& vec)const;
			/**
			* @brief ベクトル同士の距離を求める
			* @return ベクトルの長さ
			*/
			f32 distance(const Vector3& vec)const;
			/**
			* @brief ベクトルを正規化する
			* @return 自身のポインタ
			*/
			const Vector3* normalize();
			/**
			* @brief ベクトル同士の線形補間し自身に代入する
			* @param[in] from 補間ベクトル
			* @param[in] dest 補間ベクトル
			* @param[in] t 補間係数
			* @return 求めたベクトル.
			*/
			const Vector3* lerp(const Vector3& from,const Vector3& dest,f32 t);
			/**
			* @brief ベクトル同士の球面線形補間し自身に代入する
			* @param[in] from 補間ベクトル
			* @param[in] dest 補間ベクトル
			* @param[in] t 補間係数
			* @return 求めたベクトル.
			*/
			const Vector3* sLerp(const Vector3& from,const Vector3& dest,f32 t);
			/**
			* @brief ベクトルをクォータニオンで回転させて自身に代入する
			* @param[in] q クォータニオン
			* @return 自身のポインタ
			*/
			const Vector3* rotate(const Quaternion& q);
			/**
			* @brief ベクトルを任意の軸と角度で回転させて自身に代入する
			* @param[in] axis 任意の軸
			* @param[in] r 回転角
			* @return 自身のポインタ
			*/
			const Vector3* rotate(const Vector3& axis,f32 r);
			/**
			* @brief ベクトルを(x,y,z,1)として行列変換して自身に代入する
			* @param[in] m 行列
			* @return 自身のポインタ
			*/
			const Vector3* trans(const Matrix& m);
			/**
			* @brief ベクトルを(x,y,z,0)として行列変換して自身に代入する
			* @param[in] m 行列
			* @return 自身のポインタ
			*/
			const Vector3* trans3x3(const Matrix& m);
			/**
			* @brief 反射ベクトルを格納する
			* @param[in] front 進行ベクトル
			* @param[in] normal 法線ベクトル
			* @return 自身のポインタ
			*/
			const Vector3* reflect(const Vector3& front,const Vector3& normal);
			/**
			* @brief 屈折ベクトルを格納する
			* @param[in] front 進行ベクトル
			* @param[in] normal 法線ベクトル
			* @param[in] Eta 屈折率
			* @return 自身のポインタ
			*/
			const Vector3* refract(const Vector3& front ,const Vector3& normal , f32 Eta);
			/**
			* @brief 射影ベクトルを格納する
			* @param[in] from 射影するベクトル
			* @param[in] dest 射影されるベクトル
			* @return 自身のポインタ
			*/
			const Vector3* projection(const Vector3& from,const Vector3& dest);

			//*****************************************************************************
			//		成分操作関連
			//*****************************************************************************
			/**
			* @brief ベクトル同士の外積を求めた後自身に代入する
			* @param[in] in 外積を求めるベクトル
			* @return 自身のポインタ
			*/
			const Vector3* setCross(const Vector3& in);
			/**
			* @brief 仰角と方位角からベクトルを求める
			* @param[in] pitch ピッチ角(X軸)
			* @param[in] yaw ヨー角(Y軸)
			* @return 求めたベクトル.
			*/
			const Vector3* setR( f32 pitch, f32 yaw );
			/**
			* @brief ベクトルを任意の長さにする
			* @param[in] len 任意の長さ
			* @return 自身のポインタ
			*/
			const Vector3* setLength(f32 len);
			//*****************************************************************************
			//		変換関連
			//*****************************************************************************
			/**
			* @brief ベクトルを仰角と方位角に変換する
			* @param[out] out 求めた角度
			* @return 求めた角度.
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
			//		演算オペレータ
			//*****************************************************************************
			inline Vector4 operator + () const { return Vector4(x,y,z,w); }
			inline Vector4 operator - () const { return Vector4(-x,-y,-z,-w); }
			//*****************************************************************************
			//		float演算オペレータ
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
			//		Vector2演算オペレータ
			//*****************************************************************************
			inline Vector4 operator+(const Vector2& val)const{return Vector4(this->x+val.x,this->y+val.y,this->z,this->w);}
			inline Vector4 operator-(const Vector2& val)const{return Vector4(this->x-val.x,this->y-val.y,this->z,this->w);}
			inline Vector4& operator+=(const Vector2& val){this->x+=val.x;this->y+=val.y;return *this;}
			inline Vector4& operator-=(const Vector2& val){this->x-=val.x;this->y-=val.y;return *this;}
			//*****************************************************************************
			//		Vector3演算オペレータ
			//*****************************************************************************
			inline Vector4 operator+(const Vector3& val)const{return Vector4(this->x+val.x,this->y+val.y,this->z+val.z,this->w);}
			inline Vector4 operator-(const Vector3& val)const{return Vector4(this->x-val.x,this->y-val.y,this->z-val.z,this->w);}
			inline Vector4& operator+=(const Vector3& val){this->x+=val.x;this->y+=val.y;this->z+=val.z;return *this;}
			inline Vector4& operator-=(const Vector3& val){this->x-=val.x;this->y-=val.y;this->z-=val.z;return *this;}
			//*****************************************************************************
			//		Vector4演算オペレータ
			//*****************************************************************************
			inline Vector4 operator+(const Vector4& val)const{return Vector4(this->x+val.x,this->y+val.y,this->z+val.z,this->w+val.w);}
			inline Vector4 operator-(const Vector4& val)const{return Vector4(this->x-val.x,this->y-val.y,this->z-val.z,this->w-val.w);}
			inline Vector4& operator+=(const Vector4& val){this->x+=val.x;this->y+=val.y;this->z+=val.z;this->w+=val.w;return *this;}
			inline Vector4& operator-=(const Vector4& val){this->x-=val.x;this->y-=val.y;this->z-=val.z;this->w-=val.w;return *this;}
			//*****************************************************************************
			//		Matrix演算オペレータ
			//*****************************************************************************
			//*****************************************************************************
			//		代入演算オペレータ
			//*****************************************************************************
			inline Vector4& operator=(const Vector2& val){this->x=val.x;this->y=val.y;return *this;}
			inline Vector4& operator=(const Vector3& val){this->x=val.x;this->y=val.y;this->z=val.z;return *this;}
			inline Vector4& operator=(const Vector4& val){this->x=val.x;this->y=val.y;this->z=val.z;this->w=val.w;return *this;}
			inline Vector4& operator=(const Quaternion& val);
			//*****************************************************************************
			//		関数
			//*****************************************************************************
			/**
			* @brief ベクトルの内積を求める
			* @return 内積
			*/
			inline f32 dot(const Vector4& val)const { return this->x*val.x + this->y*val.y + this->z*val.z + this->w*val.w; }
			/**
			* @brief ベクトルの長さの二乗を求める
			* @return 長さの二乗
			*/
			inline f32 lengthSq()const{return this->dot(*this);}
			/**
			* @brief ベクトルの長さを求める
			* @return 長さ
			*/
			f32 length()const;
			/**
			* @brief ベクトルを正規化する
			* @return 自身のポインタ
			*/
			const Vector4* normalize();
			/**
			* @brief ベクトルを線形補間する
			* @param[in] from 補間ベクトル
			* @param[in] dest 補間ベクトル
			* @param[in] t 補間係数
			* @return 出力結果と同じ
			*/
			const Vector4* lerp(const Vector4& from,const Vector4& dest,f32 t);
			/**
			* @brief ベクトルを行列変換する
			* @param[in] m 行列
			* @return 自身のポインタ
			*/
			const Vector4* trans(const Matrix& m);
		};
	};
};