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
	VB_COLOR,
		
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

	//	Location
	void Location(int Num,char* Name)
	{
		glBindAttribLocation(m_Program,Num,Name);
	}
	//	Begin_End
	void Begin();
	void End();
	//	Link
	void Link(){glLinkProgram(m_Program);}
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



LOGI("---------------------------------------------------------\n");
LOGI("%f %f %f", p->getEffectData()->vel.x,p->getEffectData()->vel.y,p->getEffectData()->vel.z);
LOGI("%f %f %f", p->getEffectData()->rVelMin.x,p->getEffectData()->rVelMin.y,p->getEffectData()->rVelMin.z);
LOGI("%f %f %f", p->getEffectData()->rVelMax.x,p->getEffectData()->rVelMax.y,p->getEffectData()->rVelMax.z);
LOGI("---------------------------------------------------------\n");




		sEffectManager->Create(TEST,
			Vector3(
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7)));

		sEffectManager->Create(FIRE_BALL,
			Vector3(
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7)));

		sEffectManager->Create(FIRE_CHARGE,
			Vector3(
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7)));

		sEffectManager->Create(EXPRO,
			Vector3(
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7)));

		sEffectManager->Create(FIRE,
			Vector3(
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7)));

		sEffectManager->Create(FIRE_ANIME,
			Vector3(
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7)));

		sEffectManager->Create(FIRE_SP,
			Vector3(
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7)));

		sEffectManager->Create(WIND,
			Vector3(
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7)));

		sEffectManager->Create(YOSI_WIND,
			Vector3(
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7),
				getRandomNumberFloat(-7,7)));

#endif