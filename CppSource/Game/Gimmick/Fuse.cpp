#include "Fuse.h"

#include "FusePoint.h"
#include "utility\textWriter.h"

#include "utility\utility.h"

using namespace rlib;

//===============================================================
//
//		GLineInfo�N���X
//
//===============================================================
void GLineInfo::convert(GimmickLine* data, int index)
{
//���O�̐ݒ�
	//cout << "GLineInfo�Ŗ��O�ݒ�̏�������Ă�" << endl;
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
	//LOGE(TAG, "GLineInfo::makeGimmick() ����Ă�\n");
	GFuse* set = new GFuse(*this);
	return set;
}

//============================================================================
//
//		���ΐ��̃N���X��`
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
//�t���O��ON�Ȃ�R���s���鏈�����s��
	if( this->mFlag && this->mIsBurnOut )
	{
	//�R���s������
		Vector3 dir = this->mEnd-this->mStart;
		float unitLen = dir.length();
		float nowLen = (this->mPos-this->mStart).length();
		if( unitLen < nowLen ){
			this->mIsBurnOut = true;
		}else{
		//�R���s���ĂȂ��Ƃ��̏���
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
//GFusePoint�N���X����C�x���g��������R���s���J�n
//�������A�t���O�����ł�ON�Ȃ牽�����Ȃ�
	if( thiz->getType() ){
		GFusePoint* point = (GFusePoint*)thiz;
		if( !this->mFlag ){
			//�C�x���g�𑗂���GFusePoint���n�_�ɂȂ�悤�ɂ���
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
