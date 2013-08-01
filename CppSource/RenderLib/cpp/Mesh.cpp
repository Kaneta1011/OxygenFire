//	use .h
#include	"RenderLib\\Object3D\\Mesh.h"
#include	"math\\kmathf.h"
#include	<stdio.h>
#include	<stdlib.h>
#include	<GLES2/gl2.h>
#include	<GLES2/gl2ext.h>
//	use namespace
using namespace RenderLib;
using namespace klib::math;


void Transform_WorldMatrix_XYZ( 
	Matrix *Mat,
	const Vector3& Pos,
	const Vector3& Angle,
	const Vector3& Scale )
{
	float	sx, sy, sz, cx, cy, cz;

	sx = sinf(Angle.x);	sy = sinf(Angle.y);	sz = sinf(Angle.z);
	cx = cosf(Angle.x);	cy = cosf(Angle.y);	cz = cosf(Angle.z);

	Mat->_11 = (float)( cy*cz + sx*sy*sz ) * Scale.x;
	Mat->_12 = (float)( cx*sz ) * Scale.x;
	Mat->_13 = (float)(-sy*cz + sx*cy*sz ) * Scale.x;
	Mat->_14 = (float)(0.0f);

	Mat->_21 = (float)(-cy*sz + sx*sy*cz ) * Scale.y;
	Mat->_22 = (float)( cx*cz ) * Scale.y;
	Mat->_23 = (float)( sy*sz + sx*cy*cz ) * Scale.y;
	Mat->_24 = (float)(0.0f);

	Mat->_31 = (float)( cx*sy ) * Scale.z;
	Mat->_32 = (float)(-sx ) * Scale.z;
	Mat->_33 = (float)( cx*cy ) * Scale.z;
	Mat->_34 = (float)(0.0f);

	Mat->_41 = Pos.x;
	Mat->_42 = Pos.y;
	Mat->_43 = Pos.z;
	Mat->_44 = (float)(1.0f);
}



//----------------------------------------------------------------------
//	Mesh
//----------------------------------------------------------------------
void Mesh::Destroy()
{
	delete posData; posData = NULL;
}

void Mesh::Init()
{
	m_VertexMax = 0;
}

void Mesh::Update()
{
	Transform_WorldMatrix_XYZ(&m_WorldMatrix,m_Position,m_Angle,m_Scale);
}

void Mesh::Render(Shader* spShader)
{
	spShader->Send_Matrix(m_WorldMatrix);

spShader->Begin();
	//glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

	//glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,posData);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, m_VertexMax);
spShader->End();
}

void Mesh::Create_Box(float w,float h,float d)
{
	m_VertexMax = 36;
	posData = new Vector3[m_VertexMax];

	//	ê≥ñ 
	posData[0] = Vector3( -w,  h,  d );
	posData[1] = 	Vector3(  w, -h,  d );
	posData[2] = 	Vector3( -w, -h,  d );

	posData[3] = 	Vector3(  w, -h,  d );
	posData[4] = 	Vector3( -w,  h,  d );
	posData[5] = 	Vector3(  w,  h,  d );

	//	ó†ñ 
	posData[6] = Vector3(  w,  h, -d );
	posData[7] = Vector3( -w,  h, -d ); 
	posData[8] = Vector3(  w, -h, -d ); 

	posData[9] = Vector3( -w,  h, -d ); 
	posData[10] = Vector3( -w, -h, -d ); 
	posData[11] = Vector3(  w, -h, -d ); 

	//	âEñ 
	posData[12] = Vector3(  w,  h,  d ); 
	posData[13] = Vector3(  w,  h, -d ); 
	posData[14] = Vector3(  w, -h,  d ); 

	posData[15] = Vector3(  w,  h, -d ); 
	posData[16] = Vector3(  w, -h, -d ); 
	posData[17] = Vector3(  w, -h,  d ); 

	//	ç∂ñ 
	posData[18] = Vector3( -w,  h, -d ); 
	posData[19] = Vector3( -w,  h,  d ); 
	posData[20] = Vector3( -w, -h, -d ); 

	posData[21] = Vector3( -w,  h,  d );
	posData[22] = Vector3( -w, -h,  d );
	posData[23] = Vector3( -w, -h, -d ); 

	//	è„ñ 
	posData[24] = Vector3(   w,  h, d ); 
	posData[25] = Vector3(  -w,  h, d ); 
	posData[26] = Vector3(   w,  h, -d );

	posData[27] = Vector3(  -w,  h,  d );
	posData[28] = Vector3(  -w,  h, -d );
	posData[29] = Vector3(   w,  h, -d );

	//	â∫ñ 
	posData[30] = Vector3( -w, -h,  d );
	posData[31] = Vector3(  w, -h,  d );
	posData[32] = Vector3( -w, -h, -d ); 

	posData[33] = Vector3(  w, -h,  d );
	posData[34] = Vector3(  w, -h, -d );
	posData[35] = Vector3( -w, -h, -d );

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,posData);
	
	glEnableVertexAttribArray(0);
}