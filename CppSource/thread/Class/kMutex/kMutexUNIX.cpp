#include "kMutex.h"

namespace klib
{
	namespace thread
	{
		class kMutex::Impl
		{
		private:
			pthread_mutex_t m_Mutex;
		public:
			Impl()
			{
				pthread_mutexattr_t mattr;

				pthread_mutexattr_init( &mattr );
				pthread_mutex_init(&m_Mutex,&mattr);
			}

			~Impl()
			{
				pthread_mutex_lock(&m_Mutex);
				pthread_mutex_unlock(&m_Mutex);
				pthread_mutex_destroy(&m_Mutex);
			}

			void lock()
			{
				pthread_mutex_lock(&m_Mutex);
			}

			void unLock()
			{
				pthread_mutex_unlock(&m_Mutex);
			}
		};
		kMutex::kMutex():m_Impl(new Impl){}
		kMutex::~kMutex(){}
		void kMutex::lock(int val){dprintf("			Mutex Lock!!!!!!!%d",val);m_Impl->lock();}
		void kMutex::unLock(int val){dprintf("			Mutex UnLock!!!!!!!%d",val);m_Impl->unLock();}
	}
}