#ifndef _G_FAN_
#define _G_FAN_

#include "IGimmick.h"

namespace rlib
{
	struct GFanInfo : public GGimmickInfo
	{

		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual IGimmick* makeGimmick();

	};

	//
	//	êÓïóã@ÉNÉâÉX
	//
	//		
	//
	class GFan : public IGimmickObj
	{
	public:
		GFan(GFanInfo* info);
		virtual ~GFan();

		virtual int update();
		virtual bool vs(Bullet* op);

		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline);
#endif

	protected:
		klib::math::Matrix mOrientation;
		klib::math::Matrix mRota;
		float mRotaDelta;
		float mNowRota;
	protected:
	};
}

#endif