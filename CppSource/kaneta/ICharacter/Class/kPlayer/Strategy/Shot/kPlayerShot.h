#pragma once
#include "kaneta\ICharacter\Class\ICharaStrategy\ICharaStrategy.h"
#include "templateLib\kVector.h"
namespace klib
{
	struct PrevTouch
	{
		f32 time;
		int sx,sy;
	};
	//�v���C���[�̒ʏ�s���N���X
	class kPlayerShot:public ICharaStrategy
	{
		typedef ktl::kVector<PrevTouch> PrevTouchTable;
	protected:
		PrevTouchTable m_PrevTouchTable;
	private:
		//void calcAngle(ICharacter* acter);
	public:
		kPlayerShot(ICharacter* acter);
		virtual bool execute(ICharacter* acter);
	};
}