#ifndef _IGIMMICK_
#define _IGIMMICK_

#include "Game/IObject/IObject.h"
#include <list>
#include <vector>

#include "GimmickInfo.h"
#include "Game\Bullet\Bullet.h"

#ifndef ANDROID_REDNER
#include "GraphicsLib\Class\kMesh\kMesh.h"
#endif

namespace rlib
{
	class IGimmick;

	class IGimmickListener
	{
	public:
		virtual void flagOnListener(IGimmick* thiz)=0;
		virtual void flagOffListener(IGimmick* thiz)=0;
	};

	class IGimmick : public IObject, public IGimmickListener
	{
	public:
		enum MESSAGE{
			MSG_NON,
			MSG_DEAD,
		};
	public:
		IGimmick(GimmickInfoBase* info);
		virtual ~IGimmick(){}

		virtual int update()=0;

		/*
		AABB�̓����蔻����s�������Ƃ���IGimmick::vs�֐����g���Ă�������
		���̌�̏����͔h�����Ō��߂Ă�������
		*/
		virtual bool vs(Bullet* op)=0;

		//�����蔻��̂��߂̔��a�擾
		virtual float getRadius(){return this->mRange.x;}

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)=0;
#else
		void debugMessage();
#endif
	//�t���O��ON�EOFF����֐�
		void flagOn();
		void flagOff();

	//���X�i�[�̓o�^�E�폜
		bool addOnListener(IGimmick* gimmick);
		bool addOffListener(IGimmick* gimmick);
		bool removeOnListener(IGimmick* gimmick);
		bool removeOffListener(IGimmick* gimmick);

	//�Q�b�^�[
		const std::string&	getName()const{return this->mName;}
		bool				isFlag() const{return this->mFlag;}
		GIMMICK_TYPE		getType()const{return this->mType;}

	protected:
		std::string			mName;
		bool				mFlag;			//�t���O
		GIMMICK_TYPE		mType;

	private:
		typedef std::vector<IGimmick*>				ListenerContenier;
		typedef ListenerContenier::iterator			Iterator;
		typedef ListenerContenier::const_iterator	ConstIterator;

	private:
		ListenerContenier	mOnListener;
		ListenerContenier	mOffListener;
	};

	//
	//	�����蔻������M�~�b�N�̊��N���X
	//
	class IGimmickObj : public IGimmick
	{
	public:
		IGimmickObj(GGimmickInfo* info);
		virtual ~IGimmickObj(){}

		//�����蔻��̂��߂̔��a�擾
		virtual float getRadius(){return this->mRadius;}
	protected:
		float mRadius;
	};
}
#endif