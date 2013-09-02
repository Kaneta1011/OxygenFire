//	use .h
#include	"Shader.h"
#include	<stdio.h>
#include	<stdlib.h>
#include	<GLES2/gl2.h>
#include	<GLES2/gl2ext.h>
#include	"GraphicsLib\Class\tRenderState\RenderState.h"

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
//	use namespace
using namespace RenderLib;



bool isShaderError(GLenum shaderType,GLuint shader)
{
GLint compiled = 0;
glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
if (!compiled) {
	GLint infoLen = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
	//if(infoLen==0)return false;
	char* buf = new char[infoLen];
	glGetShaderInfoLog(shader, infoLen, NULL, buf);
	eprintf("Could not compile shader %d:\n%s\n",shaderType, buf);
	delete[] buf;

	glDeleteShader(shader);
	shader = 0;
}
else
{
	dprintf("Shader compile success\n");
	return false;
}
return true;
}




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



	isShaderError(Type,*Shader);
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
	glBindAttribLocation(m_Program,VB_POSITION,"Pos");
	glBindAttribLocation(m_Program,VB_TEXCOORD,"Tex");
	glBindAttribLocation(m_Program,VB_COLOR,"Color");

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
      assert(!"‘—M‚É‚µ‚Á‚Ï‚¢");
  }
}

int Shader::getUniformLocation(
	const char * name )
{
    return glGetUniformLocation(m_Program, name);
}