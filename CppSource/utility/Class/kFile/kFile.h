#pragma once
#include "..\..\..\defines.h"

namespace klib
{
	namespace utility
	{
		/**
		* @class ファイル入出力クラス
		*/
		class kFile
		{
		private:
			///@brief ファイルサイズ
			u32 m_FileSize;
			///@brief ファイルポインタ
			FILE* mp_Fp;
		public:
			kFile();
			/**
			* @brief パスとモードを指定してファイルを読み込む
			* @param[in] path ファイルパス
			* @param[in] mode 読み込みモード
			*/
			kFile(const char* path,const char* mode);
			~kFile();
			/**
			* @brief パスとモードを指定してファイルを読み込む
			* @param[in] path ファイルパス
			* @param[in] mode 読み込みモード
			* @return 読み込みに成功すればtrue
			*/
			bool open(const char* path,const char* mode);
			/**
			* @brief ファイルを閉じる
			*/
			void close();
			/**
			* @brief ファイルポインタをシークする
			* @param[in] offset 初期位置からのオフセット値
			* @param[in] origin シークする初期位置をしていする(fseekと同じ)
			* @return 成功すれば0
			*/
			int seek(s32 offset,s32 origin)const;
			/**
			* @brief ファイルからバッファに読み込む
			* @param[out] pbuffer 読み込むバッファ
			* @param[in] buffersize バッファのサイズ
			* @return 実際に読み込んだサイズ
			*/
			u32 read(char* pbuffer,u32 buffersize)const;
			/**
			* @brief バッファからファイルに書き込む
			* @param[in] pbuffer 書き込むバッファ
			* @param[in] buffersize バッファのサイズ
			* @return 実際に書き込んだサイズ
			*/
			u32 write(const char* pbuffer,u32 buffersize)const;
			/**
			* @brief 書式付でファイルに書き込む
			* @param[in] format フォーマット
			* @param[in] ... 可変長引数
			*/
			void printf(const char* format, ...)const;
			u32 getFileSize()const;
		};
	}
}