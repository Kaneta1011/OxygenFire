#ifndef _RUL_LIST_
#define _RUL_LIST_

#include <cassert>

#ifndef NULL
#define NULL 0
#endif

template <typename Type>
class SinglyLinkedList
{
public:
	class _Iterator;
	class _ConstIterator;

private:
	struct Node;

public:
	typedef Node			NODE;
	typedef _Iterator		Iterator;
	typedef _ConstIterator	ConstIterator;

public:
	SinglyLinkedList():pBgn(NULL),mSize(0){}
	~SinglyLinkedList(){clear();}

	void clear(){
		Iterator it = begin();
		while(!it.isEnd()){
			erase(it);
		}
		this->pBgn = NULL;
		this->mSize = 0;
	}

	void pushFront(Type data){
		NODE* add = new NODE();
		add->data = data;
		add->pNext = pBgn;
		this->pBgn = add;
		this->mSize++;
	}
	void insert(Iterator& front, Type data)
	{
		NODE* next = front.mData->pNext;
		NODE* add = new NODE();
		add->data = data;
		add->pNext = next;
		front.mData->pNext = add;
		this->mSize++;
	}

	void erase(NODE* data)
	{
		NODE* it = begin();
		NODE* prev = NULL;
		while(it){
			NODE* next = it->pNext;
			if( it == data ){
				delete it;
				if( prev )	prev->pNext = next;
				else		this->pBgn = next;
				this->mSize--;
				break;
			}
			prev = it;
			it = next;
		}
	}
	void erase(Iterator& it)
	{
		if( it.isEnd() ) return ;
		if( it.mPrev )	it.mPrev->pNext = it.mData->pNext;
		else			this->pBgn = it.mData->pNext;

		delete it.mData;

		if( it.mPrev )	it.mData = it.mPrev->pNext;
		else			it.mData = this->pBgn;

		this->mSize--;
	}

	int		indexOf(NODE* node)const{
		int index = 0;
		NODE* it = begin();
		while(it){
			if( node == it ) return index;
			index ++;
			it = it->pNext;
		}
		return -1;
	}

	Iterator		begin(){ return Iterator(*this);}
	ConstIterator	begin()const{ return ConstIterator(*this);}

	unsigned int	size()const{return this->mSize;}

public://イテレータの定義
	class _Iterator
	{
		friend class SinglyLinkedList;
	public:
		_Iterator(SinglyLinkedList& container):mContainer(container),mPrev(NULL),mData(mContainer._beginRaw()){}
		_Iterator(SinglyLinkedList& container, NODE* pos):mContainer(container),mPrev(NULL),mData(NULL)
		{
			NODE *it = mContainer._beginRaw(), *p = NULL;
			while(it){
				if( pos == it ){
					mPrev = p; mData = it;
					break;
				}
				it = it->pNext;
			}
		}

		void next(){
			assert( this->mData != NULL );
			this->mPrev = this->mData;
			this->mData = this->mData->pNext;
		}
		void prev(){
			assert( this->mPrev != NULL );
			this->mData = mPrev;
			this->mPrev = this->mData->pNext;
		}

		bool isBegin(){return this->mPrev == NULL; }
		bool isEnd(){ return this->mData == NULL; }

		Type& operator*(){return this->mData->data;}
		Type* operator->(){return &this->mData->data;}

		Type& operator++(){next();return mData->data;}
		Type& operator--(){prev();return mData->data;}

		Type& operator++(int){next();return mData->data;}
		Type& operator--(int){prev();return mData->data;}

	private:
		SinglyLinkedList& mContainer;
		NODE* mPrev;
		NODE* mData;
	};
	class _ConstIterator
	{
		friend class SinglyLinkedList;
	public:
		_ConstIterator(SinglyLinkedList& container):mContainer(container),mPrev(NULL),mData(mContainer._beginRaw()){}
		_ConstIterator(SinglyLinkedList& container, NODE* pos):mContainer(container),mPrev(NULL),mData(NULL)
		{
			NODE *it = mContainer._beginRaw(), *p = NULL;
			while(it){
				if( pos == it ){
					mPrev = p; mData = it;
					break;
				}
				it = it->pNext;
			}
		}

		void next(){
			assert( this->mData != NULL );
			this->mPrev = this->mData;
			this->mData = this->mData->pNext;
		}
		void prev(){
			assert( this->mPrev != NULL );
			this->mData = mPrev;
			this->mPrev = this->mData->pNext;
		}

		bool isBegin(){return this->mPrev == NULL; }
		bool isEnd(){ return this->mData == NULL; }

		const Type&		 operator*()const{return this->mData->data;}
		const Type*const operator->()const{return &this->mData->data;}

		const Type& operator++(){next();return mData->data;}
		const Type& operator--(){prev();return mData->data;}

		const Type& operator++(int){next();return mData->data;}
		const Type& operator--(int){prev();return mData->data;}

	private:
		SinglyLinkedList& mContainer;
		NODE* mPrev;
		NODE* mData;
	};

private://最下層のデータ構造
	struct Node{
		Node* pNext;
		Type data;

		Node():pNext(NULL){}
	};

private:
	NODE*				_beginRaw(){return pBgn;}
	const NODE*const	_beginRaw()const{return pBgn;}

private:
	NODE*	pBgn;
	unsigned int mSize;
};

#endif
