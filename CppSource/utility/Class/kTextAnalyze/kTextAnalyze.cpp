#include "kTextAnalyze.h"
#include "../../kutility.h"

namespace klib
{
	namespace utility
	{
		kTextAnalyze::kTextAnalyze():m_Buffer(new kBufferAnalyze)
		{
		}
		kTextAnalyze::kTextAnalyze(char* pbuffer,u32 buffersize):m_Buffer(new kBufferAnalyze(pbuffer,buffersize))
		{
		}
		kTextAnalyze::~kTextAnalyze()
		{
			delete m_Buffer;
		}
		//*****************************************************************************
		//
		//		private
		//
		//*****************************************************************************
		int	kTextAnalyze::searchTop( void )
		{
			while(1){
				unsigned char ch;
				if(!m_Buffer->read((char*)&ch,1)){return -1;}

				if( isalnum(ch) ){m_Buffer->seek(-1,SEEK_CUR);break;}
				if( IsDBCSLeadByte(ch) ){m_Buffer->seek(-1,SEEK_CUR);break;}

				if( ch == '{' ) break;
				if( ch == '}' ) break;
				if( ch == '"' ) return 1;
				if( ch == '.' ) break;
				if( ch == '-' ) break;
				if( ch == '=' ) break;
			}
			return	0;
		}
		//*****************************************************************************
		//
		//		public
		//
		//*****************************************************************************
		void kTextAnalyze::setTextPointer(char* pbuffer,u32 buffersize)
		{
			m_Buffer->setBufferPointer(pbuffer,buffersize);
		}

		bool kTextAnalyze::loadString(char* str)
		{
			char* back=str;
			int mode=searchTop();

			while(1)
			{
				unsigned char ch;
				if(!m_Buffer->read((char*)&ch,1))
				{
					*str='\0';
					return false;
				}
				if( !IsDBCSLeadByte(ch)){
					if(mode==0)
					{
						if( ch == ';' ) break;
						if( ch == ')' ) break;
						if( ch == '(' ) break;
						if( ch == ',' ) break;
						if( ch == ' ' ) break;
						if( ch == '\t' ) break;
						if( ch == 0x0d ) break;
						if( ch == 0x0a ) break;
					}
					else
					{
						if( ch == '"' ) break;
					}
				}
				*str=ch;
				str++;
			}

			*str='\0';
			return true;
		}

		float kTextAnalyze::LoadFloat()
		{
			float	ret;
			char	temp[20];
			loadString(temp);

			ret = (float)atof(temp);
			return ret;
		}

		int kTextAnalyze::LoadInt()
		{
			int		ret;
			char	temp[20];
			loadString(temp);

			ret = atoi(temp);
			return ret;
		}


		void	kTextAnalyze::nextLine()
		{
			while(1)
			{
				unsigned char ch;
				if(!m_Buffer->read((char*)&ch,1))return;
				if( ch == '\n' ) break;
				if( ch == '\r' ){m_Buffer->seek(1,SEEK_CUR);break;}
			}
		}
		void kTextAnalyze::push(){m_Buffer->push();}
		void kTextAnalyze::pop(){m_Buffer->pop();}
		bool kTextAnalyze::search(const char* str)
		{
			char temp[256];
			u32 back=m_Buffer->getPos();
			while(loadString(temp))
			{
				//文字列発見！
				if(!strcmp(temp,str))return true;
			}
			//文字列発見失敗
			m_Buffer->setPos(back);
			return false;
		}
		bool	kTextAnalyze::searchLine( const char* str )
		{
			char	temp[256];
			u32 back=m_Buffer->getPos();

			//	行頭設定
			while(1)
			{
				unsigned char ch;
				//ポインタを一つ戻る
				if(!m_Buffer->seek(-1,SEEK_CUR)){m_Buffer->seek(0,SEEK_SET);break;}
				//バッファから1byte読み込み
				if(!m_Buffer->read((char*)&ch,1))break;
				//改行コードを取得すれば行頭
				if( ch == '\n' || ch == '\r' )break;
				//行頭でなければバッファから読み取った時にポインタが進むため戻る
				else m_Buffer->seek(-1,SEEK_CUR);
			}

			//	行内検索
			while(1)
			{
				unsigned char ch;
				if(!m_Buffer->read((char*)&ch,1))break;
				if( ch == '\n' || ch == '\r' )break;
				m_Buffer->seek(-1,SEEK_CUR);
				loadString(temp);
				//文字列発見！
				if( strcmp( temp, str ) == 0 ) return true;
			}
			//文字列発見失敗
			m_Buffer->setPos(back);
			return false;
		}
	}
}