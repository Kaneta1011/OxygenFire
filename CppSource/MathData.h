#ifndef __TXITEM_H__
#define __TXITEM_H__
#include	<windows.h>
#include	<math.h>

namespace TE{
namespace MathLib{

//----------------------------------------------------------------------
//	Vector2D
//----------------------------------------------------------------------
class Vector2D 
{
public:
	//	Data
	float	x, y;
	//	BasicSet
	inline Vector2D(){}
	inline Vector2D( float X, float Y ){ x=X, y=Y; } 
	inline Vector2D( const Vector2D& V ){ x=V.x, y=V.y; }
	//	ZeroVector2D
	inline void ZeroVector2D(){x=y=0;}
	//	Length
	inline float Length()const{ return sqrtf( x*x + y*y ); }
	//	operator
	inline Vector2D& operator = ( const Vector2D& v ){ x=v.x; y=v.y; return *this; } 
	inline Vector2D operator - () const { Vector2D ret( -x, -y ); return ret; }
	inline Vector2D operator - ( const Vector2D& v ) const { return Vector2D(x-v.x, y-v.y); }
};
//----------------------------------------------------------------------
//	Vector3D
//----------------------------------------------------------------------
class Vector3D
{
public:
	//	Data
	float	x, y, z;
	//	BasicSet
	Vector3D() {}
	inline Vector3D( float X, float Y, float Z ){ x=X, y=Y, z=Z; } 
	inline Vector3D( const Vector3D& v ){ x=v.x, y=v.y, z=v.z; } 
	//	Length
	inline float Length()const{ return sqrtf(x*x + y*y + z*z); }
	inline float LengthSq()const{ return x*x + y*y + z*z; }
	inline float LengthZX()const{ return sqrtf( x*x + z*z ); }
	inline float LengthZXSq()const{ return x*x + z*z; }
	//	Normalize
	inline void Normalize(){ float l = Length();if( l != .0f ){ x /= l; y /= l; z /= l; }}
	inline void NormalizeZX(){ y = 0; Normalize(); }
	inline Vector3D Normalized() const{ Vector3D temporary( *this ); temporary.Normalize(); return temporary;}
	inline Vector3D getNormalize() const{float l = Length();if( l == .0f ){return Vector3D( 0,0,0 );}return Vector3D( x / l, y / l, z / l );}
	//	Dot
	inline float Dot( const Vector3D& v )const{return x*v.x + y*v.y + z*v.z;}
	//	Cross
	inline Vector3D Cross( const Vector3D& v ) const{return Vector3D(y*v.z - z*v.y,z*v.x - x*v.z,x*v.y - y*v.x);}
	//	ZeroVector3D
	inline void ZeroVector3D(){ x=y=z=0; }
	//	Inversion(ベクトルを反転)
	inline void Inversion(){ x=-x; y=-y; z=-z; }
	inline Vector3D getInversion(){ return	Vector3D( -x, -y, -z ); }
	//	operator
	inline Vector3D& operator = ( const Vector3D& v ){ x=v.x; y=v.y; z=v.z; return *this; } 
	inline Vector3D& operator += ( const Vector3D& v ){ x+=v.x; y+=v.y; z+=v.z; return *this; } 
	inline Vector3D& operator -= ( const Vector3D& v ){ x-=v.x; y-=v.y; z-=v.z; return *this; }
	inline Vector3D& operator *= ( float v ){ x*=v; y*=v; z*=v; return *this; }
	inline Vector3D& operator /= ( float v ){ x/=v; y/=v; z/=v; return *this; }
	inline Vector3D operator + () const { Vector3D ret( x, y, z ); return ret; }
	inline Vector3D operator - () const { Vector3D ret( -x, -y, -z ); return ret; }
	inline Vector3D operator + ( const Vector3D& v ) const { return Vector3D(x+v.x, y+v.y, z+v.z); }
	inline Vector3D operator - ( const Vector3D& v ) const { return Vector3D(x-v.x, y-v.y, z-v.z); }
	inline Vector3D operator * ( const Vector3D& v ) const { return Vector3D(x*v.x, y*v.y, z*v.z); }
	inline Vector3D operator * ( float v ) const { Vector3D ret(x*v, y*v, z*v); return ret; }
	inline Vector3D operator / ( float v ) const { Vector3D ret(x/v, y/v, z/v); return ret; }
	inline bool operator == ( const Vector3D& v ) const { return (x==v.x) && (y==v.y) && (z==v.z); }
	inline bool operator != ( const Vector3D& v ) const { return (x!=v.x) || (y!=v.y) || (z!=v.z); }
};
//----------------------------------------------------------------------
//	Vector4D
//----------------------------------------------------------------------
class Vector4D 
{
public:
	Vector4D(){}
	Vector4D( float X, float Y, float Z, float W ){ x=X; y=Y; z=Z; w=W; }
	//	Data
	float	x, y, z, w;
};
//----------------------------------------------------------------------
//	Matrix4D
//----------------------------------------------------------------------
class Matrix3D;
class Matrix4D
{
public:
	//	Data
	union {
      struct {
          float _11, _12, _13, _14;
          float _21, _22, _23, _24;
          float _31, _32, _33, _34;
          float _41, _42, _43, _44;
      };
      float m[4][4];
	float _m[16];
  };
	//	BasicSet
  Matrix4D() {};
	Matrix4D( const float* p ){ memcpy( &_11, p, sizeof(Matrix4D)); }
	Matrix4D( const Matrix4D& mat ){ memcpy(&_11, &mat, sizeof(Matrix4D)); }
  Matrix4D( float f11, float f12, float f13, float f14,
          float f21, float f22, float f23, float f24,
          float f31, float f32, float f33, float f34,
          float f41, float f42, float f43, float f44 )
	{
		_11 = f11; _12 = f12; _13 = f13; _14 = f14;
		_21 = f21; _22 = f22; _23 = f23; _24 = f24;
		_31 = f31; _32 = f32; _33 = f33; _34 = f34;
		_41 = f41; _42 = f42; _43 = f43; _44 = f44;
	}
	void ZeroMatrix4D()
	{
		_11=_12=_13=_14=					/*	0 0 0 0		*/
		_21=_22=_23=_24=					/*	0 0 0 0		*/
		_31=_32=_33=_34=					/*	0 0 0 0		*/
		_41=_42=_43=_44 = 0;			/*	0 0 0 0 	*/
	}
	void UnitMatrix()
	{
		_11=1;_12=0;_13=0;_14=0;	/*	1 0 0 0		*/
		_21=0;_22=1;_23=0;_24=0;	/*	0 1 0 0		*/
		_31=0;_32=0;_33=1;_34=0;	/*	0 0 1 0		*/
		_41=0;_42=0;_43=0;_44=1;	/*	0 0 0 1		*/
	}
	void Transpose()
	{
		Matrix4D m=*this;

		_11=m._11; _12=m._21; _13=m._31; _14=m._41;	
		_21=m._12; _22=m._22; _23=m._32; _24=m._42;	
		_31=m._13; _32=m._23; _33=m._33; _34=m._43;	
		_41=m._14; _42=m._24; _43=m._34; _44=m._44;	
	}
	//	Inverse
	void Inverse();
	//	Get
	Matrix3D GetMatrix3D()const;
	//	operator
  float& operator () ( int r, int c ){ return m[r][c]; }
	float  operator () ( int r, int c ) const { return m[r][c]; }
	operator float* (){ return (float *) &_11; }
	operator const float* () const { return (const float *) &_11; }
  Matrix4D& operator *= ( const Matrix4D& );
  inline Matrix4D& operator += ( const Matrix4D& );
  inline Matrix4D& operator -= ( const Matrix4D& );
  inline Matrix4D& operator *= ( float );
  inline Matrix4D& operator /= ( float );
  inline Matrix4D operator + () const;
  inline Matrix4D operator - () const;
  Matrix4D operator * ( const Matrix4D& ) const;
  inline Matrix4D operator + ( const Matrix4D& ) const;
  inline Matrix4D operator - ( const Matrix4D& ) const;
  inline Matrix4D operator * ( float ) const;
  inline Matrix4D operator / ( float ) const;
  inline bool operator == ( const Matrix4D& ) const;
  inline bool operator != ( const Matrix4D& ) const;
};
//----------------------------------------------------------------------
//	Matrix3D
//----------------------------------------------------------------------
class Matrix3D
{
public:
	//	Data
	union {
      struct {
          float _11, _12, _13;
          float _21, _22, _23;
          float _31, _32, _33;
      };
      float m[3][3];
	float _m[9];
  };
	//	BasicSet
    Matrix3D() {};
	Matrix3D( const float* p ){ memcpy( &_11, p, sizeof(Matrix3D)); }
	Matrix3D( const Matrix3D& mat ){ memcpy(&_11, &mat, sizeof(Matrix3D)); }
    Matrix3D( float f11, float f12, float f13,
			  float f21, float f22, float f23,
              float f31, float f32, float f33 )
	{
		_11 = f11; _12 = f12; _13 = f13; 
		_21 = f21; _22 = f22; _23 = f23; 
		_31 = f31; _32 = f32; _33 = f33; 
	}
	//	Inverse
	void Inverse();
	//----------------------------------------------
	//	operator
	//----------------------------------------------
  float& operator () ( int r, int c ){return m[r][c]; }
	float  operator () ( int r, int c ) const { return m[r][c]; }
	operator float* (){ return (float *) &_11; }
	operator const float* () const { return (const float *) &_11; }
  Matrix3D& operator *= ( const Matrix3D& );
  inline Matrix3D& operator += ( const Matrix3D& );
  inline Matrix3D& operator -= ( const Matrix3D& );
  inline Matrix3D& operator *= ( float );
  inline Matrix3D& operator /= ( float );
  inline Matrix3D operator + () const;
  inline Matrix3D operator - () const;
  Matrix3D operator * ( const Matrix3D& ) const;
  inline Matrix3D operator + ( const Matrix3D& ) const;
  inline Matrix3D operator - ( const Matrix3D& ) const;
  inline Matrix3D operator * ( float ) const;
  inline Matrix3D operator / ( float ) const;
  inline bool operator == ( const Matrix3D& ) const;
  inline bool operator != ( const Matrix3D& ) const;
};
//----------------------------------------------------------------------
//	Quaternion
//----------------------------------------------------------------------
class Quaternion
{
public:
	//	Data
	float	x, y, z, w;
	//	BasicSet
	Quaternion():x(0),y(0),z(0),w(0){}
	Quaternion( float sx, float sy, float sz, float sw ) : x(sx), y(sy), z(sz), w(sw) {}
	Quaternion( Vector3D& v, float t ){ v.Normalize(); x = v.x*sinf(t*0.5f); y = v.y*sinf(t*0.5f); z = v.z*sinf(t*0.5f); w = cosf(t*0.5f); }
	//	Identity(生成)
	void Identity(){ x = y = z = 0; w = 1; }
	//	normalize
	inline void normalize()
	{
		float legnth = getLength();
		if(legnth == .0f ) return;
		float invL = 1.0f / legnth;
		(*this) *= invL;
	}
	//	情報取得
	inline float getLength() const{ return sqrtf( x*x + y*y + z*z + w*w); }
 	void toMatrix( Matrix4D& m );
	void fromMatrix( Matrix4D& mat );
	//	operator
	inline Quaternion operator + () const { Quaternion ret( x, y, z, w ); return ret; }
	inline Quaternion operator - () const { Quaternion ret( -x, -y, -z, -w ); return ret; }
	//	VS クォータニオン
	inline Quaternion& operator +=(const Quaternion& v){ x+=v.x; y+=v.y; z+=v.z;w+=v.w; return *this; }
	inline Quaternion& operator -=(const Quaternion& v){ x-=v.x; y-=v.y; z-=v.z; w-=v.w;return *this; }
	inline Quaternion& operator *=(const Quaternion& v)
	{
		x = y * v.z - z * v.y + x * v.w + w * v.x;
		y = z * v.x - x * v.z + y * v.w + w * v.y;
		z = x * v.y - y * v.x + z * v.w + w * v.z;
		w = w * v.w - x * v.x - y * v.y - z * v.z;
		return *this;
    }
	inline Quaternion operator +(const Quaternion& v) const{ return Quaternion( x+v.x, y+v.y, z+v.z, w+v.w); }
	inline Quaternion operator -(const Quaternion& v) const{ return Quaternion( x-v.x, y-v.y, z-v.z, w-v.w); }
	inline Quaternion operator *(const Quaternion& v) const
	{
		return Quaternion(
			y * v.z - z * v.y + x * v.w + w * v.x,
			z * v.x - x * v.z + y * v.w + w * v.y,
			x * v.y - y * v.x + z * v.w + w * v.z,
			w * v.w - x * v.x - y * v.y - z * v.z
		);
    }
	//	VS 値
	inline Quaternion& operator *=(float v){ x*=v; y*=v; z*=v; w*=v; return *this; }
	inline Quaternion& operator /=(float v){ x/=v; y/=v; z/=v; w/=v; return *this; }
	inline Quaternion operator *( float v ) const{ return Quaternion( x*v, y*v, z*v, w*v); }
	inline Quaternion operator /( float v ) const{ return Quaternion( x/v, y/v, z/v, w/v); }
};
//----------------------------------------------------------------------
//	Vertex2D
//----------------------------------------------------------------------
class Vertex2D
{
public:
	//	Data
	float	x, y;
	float	tu, tv;
	union {
		DWORD color;
		struct { BYTE b, g, r, a; };
	};
};
//----------------------------------------------------------------------
//	Vertex3D
//----------------------------------------------------------------------
class Vertex3D 
{
public:
	//	Data
	float	x, y, z;
	float	nx, ny, nz;
	float	tu, tv;

