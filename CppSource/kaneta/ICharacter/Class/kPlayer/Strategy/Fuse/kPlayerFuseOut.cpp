#include "kPlayerFuseOut.h"
#include "kaneta\ICharacter\Class\kPlayer\Strategy\Move\kPlayerMove.h"
#include "kaneta\ICharacter\Class\ICharacter\ICharacter.h"
#include "input\AnalogStick.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "kaneta\ICamera\Class\kPlayCamera\kPlayCamera.h"
#include "testScene.h"
#include "kaneta\ICharacter\Class\kPlayer\Strategy\Shot\kPlayerShot.h"
#include "kaneta\ICamera\Class\kPlayCamera2\kPlayCamera2.h"
namespace klib
{
	kPlayerFuseOut::kPlayerFuseOut(ICharacter* acter):m_Time(0.0f)
	{
		ICamera* camera=testScene::_getInstance().getCamera();
		testScene::_getInstance().setCamera(new kPlayCamera2(acter,camera->getPos(),camera->getAngle()));
		m_StartPos=acter->getObj()->getPosition();
		m_EndPos=Vector3(0,0,0);
	}

	bool kPlayerFuseOut::execute(ICharacter* acter)
	{
		kSkin* obj=acter->getObj();

		obj->setPosition(kbezier(m_StartPos,(m_StartPos+m_EndPos)*0.25+Vector3(0,2,0),(m_StartPos+m_EndPos)*0.75+Vector3(0,2,0),m_EndPos,m_Time));



		m_Time+=0.05f;

		if(1.0f<m_Time)
		{
			acter->setStrategy(new kPlayerMove(acter));
			//“±‰Îü‚É“’B‚µ‚½‚Ì‚Å“±‰Îü‚ğˆÚ“®‚·‚éˆ—‚ÉˆÚ‚é
			m_Time=1.0f;
		}
		
	}
}