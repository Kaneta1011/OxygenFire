#ifndef _GIMMICK_
#define _GIMMICK_

#include "Game/IObject/IObject.h"
#include "utility\utility.h"
#include "utility\SinglyLinkedList.h"
#include "StandardLib\SmartPointer.h"
#include <list>
#include <vector>

#include "GimmickInfo.h"
#include "IGimmick.h"

#ifndef ANDROID_REDNER
#include "GraphicsLib\Class\kMesh\kMesh.h"
#endif

namespace rlib
{
	//===============================================================================
	//
	//		GimmickManager�N���X
	//		�E�V���O���g��
	//
	//===============================================================================
	class GimmickManager
	{
	public:
	//�M�~�b�N�̎�ނ𔻕ʂ��邽�߂̊֐�
		//��������?(�h�����ʁA�ؔ��A�_���{�[����)
		static bool isExplosion(GIMMICK_TYPE type);
		//����?
		static bool isWindType(GIMMICK_TYPE type);
		//���ΐ����H
		static bool isFuse(GIMMICK_TYPE type);
		//���ΐ��̗��[���H�v���C���[���߂Â����烁�b�Z�[�W���o���܂�
		static bool isFusePoint(GIMMICK_TYPE type);
		//���b�Z�[�W���o���^�C�v���H
		static bool isShowMessegeType(GIMMICK_TYPE type);
		//�Ԃ���^�C�v���H
		static bool isHitGimmick(GIMMICK_TYPE type);
		//�S�[���M�~�b�N���H
		static bool isGoalGimmick(GIMMICK_TYPE type);

	public:
		static GimmickManager& getInst(){
			static GimmickManager inst;
			return inst;
		}

	public:
		typedef std::vector<IGimmick*> ListType;
		typedef ListType::iterator Iterator;
		typedef ListType::const_iterator ConstIterator;

	public:
		enum MESSAGE
		{
			MSG_NON,
		};

	public:
		void init(const char* giFilePath);
		void clear();
		void clearData();

		int update();

		//
		//�Epos����range�͈̔͂ɂ���M�~�b�N�����X�g�ɂ��ĕԂ��܂�
		//�E���b�Z�[�W��\������M�~�b�N�������ق����ꍇ�́ugetNearShowMessageGimmick()�v�𗘗p���Ă�������
		//�E���X�g�ɂ���M�~�b�N�̎�ނ��m�肽���ꍇ�́uIGimmick::getType()�v���g�p���Ă�������
		//�E�uIGimmick::getType()�v���Ԃ��l�́uGimmickInfo.h�v�ɂ���܂��̂ł�������Q�Ƃ��Ă�������
		//
		std::list<IGimmick*> getNearGimmick(const klib::math::Vector3& pos, float range);
		//
		//�Epos����range�͈̔͂ɂ���M�~�b�N���A���b�Z�[�W��\������M�~�b�N��Ԃ��܂�
		//�E��ފ֌W�Ȃ���苗���ɂ���M�~�b�N���ق����ꍇ�́ugetNearGimmick()�v�𗘗p���Ă�������
		//�E���X�g�ɂ���M�~�b�N�̎�ނ��m�肽���ꍇ�́uIGimmick::getType()�v���g�p���Ă�������
		//�E�uIGimmick::getType()�v���Ԃ��l�́uGimmickInfo.h�v�ɂ���܂��̂ł�������Q�Ƃ��Ă�������
		//
		std::list<IGimmick*> getNearShowMessageGimmick(const klib::math::Vector3& pos, float range);

		//
		//�E�M�~�b�N�Ƃ̓����蔻����s��
		//�E��VS�����̗p���Ă���
		//�E�߂�l��y�����ɂ�pos��y�������K������	
		//
		klib::math::Vector3 collision(const klib::math::Vector3& pos, float range);

		//
		//	�Epos���󂯂镗�̗͂�Ԃ�
		//
		klib::math::Vector3 calWindPower(const klib::math::Vector3& pos, float range);

		//
		//	�S�[���������H
		//	�E�S�[������̓M�~�b�N�ōs��
		//
		bool isGoal(const klib::math::Vector3& pos);

#ifndef ANDROID_REDNER
		void render();
#endif
	public:
		Iterator begin(){return this->mData.begin();}
		ConstIterator begin()const{return this->mData.begin();}

		Iterator end(){ return this->mData.end(); }
		ConstIterator end()const{return this->mData.end(); }

		unsigned int size(){return this->mData.size();}

	private:
		void remove(unsigned int index);

	private:
		enum MESH_TYPE
		{
			eMESH_DRUM,
			eMESH_GASOLINE,
			eMESH_WOOD_BOX,
			eMESH_GABERAGE_BOX,
			eMESH_CARD_BOARD,
			eMESH_RESET_CANDLE,
			eMESH_TYPE_NUM
		};

	private:
#ifndef ANDROID_REDNER
		void loadMeshes();
		klib::kMesh* getMesh( int type, float* outUnitScale );
#endif
	private:
		ListType mData;

#ifndef ANDROID_REDNER
		sp<klib::kMesh*> mpMeshies;
#endif
	private:
		GimmickManager();
		~GimmickManager();

	};

	#define GIMMICK_MNG rlib::GimmickManager::getInst()
}
#endif