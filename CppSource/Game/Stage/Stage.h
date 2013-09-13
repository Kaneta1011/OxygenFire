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
	//ステージクラス(シングルトン)
	//・ゴール判定はGimmickで行います
	//
	class Stage
	{
	public:
		/*
		中でギミックの初期化を行ってます
		*/
		bool init(const char* modelPath);
		/*
		中でギミックも破棄してます
		*/
		void clear();

		void update();
		void render();
	//9/6 レイピックを作るのわすれない
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