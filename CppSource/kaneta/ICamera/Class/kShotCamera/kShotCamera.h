#pragma once

#include "kaneta\ICamera\Class\ICamera\ICamera.h"
#include "templateLib\kVector.h"
//�O���錾
namespace klib
{
	class ICharacter;
}
namespace klib
{
	//�v���C���[�Ǐ]�ʏ�J����
	class kShotCamera:public ICamera
	{
	protected:
		ICharacter* m_Player;
		math::Vector3 m_Angle;
	public:
		kShotCamera(ICharacter* player);
		virtual ~kShotCamera();
		void update();
	};
}