#ifndef _GDRUM_
#define _GDRUM_

#include "IGimmick.h"

namespace rlib
{
	struct GExplosionInfo : public GGimmickInfo
	{
		int limitTemperature;
		int oxygenCost;

		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual IGimmick* makeGimmick();

		GExplosionInfo():limitTemperature(100),oxygenCost(0){}
	};

	//===============================================
	//
	//	爆発物の基底クラス
	//		各爆発物には現在の温度がある
	//		温度は何もしなければ徐々に下がる
	//		温度は弾が当たったり、近くで爆発が起きた際にあがる
	//
	//		温度が爆発限界温度を越えた場合、少し間を空けてから爆発する
	//		爆発した際、酸素が減る
	//
	//		一度爆発したら、描画しない
	//
	//===============================================
	class IGExplosion : public IGimmickObj
	{
	public:
		static bool isExplosion(GIMMICK_TYPE type);
	
	public:
		IGExplosion(GExplosionInfo* info);

		virtual bool vs(Bullet* op)=0;
		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline);
#endif

	protected:
		enum UPDATE_MESSAGE{
			UPDATE_MSG_NON,
			UPDATE_MSG_OVER_TEMPERATURE,
			UPDATE_MSG_EXPLOSION,
		};
	protected:
		int udateExplosion();
		bool isOverTemperature(){ return this->mLimitTemperature <= this->mTemperature; }

	private:
		bool updateTemperature();
		bool updateExplosionCount();

	protected:
		int mTemperature;		//現在の温度	初期は必ず0
		int mLimitTemperature;	//爆発限界温度 現在の温度がこれを超えると爆発する

		int mExplosionCount;	//爆発までのカウントダウン 0のときに爆発  通常時は-1
		int mExplosionMaxCount;	//爆発するまでの時間

		int mOxygenCost;		//爆発した際の酸素の減り具合
		bool mIsExplosion;		//爆発するか？

#ifndef ANDROID_REDNER
		wp<EffectLib::EmitterSet> wpCatchFire;
		wp<EffectLib::EmitterSet> wpBurning;
#endif
		float mObjBurnInterval;
		float mEffectBurnInterval;
	};

	//===============================================
	//	ドラム
	//		弾では燃えない
	//		他の可燃物(GExplosionInfo系)からの爆発を受けて爆発する
	//===============================================
	class GDrum : public IGExplosion
	{
	public:
		GDrum(GExplosionInfo& info);
		virtual ~GDrum();

		virtual int update();
		virtual bool vs(Bullet* op);

	private:
	};

	//===============================================
	//	ガソリン
	//		弾で燃える
	//===============================================
	class GGasoline : public IGExplosion
	{
	public:
		GGasoline(GExplosionInfo& info);
		virtual ~GGasoline();

		virtual int update();
		virtual bool vs(Bullet* op);

	private:
	};

	//===============================================
	//	木箱
	//		弾で燃える
	//===============================================
	class GWoodBox : public IGExplosion
	{
	public:
		GWoodBox(GExplosionInfo& info);
		virtual ~GWoodBox();

		virtual int update();
		virtual bool vs(Bullet* op);	

	private:

	};

	//===============================================
	//	ダンボール
	//		弾で燃える
	//===============================================
	class GCardBoard : public IGExplosion
	{
	public:
		GCardBoard(GExplosionInfo& info);
		virtual ~GCardBoard();

		virtual int update();
		virtual bool vs(Bullet* op);	

	private:

	};

	//===============================================
	//	ゴミ袋
	//		弾で燃える
	//===============================================
	class GGaberageBox : public IGExplosion
	{
	public:
		GGaberageBox(GExplosionInfo& info);
		virtual ~GGaberageBox();

		virtual int update();
		virtual bool vs(Bullet* op);	

	private:

	};

}

#endif