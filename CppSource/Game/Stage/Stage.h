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
		bool init(const char* modelPath);
		void clear();

		void update();
		void render();
	//9/6 ���C�s�b�N�����̂킷��Ȃ�

		void onClearFlag(){this->mIsClear = true;}
		void offClearFlag(){this->mIsClear = false; }

		bool isClear()const{ return this->mIsClear; }

	private:
		bool mIsClear;
		klib::kMesh *mpMesh;

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