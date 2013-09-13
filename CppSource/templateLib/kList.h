/**
* @file 双方向連結リストクラスヘッダ
*/
#pragma once

namespace klib
{
	namespace ktl
	{
		/**
		* @class 双方向連結リストクラス
		*/
		template<class T>
		class kList
		{
		private:
			/**
			* @class 双方向連結リスト内ノードクラス
			*/
			class Node
			{
			private:
				///@brief 値
				T m_Val;
				///@brief 前のノードポインタ
				Node* m_Prev;
				///@brief 次のノードポインタ
				Node* m_Next;
			public:
				/**
				* @brief コンストラクタ
				* @param[in] val 値
				* @param[in] 前のポインタ
				* @param[in] 次のポインタ
				*/
				Node(T val, Node* prev, Node* next)
				{
					this->m_Val = val;
					this->m_Prev = prev;
					this->m_Next = next;
				}
				/**
				* @brief 値を取得する
				* @return 取得した値
				*/
				T& getValue(){return this->m_Val;}
				/**
				* @brief 前のポインタを取得する
				* @return 取得したポインタ
				*/
				Node* getPrev()const{return this->m_Prev;}
				/**
				* @brief 次のポインタを取得する
				* @return 取得したポインタ
				*/
				Node* getNext()const{return this->m_Next;}

				/**
				* @brief 値をセットする
				* @param[in] val 値
				*/
				void setValue(const T& val){this->m_Val=val;}
				/**
				* @brief 次のポインタをセットする
				* @param[in] val ポインタ
				*/
				void setPrev(Node* val){this->m_Prev=val;}
				/**
				* @brief 前のポインタをセットする
				* @param[in] val ポインタ
				*/
				void setNext(Node* val){this->m_Next=val;}
			};
		public:
			/**
			* @class 双方向連結リスト内イテレータクラス
			*/
			class iterator
			{
				friend kList;
			private:
				///@brief イテレータ内ノード
				Node* m_Node;
			public:
				iterator():m_Node(NULL){}
				iterator(Node* node):m_Node(node){}

				operator bool(){return m_Node!=NULL;}

				bool operator==(const iterator& it){return this->m_Node==it.m_Node;}
				bool operator!=(const iterator& it){return this->m_Node!=it.m_Node;}

				iterator& operator++(){this->m_Node=this->m_Node->getNext();return *this;}
				iterator& operator++(int){this->m_Node=this->m_Node->getNext();return *this;}
				iterator& operator--(){this->m_Node=this->m_Node->getPrev();return *this;}
				iterator& operator--(int){this->m_Node=this->m_Node->getPrev();return *this;}
				///@brief *演算子でノード内の値を取得する
				T&    operator *(){ return m_Node->getValue(); }
			};
		private:
			/**
			* @brief ダミーノード
			* このノードは使用するリストの最後の要素と先頭の要素にそれぞれ繋がっている
			*/
			Node* m_Dummy;
			///@brief リスト内の要素数
			unsigned long m_Size;
		public:
			kList()
			{
				m_Size=0;
				this->m_Dummy = new Node(T(),NULL,NULL);
				this->m_Dummy->setNext(this->m_Dummy);
				this->m_Dummy->setPrev(this->m_Dummy);
			}
			~kList()
			{
				clear();
				delete this->m_Dummy;
				//エラーチェック(この時点で要素数が0になっていなければ設計がおかしい)
				if(m_Size!=0)
				{
					delete (void*)0xdeaddead;
				}
			}
			/**
			* @brief リストが空か判定
			* @return 空なら真
			*/
			bool empty()const{return m_Size==0;}
			/**
			* @brief リストの要素数を取得する
			* @return 要素数
			*/
			unsigned long size()const{return m_Size;}
			/**
			* @brief リスト内から要素を検索する
			* 見つかればその要素のイテレータを返す
			* 見つからなければNULLが格納されたイテレータを返す
			* @return イテレータ
			*/
			iterator find(const T& val)
			{
				iterator it=first();
				while(it!=end())
				{
					if((*it)==val)return it;
					it++;
				}
				return iterator(NULL);
			}
			/**
			* @brief リストを初期化する
			*/
			void clear()
			{
				iterator it=first();
				while(it!=end())it=erase(it);
				//m_Size=0;
			}
			///@brief 先頭要素のイテレータ
			iterator first()const{return iterator(this->m_Dummy->getNext());}
			///@brief 末尾要素のイテレータ
			iterator last()const{return iterator(this->m_Dummy->getPrev());}
			///@briefダミーノードのしたイテレータ
			iterator end()const{return iterator(this->m_Dummy);}
			///@brief 先頭要素
			T front()const{return (*first());}
			///@brief 末尾要素
			T back()const{return (*last());}

			/**
			* @brief 末尾に要素を追加する
			* @param[in] val 要素
			*/
			void push_back(const T& val){insertFront(iterator(this->m_Dummy),val);}
			/**
			* @brief 先頭に要素を追加する
			* @param[in] val 要素
			*/
			void push_front(const T& val){insertBack(iterator(this->m_Dummy),val);}
			///@brief 末尾の要素を削除する
			void pop_back(){erase(last());}
			///@brief 先頭の要素を削除する
			void pop_front(){erase(first());}

			/**
			* @brief 入力したイテレータの後ろに要素を挿入する
			* @param[in] it 入力イテレータ
			* @param[in] elem 挿入する要素
			* @return 挿入した要素のイテレータ
			*/
			iterator insertBack(const iterator& it, const T& elem)
			{
				Node* m = new Node(elem, it.m_Node, it.m_Node->getNext());
				it.m_Node->getNext()->setPrev( m );
				it.m_Node->setNext( m );
				m_Size++;
				return iterator(m);
			}
			/**
			* @brief 入力したイテレータの前に要素を挿入する
			* @param[in] it 入力イテレータ
			* @param[in] elem 挿入する要素
			* @return 挿入した要素のイテレータ
			*/
			iterator insertFront(const iterator& it, const T& elem)
			{
				Node* m = new Node(elem, it.m_Node->getPrev(), it.m_Node);
				it.m_Node->getPrev()->setNext( m );
				it.m_Node->setPrev( m );
				m_Size++;
				return iterator(m);
			}
			/**
			* @brief イテレータをリストから削除する
			* @param[in] it イテレータ
			* @return 削除したイテレータの次のイテレータ
			*/
			iterator erase(const iterator& it)
			{
				if (it.m_Node == this->m_Dummy)
				{
					return iterator(NULL);
				}
				it.m_Node->getPrev()->setNext(it.m_Node->getNext());
				it.m_Node->getNext()->setPrev(it.m_Node->getPrev());
				iterator ret=iterator(it.m_Node->getNext());
				delete it.m_Node;
				m_Size--;
				return ret;
			}
			/**
			* @brief リストから要素をすべて削除する
			* @param[in] val 削除する要素
			*/
			void remove(const T& val)
			{
				iterator it=first();
				while(it!=end())
				{
					if((*it)==val)
					{
						it=erase(it);
					}
					else it++;
				}
			}
		};
	}
}