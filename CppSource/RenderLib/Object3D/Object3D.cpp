//	use .h
#include	"RenderLib\\Object3D\\Object3D.h"
//	use namespace
using namespace RenderLib;


void IObject3D::Clear()
{
	m_Position = Vector3( 0,0,0 );
	m_Scale = Vector3( 0,0,0 );
	m_Angle = Vector3( 0,0,0 );
}

void IObject3D::Destroy()
{
	
}


