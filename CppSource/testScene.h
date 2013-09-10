#pragma once
#include "math\kmathf.h"
#include "FrameWork\Class\kFrameWork\kFrameWork.h"
#include "GraphicsLib\Class\kDevice\kDevice.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"
#include "GraphicsLib\Class\r2DObj\r2DObj.h"

#include "input\AnalogStick.h"
#include "kaneta\ICharacter\Class\kPlayer\kPlayer.h"

#include "kaneta\ICamera\Class\kPlayCamera\kPlayCamera.h"
//#include "../kShotCamera.h"

#include "GraphicsLib\Class\kPlane\kPlane.h"
#include "kaneta\ActionMediate\ActionMediate.h"

#include "thread\Class\kThread\kThread.h"
#include "Game\Bullet\Bullet.h"
#include "main.h"

#include "LoadingScene.h"

#include "input\Button.h"

#include "StandardLib\SmartPointer.h"

#include "EffectLib\Effect.h"


/*

wp<EmiiterSet> wp;

wp = sEffectManager->Create(TEST,pos,scale);

if(wp->IsExist)wp->Loop();

wp->Setting_Position(pos);

wp->End();

*/

namespace klib
{
	using namespace rlib;

			static kInputElementDesc desc[]=
			{
				{"POSITION",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
				{"COLOR",0,k_VF_R32G32B32A32_FLOAT,0,eVertex,0},
				{"NORMAL",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
				{"TEXCOORD",0,k_VF_R32G32_FLOAT,0,eVertex,0}
			};
			static u32 descnum=sizeof(desc)/sizeof(kInputElementDesc);

			static kInputElementDesc desc2[]=
			{
				{"POSITION",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
				{"TEXCOORD",0,k_VF_R32G32_FLOAT,0,eVertex,0}
			};
			static u32 descnum2=sizeof(desc2)/sizeof(kInputElementDesc);

	class testScene:public IScene,public ktl::kSingleton<testScene>
	{
		friend class ktl::kSingleton<testScene>;
	private:
		ktl::sp<ICamera> m_Camera;
		kMesh* stage;
		kPlayer* mesh;
		kGraphicsPipline* pipline;
		kGraphicsPipline* bord;
		kGraphicsPipline* addBord;
		kTechnique* tec;
		r2DObj* obj;
		CircleButton* m_Button;
		r2DObj* paper;
		r2DObj* mask;
		r2DObj* ring;
		rlib::AnalogStick* mp_Stick;
	public:
		void setCamera(ICamera* camera)
		{
			m_Camera.SetPtr(camera);
		}
		ICamera* getCamera()const{return m_Camera.GetPtr();}
		kPlayer* getPlayer()const{return mesh;}

		static void testFunc(testScene* obj)
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

			obj->obj=new r2DObj;
			obj->obj->load("kanetaPlace/stick.png");
			obj->obj->setPos(0,0);
			obj->obj->setSize(100,true);

			obj->paper=new r2DObj;
			obj->paper->load("Font/font1.png");
			obj->paper->setPos(0,0);
			obj->paper->setSize(200,true);

			obj->mask=new r2DObj;
			obj->mask->load("kanetaPlace/mask.png");
			obj->mask->setPos(0,0);
			obj->mask->setSize(200,true);

			obj->ring=new r2DObj;
			obj->ring->load("kanetaPlace/ring.png");
			obj->ring->setPos(0,0);
			obj->ring->setSize(200,true);

			obj->setCamera(new kPlayCamera(NULL,Vector3(0,0,0),Vector3(0,0,0)));

			obj->mesh=new kPlayer("kanetaPlace/fireman.IEM",obj->mp_Stick,obj->m_Button);
			obj->mesh->getObj()->setScale(0.01f);
			obj->mesh->getObj()->setPosition(0,0,0);
			obj->mesh->getObj()->setAngle(0);
			obj->mesh->getObj()->SetMotion(4);
			obj->mesh->getObj()->Update();

			obj->stage=new kMesh("testStage/Stage2.imo",new kMeshLoadIMO,new kMeshGLES20Render);
			obj->stage->setScale(1.0f);
			obj->stage->setPosition(0,0,0);
			obj->stage->Update();



			obj->pipline=new kGraphicsPipline();
			//pipline->createVertexShader("a");
			//obj->pipline->createVertexShader("kanetaPlace/shader/vertex.txt");
			//obj->pipline->createPixelShader("kanetaPlace/shader/pixel.txt");
			obj->pipline->createShader("kanetaPlace/shader/vertex.txt","kanetaPlace/shader/pixel.txt");
			obj->pipline->createBlendState(k_BLEND_ALPHA);
			obj->pipline->createDepthStencilState(true,true,eLESS_EQUAL);
			obj->pipline->createRasterizerState(eSOLID,eFRONT,false);
			obj->pipline->complete(desc,descnum);

			obj->bord=new kGraphicsPipline();
			//pipline->createVertexShader("a");
			//obj->bord->createVertexShader("kanetaPlace/shader/testbordvs.txt");
			//obj->bord->createPixelShader("kanetaPlace/shader/testbordps.txt");
			obj->bord->createShader("kanetaPlace/shader/testbordvs.txt","kanetaPlace/shader/testbordps.txt");
			obj->bord->createBlendState(k_BLEND_ALPHA);
			obj->bord->createDepthStencilState(true,true,eLESS_EQUAL);
			obj->bord->createRasterizerState(eSOLID,eNONE,false);
			obj->bord->complete(desc2,descnum2);

			obj->addBord=new kGraphicsPipline();
			//pipline->createVertexShader("a");
			//obj->addBord->createVertexShader("kanetaPlace/shader/touchbordvs.txt");
			//obj->addBord->createPixelShader("kanetaPlace/shader/touchbordps.txt");
			obj->addBord->createShader("kanetaPlace/shader/touchbordvs.txt","kanetaPlace/shader/touchbordps.txt");
			obj->addBord->createBlendState(k_BLEND_ADD);
			obj->addBord->createDepthStencilState(true,false,eLESS_EQUAL);
			obj->addBord->createRasterizerState(eSOLID,eNONE,false);
			obj->addBord->complete(desc2,descnum2);

			obj->tec=new kTechnique();
			//pipline->createVertexShader("a");
			//obj->tec->createVertexShader("kanetaPlace/shader/fire.vs");
			//obj->tec->createPixelShader("kanetaPlace/shader/fire.fs");
			obj->tec->createShader("kanetaPlace/shader/fire.vs","kanetaPlace/shader/fire.fs");
			obj->tec->bindAttribLocation(0,"VPosition");
			obj->tec->bindAttribLocation(1,"VTexCoord");
			obj->tec->createBlendState(k_BLEND_ALPHA);
			obj->tec->createDepthStencilState(false,false,eLESS_EQUAL);
			obj->tec->createRasterizerState(eSOLID,eNONE,false);
			obj->tec->complete();

			

			//rlib::BulletManager::getInst().init();


			g_VM->DetachCurrentThread();
		}
		//エントリー処理
		void entry()
		{
			//wp<EffectLib::EmitterSet> week;

			//week = EffectLib::EffectManager_Singleton::getInstance()->Create(EffectLib::TEST,Vector3(0,0,0),1.0f);

			//if(week.IsExist())week->Loop();

			//week->Setting_Position(pos);

			//week->End();
			

			//float ary[3]={0.25f,0.5f,1.0f};
			//pipline->setShaderValue("val",0.8f);
			//pipline->setShaderValue("array",ary,3);
			thread::kThreadHolder* func=new thread::kThreadHolder(testFunc,this);
			LoadingScene::_getInstance().set(func);
			framework.scenePush(LoadingScene::_getInstancePtr());
		}
		//更新処理
		void update()
		{
			static float a=K_PI/4.0f;
			mp_Stick->update();
			if( this->mp_Stick->enable() )
			{
				//a += mp_Stick->getX()*0.01f;
			}

			if( mlInput::isPinch() )
			{
				//a -= mlInput::getPinchMoveLength() * 0.01f;
			}
			//a+=.001f;

			math::kclampf(K_PI/8.0f,K_PI/1.5f,&a);
			
			tec->setShaderValue("alpha",a);
			//mesh->getObj()->setAngle(a);
			m_Camera->update();
			mesh->update();
			m_Button->update();
			m_Camera->setFov(a);
			ActionMediate::update(mesh);
			EffectLib::EffectManager_Singleton::getInstance()->Update();
			//rlib::BulletManager::getInst().update();
		}
		//描画処理
		void render()
		{
			//rlib::BulletManager::getInst().render(pipline);
			mesh->render(pipline);
			stage->Render(pipline);
			//bord->setTexture("maskTex",1,mask);
			//static float a=0;
			//a+=0.01f;
			bord->setShaderValue("alpha",0.0f);
			addBord->setShaderValue("alpha",1.0f);
			kPlane::render(bord,mask,1.0f,1.0f,m_Camera->getPos(),math::Vector3(0,1,0),math::Vector3(0,4,4),0,0,0,0);
			kPlane::render(addBord,ring,0.0f,1,1,math::Vector3(0,1,0),0,0,0,0);
			kPlane::render(addBord,ring,0.0f,1,1,math::Vector3(0,4,4),0,0,0,0);
			ActionMediate::render();
			//tec->setTexture("maskTex",1,mask);
			//paper->render(tec);
			mp_Stick->render();
			m_Button->render();
			EffectLib::EffectManager_Singleton::getInstance()->Render();
			//DEBUG_MSG("mesh pos=(%2f,%2f,%2f)", mesh->getObj()->getPositionX(),mesh->getObj()->getPositionY(),mesh->getObj()->getPositionZ());
			//DEBUG_MSG("mesh angle=(%2f,%2f,%2f)", mesh->getObj()->getAngleX(),mesh->getObj()->getAngleY(),mesh->getObj()->getAngleZ());
		}
		//終了処理
		void exit()
		{
			dprintf("testScene exit");
			delete bord;
			delete stage;
			delete mesh;
			delete pipline;
			delete obj;
			delete mp_Stick;

			kPlane::release();
			//rlib::BulletManager::getInst().clear();
		}
	};
}