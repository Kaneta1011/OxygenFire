#pragma once
#include "ICharacter.h"

namespace klib
{
	class kPlayer:public ICharacter
	{
	protected:
	public:
		kPlayer(const char* filename):ICharacter(filename){setStrategy(new kPlayerMove);}
		kPlayer(const char* filename,const rlib::AnalogStick* stick):ICharacter(filename,stick){setStrategy(new kPlayerMove);}
		~kPlayer(){}

		bool update()
		{
			ICharacter::exeStrategy();
			mp_Mesh->Update();
			mp_Mesh->animation(1.0f);
		}
		bool render(kGraphicsPipline* pipline)
		{
			mp_Mesh->Render(pipline);
		}
	};
}