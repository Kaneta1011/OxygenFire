#include "kMutex.h"
namespace klib
{
	namespace thread
	{
		class kMutex::Impl
		{
		private:
			HANDLE m_Mutex;
		public:
			Impl()
			{
				m_Mutex = CreateMutex(NULL,FALSE,NULL);
			}

			~Impl()
			{
				WaitForSingleObject(m_Mutex,INFINITE);
				CloseHandle(m_Mutex);
			}

			void lock()
			{
				WaitForSingleObject(m_Mutex,INFINITE);
			}

			void unLock()
			{
				ReleaseMutex(m_Mutex);
			}
		};
		kMutex::kMutex():m_Impl(new Impl){}
		kMutex::~kMutex(){}
		void kMutex::lock(){m_Impl->lock();}
		void kMutex::unLock(){m_Impl->unLock();}
	}
}