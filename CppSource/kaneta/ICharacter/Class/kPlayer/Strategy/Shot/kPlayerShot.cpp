#include "kPlayerShot.h"
#include "kaneta\ICharacter\Class\ICharacter\ICharacter.h"
#include "input\AnalogStick.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "kaneta\Scene\GameScene\GameScene.h"
#include "kaneta\ICamera\Class\ICamera\ICamera.h"
#include "GraphicsLib\Class\tRenderState\RenderState.h"
#include "math\kmathf.h"
#include "Game\Bullet\Bullet.h"
#include "kaneta\ICharacter\Class\kPlayer\Strategy\Move\kPlayerMove.h"
#include "kaneta\ICamera\Class\kShotCamera\kShotCamera.h"
#include "Game\Bullet\Bullet.h"

namespace klib
{
	using namespace math;
	using namespace rlib;
	kPlayerShot::kPlayerShot(ICharacter* acter)
	{
		ICamera* camera=GameScene::_getInstance().getCamera();
		GameScene::_getInstance().setCamera(new kShotCamera(acter,camera->getPos(),camera->getAngle()));
		//m_PrevTouchTable.clear();
		//for(int i=0;i<mlInput::getMaxPoint();i++)
		//{
		//	m_PrevTouchTable(i).time=0.0f;
		//	m_PrevTouchTable(i).sx=0.0f;
		//	m_PrevTouchTable(i).sy=0.0f;
		//}
	}

	bool kPlayerShot::execute(ICharacter* acter)
	{


		GameScene& scene=GameScene::_getInstance();
		ICamera* camera=scene.getCamera();
		Matrix view=RenderLib::RenderState::getViewMatrix();
		Vector3 front;
		view.getCol(2,&front);

		view.transpose();
		view.decomposeR(&view);
		view._41=acter->getObj()->getPositionX();
		view._42=acter->getObj()->getPositionY();
		view._43=acter->getObj()->getPositionZ();
		view.mulS(0.01f);
		acter->getObj()->setTransMatrix(view);

		//タッチ検出8フレーム以内に離せばタッチとみなす
		bool isTouch=false;
		Vector3 touchPosN(0,0,0);
		Vector3 touchPosF(0,0,0);
		int touchIndex;
		int sx,sy;
		if((touchIndex=getTouch(&sx,&sy))!=-1)
		{
			int width=RenderLib::RenderState::getScreenWidth();
			int height=RenderLib::RenderState::getScreenHeight();
			int touchPosX=((sx+100.0f)/200.0f)*width;
			int touchPosY=(1.0f-((sy+100.0f)/200.0f))*height;
			screenToWorld(&touchPosN,touchPosX,touchPosY,0.0f);
			screenToWorld(&touchPosF,touchPosX,touchPosY,1.0f);
			Vector3 vec=touchPosF-touchPosN;
			vec.normalize();
			isTouch=true;

			rlib::BulletInfo bullet;
			bullet.pos=touchPosN;
			bullet.size=Vector3(0.005,0.005,0.005);
			bullet.velocity=vec*0.1f;
			BulletManager::getInst().add(bullet);
		}
		//for(int i=0;i<mlInput::getMaxPoint();i++)
		//{
		//	if(mlInput::getTime(i)<FEQ_EPS)
		//	{
		//		if(0.0f<m_PrevTouchTable[i].time && m_PrevTouchTable[i].time<8.0f*16.666666f)
		//		{
		//			int width=RenderLib::RenderState::getScreenWidth();
		//			int height=RenderLib::RenderState::getScreenHeight();
		//			int touchPosX=((m_PrevTouchTable[i].sx+100.0f)/200.0f)*width;
		//			int touchPosY=(1.0f-((m_PrevTouchTable[i].sy+100.0f)/200.0f))*height;
		//			screenToWorld(&touchPosN,touchPosX,touchPosY,0.0f);
		//			screenToWorld(&touchPosF,touchPosX,touchPosY,1.0f);
		//			Vector3 vec=touchPosF-touchPosN;
		//			vec.normalize();
		//			isTouch=true;
		//			
		//			rlib::BulletInfo bullet;
		//			bullet.pos=touchPosN;
		//			bullet.size=Vector3(0.005,0.005,0.005);
		//			bullet.velocity=vec*0.1f;
		//			BulletManager::getInst().add(bullet);
		//		}
		//	}
		//	m_PrevTouchTable[i].time=mlInput::getTime(i);
		//	m_PrevTouchTable[i].sx=mlInput::getX(i);
		//	m_PrevTouchTable[i].sy=mlInput::getY(i);
		//}

		if(acter->getButton()->getMode()==acter->getButton()->eDOWN)
		{
			acter->setStrategy(new kPlayerMove(acter));
		}
		return true;
	}
}