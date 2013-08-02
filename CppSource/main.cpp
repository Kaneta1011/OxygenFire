//	use .h
#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include "../CppSource/math/kmathf.h"
#include	"ShaderLib\ShaderManager.h"
#include	"RenderLib\RenderState.h"
#include	"math\\kmathf.h"
#include	"RenderLib\\Object3D\\Mesh.h"
#include	<assert.h>

//	use namespace
using namespace ShaderLib;
using namespace RenderLib;
using namespace klib::math;

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


Mesh* spMesh;


#define TEST_MAX 100
Vector3 testPos[TEST_MAX];

float getRandomNumberFloat( float Min, float Max )
{
	if( Min < 0 )
		Max += -Min;
	else
		Max -= Min;

	return ( float )rand() / ( float )RAND_MAX * Max + Min;
}

//----------------------------------------------------------------------
//	Initialize
//----------------------------------------------------------------------
bool Initialize() 
{
	ShaderManager::Init();



	//LOGI("%d\n%s",__LINE__,__FILE__);
	//assert(!""); 
	

	spMesh = new Mesh;
	spMesh->Init();
	spMesh->Create_Box(0.5f,0.5f,0.5f);
	spMesh->setPosition(0,0,0);
	spMesh->setAngle(0,0,0);
	spMesh->setScale(1,1,1);
	spMesh->Update();

	for(int n=0;n<TEST_MAX;n++)
	{
		testPos[n] = Vector3(getRandomNumberFloat(-10,10),
			getRandomNumberFloat(-10,10),getRandomNumberFloat(-10,10));
	}

  return true;
}

//----------------------------------------------------------------------
//	delete
//----------------------------------------------------------------------
void Delete()
{
	ShaderManager::Delete();

	delete spMesh; spMesh=NULL;
}

//----------------------------------------------------------------------
//	Update
//----------------------------------------------------------------------
bool Update()
{
	//	Matrixê›íË
	RenderState::Setting_ViewMatrix(Vector3(20,20,20),Vector3(0,0,0),Vector3(0,1,0));
	RenderState::Setting_PerspectiveMatrix((float)K_PI/4, 
		(float)RenderState::getScreenWidth()/(float)RenderState::getScreenHeight(),
		0.1f, 100.0f );

	//	angleÉeÉXÉg
	static float workX = .0f;
	static float workY = .0f;
	static float workZ = .0f;
	workX += 0.01f;
	workY += 0.01f;
	workZ += 0.01f;

	spMesh->setAngle(Vector3(workX,workY,workZ));
	//spMesh->Update();

	return true;
}
//----------------------------------------------------------------------
//	Render
//----------------------------------------------------------------------
void Render() 
{
	glClearColor(0.3f,0.3f,0.3f,1.0f);
  RenderState::Clear_Buffer(CLEAR_BUFFER_COLOR);
	RenderState::Clear_Buffer(CLEAR_BUFFER_DEPTH);
	RenderState::Setting_Viewport(0,0,RenderState::getScreenWidth(),RenderState::getScreenHeight());




	for(int n=0;n<TEST_MAX;n++)
	{
		spMesh->setPosition(testPos[n]);
		spMesh->Update();
		spMesh->Render(ShaderManager::getSimple());
	}
}


//----------------------------------------------------------------------
//
//	JAVAë§Ç…ìnÇ∑ä÷êî
//
//----------------------------------------------------------------------
extern "C" {
    JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_update(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
	RenderState::setScreenWidth(width);
	RenderState::setScreenHeight(height);
	RenderState::Setting_PolygonBathSides(false);
	 
	Initialize();
}
 
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_update(JNIEnv * env, jobject obj)
{
	if(Update()){Render();}
}
