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
	public:
		kShotCamera(ICharacter* player,const math::Vector3& pos,const math::Vector3& angle);
		virtual ~kShotCamera();
		void update();
	};
}