	union {
		DWORD color;
		struct { BYTE b, g, r, a; };
	};
};
//----------------------------------------------------------------------
//	Material3D
//----------------------------------------------------------------------
class Material3D
{
public:
	//	Data
	float ambientRed,
		  ambientGreen,
		  ambientBlue,
		  ambientAlpha;

	float diffuseRed,
		  diffuseGreen,
		  diffuseBlue,
		  diffuseAlpha;

	float specularRed,
		  specularGreen,
		  specularBlue,
		  specularAlpha;

	float specularPower;

};
//----------------------------------------------------------------------
//	Polygon3D
//----------------------------------------------------------------------
class Polygon3D
{
public:
	//	Data
	WORD		index[3];
	Vector3D	faceNormal;
	union {
		DWORD color;
		struct { BYTE b, g, r, a; };
	};
	float		textrue[3][2];
	Vector3D	vertexNormal[3];
	int			materials;
};
//----------------------------------------------------------------------
//	Bone3D
//----------------------------------------------------------------------
class Bone3D
{
public:
	//	Data
	Matrix4D	matrix;
	Quaternion	rotation;
	Vector3D	position;
	int			parentNumber;
	//	初期姿勢
	Matrix4D	offsetMatrix;
	Vector3D	initializePosition;
	Quaternion	initializeRotation;
	Matrix4D	workMatrix;
};
//----------------------------------------------------------------------
//	WeightBone3D
//----------------------------------------------------------------------
class WeightBone3D
{
public:
	//	Data
	int		index[4];
	float	weight[4];
};
//----------------------------------------------------------------------
//	COLOR
//----------------------------------------------------------------------
class COLOR
{
public:
	//	Data
	union {
		DWORD value;
		struct { BYTE blue, green, red, alpha; };
	}; 
	//	BasicSet   
	COLOR(){}
	COLOR(BYTE r,BYTE g,BYTE b, BYTE a){red=r;green=g;blue=b;alpha=a;}
	COLOR(DWORD Value){ value=Value; }
};
class FloatColor
{
public:
	float red, green, blue, alpha;
};



}//MathLib
}//TE


#endif