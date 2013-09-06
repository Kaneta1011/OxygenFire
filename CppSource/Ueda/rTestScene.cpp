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

#include "kaneta\ICharacter\Class\kPlayer\kPlayer.h"
#include "kaneta\ICamera\Class\kPlayCamera\kPlayCamera.h"

using namespace RenderLib;
using namespace klib::math;
using namespace PlacementLib;

const static int	SPRITE_MAX = 8000;

static const char* TAG = "rTestScene";

#define SIZE 512.f

using namespace klib;

klib::kGraphicsPipline* rTestScene::pipeline = NULL;

rTestScene::rTestScene()
{
	frameBuffer = NULL;
	mButton = NULL;
	mStick = NULL;
	mpStage = NULL;
}

#include "Game\SaveManager.h"
#include "kaneta\ActionMediate\ActionMediate.h"
#include "GraphicsLib\Class\kPlane\kPlane.h"

void rTestScene::entry()
{
	rlib::SaveManager::save();
	rlib::SaveManager::load();

	LOGI(TAG,"Execute rTestScene init");

	mButton = new rlib::CircleButton();
	mButton->init("testImage.png", 50, -50, 50.f);

	mStick = new rlib::AnalogStick();
	this->mStick->init(-80, -50, 50);
	this->mStick->loadImage("cursor.png","testImage.png");

	this->mpStage = new klib::kMesh("Placement/stage1.IMO", new klib::kMeshLoadIMO(), new klib::kMeshGLES20Render() );
	sPlacementManager->Load("Placement/stage1.mqo");


//プレイヤーの設定
	mMesh=new kPlayer("kanetaPlace/kman.IEM",mStick);
	mMesh->getObj()->setScale(0.01f);
	mMesh->getObj()->setPosition(0,0,-5);
	mMesh->getObj()->setAngle(0);
	mMesh->getObj()->SetMotion(4);
	mMesh->getObj()->Update();

	m_Camera=new kPlayCamera(mMesh);

//グラフィックパイプラインの初期化

	pipeline = new klib::kGraphicsPipline();
	pipeline->createVertexShader("kanetaPlace/shader/vertex.txt");
	pipeline->createPixelShader("kanetaPlace/shader/pixel.txt");
	pipeline->createBlendState(k_BLEND_NONE);
	pipeline->createDepthStencilState(true,true,eLESS_EQUAL);
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

	klib::kPlane::init();
	klib::ActionMediate::init(this->mMesh);

	rlib::BulletManager::getInst().init();
	GIMMICK_MNG.init("gimmick/giTest.gi");

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

	mButton->update();

//プレイヤーの移動
	static float a=K_PI/4.0f;
	mStick->update();
	if( this->mStick->enable() )
	{
		//a += mp_Stick->getX()*0.01f;
	}	
	if( mlInput::isPinch() )
	{
		//a -= mlInput::getPinchMoveLength() * 0.01f;
	}
	//a+=.001f;

	math::kclampf(K_PI/8.0f,K_PI/1.5f,&a);
	m_Camera->setFov(a);
	m_Camera->update();
	mMesh->update();

	rlib::BulletManager& bullet = rlib::BulletManager::getInst();
	if( this->mButton->getMode() == rlib::IButton::eUP )
	{
		rlib::BulletInfo info;
		info.pos = mMesh->getObj()->getPosition();

		info.velocity = -info.pos;
		info.velocity.normalize();
		//info.velocity *= 0.1f;
		BULLET_MNG.add(info);
	}

	if( mlInput::getNowTouchCount() == 3 )
	{
		BULLET_MNG.clearData();
	}
	BULLET_MNG.update();

	GIMMICK_MNG.update();
	BULLET_MNG.collision( GIMMICK_MNG );

	Vector3 playerPos = mMesh->getObj()->getPosition();
	playerPos += GIMMICK_MNG.calWindPower(playerPos, 0.25f);
	playerPos = GIMMICK_MNG.collision(playerPos, 0.25f);

	mMesh->getObj()->setPosition(playerPos);
	mMesh->getObj()->Update();

	klib::ActionMediate::update();

	sEffectManager->Update();
	//DEBUG_MSG("mesh pos=(%2f,%2f,%2f)", mMesh->getObj()->getPositionX(),mMesh->getObj()->getPositionY(),mMesh->getObj()->getPositionZ());

	DEBUG_MSG("fire count = %d", rlib::BulletManager::getInst().size() );

}

void rTestScene::render()
{
	rlib::FrameBuffer::bindScreenBuffer();

	mMesh->render(pipeline);
	this->mpStage->Render(pipeline);
	BULLET_MNG.render();
	GIMMICK_MNG.render();

	klib::ActionMediate::render();

	mButton->render();
	mStick->render();
	sEffectManager->Render();

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
