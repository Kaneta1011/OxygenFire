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
	//	”š”­•¨‚ÌŠî’êƒNƒ‰ƒX
	//		Še”š”­•¨‚É‚ÍŒ»İ‚Ì‰·“x‚ª‚ ‚é
	//		‰·“x‚Í‰½‚à‚µ‚È‚¯‚ê‚Î™X‚É‰º‚ª‚é
	//		‰·“x‚Í’e‚ª“–‚½‚Á‚½‚èA‹ß‚­‚Å”š”­‚ª‹N‚«‚½Û‚É‚ ‚ª‚é
	//
	//		‰·“x‚ª”š”­ŒÀŠE‰·“x‚ğ‰z‚¦‚½ê‡A­‚µŠÔ‚ğ‹ó‚¯‚Ä‚©‚ç”š”­‚·‚é
	//		”š”­‚µ‚½ÛA_‘f‚ªŒ¸‚é
	//
	//		ˆê“x”š”­‚µ‚½‚çA•`‰æ‚µ‚È‚¢
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
		int mTemperature;		//Œ»İ‚Ì‰·“x	‰Šú‚Í•K‚¸0
		int mLimitTemperature;	//”š”­ŒÀŠE‰·“x Œ»İ‚Ì‰·“x‚ª‚±‚ê‚ğ’´‚¦‚é‚Æ”š”­‚·‚é

		int mExplosionCount;	//”š”­‚Ü‚Å‚ÌƒJƒEƒ“ƒgƒ_ƒEƒ“ 0‚Ì‚Æ‚«‚É”š”­  ’Êí‚Í-1
		int mExplosionMaxCount;	//”š”­‚·‚é‚Ü‚Å‚ÌŠÔ

		int mOxygenCost;		//”š”­‚µ‚½Û‚Ì_‘f‚ÌŒ¸‚è‹ï‡
		bool mIsExplosion;		//”š”­‚·‚é‚©H

#ifndef ANDROID_REDNER
		wp<EffectLib::EmitterSet> wpCatchFire;
		wp<EffectLib::EmitterSet> wpBurning;
#endif
		float mObjBurnInterval;
		float mEffectBurnInterval;
	};

	//===============================================
	//	ƒhƒ‰ƒ€
	//		’e‚Å‚Í”R‚¦‚È‚¢
	//		‘¼‚Ì‰Â”R•¨(GExplosionInfoŒn)‚©‚ç‚Ì”š”­‚ğó‚¯‚Ä”š”­‚·‚é
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
	//	ƒKƒ\ƒŠƒ“
	//		’e‚Å”R‚¦‚é
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
	//	ˆê“lŠÊ
	//		’e‚Å”R‚¦‚é
	//===============================================
	class GIttokan : public IGExplosion
	{
	public:
		GIttokan(GExplosionInfo& info);
		virtual ~GIttokan();

		virtual int update();
		virtual bool vs(Bullet* op);

	private:
	};

	//===============================================
	//	–Ø” 
	//		’e‚Å”R‚¦‚é
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
	//	ƒ_ƒ“ƒ{[ƒ‹
	//		’e‚Å”R‚¦‚é
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
	//	ƒSƒ~‘Ü
	//		’e‚Å”R‚¦‚é
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