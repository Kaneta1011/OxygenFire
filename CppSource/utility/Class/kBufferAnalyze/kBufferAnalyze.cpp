#include "kBufferAnalyze.h"
namespace klib
{
	namespace utility
	{
		kBufferAnalyze::kBufferAnalyze():mp_Bufferp(NULL),mp_Nowp(NULL),m_BufferSize(0){this->m_Stackp.clear();}
		kBufferAnalyze::kBufferAnalyze(char* pbuffer,u32 buffersize){setBufferPointer(pbuffer,buffersize);}
		kBufferAnalyze::~kBufferAnalyze(){}

		//*****************************************************************************
		//
		//		private
		//
		//*****************************************************************************

		bool kBufferAnalyze::isOver(const char* pbuffer,s32 buffersize)const
		{
			return ((pbuffer+buffersize<=this->mp_Bufferp+this->m_BufferSize) && (pbuffer+buffersize>=this->mp_Bufferp))?false:true;
		}
		//*****************************************************************************
		//
		//		public
		//
		//*****************************************************************************

		bool kBufferAnalyze::isEql(const char* pbuffer,u32 buffersize)const
		{
			return (0==memcmp(this->mp_Nowp,pbuffer,buffersize));
		}

		void kBufferAnalyze::setBufferPointer(char* pbuffer,u32 buffersize)
		{
			this->mp_Bufferp=this->mp_Nowp=pbuffer;
			this->m_BufferSize=buffersize;
			this->m_Stackp.clear();
		}
		u32 kBufferAnalyze::getPos()const{return this->mp_Nowp-this->mp_Bufferp;}
		bool kBufferAnalyze::setPos(u32 pos)
		{
			if(isOver(this->mp_Bufferp,pos))return false;
			this->mp_Nowp=this->mp_Bufferp+pos;
			return true;
		}
		bool kBufferAnalyze::seek(s32 offset,s32 origin)
		{
			switch(origin)
			{
			case SEEK_SET:
				if(isOver(this->mp_Bufferp,offset))return false;
				this->mp_Nowp=this->mp_Bufferp+offset;
				break;
			case SEEK_CUR:
				if(isOver(this->mp_Nowp,offset))return false;
				this->mp_Nowp+=offset;
				break;
			case SEEK_END:
				if(isOver(this->mp_Bufferp+this->m_BufferSize,offset))return false;
				this->mp_Nowp=this->mp_Bufferp+this->m_BufferSize+offset;
				break;
			};
			return true;
		}

		bool kBufferAnalyze::read(void* pbuffer,u32 buffersize)
		{
			if(isOver(this->mp_Nowp,buffersize))return false;
			memcpy(pbuffer,this->mp_Nowp,buffersize);
			this->mp_Nowp+=buffersize;
			return true;
		}

		bool kBufferAnalyze::write(const void* pbuffer,u32 buffersize)
		{
			if(isOver(this->mp_Nowp,buffersize))return false;
			memcpy(this->mp_Nowp,pbuffer,buffersize);
			this->mp_Nowp+=buffersize;
			return true;
		}
		u32 kBufferAnalyze::getBufferSize()const{return this->m_BufferSize;}
		void kBufferAnalyze::push()
		{
			m_Stackp.push_back(this->getPos());
		}
		void kBufferAnalyze::pop()
		{
			this->setPos(m_Stackp.back());
			m_Stackp.pop_back();
		}
	}
}