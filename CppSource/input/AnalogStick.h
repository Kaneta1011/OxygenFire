#ifndef _ANALOG_STICK_
#define _ANALOG_STICK_

#include "GraphicsLib\Class\rTexture\Texture.h"

#include "notUse/IButton.h"
#include "GraphicsLib\Class\r2DObj\r2DObj.h"

namespace rlib
{
	class AnalogStick : public IButton
	{
	public:
		AnalogStick();
		~AnalogStick();

		void init(float xPosRate, float yPosRate, float range);

		void update();
		void render();

	public:
		/*
		スティックのX
		*/
		float getX()const{return this->mRate.x;}
		float getY()const{return this->mRate.y;}
		/*
		操作中か？
		*/
		bool enable()const{return this->mEnable;}

	private:
		void reset();
		bool isNowEnable(klib::math::Vector2& pos);

	private:
		klib::math::Vector2 mRate;
		int					mStartTouchCount;
		bool mEnable;
		klib::math::Vector2 mPrevTouchPos;

		r2DObj *mpBack, *mpStick;
	};
}

#endif