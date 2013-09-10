#pragma once
#include "math\kmathf.h"
#include "kaneta\ICharacter\Class\ICharaStrategy\ICharaStrategy.h"
namespace klib
{
	//�v���C���[�̒ʏ�s���N���X
	class kPlayerFuseOut:public ICharaStrategy
	{
	private:
		f32 m_Time;
		math::Vector3 m_StartPos;
		math::Vector3 m_EndPos;
	public:
		kPlayerFuseOut(ICharacter* acter);
		virtual bool execute(ICharacter* acter);
	};
}