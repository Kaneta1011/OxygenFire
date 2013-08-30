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
#include "../kPlayer.h"

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

	class testa
	{
	protected:
		int a;
	public:
		testa(){}
		virtual ~testa(){}

		void aaa()
		{
			a=0;
		}
	};
	class testb:public testa
	{
	protected:
		int b;
	public:
		testb(){}
		virtual ~testb(){}

		void bbb()
		{
			b=0;
		}
	};

	class testScene:public IScene,public ktl::kSingleton<testScene>
	{
		friend class ktl::kSingleton<testScene>;
	private:
		kPlayer* mesh;
		kGraphicsPipline* pipline;
		kTechnique* tec;
		r2DObj* obj;
		r2DObj* paper;
		r2DObj* mask;
		rlib::AnalogStick* mp_Stick;
	public:
		//エントリー処理
		void entry()
		{
			mp_Stick = new rlib::AnalogStick();
			mp_Stick->init(-60, -50, 40);

			testb* asd=new testb;
			asd->aaa();
			asd->bbb();
			obj=new r2DObj;
			obj->load("testImage.png");
			obj->setPos(0,0);
			obj->setSize(100,true);

			paper=new r2DObj;
			paper->load("oldpaper.png");
			paper->setPos(0,0);
			paper->setSize(200,true);

			mask=new r2DObj;
			mask->load("mask.png");
			mask->setPos(0,0);
			mask->setSize(200,true);

			pipline=new kGraphicsPipline();
			//pipline->createVertexShader("a");
			pipline->createVertexShader("vertex.txt");
			pipline->createPixelShader("pixel.txt");
			pipline->createBlendState(k_BLEND_ALPHA);
			pipline->createDepthStencilState(true,eLESS_EQUAL);
			pipline->createRasterizerState(eSOLID,eFRONT,false);
			pipline->complete(desc,descnum);

			tec=new kTechnique();
			//pipline->createVertexShader("a");
			tec->createVertexShader("fire.vs");
			tec->createPixelShader("fire.fs");
			tec->bindAttribLocation(0,"VPosition");
			tec->bindAttribLocation(1,"VTexCoord");
			tec->createBlendState(k_BLEND_ALPHA);
			tec->createDepthStencilState(false,eLESS_EQUAL);
			tec->createRasterizerState(eSOLID,eNONE,false);
			tec->complete();

			float ary[3]={0.25f,0.5f,1.0f};
			pipline->setShaderValue("val",0.8f);
			pipline->setShaderValue("array",ary,3);

			mesh=new kPlayer("kman.IEM",mp_Stick);
			mesh->getObj()->setScale(0.05f);
			mesh->getObj()->setPosition(0,0,0);
			mesh->getObj()->SetMotion(4);
			mesh->getObj()->Update();
		}
		//更新処理
		void update()
		{
			static float a=0;
			mp_Stick->update();
			if( this->mp_Stick->enable() )
			{
				a += mp_Stick->getX()*0.01f;
			}
			
		 if( mlInput::isPinch() )
			{
				a += mlInput::getPinchMoveLength() * 0.01f;
			}
			a+=.001f;
			math::kclampf(0.0f,1.0f,&a);
			tec->setShaderValue("alpha",a);
			mesh->getObj()->setAngle(a);
			
			mesh->update();

		}
		//描画処理
		void render()
		{
			
			mesh->render(pipline);
			tec->setTexture("maskTex",1,mask);
			paper->render(tec);
			mp_Stick->render();
		}
		//終了処理
		void exit()
		{
			delete mesh;
			delete pipline;
			delete obj;
		}
	};
}