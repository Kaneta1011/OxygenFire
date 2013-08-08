#include "kEvent.h"
namespace klib
{
	namespace thread
	{
		class kEvent::Impl
		{
		private:
			HANDLE m_event;
		public:
			Impl(void)
			{
				m_event = CreateEvent(NULL,TRUE,FALSE,NULL);
				if( !m_event )
				{
				}
			}
			~Impl(void)
			{
				CloseHandle(m_event);
			}
			void set()
			{
				SetEvent(m_event);
			}
			void wait()
			{
				if( WaitForSingleObject(m_event,INFINITE) != WAIT_OBJECT_0 )
				{
					return;
				}
				return;
			}
			void reset()
			{
				ResetEvent(m_event);
			}
		};
		kEvent::kEvent():m_Impl(new Impl){}
		kEvent::~kEvent(){}
		void kEvent::set(){m_Impl->set();}
		void kEvent::wait(){m_Impl->wait();}
		void kEvent::reset(){m_Impl->reset();}
	}
}