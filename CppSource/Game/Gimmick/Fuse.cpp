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
//フラグがONなら燃え尽きる処理を行う
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
}

void GFuse::flagOffListener(IGimmick* thiz)
{
}

#ifndef ANDROID_REDNER
void GFuse::render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)
{
}
#endif
