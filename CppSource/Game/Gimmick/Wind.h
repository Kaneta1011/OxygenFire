#ifndef _WIND_
#define _WIND_

#include "IGimmick.h"
#ifndef ANDROID_REDNER
#include "EffectLib\Effect.h"
#endif

namespace rlib
{

	struct GWindInfo : public GimmickInfoBase
	{
		klib::math::Vector3 pos;
		klib::math::Vector3 dir;
		float				power;
		klib::math::Vector3 scale;
		bool				isRender;

		void convert(WindData* data, int index);
		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);
		
		virtual IGimmick* makeGimmick();

		GWindInfo():isRender(true){}
	};
	//
	//	ステージ内の風クラス
	//		基本的にAABBの領域内にいたら風を吹かせる
	//		弾が範囲内に入ったら、風を弱める
	//
	class GWind : public IGimmick
	{
	public:
		static bool isWindType(GIMMICK_TYPE type);
	public:
		GWind(GWindInfo& info);
		virtual ~GWind();

		virtual int update();
		virtual bool vs(Bullet* op);

		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline);
#endif

	protected:
		Vector3 getEmitterPos();

	protected:
		bool mIsRender;
		float mRate;
		float mMaxPower;
		Vector3	mDirection;
#ifndef ANDROID_REDNER
		wp<EffectLib::EmitterSet> mEmitter;
#endif
	};
}

#endif