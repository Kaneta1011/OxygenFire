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

#include "GraphicsLib\Class\kPlane\kPlane.h"
#include "kaneta\ActionMediate\ActionMediate.h"

#include "thread\Class\kThread\kThread.h"

namespace klib
{
	using namespace rlib;
	kInputElementDesc desc[]=
	{
		{"POSITION",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
		{"COLOR",0,k_VF_R32G32B32A32_FLOAT,0,eVertex,0},
		{"NORMAL",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
		{"TEXCOORD",0,k_VF_R32G32_FLOAT,0,eVertex,0}
	};
	u32 descnum=sizeof(desc)/sizeof(kInputElementDesc);

	kInputElementDesc desc2[]=
	{
		{"POSITION",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
		{"TEXCOORD",0,k_VF_R32G32_FLOAT,0,eVertex,0}
	};
	u32 descnum2=sizeof(desc2)/sizeof(kInputElementDesc);
	static r2DObj* oobj;

	void func()
	{
		JNIEnv* env;
		getJNIEnv(&env);

		g_VM->AttachCurrentThread((JNIEnv**)&env, NULL);

		dprintf("aaaaaaa");
		oobj=new r2DObj;
		oobj->load("kanetaPlace/stick.png");
		oobj->setPos(0,0);
		oobj->setSize(100,true);

		g_VM->DetachCurrentThread();
	}

	class testScene:public IScene,public ktl::kSingleton<testScene>
	{
		friend class ktl::kSingleton<testScene>;
	private:
		ICamera* m_Camera;
		kMesh* stage;
		kPlayer* mesh;
		kGraphicsPipline* pipline;
		kGraphicsPipline* bord;
		kGraphicsPipline* addBord;
		kTechnique* tec;
		r2DObj* obj;
		r2DObj* paper;
		r2DObj* mask;
		r2DObj* ring;
		rlib::AnalogStick* mp_Stick;
	public:
		//エントリー処理
		void entry()
		{
			thread::kThreadPool::create(16);
			{
				//thread::kThread mt(new thread::kThreadHolder(func));
			}
			
			kPlane::init();
			mp_Stick = new rlib::AnalogStick();
			mp_Stick->init(-60, -50, 40);
			mp_Stick->loadImage("kanetaPlace/stickFrameAlpha.png","kanetaPlace/stick.png");

			obj=new r2DObj;
			obj->load("kanetaPlace/stick.png");
			obj->setPos(0,0);
			obj->setSize(100,true);

			paper=new r2DObj;
			paper->load("Font/font1.png");
			paper->setPos(0,0);
			paper->setSize(200,true);

			mask=new r2DObj;
			mask->load("kanetaPlace/mask.png");
			mask->setPos(0,0);
			mask->setSize(200,true);

			ring=new r2DObj;
			ring->load("kanetaPlace/ring.png");
			ring->setPos(0,0);
			ring->setSize(200,true);

			pipline=new kGraphicsPipline();
			//pipline->createVertexShader("a");
			pipline->createVertexShader("kanetaPlace/shader/vertex.txt");
			pipline->createPixelShader("kanetaPlace/shader/pixel.txt");
			pipline->createBlendState(k_BLEND_ALPHA);
			pipline->createDepthStencilState(true,true,eLESS_EQUAL);
			pipline->createRasterizerState(eSOLID,eFRONT,false);
			pipline->complete(desc,descnum);

			bord=new kGraphicsPipline();
			//pipline->createVertexShader("a");
			bord->createVertexShader("kanetaPlace/shader/testbordvs.txt");
			bord->createPixelShader("kanetaPlace/shader/testbordps.txt");
			bord->createBlendState(k_BLEND_ALPHA);
			bord->createDepthStencilState(true,true,eLESS_EQUAL);
			bord->createRasterizerState(eSOLID,eNONE,false);
			bord->complete(desc2,descnum2);

			addBord=new kGraphicsPipline();
			//pipline->createVertexShader("a");
			addBord->createVertexShader("kanetaPlace/shader/touchbordvs.txt");
			addBord->createPixelShader("kanetaPlace/shader/touchbordps.txt");
			addBord->createBlendState(k_BLEND_ADD);
			addBord->createDepthStencilState(true,false,eLESS_EQUAL);
			addBord->createRasterizerState(eSOLID,eNONE,false);
			addBord->complete(desc2,descnum2);

			tec=new kTechnique();
			//pipline->createVertexShader("a");
			tec->createVertexShader("kanetaPlace/shader/fire.vs");
			tec->createPixelShader("kanetaPlace/shader/fire.fs");
			tec->bindAttribLocation(0,"VPosition");
			tec->bindAttribLocation(1,"VTexCoord");
			tec->createBlendState(k_BLEND_ALPHA);
			tec->createDepthStencilState(false,false,eLESS_EQUAL);
			tec->createRasterizerState(eSOLID,eNONE,false);
			tec->complete();

			float ary[3]={0.25f,0.5f,1.0f};
			pipline->setShaderValue("val",0.8f);
			pipline->setShaderValue("array",ary,3);

			mesh=new kPlayer("kanetaPlace/kman.IEM",mp_Stick);
			mesh->getObj()->setScale(0.01f);
			mesh->getObj()->setPosition(0,0,0);
			mesh->getObj()->setAngle(0);
			mesh->getObj()->SetMotion(4);
			mesh->getObj()->Update();

			stage=new kMesh("testStage/Stage2.imo",new kMeshLoadIMO,new kMeshGLES20Render);
			stage->setScale(0.01f);
			stage->setPosition(0,0,0);
			mesh->update();

			m_Camera=new kPlayCamera(mesh);

			ActionMediate::init(mesh);
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
			m_Camera->setFov(a);
			tec->setShaderValue("alpha",a);
			//mesh->getObj()->setAngle(a);
			m_Camera->update();
			mesh->update();
			ActionMediate::update();

		}
		//描画処理
		void render()
		{
			mesh->render(pipline);
			stage->Render(pipline);
			//bord->setTexture("maskTex",1,mask);
			//static float a=0;
			//a+=0.01f;
			//bord->setShaderValue("alpha",(1+cosf(a))/2.0f);
			//kPlane::render(addBord,ring,0.0f,math::Vector3(0,0,0),math::Vector3(0,0,1),0,0,0,0);
			ActionMediate::render();
			tec->setTexture("maskTex",1,mask);
			//paper->render(tec);
			mp_Stick->render();
			//obj->render();
			DEBUG_MSG("mesh pos=(%2f,%2f,%2f)", mesh->getObj()->getPositionX(),mesh->getObj()->getPositionY(),mesh->getObj()->getPositionZ());
			DEBUG_MSG("mesh angle=(%2f,%2f,%2f)", mesh->getObj()->getAngleX(),mesh->getObj()->getAngleY(),mesh->getObj()->getAngleZ());
		}
		//終了処理
		void exit()
		{
			delete bord;
			delete stage;
			delete mesh;
			delete pipline;
			delete obj;
			kPlane::release();
			thread::kThreadPool::destroy();
		}
	};
}