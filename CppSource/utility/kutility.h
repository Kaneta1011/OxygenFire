#pragma once
//#include "Class\kFile\kFile.h"
#include "Class\kBufferAnalyze\kBufferAnalyze.h"
#include "Class\kTextAnalyze\kTextAnalyze.h"

namespace klib
{
	namespace utility
	{
		inline void pathYenToSlash(char* temp)
		{
			while( *temp++ ){if( *temp == '\\' ){*temp= '/';}}
		}
		/**
		* @brief shift-jis判定
		* @param[in] code 判定する文字コード
		* @return shift-jisならtrue
		*/
		inline bool IsDBCSLeadByte(unsigned char code)
		{
			return (code >= 0x81 && code <= 0x9F) || (code >= 0xE0 && code <= 0xFC) ? true : false;
		}
		/**
		* @brief アルファベットもしくは数字判定
		* @param[in] c 判定する文字コード
		* @return アルファベットか数字なら1それ以外は0
		*/
		inline int isalnum(int c)
		{
			(('0'<=c && c<='9') || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) ? 1 : 0;
		}
	}
}