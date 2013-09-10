#include "kShotCamera.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "kaneta\ICharacter\Class\ICharacter\ICharacter.h"

namespace klib
{
	using namespace math;
	kShotCamera::kShotCamera(ICharacter* player,const math::Vector3& pos,const math::Vector3& angle):ICamera(pos,angle),m_Player(player)
	{
		//m_Angle=math::Vector3(K_PI/4.0f,0,0);
	}
	kShotCamera::~kShotCamera(){}

	void kShotCamera::update()
	{
		const rlib::AnalogStick* stick=m_Player->getAnalogStick();
		math::Vector3 playerPos=m_Player->getObj()->getPosition();

		//右半分フリックによるカメラ操作
		Vector2 flickMaxLength(0,0);

		for(int i=0;i<mlInput::getNowTouchCount();i++)
		{
			//タッチ先が右半分なら
			if(5*16.666666f<mlInput::getTime(i))
			{
				//if(0<mlInput::getX(i))
				{
					Vector2 nowLength(mlInput::getMoveX(i),mlInput::getMoveY(i));
					if(flickMaxLength.lengthSq()<nowLength.lengthSq())
					{
						flickMaxLength=nowLength;
					}
				}
			}
		}
		//長すぎる移動値に対して補正
		if(2.0f<flickMaxLength.length())
		{
			flickMaxLength.normalize();
			flickMaxLength*=2.0f;
		}
		m_Angle.y+=flickMaxLength.x*0.16f;
		m_Angle.x-=flickMaxLength.y*0.09f;

		//mlInput::setFlickSensitivity(50.0f);
		kclampf(-1.5f,1.5f,&m_Angle.x);
		m_Angle.y=kwrapf(-K_PI2,K_PI2,m_Angle.y);

		//角度によって正面を回転
		math::Matrix rot;
		rot.identity();
		rot.setRXYZ(m_Angle);
		//Matrix view=RenderLib::RenderState::getViewMatrix();
		Vector3 side;
		Vector3 top;
		Vector3 front;
		rot.getRow(0,&side);
		rot.getRow(1,&top);
		rot.getRow(2,&front);
		m_Pos=-front*4.0f+playerPos+top*2.0f+side*1.5f;

		RenderLib::RenderState::Setting_ViewMatrix(m_Pos,playerPos+top*2.0f+front*4.0f+side*1.5f,math::Vector3(0,1,0));
	}
}