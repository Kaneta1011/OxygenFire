#ifndef __teObject3D_H__
#define __teObject3D_H__
#include	"math\\kmathf.h"


namespace RenderLib{

using namespace klib::math;

//----------------------------------------------------------------------
//	IObject3D
//----------------------------------------------------------------------
class IObject3D
{
public:
	//	add
	void addPosition(const Vector3& Value){m_Position+=Value;}
	void addPoisitionX( float Value ){ m_Position.x += Value; }
	void addPoisitionY( float Value ){ m_Position.y += Value; }
	void addPoisitionZ( float Value ){ m_Position.z += Value; }
	void addAngle( const Vector3& Value ){ m_Angle += Value; }
	void addAngleX( float Value ){ m_Angle.x += Value; }
	void addAngleY( float Value ){ m_Angle.y += Value; }
	void addAngleZ( float Value ){ m_Angle.z += Value; }
	//	set
	void setPosition( const Vector3& Value ){ m_Position = Value; }
	void setPosition( float x, float y, float z ){ m_Position.x=x;m_Position.y=y;m_Position.z=z; }
	void setPositionY( float Value ){ m_Position.y = Value; }
	void setScale( const Vector3& Value ){ m_Scale = Value; }
	void setScale( float x, float y, float z ){ m_Scale.x=x;m_Scale.y=y;m_Scale.z=z; }
	void setScale( float Value ){ m_Scale.x=m_Scale.y=m_Scale.z=Value; }
	void setAngle( const Vector3& Value ){ m_Angle = Value; }
	void setAngle( float x, float y, float z ){ m_Angle.x=x;m_Angle.y=y;m_Angle.z=z; }
	void setAngle( float Value ){ m_Angle.y = Value; }
	//	get
	Vector3 getPosition()const{ return m_Position; }
	float	 getPositionX()const{ return m_Position.x; }
	float	 getPositionY()const{ return m_Position.y; }
	float	 getPositionZ()const{ return m_Position.z; }
	Vector3 getScale()const{ return m_Scale; }
	Vector3 getAngle()const{ return m_Angle; }
	float	 getAngleX()const{ return m_Angle.x; }
	float	 getAngleY()const{ return m_Angle.y; }
	float	 getAngleZ()const{ return m_Angle.z; }
	//	BasicSet
	IObject3D(){Clear();}void Clear();
	virtual ~IObject3D(){Destroy();}void Destroy();
protected:
	//	Data
	Matrix				m_WorldMatrix;
	Vector3				m_Position;
	Vector3				m_Scale;
	Vector3				m_Angle;
};

}//_RenderLib



#endif