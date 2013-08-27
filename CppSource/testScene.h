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
		{"VPosition",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
		{"VColor",0,k_VF_R32G32B32A32_FLOAT,0,eVertex,0}
	};
	u32 descnum=sizeof(desc)/sizeof(kInputElementDesc);
	class testScene:public IScene,public ktl::kSingleton<testScene>
	{
		friend class ktl::kSingleton<testScene>;
	private:
		kSkin* mesh;
		kGraphicsPipline* shader;
	public:
		//エントリー処理
		void entry()
		{
			shader=new kGraphicsPipline();
			shader->createVertexShader("vertex.txt");
			shader->createPixelShader("pixel.txt");
			shader->complete(desc,descnum);

			mesh=new kSkin("kman.IEM",new kMeshLoadIEM(),new kMeshGLES20Render());
			mesh->setScale(0.01f);
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

			mesh->Render(shader);
		}
		//終了処理
		void exit()
		{
			delete mesh;
			delete shader;
		}
	};
}