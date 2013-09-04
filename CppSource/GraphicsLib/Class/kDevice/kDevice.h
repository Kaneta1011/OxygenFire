#pragma once
#include "defines.h"
#include "../kShader/kShader.h"
#include "../kObjectBuffer/kObjectBuffer.h"
#include "../kInputLayout/kInputLayout.h"
#include "../kGraphicsPipline/kGraphicsPipline.h"
#include "../kBlendState/kBlendState.h"
#include "../kRasterizerState/kRasterizerState.h"
#include "../kDepthStencilState/kDepthStencilState.h"

namespace klib
{
	/**
	* @class デバイスクラス
	*/
	class kDevice
	{
	private:
		static const kInputLayout* m_IAInputLayout;
		static const kObjectBuffer* m_IAVertexBuffer;
		static const kObjectBuffer* m_IAIndexBuffer;

		static const kBlendState* m_OMBlendState;
		static const kDepthStencilState* m_OMDepthStencilState;
		static const kRasterizerState* m_RasterizerState;
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
		* @brief ブレンドステートを作成する(Directx11を想定した関数)
		* @param[out] out 作成したブレンドステート
		* @param[in] BlendStateType ブレンドタイプの配列
		* @param[in] BlendStateTypeLength 配列のサイズ
		*/
		static void createBlendState( kBlendState* out,const eBlendType* BlendStateType, u32 BlendStateTypeLength );
		/**
		* @brief ブレンドステートを作成する(Directx11を想定した関数)
		* @param[out] out 作成したブレンドステート
		* @param[in] BlendStateType ブレンドタイプ
		*/
		static void createBlendStateAll( kBlendState* out,const eBlendType BlendStateType);
		/**
		* @brief デプスステンシルステートを作成する
		* @param[out] out 作成したデプスステンシルステート
		* @param[in] enable 深度テストを使用するか
		* @param[in] func 深度テスト比較関数
		*/
		static bool createDepthStencilState(kDepthStencilState* out,bool enable,bool writeenable,eDepthFunc func);
		/**
		* @brief ラスタライザステートを作成する
		* @param[out] out 作成したラスタライザステート
		* @param[in] fill ポリゴン描画モード
		* @param[in] cull カリング向き
		* @param[in] front ポリゴン正面判定
		*/
		static bool createRasterizerState(kRasterizerState* out,eFillMode fill,eCullMode cull,bool front);
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
		/**
		* @brief ブレンドステートを設定する
		* @param[in] in ブレンドステート
		*/
		static bool OMsetBlendState(const kBlendState* in);
		/**
		* @brief デプスステンシルステートを設定する
		* @param[in] in デプスステンシルステート
		*/
		static bool OMsetDepthStencilState(const kDepthStencilState* in);
		/**
		* @brief ラスタライザステートを設定する
		* @param[in] in ラスタライザステート
		*/
		static bool RSsetState(const kRasterizerState* in);
		/**
		* @brief 設定しているインデックスバッファを使用して設定している頂点バッファを描画する
		* @param[in] indexnum インデックス数
		*/
		static bool drawIndexed(u32 indexnum);

	};

}