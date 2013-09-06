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
		* @brief shift-jis����
		* @param[in] code ���肷�镶���R�[�h
		* @return shift-jis�Ȃ�true
		*/
		inline bool IsDBCSLeadByte(unsigned char code)
		{
			return (code >= 0x81 && code <= 0x9F) || (code >= 0xE0 && code <= 0xFC) ? true : false;
		}
		/**
		* @brief �A���t�@�x�b�g�������͐�������
		* @param[in] c ���肷�镶���R�[�h
		* @return �A���t�@�x�b�g�������Ȃ�1����ȊO��0
		*/
		inline int isalnum(int c)
		{
			(('0'<=c && c<='9') || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) ? 1 : 0;
		}
	}
}