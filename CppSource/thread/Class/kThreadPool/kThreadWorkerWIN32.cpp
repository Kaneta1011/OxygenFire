#include "kThreadPool.h"
#include "../kThread/kThread.h"
namespace klib
{
	namespace thread
	{
		class kThreadWorker::Impl
		{
		private:
			///@brief スレッドのID
			u32 m_ID;
			///@brief スレッドハンドル
			handle_t m_Handle;
			///@brief アクティブフラグ
			bool m_Active;
			///@brief 全スレッド共通のミューテックス
			static kMutex m_Mutex;
		private:
			///@brief スレッド実行用の静的関数
			static unsigned __stdcall thread_start_function( void* param )
			{
				kThreadWorker::Impl* threadInst=((kThreadWorker::Impl*)param);
				try
				{
					while(1)
					{
						//スレッドを非アクティブ状態にする
						threadInst->setActiveFlag(false);
						//処理をロックする(既にここに処理が来ていたらlockの中でunlockされるまで待つ)
						m_Mutex.lock();
						//プールに処理が入ってくるのを待つ(もしプールのwaitが起床してイベントが非シグナルになっているときにシグナル状態になってしまった時のためにwhileで処理が入るまでループしている)
						//上の状態のため最悪の状態でこのwhileループで一回ループされることになる
						while(kThreadPool::isEmpty())
						{
							//プールが終了待ちに入っていればスレッドを終了する
							if(!kThreadPool::isActive()){m_Mutex.unLock();return 0;}
							kThreadPool::wait();
						}
						//スレッドをアクティブ状態にする
						threadInst->setActiveFlag(true);
						//処理を取り出す
						IkThreadFunc* func=NULL;
						kThreadPool::get(&func);
						//処理をアンロックする
						m_Mutex.unLock();
						//実行
						int* ret=(int*)func->run();
						printf("ThreadID:%d Run %d\n",threadInst->getID(),*ret);
						delete ret;
						//func->run();
						//終了
						//printf("ThreadID:%d End\n",threadInst->getID());
						func->end();
					}
				}
				catch (...)
				{
				}
				return 0;
			}
		private:
			/**
			* @brief スレッドハンドルを閉じる
			*/
			void close()
			{
				if( m_Handle != 0 )
				{
					::CloseHandle( m_Handle );
					m_Handle = 0;
				}
			}
			/**
			* @brief スレッドを開始する
			*/
			void start()
			{

				if( m_Handle == 0 )
				{
					m_Handle = ( HANDLE )::_beginthreadex( 0, 0,
						&thread_start_function, this, CREATE_SUSPENDED,
						0 );
					if( m_Handle == 0 )return;
					//throw thread_resource_error();

					::ResumeThread( m_Handle );
				}
			}
			/**
			* @brief スレッドの終了を待機する。
			*/
			unsigned long wait( unsigned long timeout = INFINITE )
			{
				return ::WaitForSingleObject( m_Handle, timeout );
			}

		public:
			Impl(int i):m_Handle(0),m_ID(i)
			{
				start();
			}
			virtual ~Impl()
			{ 
				wait();
				close(); 
			}
			void setActiveFlag(bool flag){m_Active=flag;}
			bool isActive()const{return m_Active;}
			u32 getID()const{return m_ID;}
		};
		kMutex kThreadWorker::Impl::m_Mutex;
		kThreadWorker::kThreadWorker(int i):m_Impl(new Impl(i)){}
		kThreadWorker::~kThreadWorker(){}
	}
}