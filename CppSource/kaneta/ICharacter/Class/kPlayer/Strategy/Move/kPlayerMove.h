#pragma once
#include "kaneta\ICharacter\Class\ICharaStrategy\ICharaStrategy.h"
namespace klib
{
	//�v���C���[�̒ʏ�s���N���X
	class kPlayerMove:public ICharaStrategy
	{
	private:
		void calcAngle(ICharacter* acter);
	public:
		virtual bool execute(ICharacter* acter);
	};
}