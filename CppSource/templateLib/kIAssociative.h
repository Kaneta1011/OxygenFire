#pragma once
namespace klib
{
	namespace ktl
	{
		template<class U,class T>
		class kIAssociative
		{
		private:
			class Node
			{
				friend class kIAssociative;
			private:
				U m_Key;
				///@brief 値
				T m_Val;
				///@brief 前のノードポインタ
				Node* m_Prev;
				///@brief 次のノードポインタ
				Node* m_Next;
				Node* m_Brother;
			public:
				/**
				* @brief コンストラクタ
				* @param[in] val 値
				* @param[in] 前のポインタ
				* @param[in] 次のポインタ
				*/
				Node(U key,T val, Node* prev, Node* next)
				{
					this->m_Key=key;
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
				Node* getBrother()const{return this->m_Brother;}

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
				void setBrother(Node* val){this->m_Brother=val;}
			};
		public:
			class iterator
			{
				friend class kIAssociative;
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
				T&    operator *(){ return m_Node->getBrother()->getValue(); }
			};
			Node** m_TableDummy;
			//nodeList* m_AssociativeTable;
			Node* m_ElementDummy;
			unsigned long m_ElementSize;
			//nodeList m_ElementList;
			unsigned long m_TableSize;
		public:
			/**
			* @brief 入力したイテレータの後ろに要素を挿入する
			* @param[in] it 入力イテレータ
			* @param[in] elem 挿入する要素
			* @return 挿入した要素のイテレータ
			*/
			void insertTable(const U& key,unsigned long hash,const T& elem)
			{
				Node* n = new Node(key,elem, m_ElementDummy, m_ElementDummy->getNext());
				m_ElementDummy->getNext()->setPrev( n );
				m_ElementDummy->setNext( n );

				Node* m = new Node(key,elem, m_TableDummy[hash], m_TableDummy[hash]->getNext());
				m_TableDummy[hash]->getNext()->setPrev( m );
				m_TableDummy[hash]->setNext( m );
				n->setBrother(m);
				m->setBrother(n);

				m_ElementSize++;
			}
			/**
			* @brief イテレータをリストから削除する
			* @param[in] it イテレータ
			* @return 削除したイテレータの次のイテレータ
			*/
			iterator erase(const iterator& it)
			{
				if (it.m_Node == this->m_ElementDummy)
				{
					return iterator(NULL);
				}
				it.m_Node->getPrev()->setNext(it.m_Node->getNext());
				it.m_Node->getNext()->setPrev(it.m_Node->getPrev());
				iterator ret=iterator(it.m_Node->getNext());
				delete it.m_Node->m_Brother;
				delete it.m_Node;
				m_ElementSize--;
				return ret;
			}
			kIAssociative():m_TableSize(1024),m_ElementSize(0)
			{
				m_TableDummy=new Node*[m_TableSize];
				for(unsigned long i=0;i<m_TableSize;i++)
				{
					this->m_TableDummy[i]=new Node(U(),T(),NULL,NULL);
					this->m_TableDummy[i]->setNext(this->m_TableDummy[i]);
					this->m_TableDummy[i]->setPrev(this->m_TableDummy[i]);
				}
				m_ElementSize=0;
				this->m_ElementDummy=new Node(U(),T(),NULL,NULL);
				this->m_ElementDummy->setNext(this->m_ElementDummy);
				this->m_ElementDummy->setPrev(this->m_ElementDummy);
			}
			kIAssociative(unsigned long size):m_TableSize(size),m_ElementSize(0)
			{
				m_TableDummy=new Node*[m_TableSize];
				for(unsigned long i=0;i<m_TableSize;i++)
				{
					this->m_TableDummy[i]=new Node(U(),T(),NULL,NULL);
					this->m_TableDummy[i]->setNext(this->m_TableDummy[i]);
					this->m_TableDummy[i]->setPrev(this->m_TableDummy[i]);
				}
				m_ElementSize=0;
				this->m_ElementDummy=new Node(U(),T(),NULL,NULL);
				this->m_ElementDummy->setNext(this->m_ElementDummy);
				this->m_ElementDummy->setPrev(this->m_ElementDummy);
			}
			~kIAssociative()
			{
				clear();
				for(unsigned long i=0;i<m_TableSize;i++)
				{
					delete m_TableDummy[i];
				}
				delete[] m_TableDummy;
				delete m_ElementDummy;
			}
			void clear()
			{
				iterator it=first();
				while(it!=end())
				{
					it=erase(it);
				}
			}
			iterator first()
			{
				return iterator(this->m_ElementDummy->getNext());
			}
			iterator last()
			{
				return iterator(this->m_ElementDummy->getPrev());
			}
			iterator end()
			{
				return iterator(this->m_ElementDummy);
			}

			iterator firstTable(unsigned long hash)
			{
				return iterator(this->m_TableDummy[hash]->getNext());
			}
			iterator lastTable(unsigned long hash)
			{
				return iterator(this->m_TableDummy[hash]->getPrev());
			}
			iterator endTable(unsigned long hash)
			{
				return iterator(this->m_TableDummy[hash]);
			}

			unsigned long size()const{return this->m_ElementSize;}

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
			T& operator[](const U& index)
			{
				unsigned int hash=(unsigned long)index%m_TableSize;
				iterator it=firstTable(hash);
				while(it!=endTable(hash))
				{
					if(it.m_Node->m_Key==index)
					{
						return it.m_Node->getValue();
					}
					it++;
				}
				insertTable(index,hash,T());
				return firstTable(hash).m_Node->getValue();
			}
		};
	}
}