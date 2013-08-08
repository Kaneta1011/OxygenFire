/**
* @file �X���b�h�v�[��
*/
#pragma once
#include "../../../templateLib/kQueue.h"
#include "../kMutex/kMutex.h"
#include "../kEvent/kEvent.h"
#include "../../../templateLib/pImpl.h"
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
		class kThreadWorker;
		class IkThreadFunc;

		typedef klib::ktl::kQueue_Safe<IkThreadFunc*> ThreadQueue;
		/**
		* @brief �X���b�h�Ǘ��N���X
		*/
		class kThreadPool
		{
		private:
			class Impl;
			static ktl::pImpl<Impl> m_Impl;
		public:
			/*
			* @brief �X���b�h�v�[�����쐬����
			* @param[in] maxthread �����ɓ��삷��X���b�h�̐����w�肷��
			*/
			static void create(int maxthread);
			/*
			* @brief �X���b�h�v�[����j�󂷂�
			*/
			static void destroy();
			/*
			* @brief �X���b�h�v�[���Ɋ֐������N�G�X�g����
			* @param[in] v �֐��z���_�[�ɓ����Ă���֐�
			*/
			static void request(IkThreadFunc* v);
			/*
			* @brief �X���b�h�v�[���ő҂��Ă���֐����擾����
			* @param[out] ret �擾�����֐�
			* @return �X���b�h�v�[���Ɋ֐����҂��Ă����true
			*/
			static bool get(IkThreadFunc** ret);
			/*
			* @brief �X���b�h�v�[���Ŋ֐����҂��Ă��邩�ǂ����𔻒�
			* @return �֐����҂��Ă��Ȃ����true
			*/
			static bool isEmpty();
			/*
			* @brief �X���b�h�v�[�������s�����ǂ�������
			* @return ���s���Ȃ�true(���̊֐���false��Ԃ����Ƃ����ׂẴX���b�h�Ŏ��s���Ă���֐����I��������K�v������)
			*/
			static bool isActive();
			/*
			* @brief �X���b�h�v�[���Ƀ��N�G�X�g������܂őҋ@����
			* ���ɃX���b�h�v�[���Ƀ��N�G�X�g�����Ă���Α҂���Ԃ��X���[���ă��N�G�X�g�t���O������������
			*/
			static void wait();
		};

		/**
		* @brief ���ۂɎ��s�����X���b�h�����b�v�����N���X
		* WIN32��Android�Ŏ����t�@�C���𕪂��Ă���̂Œ���
		*/
		class kThreadWorker
		{
		private:
			class Impl;
			ktl::pImpl<Impl> m_Impl;
		public:
			kThreadWorker(int i);
			~kThreadWorker();
		};
	}
}