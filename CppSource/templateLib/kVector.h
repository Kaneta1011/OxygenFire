#pragma once
#include "defines.h"
#include "types.h"
namespace klib
{
	namespace ktl{
		template <class T>
		class kVector
		{
		private:
			int Size;
			int MaxSize;
			T* Data;
		public:
			kVector():Size(0)
			{
				MaxSize=32;
				Data=NEW T[MaxSize];
			}
			kVector(int size):Size(0)
			{
				MaxSize=size;
				Data=NEW T[MaxSize];
			}
			~kVector()
			{
				delete[] Data;
			}
			//�v�f����Ȃ�^��Ԃ�
			bool empty()
			{
				return (Size==0)?true:false;
			}
			//���݂̗v�f����Ԃ�
			int size()
			{
				return Size;
			}
			//�z��Ɋm�ۂł���v�f����Ԃ�
			int max_size()
			{
				return MaxSize;
			}
			//�v�f���N���A����
			void clear()
			{
				Size=0;
			}
			//�C���f�b�N�X����f�[�^�폜
			void erase(int index)
			{
				//���݂̗v�f�����傫����Ή������Ȃ�
				if(Size<=index)return;
				//�z����l�߂�
				memmove(&Data[index],&Data[index+1],sizeof(T)*(MaxSize-index-1));
				//�v�f�������炷
				Size--;

			}
			//�v�f���������錩����΃C���f�b�N�X��Ԃ��Ȃ����-1
			int find(const T& e)
			{
				for(int i=0;i<Size;i++)
				{
					if(Data[i]==e)return i;
				}
				return -1;
			}
			//�C��
			//�v�f����f�[�^���폜
			void remove(const T& e)
			{
				for(int i=0;i<Size;i++)
				{
					if(Data[i]==e)erase(i--);
				}
			}
			//�z��T�C�Y��ύX����(���݂̃T�C�Y��菬�����͂ł��Ȃ�
			bool resize(int size)
			{
				if(size<=MaxSize)return false;
				//�ꎞ�z��m��
				T* work=NEW T[MaxSize];
				//�ꎞ�z��Ƀf�[�^�R�s�[
				memcpy(work,Data,sizeof(T)*MaxSize);
				//�f�[�^�����
				delete[] Data;
				//�f�[�^�m��
				Data=NEW T[size];
				//�ꎞ�z�񂩂�f�[�^�ɃR�s�[
				memcpy(Data,work,sizeof(T)*MaxSize);
				//�ꎞ�z����
				delete[] work;
				//�ő�v�f����
				MaxSize=size;
				return true;
			}
			//��Ԑ擪�̗v�f��Ԃ�
			T top()
			{
				return Data[0];
			}
			//��ԍŌ�̗v�f��Ԃ�
			T back()
			{
				return Data[Size-1];
			}
			//��ԍŌ�̗v�f���폜����
			void pop_back()
			{
				if(Size==0)return;
				Size--;
			}
			//�z��̍Ō�ɗv�f��ǉ�����
			void push_back(const T& d)
			{
				if(MaxSize<=Size)resize(MaxSize*2);
				Data[Size]=d;
				Size++;
			}

			//[]���Z�q�I�y���[�^
			T& operator[](int index)
			{
				return Data[index];
			}

			//[]���Z�q�I�y���[�^
			T& operator()(int index)
			{
				if(MaxSize<=index)resize(index*2);
				return Data[index];
			}

			//=���Z�q�I�y���[�^
			kVector<T>& operator=(const kVector<T> d)
			{
				delete[] this->Data;
				this->Size=d.size;
				this->MaxSize=d.MaxSize;
				this->Data=NEW T[this->MaxSize];
				memcpy(this->Data,d.Data,sizeof(T)*this->size);
				return *this;
			}
		};
	}
}