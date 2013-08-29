#include "../Button.h"

#include "input\Input.h"
#include "GraphicsLib\Class\rTexture\Texture.h"
#include "GraphicsLib\Class\r2DObj\r2DObj.h"
#include "GraphicsLib\Class\tRenderState\RenderState.h"

using  namespace rlib;
using  namespace klib::math;

//============================================
//
//	円形のボタン
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
	this->mpImage->setSize(this->mRange);
}

void CircleButton::update()
{
	updateMode();
}

void CircleButton::render()
{
	{//背景
		//float drawX = this->mPos.x - this->mRange.x;
		//float drawY = this->mPos.y - this->mRange.y;
		unsigned color = (this->mIsPush) ? 0xffffffff : 0x77ffffff;
		if(this->mIsPush) this->mpImage->setColor(1,1,1,1);
		else			  this->mpImage->setColor(1,1,1,0.5);
		this->mpImage->render();
	}
}

//================================================
//
//		IButtonクラス
//
//================================================
void IButton::init(float x, float y, float range)
{
	this->mPos.x = x;
	this->mPos.y = y;

//画面上のサイズを縦横同じようにする
	this->mRange = rlib::r2DHelper::adjustSize(range, false);
}

klib::math::Vector2	IButton::getTouchPos(int i)
{
	float mx, my;
	mx = mlInput::getX(i);
	my = mlInput::getY(i);
	return klib::math::Vector2(mx,my);
}

void IButton::updateIsPush()
{
	Vector2 mpos = getTouchPos();
	Vector2 dir = mpos - this->mPos;
	if( mlInput::key() == mlInput::DOWN ){
		this->mIsPush = ( dir.lengthSq() < this->mRange.lengthSq() );
	}else if( mlInput::key() == mlInput::UP ){
		this->mIsPush = false;
	}
}

bool IButton::isTouch(int i)
{
	Vector2 mpos = getTouchPos(i);
	Vector2 dir = mpos - this->mPos;

	Vector2 range = this->mRange * 0.5f;//半径を取り出す
	float aSq = range.x*range.x;
	float bSq = range.y*range.y;
	float r = bSq*dir.x*dir.x + aSq*dir.y*dir.y;
	return ( r < aSq*bSq );
}

int IButton::updateMode()
{
	bool isInArea = false;
	int ret = -1;
	int i;
	for( i=0; i<mlInput::getNowTouchCount(); i++ )
	{
		isInArea = isTouch(i);
		if( !isInArea ) continue;
		ret = i;
		break;
	}
	static IButton::MODE m[2][2]={
		{ eFREE, eDOWN },
		{ eFar, ePUSH }
	};
	this->mMode = m[this->mIsPush][isInArea];
	if( this->mMode == eFar && mlInput::getNowTouchCount() - mlInput::getPrevTouchCount() < 0 )
	{
		this->mMode = IButton::eUP;
	}
	this->mIsPush = isInArea;
	return ret;
}
