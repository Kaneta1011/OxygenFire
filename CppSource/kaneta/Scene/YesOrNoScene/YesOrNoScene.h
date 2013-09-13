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
#include "Game\Bullet\Bullet.h"
#include "main.h"

#include "kaneta\Scene\GameScene\GameScene.h"


namespace klib
{

	class YesOrNoScene:public IScene,public ktl::kSingleton<YesOrNoScene>
	{
		friend class ktl::kSingleton<YesOrNoScene>;
	private:
		int* m_Flag;
		rlib::CircleButton* m_YesButton;
		rlib::CircleButton* m_NoButton;
	public:
		void setFlag(int* flag)
		{
			m_Flag=flag;
		}
		//エントリー処理
		void entry()
		{
			eprintf("YesOrNoScene entry");

			AssetsLoader::begin();
			kDevice::begin();

			m_YesButton=new rlib::CircleButton();
			m_NoButton=new rlib::CircleButton();

			m_YesButton->init("kanetaPlace/Yesf.png",-50,-50,50);
			m_NoButton->init("kanetaPlace/No.png",50,-50,50);

			kDevice::end();
			AssetsLoader::end();
		}
		//更新処理
		void update()
		{
			//eprintf("TitleScene update");
			m_YesButton->update();
			m_NoButton->update();
			if(m_YesButton->getMode()==m_YesButton->eDOWN)
			{
				//framework.sceneChange(testScene::_getInstancePtr());
				*m_Flag=1;
				framework.scenePop();
				return;
			}
			if(m_NoButton->getMode()==m_NoButton->eDOWN)
			{
				//framework.sceneChange(testScene::_getInstancePtr());
				*m_Flag=2;
				framework.scenePop();
				return;
			}
		}
		void render()
		{
			//eprintf("TitleScene render");
			framework.getPrevScene()->render();
			m_YesButton->render();
			m_NoButton->render();
		}
		//終了処理
		void exit()
		{
			eprintf("TitleScene exit");

			delete m_YesButton;
			delete m_NoButton;
		}
	};


}