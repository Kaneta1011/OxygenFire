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
	class kPlayCamera:public ICamera
	{
	protected:
		ICharacter* m_Player;
		math::Vector3 m_Angle;
	public:
		kPlayCamera(ICharacter* player);
		virtual ~kPlayCamera();
		void update();
	};
}