#include "rTestScene.h"
#include "GraphicsLib\Class\tRenderState\RenderState.h"

#include "utility\debugMessageMng.h"//デバッグ用の文字列表示のためのヘッダー
#include "utility\utility.h"

#include "GraphicsLib\Class\rFrameBufer\rFrameBuffer.h"
#include "EffectLib\Effect.h"
#include "Game\Gimmick\Gimmick.h"
#include "Game\Bullet\Bullet.h"

#include "input\Button.h"
#include "input\AnalogStick.h"

#include "PlacementLib\Placement.h"

#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"

using namespace RenderLib;
using namespace klib::math;
using namespace PlacementLib;

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

klib::kGraphicsPipline* pipeline = NULL;

rTestScene::rTestScene()
{
	frameBuffer = NULL;
	mButton = NULL;
	mStick = NULL;
	mpStage = NULL;
}

void rTestScene::entry()
{
	LOGI(TAG,"Execute rTestScene init");

	mButton = new rlib::CircleButton();
	mButton->init("testImage.png", 50, -50, 50.f);

	mStick = new rlib::AnalogStick();
	this->mStick->init(-80, -50, 50);

	//this->mpStage = new klib::kMesh("Placement/stobj1.mqo", new klib::kMeshLoadIMO(), new klib::kMeshGLES20Render() );
	sPlacementManager->Load("Placement/stage1.mqo");

	pipeline = new klib::kGraphicsPipline();
	pipeline->createVertexShader("vertex.txt");
	pipeline->createPixelShader("pixel.txt");
	pipeline->createBlendState(k_BLEND_NONE);
	pipeline->createDepthStencilState(true,eLESS_EQUAL);
	pipeline->createRasterizerState(eSOLID,eFRONT,false);

	kInputElementDesc desc[]=
	{
		{"POSITION",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
		{"COLOR",0,k_VF_R32G32B32A32_FLOAT,0,eVertex,0},
		{"NORMAL",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
		{"TEXCOORD",0,k_VF_R32G32_FLOAT,0,eVertex,0}
	};
	u32 descnum=sizeof(desc)/sizeof(kInputElementDesc);
	pipeline->complete(desc,descnum);

	rlib::BulletManager::getInst().init();
	GIMMICK_MNG.init();

	//rlib::GimmickInfo info("kibako128.IMO");
	//info.pos.x = info.pos.y = info.pos.z = 0.f;
	//info.size.x = info.size.y = info.size.z = 1.f;
	//GIMMICK_MNG.add(info);

	LOGI(TAG,"Complete rTestScene init");
}

#include "input\Input.h"

static int isMRT = 0;

using namespace EffectLib;

float getRandomNumberFloat( float Min, float Max )
{
	if( Min < 0 )
		Max += -Min;
	else
		Max -= Min;

	return ( float )rand() / ( float )RAND_MAX * Max + Min;
}

void rTestScene::update()
{
	DEBUG_MSG_NON_ARAG("rTestScene");

	mStick->update();
	mButton->update();

	static klib::math::Vector3 cpos(0,0,-50);
	if( this->mStick->enable() )
	{
		klib::math::Vector3 front = -cpos;
		klib::math::Vector3 side;
		front.cross(&side, klib::math::Vector3(0,1,0));
		front.normalize();
		side.normalize();

		klib::math::Vector3 move = side*this->mStick->getX();
		move.normalize();
		move *= 0.5f;
		cpos += move;
		cpos.y += this->mStick->getY();
		RenderLib::RenderState::Setting_ViewMatrix( cpos, klib::math::Vector3(0,0,0), klib::math::Vector3(0,1,0));
	}
	if( mlInput::isPinch() )
	{
		float move = mlInput::getPinchMoveLength();
		cpos.z += move;
		RenderLib::RenderState::Setting_ViewMatrix( cpos, klib::math::Vector3(0,0,0), klib::math::Vector3(0,1,0));
	}

	rlib::BulletManager& bullet = rlib::BulletManager::getInst();
	if( this->mButton->getMode() == rlib::IButton::eUP )
	{
		rlib::BulletInfo info;
		info.pos = cpos;

		info.velocity = -cpos;
		info.velocity.normalize();
		//info.velocity *= 0.1f;
		bullet.add(info);
	}

	if( mlInput::getNowTouchCount() == 3 )
	{
		bullet.clearData();
	}
	bullet.update();

	GIMMICK_MNG.update();
	bullet.collision( GIMMICK_MNG );

	sEffectManager->Update();

	DEBUG_MSG("fire count = %d", rlib::BulletManager::getInst().size() );
	DEBUG_MSG("camera pos( x=%.2f, y=%.2f, z=%.2f)", cpos.x, cpos.y, cpos.z );

	static int t = 0;
	t++;
	if( 180 < t )
	{
		sEffectManager->Create(FIRE_CHARGE,
		Vector3(
			0,
			5,
			0),
			5.0f);
		t=0;
	}
}

void rTestScene::render()
{
	rlib::FrameBuffer::bindScreenBuffer();

	//mpStage->Render(pipeline);
	rlib::BulletManager::getInst().render();
	GIMMICK_MNG.render();

	sEffectManager->Render();

	mButton->render();
	mStick->render();

}

//2D速度テストコード
	//rlib::FrameBuffer::bindScreenBuffer();
	//for( int i=0; i<50; i++ )
	//{
	//	this->mp2dObj->setPos( rand()/(float)RAND_MAX * 200.f - 100.f, rand()/(float)RAND_MAX * 200.f - 100.f );
	//	this->mp2dObj->render();
	//}

void rTestScene::exit()
{
	LOGI(TAG, "Execute rTestScene::exit");

	if( pipeline ) { delete pipeline; pipeline = NULL; }

	if( mButton ){ delete mButton; mButton = NULL; }
	LOGI(TAG,"delete Button");
	if( mStick ){ delete mStick; mStick = NULL; }
	LOGI(TAG,"delete mStick");

	rlib::BulletManager::getInst().clear();
	LOGI(TAG,"clear bullet Manager");
	GIMMICK_MNG.clear();
	LOGI(TAG,"clear gimmick Manager");

	sPlacementManager->Delete();

	LOGI(TAG, "Complete rTestScene::exit");
}
