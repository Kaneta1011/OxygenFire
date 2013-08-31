#ifndef _IOBJECT_
#define _IOBJECT_

#include "math\kmathf.h"
#include <cmath>

namespace rlib
{

	class IObject
	{
	public:
		IObject():mRange(0.5f,0.5f,0.5f){}
		virtual ~IObject(){}

		virtual bool vs(IObject* obj){
			return ( fabs(obj->mPos.x-this->mPos.x) <= this->mRange.x+obj->mRange.x &&
					 fabs(obj->mPos.y-this->mPos.y) <= this->mRange.y+obj->mRange.y &&
					 fabs(obj->mPos.z-this->mPos.z) <= this->mRange.z+obj->mRange.z );
		}

	public://ゲッター・セッター
		void setRange(const klib::math::Vector3& range){this->mRange = range;}

		klib::math::Vector3& getPos(){return this->mPos;}
		klib::math::Vector3& getVelocity(){return this->mVelocity;}
		klib::math::Vector3& getRange(){return this->mRange;}

		const klib::math::Vector3& getPos()const{return this->mPos;}
		const klib::math::Vector3& getVelocity()const{return this->mVelocity;}
		const klib::math::Vector3& getRange()const{return this->mRange;}

	protected:
		klib::math::Vector3	mPos;
		klib::math::Vector3 mVelocity;
		klib::math::Vector3 mRange;

	};

}

#endif