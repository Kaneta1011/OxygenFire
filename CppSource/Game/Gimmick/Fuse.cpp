#include "Fuse.h"

#include "FusePoint.h"
#include "utility\textWriter.h"

#include "utility\utility.h"

using namespace rlib;

//===============================================================
//
//		GLineInfoクラス
//
//===============================================================
void GLineInfo::convert(GimmickLine* data, int index)
{
//名前の設定
	//cout << "GLineInfoで名前設定の処理作ってね" << endl;
	this->name = data->spStr[index];
//
	this->type = eGIMMICK_FUSE;
	this->start = data->spStart[index];
	this->end = data->spEnd[index];
}

void GLineInfo::forFile(textWriter& w)
{
	GimmickInfoBase::forFile(w);

	w.nestIn();w.writeNest();
	w.write("start\t");
	writeVec3(start, w);
	w.br().write("end\t");
	writeVec3(end, w);
	w.br().write("speed\t");
	w.write(this->speed);
	w.nestOut();
	w.br();
}

bool GLineInfo::loadParam(textLoader& loader)
{
	if( ! GimmickInfoBase::loadParam(loader) )
	{
		if( strcmp("start",loader.tmpBuf) == 0 ){
			this->start.x = loader.LoadFloat();
			this->start.y = loader.LoadFloat();
			this->start.z = loader.LoadFloat();
			return true;
		}else if( strcmp("end",loader.tmpBuf) == 0 ){
			this->end.x = loader.LoadFloat();
			this->end.y = loader.LoadFloat();
			this->end.z = loader.LoadFloat();
			return true;
		}else if( strcmp("speed",loader.tmpBuf) == 0 ){
			this->speed = loader.LoadFloat();
			return true;
		}
		return false;
	}
	return true;
}

IGimmick* GLineInfo::makeGimmick()
{
	//LOGE(TAG, "GLineInfo::makeGimmick() 作ってね\n");
	GFuse* set = new GFuse(*this);
	return set;
}

//============================================================================
//
//		導火線のクラス定義
//
//============================================================================
GFuse::GFuse(GLineInfo& info):
IGimmick(&info),
mIsBurnOut(false)
{
	this->mStart = info.start;
	this->mEnd = info.end;
	this->mSpeed = info.speed;
}

GFuse::~GFuse()
{
}

int GFuse::update()
{
//フラグがONなら燃え尽きる処理を行う
	if( this->mFlag && this->mIsBurnOut )
	{
	//燃え尽き判定
		Vector3 dir = this->mEnd-this->mStart;
		float unitLen = dir.length();
		float nowLen = (this->mPos-this->mStart).length();
		if( unitLen < nowLen ){
			this->mIsBurnOut = true;
		}else{
		//燃え尽きてないときの処理
			dir.normalize();
			this->mPos += dir * this->mSpeed;
		}
	}
	return 0;
}

bool GFuse::vs(Bullet* op)
{
	return false;
}

void GFuse::flagOnListener(IGimmick* thiz)
{
//GFusePointクラスからイベントが来たら燃え尽き開始
//ただし、フラグがすでにONなら何もしない
	if( thiz->getType() ){
		GFusePoint* point = (GFusePoint*)thiz;
		if( !this->mFlag ){
			//イベントを送ったGFusePointを始点になるようにする
			if( (thiz->getPos()-this->mStart).length() > 0.1f ){
				Vector3 swapValue = this->mStart;
				this->mStart = this->mEnd;
				this->mEnd = swapValue;
			}
			this->mIsBurnOut = true;
			this->mFlag = true;
			LOGI("GFuse","start burning!!");
		}else{
			LOGE("GFuse","this still burn out...");
		}
	}
}

void GFuse::flagOffListener(IGimmick* thiz)
{
	LOGE("GFuse","not create flagOffLisener... | thiz name=\"%s\"", thiz->getName().c_str());
}

#ifndef ANDROID_REDNER
void GFuse::render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)
{
}
#endif
