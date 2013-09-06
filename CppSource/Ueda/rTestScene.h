#ifndef _TEST_SCENE_
#define _TEST_SCENE_

#include "FrameWork\Class\IScene\IScene.h"

namespace klib{
	class kMesh;
	class kGraphicsPipline;
	class kPlayer;
	class ICamera;
}

namespace rlib{
	class FrameBuffer;
	class CircleButton;
	class AnalogStick;
}

class rTestScene : public klib::IScene
{
public:
	static klib::kGraphicsPipline* pipeline;

public:
	rTestScene();
	void entry();
	void update();
	void render();
	void exit();

private:
	rlib::FrameBuffer			*frameBuffer;
	rlib::CircleButton			*mButton;
	rlib::AnalogStick			*mStick;
	klib::kPlayer				*mMesh;
	klib::ICamera				*m_Camera;
	klib::kMesh					*mpStage;
};

#endif