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

	class MessageScene:public IScene,public ktl::kSingleton<MessageScene>
	{
		friend class ktl::kSingleton<MessageScene>;
	private:
		rlib::r2DObj* m_Tex;
	public:
		void setTexture(rlib::r2DObj* tex)
		{
			m_Tex=tex;
		}
		//エントリー処理
		void entry()
		{
			eprintf("YesOrNoScene entry");

			AssetsLoader::begin();
			kDevice::begin();

			kDevice::end();
			AssetsLoader::end();
		}
		//更新処理
		void update()
		{
			int sx,sy;
			int touchIndex=getTouch(&sx,&sy);
			if(touchIndex!=-1)framework.scenePop();
		}
		void render()
		{
			m_Tex->render();
		}
		//終了処理
		void exit()
		{

		}
	};


}