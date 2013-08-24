#pragma once
#include "..\defines.h"
#include "..\types.h"
namespace klib
{
	namespace ktl
	{

		//UINT32
#ifndef u32
#define	u32		unsigned long
#endif

		template <class T>
		class kHash
		{
		public:

			//�n�b�V���m�[�h�\����(���X�g�\��)
			class Node
			{
				friend class kHash;
			private:
				//�v�f�l
				T* _val;
				//�I���W�i���L�[
				char* _OriginalKey;
				/////////�n�b�V�����X�g(��ɒǉ�����Ă���)/////////////
				//�O�̃n�b�V���m�[�h
				Node* _NodePrev;
				//���̃n�b�V���m�[�h
				Node* _NodeNext;
				/////////�S�̃��X�g(���ɒǉ�����Ă���)/////////////
				//��O�̃C�e���[�^
				Node* _ItePrev;
				//���̃C�e���[�^
				Node* _IteNext;
			public:
				Node(const char* k,u32 keylen,Node* nodenext,Node* iteprev)
				{
					_val=NEW T;
					//�I���W�i���L�[���i�[����z��m��
					_OriginalKey=NEW char[keylen+1];
					//�I���W�i���L�[�擾
					strcpy_s(_OriginalKey,keylen+1,k);
					//�n�b�V�����X�g�A���擪�ɒǉ�����
					if(nodenext)nodenext->_NodePrev=this;
					_NodePrev=NULL;
					_NodeNext=nodenext;
					//�S�̃��X�g�A���Ō�ɒǉ�����
					if(iteprev)iteprev->_IteNext=this;
					_ItePrev=iteprev;
					_IteNext=NULL;
				}
				~Node()
				{
					//�v�f���
					delete _val;
					//�L�[��������
					delete[] _OriginalKey;
					//�S�̃��X�g�A��
					Node* itePrev=_ItePrev;
					Node* iteNext=_IteNext;
					//���̗v�f�����݂���Ύ��̗v�f�̑O��O�̗v�f��
					if(_IteNext)_IteNext->_ItePrev=itePrev;
					//�O�̗v�f�����݂���ΑO�̗v�f�̎������̗v�f��
					if(_ItePrev)_ItePrev->_IteNext=iteNext;
					//�n�b�V�����X�g�A��
					Node* prev=_NodePrev;
					Node* next=_NodeNext;
					//���̗v�f�����݂���Ύ��̗v�f�̑O��O�̗v�f��
					if(next)next->_NodePrev=prev;
					//�O�̗v�f�����݂���ΑO�̗v�f�̎������̗v�f��
					if(prev)prev->_NodeNext=next;

				}
			};
			class iterator
			{
				friend class kHash;
			private:

				Node* _Node;//�����̊�ƂȂ�m�[�h�̃|�C���^
			public:

				//�R���X�g���N�^
				iterator():_Node(0){}
				//�f�X�g���N�^
				~iterator(){}

				operator bool(){return _Node!=NULL;}

				iterator( Node* node ){ _Node = node; }
				void operator = ( Node* node ){ _Node = node; }

				Node* operator ++ () { if( _Node )_Node= _Node->_IteNext; return _Node; }
				Node* operator ++(int){ if( _Node )_Node = _Node->_IteNext; return _Node; }

				Node* operator -- () { if( _Node ) _Node = _Node->_ItePrev; return _Node; }
				Node* operator--(int){ if( _Node ) _Node = _Node->_ItePrev; return _Node; }

				T&    operator *(){ return (T&)*_Node->_val; }

			};

		private:
			//�n�b�V���e�[�u���̃T�C�Y�̒�`
			u32 KHashTable_TableSIZE;
		private:
			//�v�f���J�E���g
			u32 NumElement;
			//�n�b�V���m�[�h�\����(���X�g�\��)
			Node** HashTable;
			Node*	_Begin;
			Node*	_End;



