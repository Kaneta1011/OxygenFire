#ifndef _FUSE_
#define _FUSE_

#include "IGimmick.h"

namespace rlib
{
	struct GLineInfo : public GimmickInfoBase
	{
		klib::math::Vector3 start;
		klib::math::Vector3 end;
		float speed;

		void convert(GimmickLine* data, int index);
		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual IGimmick* makeGimmick();

		GLineInfo():speed(1.f){}
	};

	//
	//	���ΐ��N���X
	//		GFusePoint�N���X����̃C�x���g�ɏ����͈ˑ����Ă���
	//		���ΐ����ړ����Ă���Ƃ��A���݂ǂ��ɂ��邩��m�肽���ꍇ��getPos()���g���Ă�������
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

		/*
		�E�n�_��Ԃ�
		�E�n�_�̈ʒu�����炩���ߏ������ȂǂŊo���Ă������Ƃ͂��Ȃ��ł�������
		�E�n�_�̈ʒu�͑Ή����Ă���GFusePoint�̏����̌��ʂɈˑ����Ă��܂�
		�E����͏I�_�������ł�
		*/
		const Vector3& getStart()const{return this->mStart; }
		/*
		�E�I�_��Ԃ�
		�E�I�_�̈ʒu�����炩���ߏ������ȂǂŊo���Ă������Ƃ͂��Ȃ��ł�������
		�E�I�_�̈ʒu�͑Ή����Ă���GFusePoint�̏����̌��ʂɈˑ����Ă��܂�
		�E����͎n�_�������ł�
		*/
		const Vector3& getEnd()const{ return this->mEnd; }
		/*
		�E�R���s�������H
		*/
		bool isBurnOut()const{return this->mIsBurnOut; }

		void setPos(Vector3& pos){ this->mPos = pos; }

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline);
#endif
	private:
		bool	mIsBurnOut;		//�R���s�������H
		Vector3 mStart, mEnd;
		float	mSpeed;
	};
}

#endif
