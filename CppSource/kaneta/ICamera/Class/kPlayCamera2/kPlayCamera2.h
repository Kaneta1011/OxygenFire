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
	public:
		kPlayCamera2(ICharacter* player,const math::Vector3& pos,const math::Vector3& angle);
		virtual ~kPlayCamera2();
		void update();
	};
}