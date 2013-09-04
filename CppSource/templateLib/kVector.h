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
			//要素が空なら真を返す
			bool empty()
			{
				return (Size==0)?true:false;
			}
			//現在の要素数を返す
			int size()
			{
				return Size;
			}
			//配列に確保できる要素数を返す
			int max_size()
			{
				return MaxSize;
			}
			//要素をクリアする
			void clear()
			{
				Size=0;
			}
			//インデックスからデータ削除
			void erase(int index)
			{
				//現在の要素数より大きければ何もしない
				if(Size<=index)return;
				//配列を詰める
				memmove(&Data[index],&Data[index+1],sizeof(T)*(MaxSize-index-1));
				//要素数を減らす
				Size--;

			}
			//要素を検索する見つければインデックスを返すなければ-1
			int find(const T& e)
			{
				for(int i=0;i<Size;i++)
				{
					if(Data[i]==e)return i;
				}
				return -1;
			}
			//イン
			//要素からデータを削除
			void remove(const T& e)
			{
				for(int i=0;i<Size;i++)
				{
					if(Data[i]==e)erase(i--);
				}
			}
			//配列サイズを変更する(現在のサイズより小さくはできない
			bool resize(int size)
			{
				if(size<=MaxSize)return false;
				//一時配列確保
				T* work=NEW T[MaxSize];
				//一時配列にデータコピー
				memcpy(work,Data,sizeof(T)*MaxSize);
				//データを解放
				delete[] Data;
				//データ確保
				Data=NEW T[size];
				//一時配列からデータにコピー
				memcpy(Data,work,sizeof(T)*MaxSize);
				//一時配列解放
				delete[] work;
				//最大要素数を
				MaxSize=size;
				return true;
			}
			//一番先頭の要素を返す
			T top()
			{
				return Data[0];
			}
			//一番最後の要素を返す
			T back()
			{
				return Data[Size-1];
			}
			//一番最後の要素を削除する
			void pop_back()
			{
				if(Size==0)return;
				Size--;
			}
			//配列の最後に要素を追加する
			void push_back(const T& d)
			{
				if(MaxSize<=Size)resize(MaxSize*2);
				Data[Size]=d;
				Size++;
			}

			//[]演算子オペレータ
			T& operator[](int index)
			{
				return Data[index];
			}

			//[]演算子オペレータ
			T& operator()(int index)
			{
				if(MaxSize<=index)resize(index*2);
				return Data[index];
			}

			//=演算子オペレータ
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