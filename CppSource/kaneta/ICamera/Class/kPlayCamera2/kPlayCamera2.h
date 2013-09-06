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