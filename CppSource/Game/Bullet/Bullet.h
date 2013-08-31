#ifndef _BULLET_
#define _BULLET_

#include "Game\IObject\IObject.h"
#include "utility\utility.h"
#include "utility\SinglyLinkedList.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"

namespace rlib
{
	class GimmickManager;

	struct BulletInfo
	{
		klib::math::Vector3 pos;
		klib::math::Vector3 size;
		klib::math::Vector3 velocity;
		BulletInfo():size(1,1,1){}

	};

	class Bullet : public IObject
	{
	public:
		enum MESSAGE
		{
			MSG_NON,
			MSG_DEAD,
		};
	public:
		Bullet();
		~Bullet();

		void init(BulletInfo& info);

		virtual int update();
		virtual void render();

		klib::math::Vector3& pos(){return this->mPos;}

	private:
		int mCount;
	};

	//===============================================================================
	//
	//		BulletManagerクラス
	//		・シングルトン
	//
	//===============================================================================
	class BulletManager
	{
	public:
		static BulletManager& getInst(){
			static BulletManager inst;
			return inst;
		}

	public:
		typedef SinglyLinkedList<Bullet>::Iterator Iterator;
		typedef SinglyLinkedList<Bullet>::ConstIterator ConstIterator;

	public:
		enum MESSAGE
		{
			MSG_NON,
		};

	public:
		void init();
		void clear();

		void add(BulletInfo& info);

		int update();
		void render();

		void clearData();
		
		void collision(rlib::GimmickManager& gimmickMng);

	public:
		Iterator begin(){return this->mData.begin();}
		ConstIterator begin()const{return this->mData.begin();}

		unsigned int size(){return this->mData.size();}

	private:
		SinglyLinkedList<Bullet> mData;

		klib::kGraphicsPipline* pipline;

	private:
		BulletManager();
		~BulletManager();

	};

}
#endif