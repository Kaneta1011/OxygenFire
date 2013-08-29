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
	mButton->init("testImage.png", 0, 0, 0.001f);

	frameBuffer = new rlib::FrameBuffer();
	frameBuffer->init(512,512);
}

#include "input\Input.h"

static bool isMRT = false;

void rTestScene::update()
{
	DEBUG_MSG_NON_ARAG("rTestScene");

	if( mlInput::getNowTouchCount() == 1 )
	{
		if( mlInput::key() == mlInput::MOVE )
		{
			float x = rlib::r2DHelper::convertPosX( mlInput::getX(0) );
			float y = rlib::r2DHelper::convertPosY( mlInput::getY(0) );
			x = rlib::r2DHelper::convertMoveX( mlInput::getMoveX() );
			y = rlib::r2DHelper::convertMoveY( mlInput::getMoveY() );
			this->mp2dObj->setPos(x+this->mp2dObj->getPos().x, y+this->mp2dObj->getPos().y);
		}
	}

	mButton->update();

	if( //mlInput::key(2) == mlInput::DOWN |
		mButton->isPush() )
	{
		isMRT = !isMRT;
	}
	//LOGI(TAG,"OK Update");
}

void rTestScene::render()
{
	//RenderLib::RenderState::Clear_Color(0.1,0.125,0.6f, 1.f );
	//RenderLib::RenderState::Clear_Buffer(RenderLib::CLEAR_BUFFER_COLOR );
	//RenderLib::RenderState::Clear_Buffer(RenderLib::CLEAR_BUFFER_DEPTH );

	if( isMRT ){
		this->frameBuffer->bind();
		this->mp2dObj->render();

		rlib::FrameBuffer::bindScreenBuffer();
		this->mp2dObj->render( this->frameBuffer );

	}else{
		rlib::FrameBuffer::bindScreenBuffer();
		//this->mp2dObj->render();
	}

	mButton->render();

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

	if( frameBuffer ){ delete frameBuffer; frameBuffer = NULL; }
	LOGI(TAG,"delete frameBuffer");

	LOGI(TAG, "Complete rTestScene::exit");
}
