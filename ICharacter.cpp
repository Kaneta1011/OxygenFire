#include "ICharacter.h"
namespace klib
{
	void ICharacter::setStrategy(ICharaStrategy* strategy)
	{
		mp_Strategy.SetPtr(strategy,false);
	}

	bool ICharacter::exeStrategy()
	{
		if(!mp_Strategy.GetPtr())return false;
		mp_Strategy->execute(this);
		return true;
	}
}