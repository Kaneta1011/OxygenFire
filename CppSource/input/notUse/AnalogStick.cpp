#include "../AnalogStick.h"

#include <input\Input.h>

//#define WINDOWS
#ifdef WINDOWS
#include <Graphic\RULGraphic.h>
#include <Graphic\RULWindow.h>
#include <Graphic\RUL2D.h>
#include <Input\RUL_Input.h>
#endif

#include <stdio.h>

#ifndef NULL
#define NULL 0
#endif

using namespace klib::math;
using namespace rlib;

AnalogStick::AnalogStick():
mRate(0,0),
mpBack(new r2DObj()),
mpStick(new r2DObj())
{
}

AnalogStick::~AnalogStick()
{
	if( mpBack ){ delete this->mpBack; this->mpBack = NULL;}
	if( this->mpStick ){ delete this->mpStick; this->mpStick = NULL; }
}

void AnalogStick::init(float xPosRate, float yPosRate, float range)
{
	IButton::init(xPosRate, yPosRate, range);

	this->mpBack->load("cursor.png");
	this->mpStick->load("testImage.png");

	this->mpBack->setPos( this->mPos.x, this->mPos.y, 0.f );
	this->mpBack->setSize(this->mRange);

	this->mpStick->setSize(this->mRange*0.5f);
}

inline float min(float a, float b)
{
	return (a>b)?b:a;
}

inline float max(float a, float b)
{
	return (a>b)?a:b;
}

void AnalogStick::update()
{
	updateMode();
	if( this->mMode == eDOWN )
	{
		mEnable = true;
		this->mStartTouchCount = mlInput::getNowTouchCount();
	}

	if( mEnable )
	{
		//一番近いタッチ箇所を調べる
		float near = 1000000.f;
		int index = -1;
		for( int i=0; i<mlInput::getNowTouchCount(); i++ )
		{
			Vector2 touch = getTouchPos(i);
			float lenSQ = (touch-this->mPos).lengthSq();
			if( lenSQ < near ){
				near = lenSQ;
				index = i;
			}
		}

		if( index == -1 || this->mStartTouchCount > mlInput::getNowTouchCount() )
		{//離した
			this->mRate.x = 0.f;
			this->mRate.y = 0.f;
			this->mStartTouchCount = -1;
			mEnable = false;
		}
		else
		{//押しつつけているとき
			Vector2 mpos = getTouchPos(index);
			Vector2 dir = mpos - this->mPos;
			dir.x/=this->mRange.x*0.5f;
			dir.y/=this->mRange.y*0.5f;
			dir.x = min( max( dir.x, -1), 1);
			dir.y = min( max( dir.y, -1), 1);

			this->mRate = dir;
			if( this->mRate.lengthSq() > 1.f )
			{
				this->mRate.normalize();
			}
		}
	}
}

void AnalogStick::render()
{
	{//背景
		if(mEnable)		this->mpBack->setColor(1,1,1,1);
		else			this->mpBack->setColor(1,1,1,0.5);

		this->mpBack->render();
	}
	{//スティック
		Vector2 pos = this->mRate;
		pos.x *= this->mRange.x*0.5f;
		pos.y *= this->mRange.y*0.5f;
		pos += this->mPos;

		//pos = this->mPos;

		float drawX = pos.x;
		float drawY = pos.y;

		this->mpStick->setPos(drawX, drawY);
		if(mEnable) this->mpStick->setColor(1,1,1,1);
		else			  this->mpStick->setColor(1,1,1,0.5);

		this->mpStick->render();
	}
}
