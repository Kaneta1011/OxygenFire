#include "kPlayerMove.h"
#include "kaneta\ICharacter\Class\ICharacter\ICharacter.h"
#include "input\AnalogStick.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
namespace klib
{
		void kPlayerMove::calcAngle(ICharacter* acter)
		{
			math::Vector3 Move=acter->getMove();
			math::Vector3 Angle=acter->getObj()->getAngle();

		float v1x=Move.x;
		float v1z=Move.z;
		float v2x=sinf(Angle.y);
		float v2z=cosf(Angle.y);
		float  d  = sqrtf(v1x * v1x + v1z * v1z);
		v1x /= d;
		v1z /= d;
		float a=(1-(v1x*v2x+v1z*v2z))*2.0f;
		if(a>0.3f)a=0.3f;
		if( v1x * v2z - v1z * v2x < 0 )acter->getObj()->addAngleY(-a);
		else acter->getObj()->addAngleY(a);

			
		}

		bool kPlayerMove::execute(ICharacter* acter)
		{
			acter->setMove(math::Vector3(0,0,0));
			const rlib::AnalogStick* stick=acter->getAnalogStick();
			kSkin* obj=acter->getObj();

			if(stick->enable())
			{
				obj->SetMotion(3);
				math::Matrix view=RenderLib::RenderState::getViewMatrix();
				math::Vector3 front;
				math::Vector3 side;
				math::Vector3 top;
				view.getCol(0,&side);
				view.getCol(1,&top);
				view.getCol(2,&front);
				side.y=0;
				front.y=0;
				side.normalize();
				front.normalize();
				math::Vector3 move(0,0,0);
				move+=side*stick->getX()*0.1f;
				move+=front*stick->getY()*0.1f;
				if(move.length()>0.01f)
				{
					acter->setMove(move);
					calcAngle(acter);
				}
			}
			
			obj->addPosition(acter->getMove());
		}
}