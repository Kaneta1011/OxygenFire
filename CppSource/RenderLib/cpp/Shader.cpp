//	use .h
#include	"RenderLib\\Shader\\Shader.h"
#include	<stdio.h>
#include	<stdlib.h>
#include	<GLES2/gl2.h>
#include	<GLES2/gl2ext.h>
#include	"RenderLib\\RenderState.h"
#include	"RenderLib\\Object3D\\Mesh.h"
//	use namespace
using namespace RenderLib;


//----------------------------------------------------------------------
//	Shader
//----------------------------------------------------------------------
Shader::~Shader()
{
	glDeleteProgram(m_Program);
}

void Shader::Load(GLuint* Shader,GLenum Type,const char* Filename)
{	
	*Shader = glCreateShader(Type);

	glShaderSource(*Shader, 1, &Filename, NULL);
  glCompileShader(*Shader);
}

void Shader::Init(const char* VS,const char* FS)
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLint linked;

	m_Program = glCreateProgram();

	//	Load
	Load(&vertexShader,GL_VERTEX_SHADER,VS);
	Load(&fragmentShader,GL_FRAGMENT_SHADER,FS);

	//	Attach
	glAttachShader(m_Program,vertexShader);
	glAttachShader(m_Program,fragmentShader);

	//	Bind
	glBindAttribLocation(m_Program,VERTEX_INDEX_POS,"VPosition");
	glBindAttribLocation(m_Program,VERTEX_INDEX_COLOR,"VColor");

	glLinkProgram(m_Program);


	//glGetProgramiv(
	//	m_Program,GL_LINK_STATUS,&linked);
}

void Shader::Begin()
{
	glUseProgram(m_Program);
}

void Shader::End()
{
	glUseProgram(0);
}

void Shader::Send_Matrix(const Matrix& mWorld)
{
	//	WMP
	Matrix wvp = 
		mWorld * 
		RenderState::getViewMatrix() *
		RenderState::getProjectionMatrix();

  SetValue("WVP", wvp );
}

void Shader::SetValue(const char* Name, const Matrix& Mat)
{
Begin();
	GLint work = glGetUniformLocation( m_Program, Name );
  glUniformMatrix4fv( work, 1, GL_FALSE, &Mat.m[0][0] );
End();
}