#ifndef _GIMMICK_
#define _GIMMICK_

#include "Game/IObject/IObject.h"
#include "utility\utility.h"
#include "utility\SinglyLinkedList.h"
#include "StandardLib\SmartPointer.h"
#include <list>
#include <vector>

#define ANDROID_REDNER
#ifdef ANDROID_REDNER
#include "GraphicsLib\Class\kMesh\kMesh.h"
#endif

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

#ifdef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, klib::kGraphicsPipline* pipeline);
#endif

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
		typedef std::vector<Gimmick*> ListType;
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

		void add(GimmickInfo& info);

		int update();

#ifdef ANDROID_REDNER
		void render();
#endif

	public:
		Iterator begin(){return this->mData.begin();}
		ConstIterator begin()const{return this->mData.begin();}

		Iterator end(){ return this->mData.end(); }
		ConstIterator end()const{return this->mData.end(); }

		unsigned int size(){return this->mData.size();}

	private:
#ifdef ANDROID_REDNER
		void loadMeshes();
		klib::kMesh* getMesh( int type );
#endif
	private:
		ListType mData;

#ifdef ANDROID_REDNER
		sp<klib::kMesh*> mpMeshies;
#endif
	private:
		GimmickManager();
		~GimmickManager();

	};

	#define GIMMICK_MNG rlib::GimmickManager::getInst()
}
#endif