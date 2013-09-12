#ifndef _G_CNADLE_CHECKER_
#define _G_CNADLE_CHECKER_

#include "IGimmick.h"

namespace rlib
{
	struct GCandleCheckerInfo : public GimmickInfoBase
	{
		std::string correctAnswer;	//�����̏���

		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual IGimmick* makeGimmick();

		GCandleCheckerInfo():correctAnswer(""){}
		virtual ~GCandleCheckerInfo(){}

		static std::string NON_DATA;
	};

	//
	//	�낤�����`�F�b�J�[�N���X
	//
	//	�����������ɂ낤�����̉΂�����������N���X
	//	���������Ȃ�flagOn�C�x���g�𔭐�������
	//
	//	GCandle�N���X����flagOn�C�x���g�������Ă�����A�����̏������`�F�b�N����
	//
	class GCandleChecker : public IGimmick
	{
	public:
		GCandleChecker(GCandleCheckerInfo* info);
		virtual ~GCandleChecker();

		virtual int update();
		virtual bool vs(Bullet* op);

		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

#ifndef ANDROID_REDNER
		void render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline);
#endif

	protected:
		int mNowCheckIndex;			//�ԈႦ����mCorrectAnswer�̕������ȏ�̒l�ɂȂ�
		std::string mCorrectAnswer;
	};
}

#endif