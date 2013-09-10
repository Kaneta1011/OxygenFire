#ifndef _INTERFACE_BUTTON_
#define _INTERFACE_BUTTON_

#include "math\kmathf.h"
#include "GraphicsLib\Class\r2DObj\r2DRenderer.h"

namespace rlib
{

	class IButton
	{
	public:
		enum MODE{
			eFREE,
			eDOWN,
			ePUSH,
			eUP,
			eFar,
		};
	public:
		IButton():mIsPush(false),mRange(0.f,0.f){}
		~IButton(){};

	public:
		bool isPush()const{return this->mIsPush;}
		MODE getMode()const{return this->mMode;}

		const klib::math::Vector2& getPos()const{return this->mPos;}
		const klib::math::Vector2& getRange()const{return this->mRange;}

		void setPos(const klib::math::Vector2& pos){ this->mPos = pos; }

		//
		//	サイズの自由設定
		//
		void setRange(const klib::math::Vector2& range){ this->mRange = range; }
		//
		//	自動的に画面上での縦横サイズを一致させる
		//
		void setRange(float range){ this->mRange = rlib::r2DHelper::adjustSize(range, false); }

	protected:
		float adjustPos(float p, float size, float range, float offset=10.f){
			if( p - range < 0 ) p = offset + range;
			else if( p+range > size ) p = size-offset -range;
			return p;
		}

		float adjustRange(float range, float width, float height, float maxRangeRate=0.001f){
			float square = width*height;
			range /= square;
			if( range > maxRangeRate ) range = maxRangeRate;
			range = square*range;
			return range;
		}

		void init(float x, float y, float range);

		klib::math::Vector2	getTouchPos(int i=0);
		void updateIsPush();

		bool isTouch(int i);
		int updateMode();

	protected:
		MODE mMode;
		bool mIsPush;
		klib::math::Vector2	mPos;
		klib::math::Vector2 mRange;//縦横の長さ

	};

}

#endif