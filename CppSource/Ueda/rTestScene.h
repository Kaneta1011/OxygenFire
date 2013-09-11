#ifndef _TEST_SCENE_
#define _TEST_SCENE_

#include "FrameWork\Class\kFrameWork\kFrameWork.h"

#include "FrameWork\Class\IScene\IScene.h"
#include "thread\Class\kThread\kThread.h"
#include "templateLib\kSingleton.h"

#include "LoadingScene.h"

namespace klib{
	class kMesh;
	class kPlayer;
	class ICamera;
}

namespace rlib{
	class FrameBuffer;
	class CircleButton;
	class AnalogStick;
}

class rTestScene:public klib::IScene, public klib::ktl::kSingleton<rTestScene>
{
	friend class klib::ktl::kSingleton<rTestScene>;
public:
	static void threadFunc(rTestScene* obj);

	rTestScene();
	void entry();
	void update();
	void render();
	void exit();

private:
	rlib::FrameBuffer			*frameBuffer;
	rlib::CircleButton			*mButton;
	rlib::AnalogStick			*mStick;
	klib::kPlayer				*mPlayer;
	klib::ICamera				*m_Camera;
};

#endif