		public:
			//*****************************************************************************
			//	�������E���
			//*****************************************************************************
			kHash(u32 size=1999)
			{
				_Begin=NULL;
				_End=NULL;
				//�f�[�^���̏�����
				NumElement=0;
				KHashTable_TableSIZE=size;
				//�n�b�V���e�[�u���쐬
				HashTable=NEW Node*[KHashTable_TableSIZE];
				//NULL�|�C���^�i�[���Ă���
				ZeroMemory(HashTable,sizeof(Node*)*KHashTable_TableSIZE);
			}
			~kHash()
			{
				clear();
				//�n�b�V���e�[�u�����
				delete[] HashTable;
			}

			//*****************************************************************************
			//	�n�b�V���e�[�u������
			//*****************************************************************************
			//�����񂩂�n�b�V���R�[�h���쐬����
			u32 CreateHash(const char *str) {

				u32 l = strlen(str);
				u32 h = l*31;
				u32 step = (l>>5)+1;
				u32 l1;
				for (l1=l; l1>=step; l1-=step)
					h = h ^ ((h<<5)+(h>>2)+str[l1-1]);
				return h% KHashTable_TableSIZE;
			}
			//�n�b�V���e�[�u��������������
			void clear()
			{
				iterator it=Begin();
				//�m�[�h���
				while(it)
				{
					it=erase(it);
				}
				//�f�[�^��������
				NumElement=0;
				//NULL�|�C���^�i�[���Ă���
				ZeroMemory(HashTable,sizeof(Node*)*KHashTable_TableSIZE);
			}
			//�n�b�V���e�[�u���Ɋi�[����Ă���f�[�^��
			u32 size()
			{
				return NumElement;
			}
			//�v�f���݊m�F
			//�v�f�����݂��Ă����true�A�v�f�����݂��Ȃ����false
			bool find(const char* key)
			{
				//�n�b�V���R�[�h�쐬
				u32 hash=CreateHash(key);
				//�n�b�V���R�[�h�̃|�C���^���擾����
				Node* n = HashTable[hash];
				//n�ɓY���̃f�[�^�����łɑ��݂��Ă��邩����
				while(n!=NULL && strcmp(n->_OriginalKey,key))n=n->_NextNode;
				//NULL�|�C���^�Ȃ�
				if(n==NULL)return false;
				return true;
			}
			//�v�f�폜
			//�폜�ɐ��������true�A�v�f�����݂��Ȃ����false
			Node* erase(const char* key)
			{
				u32 hash=CreateHash(key);
				//�n�b�V���R�[�h�̃|�C���^���擾����
				Node* n = HashTable[hash];
				//n�ɓY���̃f�[�^�����łɑ��݂��Ă��邩����
				while(n!=NULL && strcmp(n->_OriginalKey,key))n=n->_NodeNext;
				//NULL�|�C���^�Ȃ��������Ƀk����Ԃ�
				if(n==NULL)return NULL;

				//�S�̃��X�g�̘A��
				//�S�̃��X�g���̐擪�Ȃ玟�̗v�f��擪�ɂ���
				if(_Begin==n)_Begin=n->_IteNext;
				//�S�̃��X�g���̏I�[�Ȃ�O�̗v�f���I�[�ɂ���
				if(_End==n)_End=n->_ItePrev;

				//�O�̗v�f���Ȃ���΃e�[�u���̐擪�����̃m�[�h�ɂ���
				if(n->_NodePrev==NULL)HashTable[hash]=n->_NodeNext;

				//���
				delete n;
				//�S�̗v�f�������炷
				NumElement--;

				return next;
			}

			//*****************************************************************************
			//	�O���J��Ԃ�����
			//*****************************************************************************
			//�S�̃��X�g���̍ŏ��̗v�f��Ԃ�
			Node* Begin()
			{
				return _Begin;
			}
			//�S�̃��X�g���̍Ō�̗v�f��Ԃ�
			Node* End()
			{
				return _End;
			}

