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

			//ハッシュノード構造体(リスト構造)
			class Node
			{
				friend class kHash;
			private:
				//要素値
				T* _val;
				//オリジナルキー
				char* _OriginalKey;
				/////////ハッシュリスト(上に追加されていく)/////////////
				//前のハッシュノード
				Node* _NodePrev;
				//次のハッシュノード
				Node* _NodeNext;
				/////////全体リスト(下に追加されていく)/////////////
				//一つ前のイテレータ
				Node* _ItePrev;
				//一つ後のイテレータ
				Node* _IteNext;
			public:
				Node(const char* k,u32 keylen,Node* nodenext,Node* iteprev)
				{
					_val=NEW T;
					//オリジナルキーを格納する配列確保
					_OriginalKey=NEW char[keylen+1];
					//オリジナルキー取得
					strcpy_s(_OriginalKey,keylen+1,k);
					//ハッシュリスト連結先頭に追加する
					if(nodenext)nodenext->_NodePrev=this;
					_NodePrev=NULL;
					_NodeNext=nodenext;
					//全体リスト連結最後に追加する
					if(iteprev)iteprev->_IteNext=this;
					_ItePrev=iteprev;
					_IteNext=NULL;
				}
				~Node()
				{
					//要素解放
					delete _val;
					//キー文字列解放
					delete[] _OriginalKey;
					//全体リスト連結
					Node* itePrev=_ItePrev;
					Node* iteNext=_IteNext;
					//次の要素が存在すれば次の要素の前を前の要素に
					if(_IteNext)_IteNext->_ItePrev=itePrev;
					//前の要素が存在すれば前の要素の次を次の要素に
					if(_ItePrev)_ItePrev->_IteNext=iteNext;
					//ハッシュリスト連結
					Node* prev=_NodePrev;
					Node* next=_NodeNext;
					//次の要素が存在すれば次の要素の前を前の要素に
					if(next)next->_NodePrev=prev;
					//前の要素が存在すれば前の要素の次を次の要素に
					if(prev)prev->_NodeNext=next;

				}
			};
			class iterator
			{
				friend class kHash;
			private:

				Node* _Node;//処理の基準となるノードのポインタ
			public:

				//コンストラクタ
				iterator():_Node(0){}
				//デストラクタ
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
			//ハッシュテーブルのサイズの定義
			u32 KHashTable_TableSIZE;
		private:
			//要素数カウント
			u32 NumElement;
			//ハッシュノード構造体(リスト構造)
			Node** HashTable;
			Node*	_Begin;
			Node*	_End;



		public:
			//*****************************************************************************
			//	初期化・解放
			//*****************************************************************************
			kHash(u32 size=1999)
			{
				_Begin=NULL;
				_End=NULL;
				//データ数の初期化
				NumElement=0;
				KHashTable_TableSIZE=size;
				//ハッシュテーブル作成
				HashTable=NEW Node*[KHashTable_TableSIZE];
				//NULLポインタ格納しておく
				ZeroMemory(HashTable,sizeof(Node*)*KHashTable_TableSIZE);
			}
			~kHash()
			{
				clear();
				//ハッシュテーブル解放
				delete[] HashTable;
			}

			//*****************************************************************************
			//	ハッシュテーブル操作
			//*****************************************************************************
			//文字列からハッシュコードを作成する
			u32 CreateHash(const char *str) {

				u32 l = strlen(str);
				u32 h = l*31;
				u32 step = (l>>5)+1;
				u32 l1;
				for (l1=l; l1>=step; l1-=step)
					h = h ^ ((h<<5)+(h>>2)+str[l1-1]);
				return h% KHashTable_TableSIZE;
			}
			//ハッシュテーブルを初期化する
			void clear()
			{
				iterator it=Begin();
				//ノード解放
				while(it)
				{
					it=erase(it);
				}
				//データ数初期化
				NumElement=0;
				//NULLポインタ格納しておく
				ZeroMemory(HashTable,sizeof(Node*)*KHashTable_TableSIZE);
			}
			//ハッシュテーブルに格納されているデータ数
			u32 size()
			{
				return NumElement;
			}
			//要素存在確認
			//要素が存在していればtrue、要素が存在しなければfalse
			bool find(const char* key)
			{
				//ハッシュコード作成
				u32 hash=CreateHash(key);
				//ハッシュコードのポインタを取得する
				Node* n = HashTable[hash];
				//nに添字のデータがすでに存在しているか検索
				while(n!=NULL && strcmp(n->_OriginalKey,key))n=n->_NextNode;
				//NULLポインタなら
				if(n==NULL)return false;
				return true;
			}
			//要素削除
			//削除に成功すればtrue、要素が存在しなければfalse
			Node* erase(const char* key)
			{
				u32 hash=CreateHash(key);
				//ハッシュコードのポインタを取得する
				Node* n = HashTable[hash];
				//nに添字のデータがすでに存在しているか検索
				while(n!=NULL && strcmp(n->_OriginalKey,key))n=n->_NodeNext;
				//NULLポインタなら解放せずにヌルを返す
				if(n==NULL)return NULL;

				//全体リストの連結
				//全体リスト内の先頭なら次の要素を先頭にする
				if(_Begin==n)_Begin=n->_IteNext;
				//全体リスト内の終端なら前の要素を終端にする
				if(_End==n)_End=n->_ItePrev;

				//前の要素がなければテーブルの先頭を次のノードにする
				if(n->_NodePrev==NULL)HashTable[hash]=n->_NodeNext;

				//解放
				delete n;
				//全体要素数を減らす
				NumElement--;

				return next;
			}

			//*****************************************************************************
			//	外部繰り返し処理
			//*****************************************************************************
			//全体リスト内の最初の要素を返す
			Node* Begin()
			{
				return _Begin;
			}
			//全体リスト内の最後の要素を返す
			Node* End()
			{
				return _End;
			}

			//現在のイテレータ要素を削除する、要素が存在しなければNULLが返る
			Node* erase(const iterator& it)
			{
				//全体リストの連結
				//全体リスト内の先頭なら次の要素を先頭にする
				if(_Begin==it._Node)_Begin=it._Node->_IteNext;
				//全体リスト内の終端なら前の要素を終端にする
				if(_End==it._Node)_End=it._Node->_ItePrev;

				//前の要素がなければテーブルの先頭を次のノードにする
				if(it._Node->_NodePrev==NULL)HashTable[CreateHash(it._Node->_OriginalKey)]=it._Node->_NodeNext;

				//解放した後リスト内の次の要素を返す
				Node* ret=it._Node->_IteNext;
				delete it._Node;
				NumElement--;
				return ret;
			}

			//*****************************************************************************
			//	オペレータ
			//*****************************************************************************
			//[]演算子オペレータ
			T& operator[](const char* key)
			{
				//ハッシュコード作成
				u32 hash=CreateHash(key);
				//ハッシュコードのポインタを取得する
				Node* n = HashTable[hash];
				//nに添字のデータがすでに存在しているか検索
				while(n!=NULL && strcmp(n->_OriginalKey,key))n=n->_NodeNext;
				//NULLポインタなら
				if(n==NULL)
				{
					//添字のデータを先頭にしてリストを連結する
					HashTable[hash]=NEW Node(key,strlen(key),HashTable[hash],_End);

					if(!_Begin)_Begin=HashTable[hash];
					_End=HashTable[hash];

					//要素数カウントを増やす
					NumElement++;
					//先頭データの参照を返す
					T* ret=HashTable[hash]->_val;
					return *ret;
				}
				//こちらはデータが見つかったときにその添字データを返す
				T* ret=n->_val;
				return *ret;
			}

			//*****************************************************************************
			//	デバッグ
			//*****************************************************************************
			//デバッグ用出力関数(符号無し整数型のみ)
			void DebugProt()
			{
				printf("------------------------------------------------\n");
				printf("ハッシュテーブルに格納されているデータ\n");
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
				printf("ハッシュテーブルに格納されているデータ数\n");
				for(u32 i=0;i<KHashTable_TableSIZE;i++)
				{
					u32 size=0;
					Node* n = HashTable[i];
					while(n!=NULL){size++;n=n->_NodeNext;}
					printf("%u	:%u\n",i,size);
				}
				printf("------------------------------------------------\n");
				printf("先頭データ\n");
				if(_Begin)printf("%s\n",_Begin->_OriginalKey);
				else printf("NULL\n");
				printf("最終データ\n");
				if(_End)printf("%s\n",_End->_OriginalKey);
				else printf("NULL\n");
				printf("------------------------------------------------\n");
				printf("すべての要素数%u\n",NumElement);
				printf("------------------------------------------------\n");
			}
		};
	}
}