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
	IGimmickObj(&info),
	mpSkin(new klib::kSkin("gimmick/door/door_animation.IEM", new klib::kMeshLoadIEM, new klib::kMeshGLES20Render()))
{
	this->mpSkin->setPosition(this->mPos);
	this->mpSkin->setAngle(this->mAngle);
	this->mpSkin->setScale(this->mRange*1.f);
	this->mpSkin->SetMotion(0);
}

GGoal::~GGoal()
{
	if( this->mpSkin ){ delete this->mpSkin; this->mpSkin = NULL; }
}

int GGoal::update()
{
	this->mpSkin->animation(1.f);
	this->mpSkin->Update();
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
	this->mpSkin->SetMotion(motionNo);
	LOGI("GGoal", "flagOn motion=%d", motionNo);
}
void GGoal::flagOffListener(IGimmick* thiz)
{
	this->mFlag = !this->mFlag;
	int motionNo = ( this->mFlag )? 2:3;
	this->mpSkin->SetMotion(motionNo);
	LOGI("GGoal", "flagOn motion=%d", motionNo);
}

#ifndef ANDROID_REDNER
void GGoal::render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)
{
	this->mpSkin->Render(pipeline);
}
#endif