#ifndef _WIND_
#define _WIND_

#include "IGimmick.h"

namespace rlib
{
	//
	//	�X�e�[�W���̕��N���X
	//		��{�I��AABB�̗̈���ɂ����畗�𐁂�����
	//		�e���͈͓��ɓ�������A������߂�
	//
	class GWind : public IGimmick
	{
	public:
		static bool isWindType(GIMMICK_TYPE type);
	public:
		GWind(GWindInfo& info);
		~GWind();

		virtual int update();
		virtual bool vs(Bullet* op);

		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline);
#endif

	protected:
	};
}

#endif