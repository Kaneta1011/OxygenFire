#pragma once
#include "FrameWork\Class\kFrameWork\kFrameWork.h"
#include "GraphicsLib\Class\kDevice\kDevice.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"
#include "GraphicsLib\Class\r2DObj\r2DObj.h"


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
		kSkin* mesh;
		kGraphicsPipline* pipline;
		kTechnique* tec;
		r2DObj* obj;
	public:
		//エントリー処理
		void entry()
		{
			testb* asd=new testb;
			asd->aaa();
			asd->bbb();
			obj=new r2DObj;
			obj->load("testImage.png");
			obj->setPos(0,0);

			pipline=new kGraphicsPipline();
			//pipline->createVertexShader("a");
			pipline->createVertexShader("vertex.txt");
			pipline->createPixelShader("pixel.txt");
			pipline->createBlendState(k_BLEND_ADD);
			pipline->createDepthStencilState(true,eLESS_EQUAL);
			pipline->createRasterizerState(eSOLID,eFRONT,false);
			pipline->complete(desc,descnum);

			tec=new kTechnique();
			//pipline->createVertexShader("a");
			tec->createVertexShader("test.vs");
			tec->createPixelShader("test.fs");
			tec->bindAttribLocation(0,"VPosition");
			tec->bindAttribLocation(1,"VTexCoord");
			tec->createBlendState(k_BLEND_NONE);
			tec->createDepthStencilState(false,eLESS_EQUAL);
			tec->createRasterizerState(eSOLID,eNONE,false);
			tec->complete();

			float ary[3]={0.25f,0.5f,1.0f};
			pipline->setShaderValue("val",0.8f);
			pipline->setShaderValue("array",ary,3);

			mesh=new kSkin("kman.IEM",new kMeshLoadIEM(),new kMeshGLES20Render());
			mesh->setScale(0.05f);
			mesh->setPosition(0,0,0);
			mesh->Update();
		}
		//更新処理
		void update()
		{
			static float a=0;
			a+=.005f;
			mesh->setAngle(a);
			mesh->animation(1);
			mesh->Update();
		}
		//描画処理
		void render()
		{
			pipline->setTexture("colorTex",0,obj);
			mesh->Render(pipline);
			obj->render(tec);
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