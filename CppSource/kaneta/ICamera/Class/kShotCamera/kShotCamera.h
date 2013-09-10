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
	public:
		kShotCamera(ICharacter* player,const math::Vector3& pos,const math::Vector3& angle);
		virtual ~kShotCamera();
		void update();
	};
}