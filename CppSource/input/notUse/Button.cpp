//#include "../Button.h"
//
//
//#define WINDOWS
//#ifdef WINDOWS
//#include <Graphic\RULGraphic.h>
//#include <Graphic\RULWindow.h>
//#include <Graphic\RUL2D.h>
//#include <Input\RUL_Input.h>
//#endif
//
////============================================
////
////	‰~Œ`‚Ìƒ{ƒ^ƒ“
////
////============================================
//CircleButton::CircleButton():
//mpImage(NULL)
//{
//}
//
//CircleButton::~CircleButton()
//{
//	if( this->mpImage ){ delete this->mpImage; this->mpImage = NULL; }
//}
//
//void CircleButton::init(const char* imagePath, float xPosRate, float yPosRate, float range)
//{
//	IButton::init(xPosRate, yPosRate, range);
//	this->mpImage = new SPRITE();
//	this->mpImage->load(imagePath);
//}
//
//void CircleButton::update()
//{
//	Vector2 mpos = getTouchPos();
//	Vector2 dir = mpos - this->mPos;
//	if( RULInput::getKey(MKEY_LEFT) == RUL_KEY_DOWN )
//	{
//		this->mIsPush = ( dir.lengthSq() < this->mRange*this->mRange );
//	}
//	else if( RULInput::getKey(MKEY_LEFT) == RUL_KEY_UP )
//	{
//		this->mIsPush = false;
//	}//–¼
//}
//
//void CircleButton::render()
//{
//	{//”wŒi
//		float drawX = this->mPos.x - this->mRange;
//		float drawY = this->mPos.y - this->mRange;
//		unsigned color = (this->mIsPush) ? 0xffffffff : 0x77ffffff;
//		RUL_2D.render(&RUL_2DEFFECT, drawX, drawY, this->mRange*2.f, this->mRange*2.f, this->mpImage, color );
//	}
//}
//
////================================================
////
////		IButtonƒNƒ‰ƒX
////
////================================================
//void IButton::init(float xPosRate, float yPosRate, float range, float maxRangeRate)
//{
//	unsigned int w,h;
//#ifdef WINDOWS
//	RUL_GRAPHIC.getWindow().getSize(&w,&h);
//#endif
//
//	range= adjustRange(range, w, h, maxRangeRate);
//
//	float x = w*xPosRate, y=h*yPosRate;
//	x = adjustPos(x, w, range);
//	y = adjustPos(y, h, range);
//
//	this->mPos.x = x;
//	this->mPos.y = y;
//	this->mRange = range;
//}
//
//Vector2	IButton::getTouchPos()
//{
//	int mx, my;
//	RULInput::getCursorPosi(&mx, &my);
//	return RULMath::Vector2(mx,my);
//}
//
//void IButton::updateIsPush()
//{
//	Vector2 mpos = getTouchPos();
//	Vector2 dir = mpos - this->mPos;
//	if( RULInput::getKey(MKEY_LEFT) == RUL_KEY_DOWN ){
//		this->mIsPush = ( dir.lengthSq() < this->mRange*this->mRange );
//	}else if( RULInput::getKey(MKEY_LEFT) == RUL_KEY_UP ){
//		this->mIsPush = false;
//	}
//}
