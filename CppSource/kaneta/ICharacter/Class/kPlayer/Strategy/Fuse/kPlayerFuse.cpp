#include "math\kmathf.h"
#include "GraphicsLib\Class\r2DObj\r2DObj.h"
#include "kPlayerFuse.h"
#include "kPlayerFuseOut.h"
#include "kaneta\ICharacter\Class\ICharacter\ICharacter.h"
#include "input\AnalogStick.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "kaneta\ICamera\Class\kPlayCamera\kPlayCamera.h"
#include "kaneta/Scene/GameScene/GameScene.h"
#include "kaneta\ICharacter\Class\kPlayer\Strategy\Shot\kPlayerShot.h"
#include "kaneta\ICamera\Class\kPlayCamera2\kPlayCamera2.h"
namespace klib
{
	using namespace math;
	using namespace rlib;
	kPlayerFuse::kPlayerFuse(ICharacter* acter):m_Time(0.0f)
	{
		ICamera* camera=GameScene::_getInstance().getCamera();
		GameScene::_getInstance().setCamera(new kPlayCamera2(acter,camera->getPos(),camera->getAngle()));
		m_StartPos=acter->getObj()->getPosition();
		m_EndPos=Vector3(0,5,-5);
		m_Length=m_StartPos.distance(m_EndPos);
	}

	bool kPlayerFuse::execute(ICharacter* acter)
	{
		kSkin* obj=acter->getObj();

		obj->setPosition(klerp(m_StartPos,m_EndPos,m_Time));



		m_Time+=0.05f/m_Length;

		if(1.0f<m_Time)
		{
			acter->setStrategy(new kPlayerFuseOut(acter));
			//導火線に到達したので導火線を移動する処理に移る
			m_Time=1.0f;
		}
		
	}
}