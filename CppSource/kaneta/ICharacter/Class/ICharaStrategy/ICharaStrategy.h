#pragma once

namespace klib
{
	class ICharacter;
	//�L�����N�^�[�̏�ԈϏ��N���X
	class ICharaStrategy
	{
	protected:
	public:
		ICharaStrategy(){}
		virtual ~ICharaStrategy(){}
		virtual bool execute(ICharacter* acter)=0;
	};

}