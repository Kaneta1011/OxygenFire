#pragma once
#include "kaneta\ICharacter\Class\ICharaStrategy\ICharaStrategy.h"
namespace klib
{
	//プレイヤーの通常行動クラス
	class kPlayerMove:public ICharaStrategy
	{
	private:
		void calcAngle(ICharacter* acter);
	public:
		kPlayerMove(ICharacter* acter);
		virtual bool execute(ICharacter* acter);
	};
}