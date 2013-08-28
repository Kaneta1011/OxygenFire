#include	"Shader.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<GLES2/gl2.h>
#include	<GLES2/gl2ext.h>
#include	"GraphicsLib\Class\tRenderState\RenderState.h"
#include	"utility\utility.h"

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

void Shader::Init(char* VS,char* FS)
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
	glBindAttribLocation(m_Program,VB_POSITION,"VPosition");
	glBindAttribLocation(m_Program,VB_TEXCOORD,"VTexCoord");

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

void Shader::SetValue( 
	const char* Name, 
	int Value )
{
Begin();
	int loc = getUniformLocation(Name);
  if( loc >= 0 )
  {
      glUniform1i(loc, Value);
  }
End();
}

void Shader::SetValue( 
	const char* Name, 
	float Value )
{
Begin();
	GLint work = glGetUniformLocation(m_Program, Name);
	glUniform1f(work, Value);
End();
}

void Shader::SetValue( 
	const char* Name,
	const Vector4& Value)
{
Begin();
	GLint work = glGetUniformLocation( m_Program, Name );
	glUniform4f(work, Value.x, Value.y, Value.z, Value.w );
End();
}


void Shader::SetValue( 
	const char* Name,
	float x, float y, float z, float w)
{
Begin();
	GLint work = glGetUniformLocation( m_Program, Name );
	glUniform4f( work, x, y, z, w );
End();
}

void Shader::SetValue( 
	const char* Name,
	const Vector3& Value)
{
Begin();
	GLint work = glGetUniformLocation( m_Program, Name );
	glUniform3f(work, Value.x, Value.y, Value.z );
End();
}

void Shader::SetValue( 
	const char* Name,
	const Vector2& Value)
{
Begin();
	GLint work = glGetUniformLocation( m_Program, Name );
	glUniform2f(work, Value.x, Value.y );
End();
}

void Shader::SetValue( 
	const char* Name,
	float x, float y, float z )
{
Begin();
	GLint work = glGetUniformLocation( m_Program, Name );
	glUniform3f(work, x, y, z );
End();
}

void Shader::SetValue_No_BeginEnd( 
	const char* Name, int Value )
{
	int loc = getUniformLocation(Name);
  if( loc >= 0 )
  {
      glUniform1i(loc, Value);
  } else {
	  LOGE("TmpShader", "Failure SetValue_No_BeginEnd name=%s", Name);
      assert(!"ëóêMÇ…ÇµÇ¡ÇœÇ¢");
  }

}

int Shader::getUniformLocation(
	const char * name )
{
    return glGetUniformLocation(m_Program, name);
}