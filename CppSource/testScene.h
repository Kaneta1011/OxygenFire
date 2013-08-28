#pragma once
#include "FrameWork\Class\kFrameWork\kFrameWork.h"
#include "GraphicsLib\Class\kDevice\kDevice.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"



namespace klib
{
	kInputElementDesc desc[]=
	{
		{"POSITION",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
		{"COLOR",0,k_VF_R32G32B32A32_FLOAT,0,eVertex,0},
		{"NORMAL",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
		{"TEXCOORD",0,k_VF_R32G32_FLOAT,0,eVertex,0}
	};
	u32 descnum=sizeof(desc)/sizeof(kInputElementDesc);
	class testScene:public IScene,public ktl::kSingleton<testScene>
	{
		friend class ktl::kSingleton<testScene>;
	private:
		kSkin* mesh;
		kGraphicsPipline* pipline;

	public:
		//エントリー処理
		void entry()
		{

			pipline=new kGraphicsPipline();
			pipline->createVertexShader("vertex.txt");
			pipline->createPixelShader("pixel.txt");
			pipline->createBlendState(k_BLEND_NONE);
			pipline->createDepthStencilState(true,eLESS_EQUAL);
			pipline->createRasterizerState(eWIRE,eFRONT,false);
			pipline->complete(desc,descnum);

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

			mesh->Render(pipline);
		}
		//終了処理
		void exit()
		{
			delete mesh;
			delete pipline;
		}
	};
}