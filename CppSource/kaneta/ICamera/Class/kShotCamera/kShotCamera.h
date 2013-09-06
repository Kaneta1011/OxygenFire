#pragma once

#include "kaneta\ICamera\Class\ICamera\ICamera.h"
#include "templateLib\kVector.h"
//前方宣言
namespace klib
{
	class ICharacter;
}
namespace klib
{
	//プレイヤー追従通常カメラ
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