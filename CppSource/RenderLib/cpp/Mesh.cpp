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
	delete m_VertexPos; m_VertexPos = NULL;
	delete m_VertexColor; m_VertexColor = NULL;
}

void Mesh::Init()
{
	m_VertexMax = 0;

	glEnableVertexAttribArray(VERTEX_INDEX_POS);
	glEnableVertexAttribArray(VERTEX_INDEX_COLOR);


	
	

}

void Mesh::Update()
{ 
	Transform_WorldMatrix_XYZ(&m_WorldMatrix,m_Position,m_Angle,m_Scale);
}

void Mesh::Render(Shader* spShader)
{
	spShader->Send_Matrix(m_WorldMatrix);

spShader->Begin();
	glDrawArrays(GL_TRIANGLES, 0, m_VertexMax);
spShader->End();
}

void Mesh::Create_Box(float w,float h,float d)
{
	m_VertexMax = 36;

	//--------------------------------------------
	//	座標
	//--------------------------------------------
	//	正面
	m_VertexPos = new Vector3[m_VertexMax];

	m_VertexPos[0] = Vector3( -w,  h,  d );
	m_VertexPos[1] = 	Vector3(  w, -h,  d );
	m_VertexPos[2] = 	Vector3( -w, -h,  d );

	m_VertexPos[3] = 	Vector3(  w, -h,  d );
	m_VertexPos[4] = 	Vector3( -w,  h,  d );
	m_VertexPos[5] = 	Vector3(  w,  h,  d );

	//	裏面
	m_VertexPos[6] = Vector3(  w,  h, -d );
	m_VertexPos[7] = Vector3( -w,  h, -d ); 
	m_VertexPos[8] = Vector3(  w, -h, -d ); 

	m_VertexPos[9] = Vector3( -w,  h, -d ); 
	m_VertexPos[10] = Vector3( -w, -h, -d ); 
	m_VertexPos[11] = Vector3(  w, -h, -d ); 

	//	右面
	m_VertexPos[12] = Vector3(  w,  h,  d ); 
	m_VertexPos[13] = Vector3(  w,  h, -d ); 
	m_VertexPos[14] = Vector3(  w, -h,  d ); 

	m_VertexPos[15] = Vector3(  w,  h, -d ); 
	m_VertexPos[16] = Vector3(  w, -h, -d ); 
	m_VertexPos[17] = Vector3(  w, -h,  d ); 

	//	左面
	m_VertexPos[18] = Vector3( -w,  h, -d ); 
	m_VertexPos[19] = Vector3( -w,  h,  d ); 
	m_VertexPos[20] = Vector3( -w, -h, -d ); 

	m_VertexPos[21] = Vector3( -w,  h,  d );
	m_VertexPos[22] = Vector3( -w, -h,  d );
	m_VertexPos[23] = Vector3( -w, -h, -d ); 

	//	上面
	m_VertexPos[24] = Vector3(   w,  h, d ); 
	m_VertexPos[25] = Vector3(  -w,  h, d ); 
	m_VertexPos[26] = Vector3(   w,  h, -d );

	m_VertexPos[27] = Vector3(  -w,  h,  d );
	m_VertexPos[28] = Vector3(  -w,  h, -d );
	m_VertexPos[29] = Vector3(   w,  h, -d );

	//	下面
	m_VertexPos[30] = Vector3( -w, -h,  d );
	m_VertexPos[31] = Vector3(  w, -h,  d );
	m_VertexPos[32] = Vector3( -w, -h, -d ); 

	m_VertexPos[33] = Vector3(  w, -h,  d );
	m_VertexPos[34] = Vector3(  w, -h, -d );
	m_VertexPos[35] = Vector3( -w, -h, -d );

	//--------------------------------------------
	//	色
	//--------------------------------------------
	m_VertexColor = new Vector4[m_VertexMax];
	//	正面
	m_VertexColor[0] = Vector4( 0, 0, 1, 1 );
	m_VertexColor[1] = 	Vector4(  0, 0, 1, 1 );
	m_VertexColor[2] = 	Vector4( 0, 0, 1, 1 );

	m_VertexColor[3] = 	Vector4(  0, 0, 1, 1 );
	m_VertexColor[4] = 	Vector4( 0, 0, 1, 1 );
	m_VertexColor[5] = 	Vector4(  0, 0, 1, 1);

	//	裏面
	m_VertexColor[6] = Vector4( 0, 0, 1, 1);
	m_VertexColor[7] = Vector4( 0, 0, 1, 1); 
	m_VertexColor[8] = Vector4( 0, 0, 1, 1); 

	m_VertexColor[9] =	Vector4( 0, 0, 0, 1);
	m_VertexColor[10] =	Vector4( 0, 0, 0, 1);
	m_VertexColor[11] =	Vector4( 0, 0, 0, 1);

	//	右面
	m_VertexColor[12] = Vector4( 1, 0, 0, 1); 
	m_VertexColor[13] = Vector4( 1, 0, 0, 1); 
	m_VertexColor[14] = Vector4( 1, 0, 0, 1); 

	m_VertexColor[15] = Vector4( 1, 0, 0, 1); 
	m_VertexColor[16] = Vector4( 1, 0, 0, 1); 
	m_VertexColor[17] = Vector4( 1, 0, 0, 1); 

	//	左面
	m_VertexColor[18] = Vector4( 1, 0, 0, 1); 
	m_VertexColor[19] = Vector4( 1, 0, 0, 1); 
	m_VertexColor[20] = Vector4( 1, 0, 0, 1); 

	m_VertexColor[21] = Vector4( 0, 0, 0, 1);
	m_VertexColor[22] = Vector4( 0, 0, 0, 1);
	m_VertexColor[23] = Vector4( 0, 0, 0, 1); 

	//	上面
	m_VertexColor[24] = Vector4( 0, 1, 0, 1); 
	m_VertexColor[25] = Vector4( 0, 1, 0, 1); 
	m_VertexColor[26] = Vector4( 0, 1, 0, 1 );

	m_VertexColor[27] = Vector4( 0, 1, 0, 1 );
	m_VertexColor[28] = Vector4( 0, 1, 0, 1 );
	m_VertexColor[29] = Vector4( 0, 1, 0, 1 );

	//	下面
	m_VertexColor[30] = Vector4( 0, 1, 0, 1);
	m_VertexColor[31] = Vector4( 0, 1, 0, 1);
	m_VertexColor[32] = Vector4( 0, 1, 0, 1); 

	m_VertexColor[33] = Vector4( 0, 0, 0, 1);
	m_VertexColor[34] = Vector4( 0, 0, 0, 1);
	m_VertexColor[35] = Vector4( 0, 0, 0, 1);

	glVertexAttribPointer(VERTEX_INDEX_POS,VERTEX_SIZE_POS,GL_FLOAT,GL_FALSE,0,m_VertexPos);
	glVertexAttribPointer(VERTEX_INDEX_COLOR,VERTEX_SIZE_COLOR,GL_FLOAT,GL_FALSE,0,m_VertexColor);

	glGenBuffers(VERTEX_INDEX_MAX,m_VBO);
	//	頂点属性データ用のバッファオブジェクトをバインドする
	glBindBuffer(GL_ARRAY_BUFFER,m_VBO[VERTEX_INDEX_POS]);
	glBufferData(GL_ARRAY_BUFFER,m_VertexMax*sizeof(Vector3),m_VertexPos,GL_STATIC_DRAW);

	//	要素インデックス用のバッファオブジェクトをバインドする
	glBindBuffer(GL_ARRAY_BUFFER,m_VBO[VERTEX_INDEX_COLOR]);
	glBufferData(GL_ARRAY_BUFFER,m_VertexMax*sizeof(Vector4),m_VertexColor,GL_STATIC_DRAW);

}