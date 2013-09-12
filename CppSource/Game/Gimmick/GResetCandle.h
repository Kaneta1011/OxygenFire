#ifndef _G_RESET_CANDLE_
#define _G_RESET_CANDLE_

#include "IGimmick.h"

namespace rlib
{
	struct GResetCandleInfo : public GGimmickInfo
	{
		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual IGimmick* makeGimmick();

	};

	//
	//	���Z�b�g�낤�����N���X
	//
	//		�e�ɓ������flagOn�C�x���g�𔭐������A�����̊ԔR���n�߂�
	//		�΂��������u��flagOff�C�x���g�𔭐������A�Ή����Ă���GCandle�N���X�ɒʒm����
	//		����Ɠ������A�Ή�����낤�����̉΂����������L������GCandleChecker�N���X�ɂ�flagOff�C�x���g���΂�
	//
	class GResetCandle : public IGimmickObj
	{
	public:
		GResetCandle(GResetCandleInfo* info);
		virtual ~GResetCandle();

		virtual int update();
		virtual bool vs(Bullet* op);

		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline);
#endif
	protected:
		int mCount;
#ifndef ANDROID_REDNER
		wp<EffectLib::EmitterSet>	wpFire;
#endif
	};

}

#endif