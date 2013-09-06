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


namespace klib
{

	class LoadingScene:public IScene,public ktl::kSingleton<LoadingScene>
	{
		friend class ktl::kSingleton<LoadingScene>;
	private:
		thread::kThread* mp_LoadingThread;
		thread::kThreadHolder* m_LoadingFunc;
	public:

		void set(thread::kThreadHolder* func)
		{
			m_LoadingFunc=func;
		}

		//エントリー処理
		void entry()
		{
			eprintf("LoadScene entry");
			AssetsLoader::begin();
			kDevice::begin();
			
			mp_LoadingThread=new thread::kThread(m_LoadingFunc);
			
		}
		//更新処理
		void update()
		{
			eprintf("LoadScene update");
			if(mp_LoadingThread->isEnd())framework.scenePop();//次シーンへの遷移
		}
		void render()
		{
			eprintf("LoadScene render");
		}
		//終了処理
		void exit()
		{
			eprintf("LoadScene exit");
			//delete mp_LoadingFunc;
			delete mp_LoadingThread;
			
			kDevice::end();
			AssetsLoader::end();
		}
	};
}