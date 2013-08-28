#ifndef _BUTTON_
#define _BUTTON_

#include "notUse/IButton.h"

typedef RULTexture SPRITE;

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
	SPRITE *mpImage;
};

#endif