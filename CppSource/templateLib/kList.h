/**
* @file �o�����A�����X�g�N���X�w�b�_
*/
#pragma once

namespace klib
{
	namespace ktl
	{
		/**
		* @class �o�����A�����X�g�N���X
		*/
		template<class T>
		class kList
		{
		private:
			/**
			* @class �o�����A�����X�g���m�[�h�N���X
			*/
			class Node
			{
			private:
				///@brief �l
				T m_Val;
				///@brief �O�̃m�[�h�|�C���^
				Node* m_Prev;
				///@brief ���̃m�[�h�|�C���^
				Node* m_Next;
			public:
				/**
				* @brief �R���X�g���N�^
				* @param[in] val �l
				* @param[in] �O�̃|�C���^
				* @param[in] ���̃|�C���^
				*/
				Node(T val, Node* prev, Node* next)
				{
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
			};
		public:
			/**
			* @class �o�����A�����X�g���C�e���[�^�N���X
			*/
			class iterator
			{
				friend kList;
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
				T&    operator *(){ return m_Node->getValue(); }
			};
		private:
			/**
			* @brief �_�~�[�m�[�h
			* ���̃m�[�h�͎g�p���郊�X�g�̍Ō�̗v�f�Ɛ擪�̗v�f�ɂ��ꂼ��q�����Ă���
			*/
			Node* m_Dummy;
			///@brief ���X�g���̗v�f��
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
				//�G���[�`�F�b�N(���̎��_�ŗv�f����0�ɂȂ��Ă��Ȃ���ΐ݌v����������)
				if(m_Size!=0)
				{
					delete (void*)0xdeaddead;
				}
			}
			/**
			* @brief ���X�g���󂩔���
			* @return ��Ȃ�^
			*/
			bool empty()const{return m_Size==0;}
			/**
			* @brief ���X�g�̗v�f�����擾����
			* @return �v�f��
			*/
			unsigned long size()const{return m_Size;}
			/**
			* @brief ���X�g������v�f����������
			* ������΂��̗v�f�̃C�e���[�^��Ԃ�
			* ������Ȃ����NULL���i�[���ꂽ�C�e���[�^��Ԃ�
			* @return �C�e���[�^
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
			* @brief ���X�g������������
			*/
			void clear()
			{
				iterator it=first();
				while(it!=end())it=erase(it);
				//m_Size=0;
			}
			///@brief �擪�v�f�̃C�e���[�^
			iterator first()const{return iterator(this->m_Dummy->getNext());}
			///@brief �����v�f�̃C�e���[�^
			iterator last()const{return iterator(this->m_Dummy->getPrev());}
			///@brief�_�~�[�m�[�h�̂����C�e���[�^
			iterator end()const{return iterator(this->m_Dummy);}
			///@brief �擪�v�f
			T front()const{return (*first());}
			///@brief �����v�f
			T back()const{return (*last());}

			/**
			* @brief �����ɗv�f��ǉ�����
			* @param[in] val �v�f
			*/
			void push_back(const T& val){insertFront(iterator(this->m_Dummy),val);}
			/**
			* @brief �擪�ɗv�f��ǉ�����
			* @param[in] val �v�f
			*/
			void push_front(const T& val){insertBack(iterator(this->m_Dummy),val);}
			///@brief �����̗v�f���폜����
			void pop_back(){erase(last());}
			///@brief �擪�̗v�f���폜����
			void pop_front(){erase(first());}

			/**
			* @brief ���͂����C�e���[�^�̌��ɗv�f��}������
			* @param[in] it ���̓C�e���[�^
			* @param[in] elem �}������v�f
			* @return �}�������v�f�̃C�e���[�^
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
			* @brief ���͂����C�e���[�^�̑O�ɗv�f��}������
			* @param[in] it ���̓C�e���[�^
			* @param[in] elem �}������v�f
			* @return �}�������v�f�̃C�e���[�^
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
			* @brief �C�e���[�^�����X�g����폜����
			* @param[in] it �C�e���[�^
			* @return �폜�����C�e���[�^�̎��̃C�e���[�^
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
			* @brief ���X�g����v�f�����ׂč폜����
			* @param[in] val �폜����v�f
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