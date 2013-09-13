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
				///@brief �l
				T m_Val;
				///@brief �O�̃m�[�h�|�C���^
				Node* m_Prev;
				///@brief ���̃m�[�h�|�C���^
				Node* m_Next;
				Node* m_Brother;
			public:
				/**
				* @brief �R���X�g���N�^
				* @param[in] val �l
				* @param[in] �O�̃|�C���^
				* @param[in] ���̃|�C���^
				*/
				Node(U key,T val, Node* prev, Node* next)
				{
					this->m_Key=key;
					this->m_Val = val;
					this->m_Prev = prev;
					this->m_Next = next;
				}
				/**
				* @brief �l���擾����
				* @return �擾�����l
				*/
				T& getValue(){return this->m_Val;}
				/**
				* @brief �O�̃|�C���^���擾����
				* @return �擾�����|�C���^
				*/
				Node* getPrev()const{return this->m_Prev;}
				/**
				* @brief ���̃|�C���^���擾����
				* @return �擾�����|�C���^
				*/
				Node* getNext()const{return this->m_Next;}
				Node* getBrother()const{return this->m_Brother;}

				/**
				* @brief �l���Z�b�g����
				* @param[in] val �l
				*/
				void setValue(const T& val){this->m_Val=val;}
				/**
				* @brief ���̃|�C���^���Z�b�g����
				* @param[in] val �|�C���^
				*/
				void setPrev(Node* val){this->m_Prev=val;}
				/**
				* @brief �O�̃|�C���^���Z�b�g����
				* @param[in] val �|�C���^
				*/
				void setNext(Node* val){this->m_Next=val;}
				void setBrother(Node* val){this->m_Brother=val;}
			};
		public:
			class iterator
			{
				friend class kIAssociative;
			private:
				///@brief �C�e���[�^���m�[�h
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
				///@brief *���Z�q�Ńm�[�h���̒l���擾����
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
			* @brief ���͂����C�e���[�^�̌��ɗv�f��}������
			* @param[in] it ���̓C�e���[�^
			* @param[in] elem �}������v�f
			* @return �}�������v�f�̃C�e���[�^
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
			* @brief �C�e���[�^�����X�g����폜����
			* @param[in] it �C�e���[�^
			* @return �폜�����C�e���[�^�̎��̃C�e���[�^
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