#include "Fuse.h"

using namespace rlib;

GFuse::GFuse(GLineInfo& info):
IGimmick(&info)
{
	this->mStart = info.start;
	this->mEnd = info.end;
}

GFuse::~GFuse()
{
}

int GFuse::update()
{
//�t���O��ON�Ȃ�R���s���鏈�����s��
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
}

void GFuse::flagOffListener(IGimmick* thiz)
{
}

#ifndef ANDROID_REDNER
void GFuse::render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)
{
}
#endif
