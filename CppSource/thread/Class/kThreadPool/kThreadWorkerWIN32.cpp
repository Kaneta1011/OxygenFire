#include "kThreadPool.h"
#include "../kThread/kThread.h"
namespace klib
{
	namespace thread
	{
		class kThreadWorker::Impl
		{
		private:
			///@brief �X���b�h��ID
			u32 m_ID;
			///@brief �X���b�h�n���h��
			handle_t m_Handle;
			///@brief �A�N�e�B�u�t���O
			bool m_Active;
			///@brief �S�X���b�h���ʂ̃~���[�e�b�N�X
			static kMutex m_Mutex;
		private:
			///@brief �X���b�h���s�p�̐ÓI�֐�
			static unsigned __stdcall thread_start_function( void* param )
			{
				kThreadWorker::Impl* threadInst=((kThreadWorker::Impl*)param);
				try
				{
					while(1)
					{
						//�X���b�h���A�N�e�B�u��Ԃɂ���
						threadInst->setActiveFlag(false);
						//���������b�N����(���ɂ����ɏ��������Ă�����lock�̒���unlock�����܂ő҂�)
						m_Mutex.lock();
						//�v�[���ɏ����������Ă���̂�҂�(�����v�[����wait���N�����ăC�x���g����V�O�i���ɂȂ��Ă���Ƃ��ɃV�O�i����ԂɂȂ��Ă��܂������̂��߂�while�ŏ���������܂Ń��[�v���Ă���)
						//��̏�Ԃ̂��ߍň��̏�Ԃł���while���[�v�ň�񃋁[�v����邱�ƂɂȂ�
						while(kThreadPool::isEmpty())
						{
							//�v�[�����I���҂��ɓ����Ă���΃X���b�h���I������
							if(!kThreadPool::isActive()){m_Mutex.unLock();return 0;}
							kThreadPool::wait();
						}
						//�X���b�h���A�N�e�B�u��Ԃɂ���
						threadInst->setActiveFlag(true);
						//���������o��
						IkThreadFunc* func=NULL;
						kThreadPool::get(&func);
						//�������A�����b�N����
						m_Mutex.unLock();
						//���s
						int* ret=(int*)func->run();
						printf("ThreadID:%d Run %d\n",threadInst->getID(),*ret);
						delete ret;
						//func->run();
						//�I��
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
			* @brief �X���b�h�n���h�������
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
			* @brief �X���b�h���J�n����
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
			* @brief �X���b�h�̏I����ҋ@����B
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