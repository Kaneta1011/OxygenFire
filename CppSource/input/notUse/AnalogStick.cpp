//#include "../AnalogStick.h"
//
////#define WINDOWS
//#ifdef WINDOWS
//#include <Graphic\RULGraphic.h>
//#include <Graphic\RULWindow.h>
//#include <Graphic\RUL2D.h>
//#include <Input\RUL_Input.h>
//#endif
//
//#ifndef NULL
//#define NULL 0
//#endif
//
//AnalogStick::AnalogStick():
//mRate(0,0),
//mpBack(new SPRITE()),
//mpStick(new SPRITE())
//{
//	this->mpBack->load("Data/Image/cursor.png");
//	this->mpStick->load("Data/Image/cursor.png");
//}
//
//AnalogStick::~AnalogStick()
//{
//	if( mpBack ){ delete this->mpBack; this->mpBack = NULL;}
//	if( this->mpStick ){ delete this->mpStick; this->mpStick = NULL; }
//}
//
//void AnalogStick::init(float xPosRate, float yPosRate, float range)
//{
//	IButton::init(xPosRate, yPosRate, range);
//}
//
//void AnalogStick::update()
//{
//	updateIsPush();
//
//	if( this->mIsPush )
//	{
//		Vector2 mpos = getTouchPos();
//		Vector2 dir = mpos - this->mPos;
//		dir /= this->mRange;
//		dir.x = min( 1.f, max(dir.x, -1.f) );
//		dir.y = min( 1.f, max(dir.y, -1.f) );
//		this->mRate = dir;
//		if( this->mRate.lengthSq() > 1.f )
//			this->mRate.normalize();
//	}
//	else
//	{
//		this->mRate.x = 0.f;
//		this->mRate.y = 0.f;
//	}
//}
//
//void AnalogStick::render()
//{
//	{//背景
//		float drawX = this->mPos.x - this->mRange;
//		float drawY = this->mPos.y - this->mRange;
//		unsigned color = (this->mIsPush) ? 0xffffffff : 0x77ffffff;
//		RUL_2D.render(&RUL_2DEFFECT, drawX, drawY, this->mRange*2.f, this->mRange*2.f, this->mpBack, color );
//	}
//	{//スティック
//		Vector2 pos = this->mPos + this->mRate*this->mRange;
//		float drawX = pos.x - this->mRange*0.5f;
//		float drawY = pos.y - this->mRange*0.5f;
//		unsigned color = (this->mIsPush) ? 0xffffffff : 0x77ffffff;
//		RUL_2D.render(&RUL_2DEFFECT, drawX, drawY, this->mRange, this->mRange, this->mpStick, color );
//	}
//}
