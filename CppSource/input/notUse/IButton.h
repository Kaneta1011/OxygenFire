#ifndef _INTERFACE_BUTTON_
#define _INTERFACE_BUTTON_

#include "math\kmathf.h"
#include "GraphicsLib\Class\r2DObj\r2DRenderer.h"

namespace rlib
{

	class IButton
	{
	public:
		IButton():mIsPush(false),mRange(0.f){}
		~IButton(){};

	public:
		bool isPush()const{return this->mIsPush;}

		const klib::math::Vector2& getPos()const{return this->mPos;}
		float getRange()const{return this->mRange;}

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

		void init(float xPosRate, float yPosRate, float range, float maxRangeRate=0.001f);

		klib::math::Vector2	getTouchPos();
		void updateIsPush();

	protected:
		bool mIsPush;
		klib::math::Vector2	mPos;
		float mRange;

	};

}

#endif