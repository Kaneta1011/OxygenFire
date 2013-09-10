#pragma once
#include "math\kmathf.h"
#include "kaneta\ICharacter\Class\ICharaStrategy\ICharaStrategy.h"
namespace klib
{
	//プレイヤーの通常行動クラス
	class kPlayerFuseIn:public ICharaStrategy
	{
	private:
		f32 m_Time;
		math::Vector3 m_StartPos;
		math::Vector3 m_EndPos;
	public:
		kPlayerFuseIn(ICharacter* acter);
		virtual bool execute(ICharacter* acter);
	};
}