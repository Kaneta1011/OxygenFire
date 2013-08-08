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
			///@brief キュー先頭データ(取り出すときはここから取り出す)
			node *m_Head;
			///@brief キュー末端データ
			node *m_Tail;
			///@brief キューサイズ
			int m_Size;
		public:
			inline kQueue():m_Head(NULL),m_Tail(NULL),m_Size(0){}
			inline virtual ~kQueue()
			{
				while(!isEmpty())
				{
					node* next=m_Head->m_Next;
					//先頭ノードを解放する
					delete m_Head;
					m_Head=next;
					if(m_Head)m_Head->m_Prev=NULL;
					m_Size--;
				}
			}
			/**
			* @brief キューにデータを追加する
			* @param[in] v データ
			*/
			inline virtual void put(T v){
				//新規ノードを作成する
				node* n=new node;
				node* tmp=m_Tail;
				//ノードにデータを格納しリストに追加する
				n->m_Key=v;
				n->m_Prev=tmp;
				n->m_Next=NULL;
				//末端ノードにノードを入れる
				m_Tail=n;
				//先頭ノードにデータが存在しなければ先頭にノードを入れる
				if(m_Head==NULL)m_Head=n;
				//一つ前のノードの次を末端ノードにする
				else tmp->m_Next=m_Tail;
				m_Size++;
			}
			/**
			* @brief 頭からデータを取り出す
			* @param[out] 取得したデータ
			* @return データを取得すればtrue
			*/
			inline virtual bool get(T* ret){
				//データがなければデータを取得せずにfalseを返す
				if(isEmpty())return false;
				//先頭ノードの値を戻り値とする
				*ret=m_Head->m_Key;
				node* next=m_Head->m_Next;
				//先頭ノードを解放する
				delete m_Head;
				m_Head=next;
				if(m_Head)m_Head->m_Prev=NULL;
				m_Size--;
				return true;
			}
			/**
			* @brief キューに存在するデータの数を取得する
			* @return データ数
			*/
			inline virtual int size()
			{
				return m_Size;
			}
			/**
			* @brief キューにデータが存在するかチェック
			* @return データが存在していなければ真
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
			* @brief キューにデータを追加する
			* @param[in] v データ
			*/
			inline void put(T v)
			{
				m_Mutex.lock();
				kQueue<T>::put(v);
				m_Mutex.unLock();
			}
			/**
			* @brief 頭からデータを取り出す
			* @param[out] 取得したデータ
			* @return データを取得すればtrue
			*/
			inline virtual bool get(T* ret){
				m_Mutex.lock();
				bool flag=kQueue<T>::get(ret);
				m_Mutex.unLock();
				return flag;
			}
			/**
			* @brief キューに存在するデータの数を取得する
			* @return データ数
			*/
			inline virtual int size()
			{
				m_Mutex.lock();
				int ret=kQueue<T>::size();
				m_Mutex.unLock();
				return ret;
			}
			/**
			* @brief キューにデータが存在するかチェック
			* @return データが存在していなければ真
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