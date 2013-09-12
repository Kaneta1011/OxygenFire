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
	//	�������̊��N���X
	//		�e�������ɂ͌��݂̉��x������
	//		���x�͉������Ȃ���Ώ��X�ɉ�����
	//		���x�͒e������������A�߂��Ŕ������N�����ۂɂ�����
	//
	//		���x���������E���x���z�����ꍇ�A�����Ԃ��󂯂Ă��甚������
	//		���������ہA�_�f������
	//
	//		��x����������A�`�悵�Ȃ�
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
		int mTemperature;		//���݂̉��x	�����͕K��0
		int mLimitTemperature;	//�������E���x ���݂̉��x������𒴂���Ɣ�������

		int mExplosionCount;	//�����܂ł̃J�E���g�_�E�� 0�̂Ƃ��ɔ���  �ʏ펞��-1
		int mExplosionMaxCount;	//��������܂ł̎���

		int mOxygenCost;		//���������ۂ̎_�f�̌���
		bool mIsExplosion;		//�������邩�H

#ifndef ANDROID_REDNER
		wp<EffectLib::EmitterSet> wpCatchFire;
		wp<EffectLib::EmitterSet> wpBurning;
#endif
		float mObjBurnInterval;
		float mEffectBurnInterval;
	};

	//===============================================
	//	�h����
	//		�e�ł͔R���Ȃ�
	//		���̉R��(GExplosionInfo�n)����̔������󂯂Ĕ�������
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
	//	�K�\����
	//		�e�ŔR����
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
	//	��l��
	//		�e�ŔR����
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
	//	�ؔ�
	//		�e�ŔR����
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
	//	�_���{�[��
	//		�e�ŔR����
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
	//	�S�~��
	//		�e�ŔR����
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