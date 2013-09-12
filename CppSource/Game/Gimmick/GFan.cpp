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
//		G2DInfoクラス
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
//		GFanクラス
//
//=======================================================================
GFan::GFan(GFanInfo* info):
	IGimmickObj(info),
	mRotaDelta(0.f),
	mNowRota(0.f)
{
//基本姿勢の設定
	this->mOrientation.identity();
	this->mOrientation.setRZXY(this->mAngle);
	this->mOrientation.setT(this->mPos);
//プロペラの回転具合の設定
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
		const float MAX_DELTA = 0.2f;
		if( this->mRotaDelta > MAX_DELTA ) this->mRotaDelta = MAX_DELTA;

		this->mNowRota += this->mRotaDelta;

		this->mRota.identity();
		this->mRota.setRY(this->mNowRota);
		//this->mRota = this->mOrientation * this->mRota;
	}

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
	mesh->setTransMatrix(this->mRota);
	mesh->Render(pipeline);
	//IGimmickObj::render(mesh, scale, pipeline);
	//Matrix MatScale;
	//MatScale.identity();
	//Vector3 s = scale;
	//s.x *= this->mRange.x;
	//s.y *= this->mRange.y;
	//s.z *= this->mRange.z;
	//MatScale.mulS(s);
	////mesh->setTransMatrix( MatScale * this->mRota );
	//mesh->setTransMatrix( mOrientation );
	//mesh->Render(pipeline);
}
#endif
