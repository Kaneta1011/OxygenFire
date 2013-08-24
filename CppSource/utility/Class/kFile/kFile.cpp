#include "kFile.h"

namespace klib
{
	namespace utility
	{

		kFile::kFile():mp_Fp(NULL),m_FileSize(0){}
		kFile::kFile(const char* path,const char* mode):m_FileSize(0),mp_Fp(NULL)
		{
			bool ret=open(path,mode);
			//if(!ret)/*エラー処理*/;
		}
		kFile::~kFile()
		{
			close();
		}
		bool kFile::open(const char* path,const char* mode)
		{
			fopen_s(&mp_Fp,path,mode); 
			if(!mp_Fp)return false;
			//ファイルサイズを調査
			fseek(mp_Fp,0,SEEK_END); 
			s64 size;
			fgetpos(mp_Fp,&size); 
			m_FileSize=(u32)size;
			//ファイルポインタ位置を先頭にリセット
			fseek(mp_Fp,0,SEEK_SET); 
			return true;
		}
		void kFile::close()
		{
			if(!mp_Fp)return;
			fclose(mp_Fp);
			mp_Fp=NULL;
		}
		int kFile::seek(s32 offset,s32 origin)const
		{
			return fseek(mp_Fp,offset,origin);
		}
		u32 kFile::read(char* pbuffer,u32 buffersize)const
		{
			return fread_s(pbuffer,buffersize,buffersize,1,mp_Fp)*buffersize;
		}
		u32 kFile::write(const char* pbuffer,u32 buffersize)const
		{
			return fwrite(pbuffer,buffersize,1,mp_Fp)*buffersize;
		}
		void kFile::printf(const char* format, ...)const
		{
			va_list args;
			va_start( args, format );
			vfprintf_s(mp_Fp,format,args);
			va_end( args );
		}

		u32 kFile::getFileSize()const{return m_FileSize;}
	}
}