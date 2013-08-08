#include "kEvent.h"
namespace klib
{
	namespace thread
	{
		class kEvent::Impl
		{
		private:

			pthread_cond_t m_Ready;
			pthread_mutex_t m_Lock;
			bool m_Signald;

		public:
			Impl()
			{

				//LOGI("kEvent");
				pthread_mutexattr_t mattr;

				pthread_mutexattr_init(&mattr);
				pthread_mutex_init(&m_Lock,&mattr);
				pthread_cond_init(&m_Ready,NULL);
				m_Signald=false;

			}

			~Impl()
			{

				//LOGI("~kEvent");
				pthread_cond_destroy(&m_Ready);
				pthread_mutex_destroy(&m_Lock);

			}

			void set()
			{

				pthread_mutex_lock(&m_Lock);
				//LOGI("kEvent Set");
				m_Signald=true;
				pthread_cond_broadcast(&m_Ready);
				pthread_mutex_unlock(&m_Lock);

			}

			void wait()
			{

				pthread_mutex_lock(&m_Lock);
				//LOGI("kEvent Wait%d",m_Signald);
				if(m_Signald)
				{
					//LOGI("kEvent Wait::Signald");
					pthread_mutex_unlock(&m_Lock);
					return;
				}
				pthread_cond_wait(&m_Ready,&m_Lock);
				//LOGI("kEvent Wait::cond");
				pthread_mutex_unlock(&m_Lock);

				return;
			}

			void reset()
			{

				pthread_mutex_lock(&m_Lock);
				//LOGI("kEvent Reset");
				m_Signald=false;
				pthread_mutex_unlock(&m_Lock);

				//pthread_mutex_unlock(&m_Lock);

			}
		};

		kEvent::kEvent():m_Impl(new Impl){}
		kEvent::~kEvent(){}
		void kEvent::set(){m_Impl->set();}
		void kEvent::wait(){m_Impl->wait();}
		void kEvent::reset(){m_Impl->reset();}
	}
}