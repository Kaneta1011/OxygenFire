#pragma once
#include "..\kBufferAnalyze\kBufferAnalyze.h"

namespace klib
{
	namespace utility
	{

		class kTextAnalyze
		{
		private:
			kBufferAnalyze* m_Buffer;
		private:
			int searchTop( void );
		public:
			kTextAnalyze();
			kTextAnalyze(char* pbuffer,u32 buffersize);
			~kTextAnalyze();
			/**
			* @brief ポインタを指定する
			* @param[in] pbuffer バッファ
			* @param[in] buffersize バッファサイズ
			*/
			void setTextPointer(char* pbuffer,u32 buffersize);
			/**
			* @brief テキストから文字列を読み取る
			* @param[out] str 読み取った文字列
			* @return 取得に成功すればtrue
			*/
			bool loadString(char* str);
			/**
			* @brief テキストから小数を読み取る
			* @return 読み取った小数
			*/
			float LoadFloat();
			/**
			* @brief テキストから符号付整数を読み取る
			* @return 読み取った符号付整数
			*/
			int LoadInt();
			/**
			* @brief テキストポインタを次の行の先頭に移動する
			*/
			void	nextLine();
			/**
			* @brief 現在のポインタ位置をスタックにプッシュする
			*/
			void push();
			/**
			* @brief スタックからポインタ位置をポップする
			*/
			void pop();
			//*****************************************************************************
			//		検索系
			//*****************************************************************************
			/**
			* @brief テキストから文字列を検索する
			* 検索後発見できたら文字列の次の位置にポインタを移動する
			* 検索に失敗すれば検索を開始した位置にポインタを戻す
			* @param[in] str 検索する文字列
			* @return 発見できればtrue
			*/
			bool search(const char* str);
			/**
			* @brief テキストの行内から文字列を検索する
			* 検索後発見できたら文字列の次の位置にポインタを移動する
			* 検索に失敗すれば検索を開始した位置にポインタを戻す
			* @param[in] str 検索する文字列
			* @return 発見できればtrue
			*/
			bool	searchLine( const char* str );
		};
	}
}