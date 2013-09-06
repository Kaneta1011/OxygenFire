/**
* @file スレッドクラスヘッダ
*/
#pragma once
#include "../kThreadPool/kThreadPool.h"
#include "../../../templateLib/kFuncHolder.h"
#include "../../../defines.h"

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
		typedef ktl::kFuncHolder<void> kThreadHolder;

		/**
		* @class スレッド関数実行クラス
		*/
		class IkThreadFunc
		{
		private:
			bool* m_EndFlag;
			///@brief 関数終了待ちイベント(デストラクタでセットしてスレッドクラスに知らせる)
			kEvent m_Event;
			///@関数または関数オブジェクトを保持する
			kThreadHolder* m_Func;
		public:
			~IkThreadFunc()
			{
				//LOGI("IkThreadFunc ~IkThreadFunc");
				delete m_Func;
			}
			IkThreadFunc(){}
			IkThreadFunc( kThreadHolder* f ,bool* flag) : m_Func( f ),m_EndFlag(flag){}
		public:
			/**
			* @brief 関数または関数オブジェクトの実行
			*/
			void run()
			{
				//LOGI("IkThreadFunc run");
				(*m_Func)(); 
				//LOGI("IkThreadFunc return");
			}
			/**
			* @brief スレッド終了待ち関数
			* イベントがシグナル状態になるまで待機
			*/
			void wait()
			{
				//LOGI("IkThreadFunc::wait");
				m_Event.wait();
			}
			void end()
			{
				//LOGI("IkThreadFunc::end");
				m_Event.set();
				*m_EndFlag=true;
			}
		private:
			/**
			* @brief コピーコンストラクタ禁止
			*/
			IkThreadFunc( const IkThreadFunc& );
			/**
			* @brief 代入演算子禁止
			*/
			void operator=( const IkThreadFunc& );
		};
		/**
		* @class スレッドクラス
		* コンストラクタで受け取った関数を
		* スレッドプールにリクエストする
		* デストラクタではリクエストを送った関数が終了するまで待機する
		*/
		class kThread
		{
		private:
			bool m_EndFlag;
			///@関数または関数オブジェクトを保持した親クラス
			IkThreadFunc* m_Data;
		public:
			/**
			* @brief コンストラクタ
			* 関数または関数オブジェクトを受け取り
			* 関数クラスを作成してスレッドプールにリクエストする
			*/
			kThread( kThreadHolder* f )
				: m_EndFlag(false),m_Data( new IkThreadFunc(f,&m_EndFlag) )
			{
				//LOGI("kThread");
				kThreadPool::request(m_Data);
			}
			/**
			* @brief デストラクタ
			* リクエストを送った関数が終了(関数クラスのデストラクタが実行)するまで待機する
			*/
			virtual ~kThread()
			{
				//LOGI("~kThread");
				//LOGI("kThread::wait");
				this->wait();
				//LOGI("kThread::m_Data delete");
				delete this->m_Data;
				//LOGI("kThread::destractor end");
			}

		public:
			/**
			* @brief スレッドの終了を待機する
			*/
			void wait()
			{
				//LOGI("kThread wait");
				m_Data->wait();
			}
			bool isEnd()const{return m_EndFlag;}
		private:
			/**
			* @brief コピーコンストラクタ禁止
			*/
			kThread( const kThread& );
			/**
			* @brief 代入演算子禁止
			*/
			kThread& operator=( const kThread& );
		};



	}
}