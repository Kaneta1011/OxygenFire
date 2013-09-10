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

void rTestScene::threadFunc(rTestScene* obj)
{
	JNIEnv* env;
	getJNIEnv(&env);
	g_VM->AttachCurrentThread((JNIEnv**)&env, NULL);

	rlib::SaveManager::save();
	rlib::SaveManager::load();

	LOGI(TAG,"Execute rTestScene init");

	obj->mButton = new rlib::CircleButton();
	obj->mButton->init("testImage.png", 50, -50, 50.f);

	obj->mStick = new rlib::AnalogStick();
	obj->mStick->init(-80, -50, 50);
	obj->mStick->loadImage("cursor.png","testImage.png");

	GameCommonPipeline::init();
	STAGE.init("Placement/stage1.IMO");
	sPlacementManager->Load("Placement/stage1.mqo");

//プレイヤーの設定
	obj->mMesh=new kPlayer("kanetaPlace/kman.IEM",obj->mStick,obj->mButton);
	obj->mMesh->getObj()->setScale(0.01f);
	obj->mMesh->getObj()->setPosition(0,0,-5);
	obj->mMesh->getObj()->setAngle(0);
	obj->mMesh->getObj()->SetMotion(4);
	obj->mMesh->getObj()->Update();

	obj->m_Camera=new kPlayCamera(obj->mMesh);

	klib::kPlane::init();
	klib::ActionMediate::init();

	rlib::BulletManager::getInst().init();
	GIMMICK_MNG.init("gimmick/stage1.gi");

	LOGI(TAG,"Complete rTestScene init");

	g_VM->DetachCurrentThread();
}

void rTestScene::entry()
{
	thread::kThreadHolder* func=new thread::kThreadHolder(threadFunc,this);
	LoadingScene::_getInstance().set(func);
	framework.scenePush(LoadingScene::_getInstancePtr());

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

	if( mlInput::getNowTouchCount() == 3 )
	{
		BULLET_MNG.clearData();
	}
	STAGE.update();
	BULLET_MNG.update();
	//GIMMICK_MNG.update();
	//BULLET_MNG.collision( GIMMICK_MNG );

	//{//プレイヤーのギミックとの当たり判定
	//	Vector3 playerPos = mMesh->getObj()->getPosition();
	//	playerPos += GIMMICK_MNG.calWindPower(playerPos, 0.25f);
	//	playerPos = GIMMICK_MNG.collision(playerPos, 0.25f);

	//	mMesh->getObj()->setPosition(playerPos);
	//	mMesh->getObj()->Update();
	//}

	klib::ActionMediate::update(mMesh);

	sEffectManager->Update();

	if( STAGE.isClear() ){
		DEBUG_MSG_NON_ARAG("Stage Clear!!");
	}
	DEBUG_MSG("fire count = %d", rlib::BulletManager::getInst().size() );
}

void rTestScene::render()
{
	rlib::FrameBuffer::bindScreenBuffer();

	mMesh->render(GameCommonPipeline::getPipeline());
	STAGE.render();
	BULLET_MNG.render();
	//GIMMICK_MNG.render();

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

	if( this->mMesh){ delete this->mMesh; }
	if( this->m_Camera ){ delete this->m_Camera; }

	GameCommonPipeline::clear();
	sPlacementManager->Delete();

	LOGI(TAG, "Complete rTestScene::exit");
}
