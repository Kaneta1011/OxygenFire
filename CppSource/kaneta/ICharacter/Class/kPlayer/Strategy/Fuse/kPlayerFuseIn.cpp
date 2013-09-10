#include "kPlayerFuseIn.h"
#include "kPlayerFuse.h"
#include "kaneta\ICharacter\Class\ICharacter\ICharacter.h"
#include "input\AnalogStick.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "kaneta\ICamera\Class\kPlayCamera\kPlayCamera.h"
#include "testScene.h"
#include "kaneta\ICharacter\Class\kPlayer\Strategy\Shot\kPlayerShot.h"
#include "kaneta\ICamera\Class\kPlayCamera2\kPlayCamera2.h"
namespace klib
{
	kPlayerFuseIn::kPlayerFuseIn(ICharacter* acter):m_Time(0.0f)
	{
		ICamera* camera=testScene::_getInstance().getCamera();
		testScene::_getInstance().setCamera(new kPlayCamera2(acter,camera->getPos(),camera->getAngle()));
		m_StartPos=acter->getObj()->getPosition();
		m_EndPos=Vector3(0,0,10);
	}

	bool kPlayerFuseIn::execute(ICharacter* acter)
	{
		kSkin* obj=acter->getObj();

		obj->setPosition(klerp(m_StartPos,m_EndPos,kcube(m_Time)));



		m_Time+=0.01f;

		if(1.0f<m_Time)
		{
			acter->setStrategy(new kPlayerFuse(acter));
			//“±‰Îü‚É“’B‚µ‚½‚Ì‚Å“±‰Îü‚ğˆÚ“®‚·‚éˆ—‚ÉˆÚ‚é
			m_Time=1.0f;
		}
		
	}
}