#include "math\kmathf.h"
#include "GameScene.h"
#include "GraphicsLib\Class\kDevice\kDevice.h"
#include "FrameWork\Class\kFrameWork\kFrameWork.h"
#include "kaneta\ICharacter\Class\kPlayer\kPlayer.h"
#include "kaneta\ICamera\Class\kPlayCamera\kPlayCamera.h"
#include "GraphicsLib\Class\kPlane\kPlane.h"
#include "kaneta\ActionMediate\ActionMediate.h"
#include "thread\Class\kThread\kThread.h"
#include "Game\Bullet\Bullet.h"
#include "main.h"
#include "kaneta/Scene/LoadingScene/LoadingScene.h"
#include "input\Button.h"
#include "EffectLib\Effect.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "input\AnalogStick.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"
#include "GraphicsLib\Class\r2DObj\r2DObj.h"

//ステージ系
#include "utility\debugMessageMng.h"
#include "Game\Stage\Stage.h"
#include "Game\Gimmick\Gimmick.h"
#include "Game\CommonPipeline\GameCommonPipeline.h"

namespace klib
{
	using namespace rlib;
		void GameScene::setCamera(ICamera* camera)
		{
			m_Camera.SetPtr(camera);
		}
		ICamera* GameScene::getCamera()const{return m_Camera.GetPtr();}
		kPlayer* GameScene::getPlayer()const{return mesh;}

		void GameScene::sceneLoad(GameScene* obj)
		{
			JNIEnv* env;
			getJNIEnv(&env);

			g_VM->AttachCurrentThread((JNIEnv**)&env, NULL);

			kPlane::init();

			obj->m_Button = new rlib::CircleButton();
			obj->m_Button->init("kanetaPlace/stick.png", 50, -50, 50.f);

			obj->mp_Stick = new rlib::AnalogStick();
			obj->mp_Stick->init(-60, -50, 40);
			obj->mp_Stick->loadImage("kanetaPlace/stickFrameAlpha.png","kanetaPlace/stick.png");

			obj->mask=new r2DObj;
			obj->mask->load("kanetaPlace/mask.png");
			obj->mask->setPos(0,0);
			obj->mask->setSize(200,true);

			obj->ring=new r2DObj;
			obj->ring->load("kanetaPlace/ring.png");
			obj->ring->setPos(0,0);
			obj->ring->setSize(200,true);

			obj->setCamera(new kPlayCamera(NULL,Vector3(0,0,0),Vector3(0,0,0)));

		//ステージ初期化
			GameCommonPipeline::init();
			STAGE.init("stageInfo.txt");
			BULLET_MNG.init();

			obj->mesh=new kPlayer("kanetaPlace/fireman.IEM",obj->mp_Stick,obj->m_Button);
			obj->mesh->getObj()->setScale(0.01f);
			obj->mesh->getObj()->setPosition(STAGE.getPlayerStartPos());
			obj->mesh->getObj()->setAngle(0);
			obj->mesh->getObj()->SetMotion(4);
			obj->mesh->getObj()->Update();

			obj->pipline=new kGraphicsPipline();
			obj->pipline->createShader("kanetaPlace/shader/vertex.txt","kanetaPlace/shader/pixel.txt");
			obj->pipline->createBlendState(k_BLEND_ALPHA);
			obj->pipline->createDepthStencilState(true,true,eLESS_EQUAL);
			obj->pipline->createRasterizerState(eSOLID,eFRONT,false);
			obj->pipline->complete(desc,descnum);

			obj->bord=new kGraphicsPipline();
			obj->bord->createShader("kanetaPlace/shader/testbordvs.txt","kanetaPlace/shader/testbordps.txt");
			obj->bord->createBlendState(k_BLEND_ALPHA);
			obj->bord->createDepthStencilState(true,true,eLESS_EQUAL);
			obj->bord->createRasterizerState(eSOLID,eNONE,false);
			obj->bord->complete(desc2,descnum2);

			obj->addBord=new kGraphicsPipline();
			obj->addBord->createShader("kanetaPlace/shader/touchbordvs.txt","kanetaPlace/shader/touchbordps.txt");
			obj->addBord->createBlendState(k_BLEND_ADD);
			obj->addBord->createDepthStencilState(true,false,eLESS_EQUAL);
			obj->addBord->createRasterizerState(eSOLID,eNONE,false);
			obj->addBord->complete(desc2,descnum2);

			g_VM->DetachCurrentThread();
		}
		//エントリー処理
		void GameScene::entry()
		{
			thread::kThreadHolder* func=new thread::kThreadHolder(sceneLoad,this);
			LoadingScene::_getInstance().set(func);
			framework.scenePush(LoadingScene::_getInstancePtr());
		}
		//更新処理
		void GameScene::update()
		{
			static float a=K_PI/4.0f;
			mp_Stick->update();

			m_Camera->update();
			mesh->update();
			m_Button->update();
			m_Camera->setFov(a);
			ActionMediate::update(mesh);

		//ステージと弾とギミックの更新
			STAGE.update();
			BULLET_MNG.update();
			GIMMICK_MNG.update();
			BULLET_MNG.collision(GIMMICK_MNG);
			{//プレイヤーのギミックとの当たり判定＋風の影響
				Vector3 playerPos = mesh->getObj()->getPosition();
				playerPos += GIMMICK_MNG.calWindPower(playerPos, 0.25f);
				playerPos = GIMMICK_MNG.collision(playerPos, 0.25f);

				mesh->getObj()->setPosition(playerPos);
				mesh->getObj()->Update();
			}

			EffectLib::EffectManager_Singleton::getInstance()->Update();

			//クリアー判定
			if( STAGE.isClear() ){
				DEBUG_MSG_NON_ARAG("Stage Clear!!");
			}
		}
		//描画処理
		void GameScene::render()
		{
			mesh->render(pipline);
			STAGE.render();
			BULLET_MNG.render();
			GIMMICK_MNG.render();

			Vector3 out,vec(0,-1,0);
			float dist=10.0f;
			STAGE.rayPickUD(&out, mesh->getObj()->getPosition()+(Vector3(0,10,0)),&vec,&dist);

			kPlane::render(bord,mask,0.5f,0.5f,m_Camera->getPos(),mesh->getObj()->getPosition()+(Vector3(0,10,0)),mesh->getObj()->getPosition()+(Vector3(0,10,0))+Vector3(0,-10,0),0,0,0,0);
			addBord->setShaderValue("alpha",1.0f);
			kPlane::render(addBord,ring,0.0f,3,3,out,0,0,0,0);

			kPlane::render(bord,mask,1.0f,1.0f,m_Camera->getPos(),math::Vector3(0,1,0),math::Vector3(0,4,4),0,0,0,0);
			addBord->setShaderValue("alpha",1.0f);
			kPlane::render(addBord,ring,0.0f,1,1,math::Vector3(0,1,0),0,0,0,0);
			kPlane::render(addBord,ring,0.0f,1,1,math::Vector3(0,4,4),0,0,0,0);
			ActionMediate::render();

			mp_Stick->render();
			m_Button->render();
			EffectLib::EffectManager_Singleton::getInstance()->Render();

		}
		//終了処理
		void GameScene::exit()
		{
			dprintf("testScene exit");
			delete bord;
			delete mesh;
			delete pipline;
			delete mp_Stick;

			kPlane::release();
			STAGE.clear();
			BULLET_MNG.clear();
			GameCommonPipeline::clear();
		}
}