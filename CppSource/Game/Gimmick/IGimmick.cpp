#include "IGimmick.h"

#include "utility\utility.h"

using namespace rlib;

IGimmick::IGimmick(GimmickInfoBase* info):
mName(info->name),
mFlag(false),		//フラグ
mType(info->type)
{
}

bool IGimmick::vs(Bullet* op)
{
	return IObject::vs(op);
}

//
//	リスナーパターンの関数
//
void IGimmick::flagOn()
{
	if( !this->mFlag )
	{
		this->mFlag = true;
		Iterator it = this->mOnListener.begin();
		while( it != this->mOnListener.end() )
		{
			(*it)->flagOnListener(this);
			it++;
		}
	}
}

void IGimmick::flagOff()
{
	if( this->mFlag )
	{
		this->mFlag = false;
		Iterator it = this->mOffListener.begin();
		while( it != this->mOffListener.end() )
		{
			(*it)->flagOffListener(this);
			it++;
		}
	}
}

bool IGimmick::addOnListener(IGimmick* gimmick)
{
	Iterator it;
	for( it = mOnListener.begin(); it != mOnListener.end(); it++ )
	{
		if( *it == gimmick )
			break;
	}

	if( it == this->mOnListener.end() ){
		this->mOnListener.push_back(gimmick);
		return true;
	}else{
		return false;
	}
}

bool IGimmick::removeOnListener(IGimmick* gimmick)
{
	for( Iterator it = mOnListener.begin(); it != mOnListener.end(); it++ )
	{
		if( *it == gimmick )
		{
			this->mOnListener.erase(it);
			return true;
		}
	}
	return false;
}

bool IGimmick::addOffListener(IGimmick* gimmick)
{
	Iterator it;
	for( it = mOffListener.begin(); it != mOffListener.end(); it++ )
	{
		if( *it == gimmick )
			break;
	}

	if( it == this->mOffListener.end() ){
		this->mOffListener.push_back(gimmick);
		return true;
	}else{
		return false;
	}
}

bool IGimmick::removeOffListener(IGimmick* gimmick)
{
	for( Iterator it = this->mOffListener.begin(); it != this->mOffListener.end(); it++ )
	{
		if( *it == gimmick )
		{
			this->mOffListener.erase(it);
			return true;
		}
	}
	return false;
}

#ifdef ANDROID_REDNER
#include <iostream>
using namespace std;
void IGimmick::debugMessage()
{
	cout << "name = " << this->mName;
	cout << " | type = " << this->mType;
	cout << " | on count=" << this->mOnListener.size();
	cout << " | off count=" << this->mOffListener.size();
	cout << endl;
}
#endif

inline float toRadius(float degree){
	static float DtoR = K_PI / 180.f;
	return degree * DtoR;
}

IGimmickObj::IGimmickObj(GGimmickInfo* info):
	IGimmick(info)
{
	mPos = info->pos;
	mAngle = info->angle;
	mRange = info->scale;
	mAngle.x = toRadius(mAngle.x);
	mAngle.y = toRadius(mAngle.y);
	mAngle.z = toRadius(mAngle.z);
//半径を求める
	float max = info->scale.x;
	if( max < info->scale.y ) max = info->scale.y;
	if( max < info->scale.z ) max = info->scale.z;
	this->mRadius = max;
}

#ifndef ANDROID_REDNER
void IGimmickObj::render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline)
{
	mesh->setPosition(this->mPos);
	mesh->setAngle(this->mAngle);
	Vector3 s = this->mRange;
	s.x *= scale.x;
	s.y *= scale.y;
	s.z *= scale.z;
	mesh->setScale(s);
	mesh->Update();
	mesh->Render(pipeline);
}
#endif
