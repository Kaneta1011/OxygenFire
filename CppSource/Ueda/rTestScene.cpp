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

#include "EffectLib/Effect.h"
#include "Game\SaveManager.h"
#include "kaneta\ActionMediate\ActionMediate.h"
#include "GraphicsLib\Class\kPlane\kPlane.h"
#include "kaneta\Scene\GameScene\GameScene.h"

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
	mPlayer = NULL;
	m_Camera = NULL;
}

using namespace EffectLib;

wp<EmitterSet> wpE;
wp<EmitterSet> wpE2;

#define USE_PLAYER

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
	sPlacementManager->Load("Placement/stage1.mqo");

//プレイヤーの設定
#ifdef USE_PLAYER
	GameScene::_getInstance().setCamera( new kPlayCamera(obj->mPlayer,Vector3(0,0,0),Vector3(0,0,0)) );
	obj->mPlayer=new kPlayer("kanetaPlace/fireman.IEM",obj->mStick,obj->mButton);
	obj->mPlayer->getObj()->setScale(0.01f);
	obj->mPlayer->getObj()->setPosition(0,0,-5);
	obj->mPlayer->getObj()->setAngle(0);
	obj->mPlayer->getObj()->SetMotion(4);
	obj->mPlayer->getObj()->Update();

#endif
	klib::kPlane::init();
	klib::ActionMediate::init();

	rlib::BulletManager::getInst().init();
	STAGE.init("stageInfo.txt");

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
#ifdef USE_PLAYER
	math::kclampf(K_PI/8.0f,K_PI/1.5f,&a);
	GameScene::_getInstance().getCamera()->setFov(a);
	GameScene::_getInstance().getCamera()->update();
	mPlayer->update();
#else
	if( this->mStick->enable() )
	{
		static Vector3 cpos(0,2,10);
		cpos.x += this->mStick->getX() * 0.1f;
		cpos.z += this->mStick->getY() * 0.1f;

		RenderState::Setting_ViewMatrix(cpos, Vector3(0,0,0), Vector3(0,1,0));
		//a += mp_Stick->getX()*0.01f;
	}
#endif

	if( mlInput::getNowTouchCount() == 3 )
	{
		BULLET_MNG.clearData();
	}
	STAGE.update();
	BULLET_MNG.update();
	GIMMICK_MNG.update();
	//BULLET_MNG.collision( GIMMICK_MNG );

#ifdef USE_PLAYER
	{//プレイヤーのギミックとの当たり判定
		Vector3 playerPos = mPlayer->getObj()->getPosition();
		playerPos += GIMMICK_MNG.calWindPower(playerPos, 0.25f);
		playerPos = GIMMICK_MNG.collision(playerPos, 0.25f);

		mPlayer->getObj()->setPosition(playerPos);
		mPlayer->getObj()->Update();
	}
	klib::ActionMediate::update(this->mPlayer);
#endif

	sEffectManager->Update();

	if( STAGE.isClear() ){
		DEBUG_MSG_NON_ARAG("Stage Clear!!");
	}
	DEBUG_MSG("fire count = %d", rlib::BulletManager::getInst().size() );
}

void rTestScene::render()
{
	rlib::FrameBuffer::bindScreenBuffer();

#ifdef USE_PLAYER
	mPlayer->render(GameCommonPipeline::getPipeline());
#endif
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
	STAGE.clear();

	if( this->mPlayer){ delete this->mPlayer; }
	if( this->m_Camera ){ delete this->m_Camera; }

	GameCommonPipeline::clear();
	sPlacementManager->Delete();

	LOGI(TAG, "Complete rTestScene::exit");
}
