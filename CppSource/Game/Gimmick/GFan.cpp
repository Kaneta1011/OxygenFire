#include "GFan.h"

#include "utility\textWriter.h"
#include "utility\utility.h"

#ifndef ANDROID_REDNER
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"

#include "GraphicsLib\Class\r2DObj\r2DObj.h"

using namespace EffectLib;

#endif

using namespace rlib;

//=======================================================================
//
//		G2DInfoƒNƒ‰ƒX
//
//=======================================================================
void GFanInfo::forFile(textWriter&  w)
{
	GGimmickInfo::forFile(w);
}

bool GFanInfo::loadParam(textLoader& loader)
{
	if( ! GGimmickInfo::loadParam(loader) )
	{
		return false;
	}
	return true;
}

IGimmick* GFanInfo::makeGimmick()
{
	GFan* set = new GFan(this);
	return set;
}

//=======================================================================
//
//		GFanƒNƒ‰ƒX
//
//=======================================================================
GFan::GFan(GFanInfo* info):
	IGimmickObj(info),
	mRotaDelta(0.f),
	mNowRota(0.f)
{
//Šî–{Žp¨‚ÌÝ’è
	this->mOrientation.identity();
	this->mOrientation.setRZXY(this->mAngle);
	this->mOrientation.setT(this->mPos);
//ƒvƒƒyƒ‰‚Ì‰ñ“]‹ï‡‚ÌÝ’è
	this->mFlag = true;
	this->mRota.identity();
	this->mRota.setRY(0.f);
}

GFan::~GFan()
{
}

int GFan::update()
{
	if( isFlag() ){
		this->mRotaDelta += 0.01f;
		const float MAX_DELTA = 0.3f;
		if( this->mRotaDelta > MAX_DELTA ) this->mRotaDelta = MAX_DELTA;
	}else{
	//off1‚È‚ç‰ñ“]‚ðŽ~‚ß‚é
		this->mRotaDelta -= 0.01f;
		const float MIN_DELTA = 0.f;
		if( this->mRotaDelta < MIN_DELTA ) this->mRotaDelta = MIN_DELTA;
	}
	this->mNowRota -= this->mRotaDelta;

	this->mRota.identity();
	this->mRota.setRZ(this->mNowRota);
	this->mRota *= this->mOrientation;

	return MSG_NON;
}

bool GFan::vs(Bullet* op)
{
	return IGimmickObj::vs(op);
}

void GFan::flagOnListener(IGimmick* thiz)
{
}

void GFan::flagOffListener(IGimmick* thiz)
{
}

#ifndef ANDROID_REDNER
void GFan::render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline)
{
	Vector3 s = scale;
	s.x *= this->mRange.x;
	s.y *= this->mRange.y;
	s.z *= this->mRange.z;
	Matrix S;
	S.identity();
	S.mulS(s);
	mesh->setTransMatrix(S*this->mRota);
	mesh->Render(pipeline);
}
#endif
