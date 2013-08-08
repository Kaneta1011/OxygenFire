/**
* @file スレッドプール
*/
#pragma once
#include "../../../templateLib/kQueue.h"
#include "../kMutex/kMutex.h"
#include "../kEvent/kEvent.h"
#include "../../../templateLib/pImpl.h"
/**
* @namespace かねたライブラリ
*/
namespace klib
{
	/**
	* @namespace スレッド
	*/
	namespace thread
	{
		class kThreadWorker;
		class IkThreadFunc;

		typedef klib::ktl::kQueue_Safe<IkThreadFunc*> ThreadQueue;
		/**
		* @brief スレッド管理クラス
		*/
		class kThreadPool
		{
		private:
			class Impl;
			static ktl::pImpl<Impl> m_Impl;
		public:
			/*
			* @brief スレッドプールを作成する
			* @param[in] maxthread 同時に動作するスレッドの数を指定する
			*/
			static void create(int maxthread);
			/*
			* @brief スレッドプールを破壊する
			*/
			static void destroy();
			/*
			* @brief スレッドプールに関数をリクエストする
			* @param[in] v 関数ホルダーに入っている関数
			*/
			static void request(IkThreadFunc* v);
			/*
			* @brief スレッドプールで待っている関数を取得する
			* @param[out] ret 取得した関数
			* @return スレッドプールに関数が待っていればtrue
			*/
			static bool get(IkThreadFunc** ret);
			/*
			* @brief スレッドプールで関数が待っているかどうかを判定
			* @return 関数が待っていなければtrue
			*/
			static bool isEmpty();
			/*
			* @brief スレッドプールが実行中かどうか判定
			* @return 実行中ならtrue(この関数がfalseを返したときすべてのスレッドで実行している関数を終了させる必要がある)
			*/
			static bool isActive();
			/*
			* @brief スレッドプールにリクエストが来るまで待機する
			* 既にスレッドプールにリクエストが来ていれば待ち状態をスルーしてリクエストフラグを初期化する
			*/
			static void wait();
		};

		/**
		* @brief 実際に実行されるスレッドをラップしたクラス
		* WIN32とAndroidで実装ファイルを分けているので注意
		*/
		class kThreadWorker
		{
		private:
			class Impl;
			ktl::pImpl<Impl> m_Impl;
		public:
			kThreadWorker(int i);
			~kThreadWorker();
		};
	}
}