#ifndef _GIMMICK_
#define _GIMMICK_

#include "Game/IObject/IObject.h"
#include "utility\utility.h"
#include "utility\SinglyLinkedList.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "StandardLib\SmartPointer.h"

namespace rlib
{
	struct GimmickInfo
	{
		int type;
		klib::math::Vector3 pos;
		klib::math::Vector3 size;
		klib::math::Vector3 angle;
		//klib::math::Vector3 velocity;

		GimmickInfo():type(-1){}
	};

	class Gimmick : public IObject
	{
	public:
		enum MESSAGE
		{
			MSG_NON,
			MSG_DEAD,
		};

		enum TYPE
		{
			eTYPE_TEST,
			eTYPE_NUM
		};

	public:
		Gimmick();
		~Gimmick();

		void init(GimmickInfo& info);

		virtual int update();
		virtual void render(klib::kMesh* mesh, klib::kGraphicsPipline* pipeline);

		klib::math::Vector3& pos(){return this->mPos;}

	public:
		void on(){this->mIsOn = true;}
		int getType()const{ return this->mType; }

	private:
		int mType;
		bool mIsOn;
		int mCount;
		
	};

	//===============================================================================
	//
	//		GimmickManagerクラス
	//		・シングルトン
	//
	//===============================================================================
	class GimmickManager
	{
	public:
		static GimmickManager& getInst(){
			static GimmickManager inst;
			return inst;
		}

	public:
		typedef SinglyLinkedList<Gimmick>::Iterator Iterator;
		typedef SinglyLinkedList<Gimmick>::ConstIterator ConstIterator;

	public:
		enum MESSAGE
		{
			MSG_NON,
		};

	public:
		void init();
		void clear();

		void add(GimmickInfo& info);

		int update();
		void render();

		void clearData();

	public:
		Iterator begin(){return this->mData.begin();}
		ConstIterator begin()const{return this->mData.begin();}

		unsigned int size(){return this->mData.size();}

	private:
		klib::kMesh* getMesh( int type ); 
	private:
		SinglyLinkedList<Gimmick> mData;

		klib::kGraphicsPipline* pipline;
		sp<klib::kMesh*> mpMeshies;

	private:
		GimmickManager();
		~GimmickManager();

	};

	#define GIMMICK_MNG rlib::GimmickManager::getInst()
}
#endif