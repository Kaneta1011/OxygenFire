#ifndef _FUSE_
#define _FUSE_

#include "IGimmick.h"

namespace rlib
{
	//
	//	���ΐ��N���X
	//		GFusePoint�N���X����̃C�x���g�ɏ����͈ˑ����Ă���
	//		�e�Ƃ͓����蔻�肵�Ȃ�
	//
	class GFuse : public IGimmick
	{
	public:
		GFuse(GLineInfo& info);
		virtual ~GFuse();

		virtual int update();
		/*
		�e�Ƃ͓����蔻�肵�Ȃ�
		*/
		virtual bool vs(Bullet* op);

		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline);
#endif
	private:
		Vector3 mStart, mEnd;

	};
}

#endif
