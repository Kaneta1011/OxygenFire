#include "GGoal.h"

#include "utility\utility.h"

#ifndef ANDROID_REDNER
#include "GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"
#endif

using namespace rlib;

//===============================================================
//
//		GGoalInfoƒNƒ‰ƒX
//
//===============================================================
void GGoalInfo::forFile(textWriter& writer)
{
	GGimmickInfo::forFile(writer);
}

bool GGoalInfo::loadParam(textLoader& loader)
{
	return GGimmickInfo::loadParam(loader);
}

IGimmick* GGoalInfo::makeGimmick()
{
	GGoal* set = new GGoal(*this);
	return set;
}

//===========================================================
//
//	ƒS[ƒ‹
//
//===========================================================
GGoal::GGoal(GGoalInfo& info):
	IGimmickObj(&info)
#ifndef ANDROID_REDNER
	,mpSkin(new klib::kSkin("gimmick/door/door_animation.IEM", new klib::kMeshLoadIEM, new klib::kMeshGLES20Render()))
#endif
{
#ifndef ANDROID_REDNER
	this->mpSkin->setPosition(this->mPos);
	this->mpSkin->setAngle(this->mAngle);
	this->mpSkin->setScale(this->mRange*1.f);
	this->mpSkin->SetMotion(0);
#endif
}

GGoal::~GGoal()
{
#ifndef ANDROID_REDNER
	if( this->mpSkin ){ delete this->mpSkin; this->mpSkin = NULL; }
#endif
}

int GGoal::update()
{
#ifndef ANDROID_REDNER
	this->mpSkin->animation(1.f);
	this->mpSkin->Update();
#endif
	return 0;
}

bool GGoal::vs(Bullet* op)
{
	return IGimmickObj::vs(op);
}

void GGoal::flagOnListener(IGimmick* thiz)
{
	this->mFlag = !this->mFlag;
	int motionNo = ( this->mFlag )? 2:3;
#ifndef ANDROID_REDNER
	this->mpSkin->SetMotion(motionNo);
#endif
	LOGI("GGoal", "flagOn motion=%d", motionNo);
}
void GGoal::flagOffListener(IGimmick* thiz)
{
	this->mFlag = !this->mFlag;
	int motionNo = ( this->mFlag )? 2:3;
#ifndef ANDROID_REDNER
	this->mpSkin->SetMotion(motionNo);
#endif
	LOGI("GGoal", "flagOn motion=%d", motionNo);
}

#ifndef ANDROID_REDNER
void GGoal::render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)
{
	this->mpSkin->Render(pipeline);
}
#endif