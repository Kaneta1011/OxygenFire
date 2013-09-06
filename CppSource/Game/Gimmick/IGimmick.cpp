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

IGimmickObj::IGimmickObj(GGimmickInfo* info):
	IGimmick(info)
{
	mPos = info->pos;
	mAngle = info->angle;
	mRange = info->scale;

//半径を求める
	float max = info->scale.x;
	if( max < info->scale.y ) max = info->scale.y;
	if( max < info->scale.z ) max = info->scale.z;
	this->mRadius = max;
}
