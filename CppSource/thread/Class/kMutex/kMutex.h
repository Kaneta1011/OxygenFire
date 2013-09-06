#pragma once
/**
* @file スレッドミューテックス
*/
#include "../../../defines.h"
#include "../../../templateLib/pImpl.h"

namespace klib
{
	namespace thread
	{
		/**
		* @class ミューテックスオブジェクトクラス
		* brief WIN32とAndroidで実装ファイルを分けているので注意
		*/
		class kMutex
		{
		private:
			///@brief 実装クラス
			class Impl;
			ktl::pImpl<Impl> m_Impl;
		public:
			kMutex();
			~kMutex();
			///@brief ミューテックスをロックする
			void lock(int val=0);
			///@brief ミューテックスを解除する
			void unLock(int val=0);
		};

	}
}