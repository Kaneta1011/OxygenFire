#ifndef _FUSE_POINT_
#define _FUSE_POINT_

#include "IGimmick.h"

namespace rlib
{
	struct GFusePointInfo : public GGimmickInfo
	{
		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual IGimmick* makeGimmick();

	};

	//===========================================================
	//
	//	���ΐ��̗��[
	//		�v���C���[���߂Â����瓱�ΐ��ɔR���ڂ邩�𕷂����b�Z�[�W��\������
	//		���������ʁA�R���ڂ�Ȃ�Ή����Ă��铱�ΐ���flagOn�C�x���g��ʒm����
	//		��x�R���ڂ�����2�x�ƃ��b�Z�[�W���o���Ȃ��H
	//		����Ƃ��G���[���b�Z�[�W�I�Ȃ��̂������H
	//
	//		�����ĔR���Ȃ�
	//
	//===========================================================
	class GFusePoint : public IGimmickObj
	{
	public:
		GFusePoint(GFusePointInfo& info);
		virtual ~GFusePoint();

		virtual int update();
		virtual bool vs(Bullet* op);

		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline);
#endif
	private:
	};
}

#endif