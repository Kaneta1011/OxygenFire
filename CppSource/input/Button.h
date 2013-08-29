#ifndef _BUTTON_
#define _BUTTON_

#include "notUse/IButton.h"

namespace rlib
{
	class r2DObj;

	//============================================
	//
	//	‰~Œ`‚Ìƒ{ƒ^ƒ“
	//
	//============================================
	class CircleButton : public IButton
	{
	public:
		CircleButton();
		~CircleButton();

		void init(const char* imagePath, float xPosRate, float yPosRate, float range);

		void update();
		void render();

	private:
		r2DObj*	mpImage;
	};

}
#endif