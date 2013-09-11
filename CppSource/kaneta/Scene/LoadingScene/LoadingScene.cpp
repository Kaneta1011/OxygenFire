#include "LoadingScene.h"
#include "FrameWork\Class\kFrameWork\kFrameWork.h"
#include "input\AnalogStick.h"
#include "thread\Class\kThread\kThread.h"
#include "utility\assetsLoader.h"
#include "GraphicsLib\Class\kDevice\kDevice.h"
#include "main.h"


namespace klib
{
		void LoadingScene::set(thread::kThreadHolder* func)
		{
			m_LoadingFunc=func;
		}

		//エントリー処理
		void LoadingScene::entry()
		{
			eprintf("LoadScene entry");
			AssetsLoader::begin();
			kDevice::begin();
			
			mp_LoadingThread=new thread::kThread(m_LoadingFunc);
			
		}
		//更新処理
		void LoadingScene::update()
		{
			eprintf("LoadScene update");
			if(mp_LoadingThread->isEnd())framework.scenePop();//次シーンへの遷移
		}
		void LoadingScene::render()
		{
			eprintf("LoadScene render");
		}
		//終了処理
		void LoadingScene::exit()
		{
			eprintf("LoadScene exit");
			//delete mp_LoadingFunc;
			delete mp_LoadingThread;
			
			kDevice::end();
			AssetsLoader::end();
		}
}