#pragma once

#include "kaneta\ICamera\Class\ICamera\ICamera.h"
//�O���錾
namespace klib
{
	class ICharacter;
}
namespace klib
{
	//�v���C���[�Ǐ]�ʏ�J����
	class kPlayCamera2:public ICamera
	{
	protected:
		ICharacter* m_Player;
		math::Vector3 m_Angle;
	public:
		kPlayCamera2(ICharacter* player);
		virtual ~kPlayCamera2();
		void update();
	};
}