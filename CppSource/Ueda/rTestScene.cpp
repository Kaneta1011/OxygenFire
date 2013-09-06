#include "rTestScene.h"
#include "GraphicsLib\Class\tRenderState\RenderState.h"

#include "utility\debugMessageMng.h"//デバッグ用の文字列表示のためのヘッダー
#include "utility\utility.h"

#include "GraphicsLib\Class\rFrameBufer\rFrameBuffer.h"
#include "EffectLib\Effect.h"
#include "Game\Gimmick\Gimmick.h"
#include "Game\Bullet\Bullet.h"
#include "Game\Stage\Stage.h"
#include "Game\CommonPipeline\GameCommonPipeline.h"

#include "input\Button.h"
#include "input\AnalogStick.h"

#include "PlacementLib\Placement.h"

#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"

#include "kaneta\ICharacter\Class\kPlayer\kPlayer.h"
#include "kaneta\ICamera\Class\kPlayCamera\kPlayCamera.h"

#include "EffectLib\/Effect.h"

using namespace RenderLib;
using namespace klib::math;
using namespace PlacementLib;

const static int	SPRITE_MAX = 8000;

static const char* TAG = "rTestScene";

#define SIZE 512.f

using namespace rlib;
using namespace klib;

rTestScene::rTestScene()
{
	frameBuffer = NULL;
	mButton = NULL;
	mStick = NULL;
}

#include "Game\SaveManager.h"
#include "kaneta\ActionMediate\ActionMediate.h"
#include "GraphicsLib\Class\kPlane\kPlane.h"

using namespace EffectLib;

wp<EmitterSet> wpE;
wp<EmitterSet> wpE2;

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

	GameCommonPipeline::init();
	STAGE.init("Placement/stage1.IMO");
	sPlacementManager->Load("Placement/stage1.mqo");


//プレイヤーの設定
	mMesh=new kPlayer("kanetaPlace/kman.IEM",mStick);
	mMesh->getObj()->setScale(0.01f);
	mMesh->getObj()->setPosition(0,0,-5);
	mMesh->getObj()->setAngle(0);
	mMesh->getObj()->SetMotion(4);
	mMesh->getObj()->Update();

	m_Camera=new kPlayCamera(mMesh);

	klib::kPlane::init();
	klib::ActionMediate::init(this->mMesh);

	rlib::BulletManager::getInst().init();
	GIMMICK_MNG.init("gimmick/giTest.gi");

	LOGI(TAG,"Complete rTestScene init");


	//===============　田代デバッグ用  =======================================
	//wpE = sEffectManager->Create(FIRE_BALL,Vector3(0,0,0));
	//wpE->Setting_Velocity(Vector3(0,1,0));
	//wpE->Loop();


	//wpE2 = sEffectManager->Create(FIRE_CHARGE,Vector3(0,0,-5),0.1f);
	//wpE2->Setting_Velocity(Vector3(0,1,0));
	//wpE2->Loop();
	//===============　田代デバッグ用  =======================================
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
	STAGE.update();
	BULLET_MNG.update();
	GIMMICK_MNG.update();
	BULLET_MNG.collision( GIMMICK_MNG );

	{//プレイヤーのギミックとの当たり判定
		Vector3 playerPos = mMesh->getObj()->getPosition();
		playerPos += GIMMICK_MNG.calWindPower(playerPos, 0.25f);
		playerPos = GIMMICK_MNG.collision(playerPos, 0.25f);

		mMesh->getObj()->setPosition(playerPos);
		mMesh->getObj()->Update();
	}

	klib::ActionMediate::update();

	sEffectManager->Update();

	DEBUG_MSG("fire count = %d", rlib::BulletManager::getInst().size() );
}

void rTestScene::render()
{
	rlib::FrameBuffer::bindScreenBuffer();

	mMesh->render(GameCommonPipeline::getPipeline());
	STAGE.render();
	BULLET_MNG.render();
	GIMMICK_MNG.render();

	klib::ActionMediate::render();

	mButton->render();
	mStick->render();
	sEffectManager->Render();
}

void rTestScene::exit()
{
	LOGI(TAG, "Execute rTestScene::exit");

	if( mButton ){ delete mButton; mButton = NULL; }
	LOGI(TAG,"delete Button");
	if( mStick ){ delete mStick; mStick = NULL; }
	LOGI(TAG,"delete mStick");

	rlib::BulletManager::getInst().clear();
	GIMMICK_MNG.clear();
	STAGE.clear();

	GameCommonPipeline::clear();
	sPlacementManager->Delete();

	LOGI(TAG, "Complete rTestScene::exit");
}
