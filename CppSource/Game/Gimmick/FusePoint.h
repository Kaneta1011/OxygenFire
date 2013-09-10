#ifndef _FUSE_POINT_
#define _FUSE_POINT_

#include "IGimmick.h"
#include "Fuse.h"

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
	//		�R���ڂ肽���ꍇ��flagOn()�������Ă���getFuse()���Ăяo���Ă�������
	//		enableBurning()��1�x�ʂ������ΐ����𔻒f�ł��܂�(true�ŉ\)
	//
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

		void setFuse(GFuse* fuse){ this->mParent = fuse; }

		//
		//�E�Ή����Ă���GFuse��Ԃ�
		//�E�g���O�ɕK���AflagOn()���Ăяo���Ă�������
		//
		GFuse*	getFuse(){ return this->mParent; }

		/*
		�R���ڂ邱�Ƃ��\���H
		*/
		bool enableBurning()const{ return !this->mParent->isBurnOut(); }

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline);
#endif
	private:
		GFuse* mParent;
	};
}

#endif