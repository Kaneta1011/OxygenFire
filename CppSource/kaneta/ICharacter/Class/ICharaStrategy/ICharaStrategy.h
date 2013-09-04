#pragma once

namespace klib
{
	class ICharacter;
	//キャラクターの状態委譲クラス
	class ICharaStrategy
	{
	protected:
	public:
		ICharaStrategy(){}
		virtual ~ICharaStrategy(){}
		virtual bool execute(ICharacter* acter)=0;
	};

}