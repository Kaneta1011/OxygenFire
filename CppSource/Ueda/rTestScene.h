#ifndef _TEST_SCENE_
#define _TEST_SCENE_

#include "FrameWork\Class\IScene\IScene.h"
#include "GraphicsLib\Class\r2DObj\r2DObj.h"
#include "input\Button.h"

namespace rlib{
	class FrameBuffer;
}

class rTestScene : public klib::IScene
{
public:
	rTestScene();
	void entry();
	void update();
	void render();
	void exit();
private:
	rlib::r2DObj* mp2dObj;
	klib::math::Vector3			*Pos;
	rlib::Texture				*Tex;
	rlib::FrameBuffer			*frameBuffer;
	rlib::CircleButton			*mButton;
};

#endif