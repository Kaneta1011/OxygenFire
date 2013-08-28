#ifndef _ANALOG_STICK_
#define _ANALOG_STICK_

#include <Graphic\RULTexture.h>

#include "IButton.h"

typedef RULTexture SPRITE;

class AnalogStick : public IButton
{
public:
	AnalogStick();
	~AnalogStick();

	void init(float xPosRate, float yPosRate, float range);

	void update();
	void render();

public:
	float getX()const{return this->mRate.x;}
	float getY()const{return this->mRate.y*-1.f;}

private:

private:
	Vector2 mRate;

	SPRITE *mpBack, *mpStick;
};

#endif