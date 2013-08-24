#pragma once
#include "..\..\..\defines.h"
#include "..\..\..\templateLib\kVector.h"

namespace klib
{
	namespace utility
	{
		class kBufferAnalyze
		{
		private:
			u32 m_BufferSize;
			char* mp_Bufferp;
			char* mp_Nowp;
			ktl::kVector<u32> m_Stackp;
		private:
			/**
			* @brief 引数で指定したサイズがバッファからはみ出ないかチェック
			* @param[in] pbuffer ポインタ
			* @param[in] buffersize バッファサイズ
			* @return はみ出たらtrue
			*/
			bool isOver(const char* pbuffer,s32 buffersize)const;
		public:
			kBufferAnalyze();
			kBufferAnalyze(char* pbuffer,u32 buffersize);
			~kBufferAnalyze();

			/**
			* @brief 現在位置から引数バッファが引数サイズ一致しているか判定
			* @param[in] pbuffer バッファポインタ
			* @param[in] buffersize バッファサイズ
			* @return 一致していたらtrue
			*/
			bool isEql(const char* pbuffer,u32 buffersize)const;
			/**
			* @brief ポインタを指定する
			* @param[in] pbuffer バッファ
			* @param[in] buffersize バッファサイズ
			*/
			void setBufferPointer(char* pbuffer,u32 buffersize);
			/**
			* @brief 現在の先頭ポインタからの相対位置を取得する
			* @return 相対位置
			*/
			u32 getPos()const;
			/**
			* @brief 先頭ポインタからの相対位置でポインタを指定する
			* @param[in] pos 相対位置
			* @return 指定に成功すればtrue
			*/
			bool setPos(u32 pos);
			/**
			* @brief ポインタをシークする
			* @param[in] offset 初期位置からのオフセット値
			* @param[in] origin シークする初期位置をしていする(fseekと同じ)
			* @return シークに成功すればtrue
			*/
			bool seek(s32 offset,s32 origin);
			/**
			* @brief 指定したバッファに指定したサイズ分読み込みポインタを進める(読み込みできなければポインタを進めずにfalseを返す)
			* @param[out] pbuffer バッファ
			* @param[in] buffersize バッファサイズ
			* @return 読み込みに成功すればtrue
			*/
			bool read(void* pbuffer,u32 buffersize);
			/**
			* @brief 指定したバッファに指定したサイズ分書き込みポインタを進める
			* @param[in] pbuffer バッファ
			* @param[in] buffersize バッファサイズ
			* @return 書き込みに成功すればtrue
			*/
			bool write(const void* pbuffer,u32 buffersize);
			u32 getBufferSize()const;
			/**
			* @brief 現在のポインタ位置をスタックにプッシュする
			*/
			void push();
			/**
			* @brief スタックからポインタ位置をポップする
			*/
			void pop();
		};
	}
}