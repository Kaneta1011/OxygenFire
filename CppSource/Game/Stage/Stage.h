#ifndef _STAGE_
#define _STAGE_

#include "math\kmathf.h"

namespace klib
{
	class kMesh;
}

namespace rlib
{
	//
	//�X�e�[�W�N���X(�V���O���g��)
	//�E�S�[�������Gimmick�ōs���܂�
	//
	class Stage
	{
	public:
		/*
		���ŃM�~�b�N�̏��������s���Ă܂�
		*/
		bool init(const char* modelPath);
		/*
		���ŃM�~�b�N���j�����Ă܂�
		*/
		void clear();

		void update();
		void render();
	//9/6 ���C�s�b�N�����̂킷��Ȃ�
		int	rayPick( klib::math::Vector3* out, const klib::math::Vector3& pos, klib::math::Vector3* vec,float* Dist);
		int	rayPickUD( klib::math::Vector3* out, const klib::math::Vector3& pos, klib::math::Vector3* vec,float* Dist );

		void onClearFlag(){this->mIsClear = true;}
		void offClearFlag(){this->mIsClear = false; }

		bool isClear()const{ return this->mIsClear; }

		klib::math::Vector3 getPlayerStartPos()const{return this->mPlayerStartPos; }
	private:
		bool mIsClear;
		klib::kMesh *mpMesh;
		klib::math::Vector3		mPlayerStartPos;

	public:
		static Stage& getInst(){
			static Stage inst;
			return inst;
		}

	private:
		Stage():mpMesh(0){}
		~Stage(){}

	};

#define STAGE Stage::getInst()
}
#endif