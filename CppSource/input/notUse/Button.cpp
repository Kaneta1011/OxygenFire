#include "../Button.h"

#include "input\Input.h"
#include "GraphicsLib\Class\rTexture\Texture.h"
#include "GraphicsLib\Class\r2DObj\r2DObj.h"
#include "GraphicsLib\Class\tRenderState\RenderState.h"

#ifdef WINDOWS
#include <Graphic\RULGraphic.h>
#include <Graphic\RULWindow.h>
#include <Graphic\RUL2D.h>
#include <Input\RUL_Input.h>
#endif

using  namespace rlib;
using  namespace klib::math;

//============================================
//
//	‰~Œ`‚Ìƒ{ƒ^ƒ“
//
//============================================
CircleButton::CircleButton():
mpImage(NULL)
{
}

CircleButton::~CircleButton()
{
	if( this->mpImage ){ delete this->mpImage; this->mpImage = NULL; }
}

void CircleButton::init(const char* imagePath, float xPosRate, float yPosRate, float range)
{
	IButton::init(xPosRate, yPosRate, range);
	this->mpImage = new r2DObj();
	this->mpImage->load(imagePath);

	this->mpImage->setPos( this->mPos.x, this->mPos.y, 0.f );
	this->mpImage->setSize(this->mRange, this->mRange);
}

void CircleButton::update()
{
	Vector2 mpos = getTouchPos();
	Vector2 dir = mpos - this->mPos;
	if( mlInput::key() == mlInput::DOWN )
	{
		this->mIsPush = ( dir.lengthSq() < this->mRange*this->mRange );
	}
	else if( mlInput::key() == mlInput::UP )
	{
		this->mIsPush = false;
	}
}

void CircleButton::render()
{
	{//”wŒi
		float drawX = this->mPos.x - this->mRange;
		float drawY = this->mPos.y - this->mRange;
		unsigned color = (this->mIsPush) ? 0xffffffff : 0x77ffffff;
		this->mpImage->render();
		//RUL_2D.render(&RUL_2DEFFECT, drawX, drawY, this->mRange*2.f, this->mRange*2.f, this->mpImage, color );
	}
}

//================================================
//
//		IButtonƒNƒ‰ƒX
//
//================================================
void IButton::init(float xPosRate, float yPosRate, float range, float maxRangeRate)
{
	unsigned int w,h;
#ifdef WINDOWS
	RUL_GRAPHIC.getWindow().getSize(&w,&h);
#endif

	range= adjustRange(range, w, h, maxRangeRate);

	float x = w*xPosRate, y=h*yPosRate;
	x = adjustPos(x, w, range);
	y = adjustPos(y, h, range);

	this->mPos.x = x;
	this->mPos.y = y;
	this->mRange = range;

	this->mPos.x = 0.f;
	this->mPos.y = 0.f;
	this->mRange = 0.5f;
}

klib::math::Vector2	IButton::getTouchPos()
{
	int mx, my;
	mx = mlInput::getX();
	my = mlInput::getY();
	return klib::math::Vector2(mx,my);
}

void IButton::updateIsPush()
{
	Vector2 mpos = getTouchPos();
	Vector2 dir = mpos - this->mPos;
	if( mlInput::key() == mlInput::DOWN ){
		this->mIsPush = ( dir.lengthSq() < this->mRange*this->mRange );
	}else if( mlInput::key() == mlInput::UP ){
		this->mIsPush = false;
	}
}
