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

		int mOxygenCost;		//���������ۂ̎_�f�̌���
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
		~GDrum();

		virtual int update();
		virtual bool vs(Bullet* op);

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline);
#endif
	private:
		int mOxygenCost;
	};

	//===============================================
	//	�K�\����
	//		�e�ŔR����
	//===============================================
	class GGasoline : public IGExplosion
	{
	public:
		GGasoline(GExplosionInfo& info);
		~GGasoline();

		virtual int update();
		virtual bool vs(Bullet* op);

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline);
#endif
	private:
		int mOxygenCost;
	};

	//===============================================
	//	�ؔ�
	//		�e�ŔR����
	//===============================================
	class GWoodBox : public IGExplosion
	{
	public:
		GWoodBox(GExplosionInfo& info);
		~GWoodBox();

		virtual int update();
		virtual bool vs(Bullet* op);	

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline);
#endif
	private:
		int mOxygenCost;

	};
}

#endif