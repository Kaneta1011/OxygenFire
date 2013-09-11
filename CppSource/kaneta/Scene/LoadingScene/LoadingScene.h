#pragma once
#include "math\kmathf.h"
#include "FrameWork\Class\IScene\IScene.h"
#include "templateLib\kSingleton.h"
#include "thread\Class\kThread\kThread.h"

namespace rlib
{
	class r2DObj;
}

namespace klib
{

}


namespace klib
{

	class LoadingScene:public IScene,public ktl::kSingleton<LoadingScene>
	{
		friend class ktl::kSingleton<LoadingScene>;
	private:
		thread::kThread* mp_LoadingThread;
		thread::kThreadHolder* m_LoadingFunc;
	public:

		void set(thread::kThreadHolder* func);
		//エントリー処理
		void entry();
		//更新処理
		void update();
		void render();
		//終了処理
		void exit();
	};
}