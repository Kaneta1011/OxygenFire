#ifndef _GIMMICK_
#define _GIMMICK_

#include "Game/IObject/IObject.h"
#include "utility\utility.h"
#include "utility\SinglyLinkedList.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"

namespace rlib
{
	struct GimmickInfo
	{
		const char* meshFilePath;
		klib::math::Vector3 pos;
		klib::math::Vector3 size;
		//klib::math::Vector3 velocity;

		GimmickInfo(const char* meshFilePath):meshFilePath(meshFilePath){}
	};

	class Gimmick : public IObject
	{
	public:
		enum MESSAGE
		{
			MSG_NON,
			MSG_DEAD,
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

	private:
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
		SinglyLinkedList<Gimmick> mData;

		klib::kGraphicsPipline* pipline;
		klib::kMesh* mpMesh;

	private:
		GimmickManager();
		~GimmickManager();

	};

	#define GIMMICK_MNG rlib::GimmickManager::getInst()
}
#endif