			//���݂̃C�e���[�^�v�f���폜����A�v�f�����݂��Ȃ����NULL���Ԃ�
			Node* erase(const iterator& it)
			{
				//�S�̃��X�g�̘A��
				//�S�̃��X�g���̐擪�Ȃ玟�̗v�f��擪�ɂ���
				if(_Begin==it._Node)_Begin=it._Node->_IteNext;
				//�S�̃��X�g���̏I�[�Ȃ�O�̗v�f���I�[�ɂ���
				if(_End==it._Node)_End=it._Node->_ItePrev;

				//�O�̗v�f���Ȃ���΃e�[�u���̐擪�����̃m�[�h�ɂ���
				if(it._Node->_NodePrev==NULL)HashTable[CreateHash(it._Node->_OriginalKey)]=it._Node->_NodeNext;

				//��������ナ�X�g���̎��̗v�f��Ԃ�
				Node* ret=it._Node->_IteNext;
				delete it._Node;
				NumElement--;
				return ret;
			}

			//*****************************************************************************
			//	�I�y���[�^
			//*****************************************************************************
			//[]���Z�q�I�y���[�^
			T& operator[](const char* key)
			{
				//�n�b�V���R�[�h�쐬
				u32 hash=CreateHash(key);
				//�n�b�V���R�[�h�̃|�C���^���擾����
				Node* n = HashTable[hash];
				//n�ɓY���̃f�[�^�����łɑ��݂��Ă��邩����
				while(n!=NULL && strcmp(n->_OriginalKey,key))n=n->_NodeNext;
				//NULL�|�C���^�Ȃ�
				if(n==NULL)
				{
					//�Y���̃f�[�^��擪�ɂ��ă��X�g��A������
					HashTable[hash]=NEW Node(key,strlen(key),HashTable[hash],_End);

					if(!_Begin)_Begin=HashTable[hash];
					_End=HashTable[hash];

					//�v�f���J�E���g�𑝂₷
					NumElement++;
					//�擪�f�[�^�̎Q�Ƃ�Ԃ�
					T* ret=HashTable[hash]->_val;
					return *ret;
				}
				//������̓f�[�^�����������Ƃ��ɂ��̓Y���f�[�^��Ԃ�
				T* ret=n->_val;
				return *ret;
			}

			//*****************************************************************************
			//	�f�o�b�O
			//*****************************************************************************
			//�f�o�b�O�p�o�͊֐�(�������������^�̂�)
			void DebugProt()
			{
				printf("------------------------------------------------\n");
				printf("�n�b�V���e�[�u���Ɋi�[����Ă���f�[�^\n");
				for(u32 i=0;i<KHashTable_TableSIZE;i++)
				{
					if(HashTable[i]==nullptr)printf("NULL\n");
					else
					{
						printf("%u\n",i);
						Node* n = HashTable[i];
						while(n!=NULL)
						{
							T* val=n->_val;
							printf("		%s	%10u\n",n->_OriginalKey,*val);
							n=n->_NodeNext;
						}
					}
				}
				printf("------------------------------------------------\n");
				printf("�n�b�V���e�[�u���Ɋi�[����Ă���f�[�^��\n");
				for(u32 i=0;i<KHashTable_TableSIZE;i++)
				{
					u32 size=0;
					Node* n = HashTable[i];
					while(n!=NULL){size++;n=n->_NodeNext;}
					printf("%u	:%u\n",i,size);
				}
				printf("------------------------------------------------\n");
				printf("�擪�f�[�^\n");
				if(_Begin)printf("%s\n",_Begin->_OriginalKey);
				else printf("NULL\n");
				printf("�ŏI�f�[�^\n");
				if(_End)printf("%s\n",_End->_OriginalKey);
				else printf("NULL\n");
				printf("------------------------------------------------\n");
				printf("���ׂĂ̗v�f��%u\n",NumElement);
				printf("------------------------------------------------\n");
			}
		};
	}
}