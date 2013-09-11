#pragma once


#include "kaneta\ICamera\Class\ICamera\ICamera.h"
#include "templateLib\smartptr.h"

#include "templateLib\kSingleton.h"
#include "FrameWork\Class\IScene\IScene.h"

namespace rlib
{
	class r2DObj;
	class CircleButton;
	class AnalogStick;
}
namespace klib
{
	class kMesh;
	class kPlayer;
	class kGraphicsPipline;
}

namespace klib
{
	

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

	class GameScene:public IScene,public ktl::kSingleton<GameScene>
	{
		friend class ktl::kSingleton<GameScene>;
	private:
		ktl::sp<ICamera> m_Camera;
		kMesh* stage;
		kPlayer* mesh;
		kGraphicsPipline* pipline;
		kGraphicsPipline* bord;
		kGraphicsPipline* addBord;
		rlib::CircleButton* m_Button;
		rlib::r2DObj* mask;
		rlib::r2DObj* ring;
		rlib::AnalogStick* mp_Stick;
	public:
		void setCamera(ICamera* camera);
		ICamera* getCamera()const;
		kPlayer* getPlayer()const;

		static void sceneLoad(GameScene* obj);
		//エントリー処理
		void entry();
		//更新処理
		void update();
		//描画処理
		void render();
		//終了処理
		void exit();
	};
}