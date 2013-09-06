/**
* @file �X���b�h�N���X�w�b�_
*/
#pragma once
#include "../kThreadPool/kThreadPool.h"
#include "../../../templateLib/kFuncHolder.h"
#include "../../../defines.h"

/**
* @namespace ���˂����C�u����
*/
namespace klib
{
	/**
	* @namespace �X���b�h
	*/
	namespace thread
	{
		typedef ktl::kFuncHolder<void> kThreadHolder;

		/**
		* @class �X���b�h�֐����s�N���X
		*/
		class IkThreadFunc
		{
		private:
			bool* m_EndFlag;
			///@brief �֐��I���҂��C�x���g(�f�X�g���N�^�ŃZ�b�g���ăX���b�h�N���X�ɒm�点��)
			kEvent m_Event;
			///@�֐��܂��͊֐��I�u�W�F�N�g��ێ�����
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
			* @brief �֐��܂��͊֐��I�u�W�F�N�g�̎��s
			*/
			void run()
			{
				//LOGI("IkThreadFunc run");
				(*m_Func)(); 
				//LOGI("IkThreadFunc return");
			}
			/**
			* @brief �X���b�h�I���҂��֐�
			* �C�x���g���V�O�i����ԂɂȂ�܂őҋ@
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
			* @brief �R�s�[�R���X�g���N�^�֎~
			*/
			IkThreadFunc( const IkThreadFunc& );
			/**
			* @brief ������Z�q�֎~
			*/
			void operator=( const IkThreadFunc& );
		};
		/**
		* @class �X���b�h�N���X
		* �R���X�g���N�^�Ŏ󂯎�����֐���
		* �X���b�h�v�[���Ƀ��N�G�X�g����
		* �f�X�g���N�^�ł̓��N�G�X�g�𑗂����֐����I������܂őҋ@����
		*/
		class kThread
		{
		private:
			bool m_EndFlag;
			///@�֐��܂��͊֐��I�u�W�F�N�g��ێ������e�N���X
			IkThreadFunc* m_Data;
		public:
			/**
			* @brief �R���X�g���N�^
			* �֐��܂��͊֐��I�u�W�F�N�g���󂯎��
			* �֐��N���X���쐬���ăX���b�h�v�[���Ƀ��N�G�X�g����
			*/
			kThread( kThreadHolder* f )
				: m_EndFlag(false),m_Data( new IkThreadFunc(f,&m_EndFlag) )
			{
				//LOGI("kThread");
				kThreadPool::request(m_Data);
			}
			/**
			* @brief �f�X�g���N�^
			* ���N�G�X�g�𑗂����֐����I��(�֐��N���X�̃f�X�g���N�^�����s)����܂őҋ@����
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
			* @brief �X���b�h�̏I����ҋ@����
			*/
			void wait()
			{
				//LOGI("kThread wait");
				m_Data->wait();
			}
			bool isEnd()const{return m_EndFlag;}
		private:
			/**
			* @brief �R�s�[�R���X�g���N�^�֎~
			*/
			kThread( const kThread& );
			/**
			* @brief ������Z�q�֎~
			*/
			kThread& operator=( const kThread& );
		};



	}
}