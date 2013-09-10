#pragma once

#include "kaneta\ICamera\Class\ICamera\ICamera.h"
//前方宣言
namespace klib
{
	class ICharacter;
}
namespace klib
{
	//プレイヤー追従通常カメラ
	class kPlayCamera:public ICamera
	{
	protected:
		ICharacter* m_Player;
	public:
		kPlayCamera(ICharacter* player,const math::Vector3& pos,const math::Vector3& angle);
		virtual ~kPlayCamera();
		void update();
	};
}