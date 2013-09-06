#include "kPlayCamera.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "kaneta\ICharacter\Class\ICharacter\ICharacter.h"

namespace klib
{
	using namespace math;
	kPlayCamera::kPlayCamera(ICharacter* player):m_Player(player)
	{
		m_Angle=math::Vector3(K_PI/4.0f,0,0);
	}
	kPlayCamera::~kPlayCamera(){}

	void kPlayCamera::update()
	{
		const rlib::AnalogStick* stick=m_Player->getAnalogStick();
		math::Vector3 playerPos=m_Player->getObj()->getPosition();

		//右半分フリックによるカメラ操作
		Vector2 flickMaxLength(0,0);
		for(int i=0;i<mlInput::getNowTouchCount();i++)
		{
			//タッチ先が右半分なら
			if(0<mlInput::getX(i))
			{
				Vector2 nowLength(mlInput::getMoveX(i),mlInput::getMoveY(i));
				if(flickMaxLength.lengthSq()<nowLength.lengthSq())
				{
					flickMaxLength=nowLength;
				}
			}
		}
		//長すぎる移動値に対して補正
		float range = 2.0f;
		if(range<flickMaxLength.length())
		{
				flickMaxLength.normalize();
				flickMaxLength*=range;
		}
		m_Angle.y+=flickMaxLength.x*0.16f;
		m_Angle.x-=flickMaxLength.y*0.09f;

		kclampf(-1.5f,1.5f,&m_Angle.x);

		//角度によって正面を回転
		math::Matrix rot;
		rot.identity();
		rot.setRXYZ(m_Angle);
		math::Vector3 front(0,0,1);
		front.trans3x3(rot);

		RenderLib::RenderState::Setting_ViewMatrix(-front*3.0f+playerPos+Vector3(0,0.01f,0),playerPos+Vector3(0,1,0),math::Vector3(0,1,0));
	}
}