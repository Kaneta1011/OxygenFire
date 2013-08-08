#include "kThreadPool.h"
#include "../kThread/kThread.h"
namespace klib
{
	namespace thread
	{
		//*****************************************************************************
		//
		//		�X���b�h�v�[�������N���X
		//
		//*****************************************************************************
		ktl::pImpl<kThreadPool::Impl> kThreadPool::m_Impl;
		class kThreadPool::Impl
		{
		private:
			///@brief ���������N�G�X�g���ꂽ���ɃV�O�i���ɂȂ�C�x���g
			static kEvent m_Event;
			///@brief �X���b�h�Ǘ��N���X�����s���Ȃ�true
			static bool m_Active;
			///@brief �Ǘ�����X���b�h�̐�
			static int m_ThreadMax;
			///@brief brief ���s�҂����̃X���b�h�L���[
			static ThreadQueue m_WaitThread;
			///@���ۂɎ��s�����X���b�h�̃N���X�z��
			static kThreadWorker** m_WorkThread;
		public:

			static void create(int maxthread)
			{
				m_ThreadMax=maxthread;
				m_Active=true;
				m_WorkThread=new kThreadWorker*[maxthread];
				for(int i=0;i<maxthread;i++)
				{
					m_WorkThread[i]=new kThreadWorker(i);
				}
			}
			static void destroy()
			{
				m_Active=false;
				m_Event.set();
				//m_Event.Reset();
				for(int i=0;i<m_ThreadMax;i++)
				{
					delete m_WorkThread[i];
				}
				delete[] m_WorkThread;
			}
			static void request(IkThreadFunc* v)
			{
				//LOGI("kThreadPool request");
				m_WaitThread.put(v);
				m_Event.set();
				//m_Event.Reset();
			}
			static bool get(IkThreadFunc** ret)
			{
				//LOGI("kThreadPool get:Queue%d",m_WaitThread.size());
				return m_WaitThread.get(ret);
			}
			static bool isEmpty(){return m_WaitThread.isEmpty();}
			static bool isActive(){return m_Active;}
			static void wait()
			{
				m_Event.wait();
				m_Event.reset();
			}
		};
		kEvent kThreadPool::Impl::m_Event;
		bool kThreadPool::Impl::m_Active=true;
		ThreadQueue kThreadPool::Impl::m_WaitThread;
		int kThreadPool::Impl::m_ThreadMax=0;
		kThreadWorker** kThreadPool::Impl::m_WorkThread=NULL;

		//*****************************************************************************
		//
		//		�X���b�h�N���X
		//
		//*****************************************************************************
		void kThreadPool::create(int maxthread)
		{
			m_Impl=new Impl;
			m_Impl->create(maxthread);
		}
		void kThreadPool::destroy(){m_Impl->destroy();}
		void kThreadPool::request(IkThreadFunc* v){m_Impl->request(v);}
		bool kThreadPool::get(IkThreadFunc** ret){return m_Impl->get(ret);}
		bool kThreadPool::isEmpty(){return m_Impl->isEmpty();}
		bool kThreadPool::isActive(){return m_Impl->isActive();}
		void kThreadPool::wait(){m_Impl->wait();}
	}
}