#pragma once
#include "defines.h"
#include "../kShader/kShader.h"
#include "../kObjectBuffer/kObjectBuffer.h"
#include "../kInputLayout/kInputLayout.h"
#include "../kGraphicsPipline/kGraphicsPipline.h"

namespace klib
{
	/**
	* @class デバイスクラス
	*/
	class kDevice
	{
	private:
	public:
		/**
		* @brief 頂点シェーダーをシェーダー文字列から作成する
		* @param[out] out 作成したシェーダー
		* @param[in] buffer シェーダーが記述された文字列
		*/
		static bool createVertexShaderFromMemory(kShader* out,const char* buffer,s32 length);
		/**
		* @brief ピクセルシェーダーをシェーダー文字列から作成する
		* @param[out] out 作成したシェーダー
		* @param[in] buffer シェーダーが記述された文字列
		*/
		static bool createPixelShaderFromMemory(kShader* out,const char* buffer,s32 length);
		/**
		* @brief 頂点レイアウトを作成する
		* @param[out] out 作成したレイアウト
		* @param[in] usePipline レイアウトを使用する描画パイプライン
		* @param[in] desc 頂点定義
		* @param[in] descsize 定義数
		*/
		static bool createInputLayout(kInputLayout* out,const kGraphicsPipline* usePipline,const kInputElementDesc* desc,u32 descsize);
		/**
		* @brief 頂点バッファーを作成する
		* @param[out] out 作成した頂点バッファ
		* @param[in] data バッファの初期値
		* @param[in] datasize バッファサイズ
		* @param[in] type バッファタイプ
		*/
		static bool createVertexBuffer(kObjectBuffer*out,const void* data,u32 datasize,s32 type);
		/**
		* @brief インデックスバッファーを作成する
		* @param[out] out 作成したインデックスバッファ
		* @param[in] data バッファの初期値
		* @param[in] datasize バッファサイズ
		* @param[in] type バッファタイプ
		*/
		static bool createIndexBuffer(kObjectBuffer* out,const void* data,u32 datasize,s32 type);
		/**
		* @brief バッファの内容を更新する
		* @param[in] in 更新するバッファ
		* @param[in] data 更新する値
		* @param[in] datasize 更新するサイズ
		*/
		static bool updateSubResource(const kObjectBuffer* in,const void* data,u32 datasize);
		/**
		* @brief 頂点レイアウトを設定する
		* @param[in] in 頂点レイアウト
		*/
		static bool IAsetInputLayout(const kInputLayout* in);
		/**
		* @brief 頂点バッファを設定する
		* @param[in] in 頂点レイアウト
		*/
		static bool IAsetVertexBuffer(const kObjectBuffer* in);
		/**
		* @brief インデックスバッファを設定する
		* @param[in] in 頂点レイアウト
		*/
		static bool IAsetIndexBuffer(const kObjectBuffer* in);
	};
}