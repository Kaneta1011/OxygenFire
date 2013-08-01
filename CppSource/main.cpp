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
//	use namespace
using namespace ShaderLib;
using namespace RenderLib;
using namespace klib::math;


Mesh* spMesh;

//----------------------------------------------------------------------
//	Initialize
//----------------------------------------------------------------------
bool Initialize(int Width, int Height) 
{
	ShaderManager::Init();

	RenderState::Setting_Viewport(0,0,Width,Height);

	spMesh = new Mesh;
	spMesh->Init();
	spMesh->Create_Box(0.5f,0.5f,0.5f);
	spMesh->setPosition(0,0,0);
	spMesh->setAngle(0,0,0);
	spMesh->setScale(1,1,1);
	spMesh->Update();

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
	RenderState::Setting_ViewMatrix( 
		Vector3(2,2,2),Vector3(0,0,0),Vector3(0,1,0));

	RenderState::Setting_PerspectiveMatrix(
		(float)K_PI/4, 
		(float)RenderState::getScreenWidth()
		/(float)RenderState::getScreenHeight(),
		0.1f, 100.0f );

	spMesh->Update();

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

spMesh->Render(ShaderManager::getSimple());

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
	Initialize(width, height);

	RenderState::setScreenWidth(width);
	RenderState::setScreenHeight(height);
}


JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_update(JNIEnv * env, jobject obj)
{
	if(Update()){Render();}
}
