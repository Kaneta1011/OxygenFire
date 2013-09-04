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
		math::Vector3 m_Angle;
	public:
		kPlayCamera(ICharacter* player);
		virtual ~kPlayCamera();
		void update();
	};
}