#pragma once
//----------------------------------------------------------------------
//	Singleton
//----------------------------------------------------------------------
namespace klib
{
	namespace ktl
	{
#define SINGLETON_INST_REF 0
		template <class T>
		class kSingleton
		{
		private :
			///@brief シングルトンクラスのインスタンス
			static T* mp_Instance;
		protected:
			///@brief 作成、コピー禁止
			kSingleton() {}
			virtual ~kSingleton() {}
		private:
			kSingleton(const kSingleton&);
			kSingleton& operator=(const kSingleton&);
		public :
			/**
			* @brief シングルトンクラスの作成
			*/
			static void _create()
			{ 
				//インスタンスポインタが真なら多重作成
				DYNAMIC_ASSERT(!mp_Instance,"シングルトンクラスの多重作成");
				mp_Instance = new T();
			}
			/**
			* @brief シングルトンクラスの破壊
			*/
			static void _destroy(){SAFE_DELETE(mp_Instance);}
			/**
			* @brief シングルトンクラスのインスタンスの取得
			* @プリプロセッサで参照とポインタを切り替え
			* @return インスタンスの参照もしくはポインタ
			*/
#ifdef SINGLETON_INST_REF
			static T& _getInstance()
			{
				if(!_exists()){_create();}
				return *mp_Instance;
			}
#else
			static T* _getInstance()
			{
				if(!_exists()){_create();}
				return mp_Instance;
			}
#endif
			/**
			* @brief シングルトンクラスのインスタンスの参照取得
			* @return インスタンスの参照
			*/
			static T& _getInstanceRef()
			{
				if(!_exists()){_create();}
				return *mp_Instance;
			}
			/**
			* @brief シングルトンクラスのインスタンスのポインタ取得
			* @return インスタンスのポインタ
			*/
			static T* _getInstancePtr()
			{
				if(!_exists()){_create();}
				return mp_Instance;
			}
			/**
			* @brief シングルトンクラスのインスタンスの存在を確認
			* @return インスタンスが存在していればtrue
			*/
			static bool _exists(){return mp_Instance!=NULL;}
		};

		template <class T>
		T* kSingleton<T>::mp_Instance = NULL;
	}
}