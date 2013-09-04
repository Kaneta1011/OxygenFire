#pragma once
#include "kaneta\ICharacter\Class\ICharacter\ICharacter.h"
#include "kaneta\ICharacter\Class\ICharaStrategy\ICharaStrategy.h"

namespace klib
{
	//プレイヤークラス
	class kPlayer:public ICharacter
	{
	protected:
		
		
	public:
		kPlayer(const char* filename);
		kPlayer(const char* filename,const rlib::AnalogStick* stick);
		~kPlayer();
		bool update();
		bool render(kGraphicsPipline* pipline);
	};
}