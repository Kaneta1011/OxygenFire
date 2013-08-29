#include "rTestScene.h"
#include "GraphicsLib\Class\tRenderState\RenderState.h"

#include "utility\debugMessageMng.h"//デバッグ用の文字列表示のためのヘッダー
#include "utility\utility.h"

#include "Ueda\TmpShader\ShaderManager.h"
#include "GraphicsLib\Class\rPointSprite\rPointSprite.h"
#include "GraphicsLib\Class\rFrameBufer\rFrameBuffer.h"

using namespace ShaderLib;
using namespace RenderLib;
using namespace klib::math;

const static int	SPRITE_MAX = 8000;

static const char* TAG = "rTestScene";

#define SIZE 512.f

#include <GLES2\gl2.h>
#include <GLES2\gl2ext.h>
#include <GLES2\gl2platform.h>
#include <EGL\egl.h>
#include <EGL\eglext.h>
#include <EGL\eglplatform.h>

using namespace klib;

kInputElementDesc desc[]=
{
	{"POSITION",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
	{"COLOR",0,k_VF_R32G32B32A32_FLOAT,0,eVertex,0},
	{"NORMAL",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
	{"TEXCOORD",0,k_VF_R32G32_FLOAT,0,eVertex,0}
};
u32 descnum=sizeof(desc)/sizeof(kInputElementDesc);

rTestScene::rTestScene():
	mp2dObj(NULL)
{
	Pos=NULL;
	Tex=NULL;
	frameBuffer = NULL;
	mButton = NULL;
}

void rTestScene::entry()
{
	mp2dObj = new rlib::r2DObj();
	this->mp2dObj->load("testImage.png");
	this->mp2dObj->setPos(0,0);

	//rlib::PointSprite::init();
	//Pos = new Vector3[SPRITE_MAX];
	LOGI(TAG,"OK Pos count=%d", SPRITE_MAX);
	Tex = new rlib::Texture();
	Tex->Initilize("testImage.png");
	LOGI(TAG,"OK Tex");
	LOGI(TAG,"sprite size = %.2f", SIZE);

	mButton = new rlib::CircleButton();
	mButton->init("testImage.png", 50, -50, 50.f);

	mStick = new rlib::AnalogStick();
	this->mStick->init(-60, -50, 40);

	frameBuffer = new rlib::FrameBuffer();
	frameBuffer->init(512,512);

	pipline=new kGraphicsPipline();
	pipline->createVertexShader("vertex.txt");
	pipline->createPixelShader("pixel.txt");
	pipline->createBlendState(k_BLEND_NONE);
	pipline->createDepthStencilState(true,eLESS_EQUAL);
	pipline->createRasterizerState(eSOLID,eNONE,false);
	pipline->complete(desc,descnum);

	float ary[3]={0.25f,0.5f,1.0f};
	pipline->setShaderValue("val",0.8f);
	pipline->setShaderValue("array",ary,3);

	mesh=new kSkin("kman.IEM",new kMeshLoadIEM(),new kMeshGLES20Render());
	mesh->setScale(0.05f);
	mesh->setPosition(0,0,0);
	mesh->Update();
}

#include "input\Input.h"

static int isMRT = 0;

void rTestScene::update()
{
	DEBUG_MSG_NON_ARAG("rTestScene");

	if( mlInput::getNowTouchCount() == 1 )
	{
		//if( mlInput::key() == mlInput::MOVE )
		//{
		//	float x = rlib::r2DHelper::convertPosX( mlInput::getX(0) );
		//	float y = rlib::r2DHelper::convertPosY( mlInput::getY(0) );
		//	x = mlInput::getMoveX();
		//	y = mlInput::getMoveY();
		//	this->mp2dObj->setPos(x+this->mp2dObj->getPos().x, y+this->mp2dObj->getPos().y);
		//}
	}
	else if( mlInput::isPinch() )
	{
		klib::math::Vector2 size = this->mp2dObj->getSize();
		size.x += mlInput::getPinchMoveLength() * 2.f;
		size.y += mlInput::getPinchMoveLength();
		this->mp2dObj->setSize(size);
	}

	mStick->update();
	mButton->update();

	if( this->mStick->enable() )
	{
		float x = this->mStick->getX();
		float y = this->mStick->getY();
		this->mp2dObj->setPos(x+this->mp2dObj->getPos().x, y+this->mp2dObj->getPos().y);
	}
	DEBUG_MSG("x=%.2f, y=%.2f", this->mp2dObj->getPos().x, this->mp2dObj->getPos().y );
	DEBUG_MSG("sx=%.2f, sy=%.2f", this->mp2dObj->getSize().x, this->mp2dObj->getSize().y );
	DEBUG_MSG("button %s", this->mStick->isPush()?"push":"non push");
	DEBUG_MSG("mode %d", this->mStick->getMode() );
	DEBUG_MSG("enable = %d", this->mStick->enable()?1:0 );
	DEBUG_MSG("rate.x=%.2f, rate.y=%.2f", this->mStick->getX(), this->mStick->getY() );
	DEBUG_MSG("sx=%.2f, sy=%.2f", this->mStick->getRange().x, this->mStick->getRange().y );

	if( //mlInput::key(2) == mlInput::DOWN )
		mButton->getMode() == rlib::IButton::eUP )
	{
		isMRT ++;

		isMRT %= 2;
	}
	//LOGI(TAG,"OK Update");

	static float a=0;
	a+=.005f;
	mesh->setAngle(a);
	mesh->animation(1);
	mesh->Update();
}

void rTestScene::render()
{
	rlib::FrameBuffer::bindScreenBuffer();

	if( isMRT == 0){
		//this->frameBuffer->bind();
		//mesh->Render(pipline);
		//this->mp2dObj->render();

		//rlib::FrameBuffer::bindScreenBuffer();
		//this->frameBuffer->setPos( this->mp2dObj->getPos() );
		//this->frameBuffer->setSize( this->mp2dObj->getSize() );
		//this->frameBuffer->render();
		this->mp2dObj->render();
	}
	else if( isMRT  == 1 )
	{
		mesh->Render(pipline);
		//this->frameBuffer->bind();
		//mesh->Render(pipline);

		//rlib::FrameBuffer::bindScreenBuffer();
		//this->frameBuffer->setPos( this->mp2dObj->getPos() );
		//this->frameBuffer->setSize( this->mp2dObj->getSize() );
		//this->frameBuffer->render();
	}
	else
	{
		//rlib::FrameBuffer::bindScreenBuffer();
		//this->mp2dObj->render();
		//mesh->Render(pipline);
	}

	//rlib::FrameBuffer::bindScreenBuffer();
	//for( int i=0; i<50; i++ )
	//{
	//	this->mp2dObj->setPos( rand()/(float)RAND_MAX * 200.f - 100.f, rand()/(float)RAND_MAX * 200.f - 100.f );
	//	this->mp2dObj->render();
	//}
	mButton->render();
	mStick->render();

}

void rTestScene::exit()
{
	LOGI(TAG, "Execute rTestScene::exit");

	rlib::PointSprite::clear();
	if( this->mp2dObj ){ delete this->mp2dObj; this->mp2dObj = NULL; }
	if( Tex ){ delete Tex; Tex = NULL; }
	LOGI(TAG,"delete Tex");
	if( Pos ){ delete[] Pos; Pos = NULL; }
	LOGI(TAG,"delete Pos");
	if( mButton ){ delete mButton; mButton = NULL; }
	LOGI(TAG,"delete Button");
	if( mStick ){ delete mStick; mStick = NULL; }
	LOGI(TAG,"delete mStick");


	if( frameBuffer ){ delete frameBuffer; frameBuffer = NULL; }
	LOGI(TAG,"delete frameBuffer");

	delete mesh;
	LOGI(TAG,"delete mesh");
	delete pipline;
	LOGI(TAG,"delete pipline");

	LOGI(TAG, "Complete rTestScene::exit");
}
