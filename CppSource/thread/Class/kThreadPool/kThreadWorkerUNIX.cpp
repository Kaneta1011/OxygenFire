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
			int m_ID;
			///@brief �X���b�h�n���h��
			pthread_t m_Handle;
			///@brief �A�N�e�B�u�t���O
			bool m_Active;
			///@brief �S�X���b�h���ʂ̃~���[�e�b�N�X
			static kMutex m_Mutex;
		private:
			///@brief �X���b�h���s�p�̐ÓI�֐�
			static void* thread_start_function( void* param )
			{
				kThreadWorker::Impl* threadInst=((kThreadWorker::Impl*)param);
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
					dprintf("ThreadID:%d Run %d\n",threadInst->getID(),*ret);
					delete ret;
					//func->run();
					//�I��
					//dprintf("ThreadID:%d End\n",threadInst->getID());
					func->end();
				}

				return 0;
			}
		private:
			/**
			* @brief �X���b�h�n���h�������
			*/
			void close()
			{
			}
			/**
			* @brief �X���b�h���J�n����
			*/
			void start()
			{
				pthread_create(&m_Handle, NULL, thread_start_function, this);
			}
			/**
			* @brief �X���b�h�̏I����ҋ@����B
			*/
			unsigned long wait( )
			{
				return pthread_join(m_Handle,NULL);
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