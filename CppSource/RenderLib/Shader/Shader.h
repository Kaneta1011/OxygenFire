#ifndef __teShader_H__
#define __teShader_H__
#include <GLES2/gl2.h>
#include	"math\\kmathf.h"

namespace RenderLib{

using namespace klib::math;

//----------------------------------------------------------------------
//	Shader
//----------------------------------------------------------------------
class Shader
{
public:
	//	Load
	void Load(GLuint* Shader,GLenum Type,const char* Filename);
	//	Init
	void Init(char* VS,char* FS);
	//	Begin_End
	void Begin();
	void End();
	//	Send
	void Send_Matrix(const Matrix& mWorld);
	//	SetValue
	void SetValue(const char* Name, const Matrix& Mat);
	//	BasicSet
	Shader(){}
	~Shader();
private:
	//	Data
	GLuint	m_Program;
};

}//RenderLib


#endif