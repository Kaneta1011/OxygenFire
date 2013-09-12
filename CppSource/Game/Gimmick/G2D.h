#ifndef _G_2D_
#define _G_2D_

#include "IGimmick.h"

namespace rlib
{
	class r2DObj;

	struct G2DInfo : public GGimmickInfo
	{
		std::string imagePath;

		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual IGimmick* makeGimmick();

		G2DInfo():imagePath(NON_PATH){}
		~G2DInfo(){}

		static std::string NON_PATH;
	};

	//
	//	ろうそくクラス
	//
	//		弾に当たると燃え始める
	//		燃えた瞬間、flagOnイベントを発生させる
	//
	//		対応するGResetCandleクラスからflagOffイベントが来たら火は消える
	//
	class G2D : public IGimmickObj
	{
	public:
		G2D(G2DInfo* info);
		virtual ~G2D();

		virtual int update();
		virtual bool vs(Bullet* op);

		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline);

		r2DObj*	getImage(){return this->mpImage;}
#endif
	protected:
		std::string mImagePath;
#ifndef ANDROID_REDNER
		r2DObj* mpImage;
#endif
	};
}

#endif