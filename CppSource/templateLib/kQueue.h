#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "../Thread/Class/kMutex/kMutex.h"
namespace klib
{
	namespace ktl
	{
		template<class T>
		class kQueue
		{
		protected:

			struct node {
				T m_Key;
				node *m_Prev;
				node *m_Next;
			};
		protected:
			///@brief �L���[�擪�f�[�^(���o���Ƃ��͂���������o��)
			node *m_Head;
			///@brief �L���[���[�f�[�^
			node *m_Tail;
			///@brief �L���[�T�C�Y
			int m_Size;
		public:
			inline kQueue():m_Head(NULL),m_Tail(NULL),m_Size(0){}
			inline virtual ~kQueue()
			{
				while(!isEmpty())
				{
					node* next=m_Head->m_Next;
					//�擪�m�[�h���������
					delete m_Head;
					m_Head=next;
					if(m_Head)m_Head->m_Prev=NULL;
					m_Size--;
				}
			}
			/**
			* @brief �L���[�Ƀf�[�^��ǉ�����
			* @param[in] v �f�[�^
			*/
			inline virtual void put(T v){
				//�V�K�m�[�h���쐬����
				node* n=new node;
				node* tmp=m_Tail;
				//�m�[�h�Ƀf�[�^���i�[�����X�g�ɒǉ�����
				n->m_Key=v;
				n->m_Prev=tmp;
				n->m_Next=NULL;
				//���[�m�[�h�Ƀm�[�h������
				m_Tail=n;
				//�擪�m�[�h�Ƀf�[�^�����݂��Ȃ���ΐ擪�Ƀm�[�h������
				if(m_Head==NULL)m_Head=n;
				//��O�̃m�[�h�̎��𖖒[�m�[�h�ɂ���
				else tmp->m_Next=m_Tail;
				m_Size++;
			}
			/**
			* @brief ������f�[�^�����o��
			* @param[out] �擾�����f�[�^
			* @return �f�[�^���擾�����true
			*/
			inline virtual bool get(T* ret){
				//�f�[�^���Ȃ���΃f�[�^���擾������false��Ԃ�
				if(isEmpty())return false;
				//�擪�m�[�h�̒l��߂�l�Ƃ���
				*ret=m_Head->m_Key;
				node* next=m_Head->m_Next;
				//�擪�m�[�h���������
				delete m_Head;
				m_Head=next;
				if(m_Head)m_Head->m_Prev=NULL;
				m_Size--;
				return true;
			}
			/**
			* @brief �L���[�ɑ��݂���f�[�^�̐����擾����
			* @return �f�[�^��
			*/
			inline virtual int size()
			{
				return m_Size;
			}
			/**
			* @brief �L���[�Ƀf�[�^�����݂��邩�`�F�b�N
			* @return �f�[�^�����݂��Ă��Ȃ���ΐ^
			*/
			inline bool isEmpty()
			{
				return m_Size==0;
			}
		};
		template<class T>
		class kQueue_Safe:public	kQueue<T>
		{
		protected:
			thread::kMutex m_Mutex;
		public:
			inline kQueue_Safe():kQueue<T>(){}
			inline ~kQueue_Safe(){}
			/**
			* @brief �L���[�Ƀf�[�^��ǉ�����
			* @param[in] v �f�[�^
			*/
			inline void put(T v)
			{
				m_Mutex.lock();
				kQueue<T>::put(v);
				m_Mutex.unLock();
			}
			/**
			* @brief ������f�[�^�����o��
			* @param[out] �擾�����f�[�^
			* @return �f�[�^���擾�����true
			*/
			inline virtual bool get(T* ret){
				m_Mutex.lock();
				bool flag=kQueue<T>::get(ret);
				m_Mutex.unLock();
				return flag;
			}
			/**
			* @brief �L���[�ɑ��݂���f�[�^�̐����擾����
			* @return �f�[�^��
			*/
			inline virtual int size()
			{
				m_Mutex.lock();
				int ret=kQueue<T>::size();
				m_Mutex.unLock();
				return ret;
			}
			/**
			* @brief �L���[�Ƀf�[�^�����݂��邩�`�F�b�N
			* @return �f�[�^�����݂��Ă��Ȃ���ΐ^
			*/
			inline bool isEmpty()
			{
				m_Mutex.lock();
				bool ret=kQueue<T>::isEmpty();
				m_Mutex.unLock();
				return ret;
			}
		};
	}
}