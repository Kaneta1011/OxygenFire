#pragma once
/**
* @file スレッドイベント
*/
#include "../../../kdefines.h"
#include "../../../KTL/pImpl.h"

namespace klib
{
	namespace thread
	{
		/**
		* @class イベントオブジェクトクラス
		* @brief WIN32とAndroidで実装ファイルを分けているので注意
		*/
		class kEvent
		{
		private:
			///@brief 実装クラス
			class Impl;
			ktl::pImpl<Impl> m_Impl;
		public:
			kEvent();
			~kEvent();
			///@brief イベントをシグナル状態にする
			void set();
			///@brief イベントのシグナル状態を待つ
			void wait();
			///@brief イベントを非シグナル状態にする
			void reset();

		};
	}
}