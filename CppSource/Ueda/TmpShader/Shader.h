#ifndef __teShader_H__
#define __teShader_H__

#include <GLES2/gl2.h>
#include	"math\\kmathf.h"

namespace RenderLib{

using namespace klib::math;
enum eVB_TYPE
{
	VB_POSITION,
	VB_TEXCOORD,

	VB_MAX,
};
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
	void SetValue(const char* Name,int Value);
	void SetValue(const char* Name,float Value);
	void SetValue(const char* Name,float x,float y, float z);
	void SetValue(const char* Name,float x,float y, float z, float w);
	void SetValue(const char* Name,const Vector2& Value);
	void SetValue(const char* Name,const Vector3& Value);
	void SetValue(const char* Name,const Vector4& Value);
	void SetValue_No_BeginEnd(const char* Name,int Value);

	int  getUniformLocation(const char *name);

	//	BasicSet
	Shader(){}
	~Shader();
private:
	//	Data
	GLuint	m_Program;
};

}//RenderLib


#endif






#if 0
#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");
assert(!"");


#endif