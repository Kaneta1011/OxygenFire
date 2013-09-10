#include "kPlayCamera.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "kaneta\ICharacter\Class\ICharacter\ICharacter.h"

namespace klib
{
	using namespace math;
	kPlayCamera::kPlayCamera(ICharacter* player,const math::Vector3& pos,const math::Vector3& angle):ICamera(pos,angle),m_Player(player)
	{
		//m_Angle=math::Vector3(K_PI/4.0f,0,0);
	}
	kPlayCamera::~kPlayCamera(){}

	void kPlayCamera::update()
	{
		const rlib::AnalogStick* stick=m_Player->getAnalogStick();
		math::Vector3 playerPos=m_Player->getObj()->getPosition();

		//�E�����t���b�N�ɂ��J��������
		Vector2 flickMaxLength(0,0);
		for(int i=0;i<mlInput::getNowTouchCount();i++)
		{
			//�^�b�`�悪�E�����Ȃ�
			if(0<mlInput::getX(i))
			{
				Vector2 nowLength(mlInput::getMoveX(i),mlInput::getMoveY(i));
				if(flickMaxLength.lengthSq()<nowLength.lengthSq())
				{
					flickMaxLength=nowLength;
				}
			}
		}
		//��������ړ��l�ɑ΂��ĕ␳
		if(2.0f<flickMaxLength.length())
		{
				flickMaxLength.normalize();
				flickMaxLength*=2.0f;
		}
		m_Angle.y+=flickMaxLength.x*0.16f;
		m_Angle.x-=flickMaxLength.y*0.09f;

		kclampf(-1.5f,1.5f,&m_Angle.x);
		m_Angle.y=kwrapf(-K_PI2,K_PI2,m_Angle.y);

		//�p�x�ɂ���Đ��ʂ���]
		math::Matrix rot;
		rot.identity();
		rot.setRXYZ(m_Angle);
		math::Vector3 front(0,0,1);
		front.trans3x3(rot);

		RenderLib::RenderState::Setting_ViewMatrix(-front*10.0f+playerPos+Vector3(0,1,0),playerPos+Vector3(0,1,0),math::Vector3(0,1,0));
	}
}