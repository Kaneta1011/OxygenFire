#include "kPlayCamera2.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "kaneta\ICharacter\Class\ICharacter\ICharacter.h"

namespace klib
{
	using namespace math;
	kPlayCamera2::kPlayCamera2(ICharacter* player,const math::Vector3& pos,const math::Vector3& angle):ICamera(pos,angle),m_Player(player)
	{
		math::Matrix rot;
		rot.identity();
		rot.setRXYZ(m_Angle);
		math::Vector3 front;
		rot.getRow(2,&front);
		this->m_Pos=-front*10.0f+player->getObj()->getPosition()+Vector3(0,1,0);
		//m_Angle=math::Vector3(K_PI/4.0f,0,0);
	}
	kPlayCamera2::~kPlayCamera2(){}

	void kPlayCamera2::update()
	{
		f32 cameraLength=10.0f;
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
		if(2.0f<flickMaxLength.length())
		{
			flickMaxLength.normalize();
			flickMaxLength*=2.0f;
		}
		m_Angle.y+=flickMaxLength.x*0.16f;
		m_Angle.x-=flickMaxLength.y*0.09f;
		if(flickMaxLength.length()<0.01f)
		{
			Vector3 vec=m_Pos-(playerPos+Vector3(0,1,0));
			f32 len=vec.length();
			vec.normalize();

			Vector3 ret;
			math::Vector3toEuler(&ret,-vec);
			m_Angle.y=ret.y;

		}
		kclampf(-1.5f,1.5f,&m_Angle.x);
		m_Angle.y=kwrapf(-K_PI2,K_PI2,m_Angle.y);

		//角度によって正面を回転
		math::Matrix rot;
		rot.identity();
		rot.setRXYZ(m_Angle);
		math::Vector3 front;
		rot.getRow(2,&front);
		this->m_Pos=-front*cameraLength+playerPos+Vector3(0,1,0);

		RenderLib::RenderState::Setting_ViewMatrix(m_Pos,playerPos+Vector3(0,1,0),math::Vector3(0,1,0));
	}
}