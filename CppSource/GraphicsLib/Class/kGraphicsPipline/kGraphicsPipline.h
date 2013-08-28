#pragma once
#include "defines.h"
#include "../kShader/kShader.h"
#include "../kInputLayout/kInputLayout.h"
#include "math\kmathf.h"
#include "../kBlendState/kBlendState.h"
#include "../kRasterizerState/kRasterizerState.h"
#include "../kDepthStencilState/kDepthStencilState.h"

namespace klib
{
	class kGraphicsPipline
	{
		friend class kDevice;
	private:
		GLuint	m_Program;
		kInputLayout* mp_InputLayout;
		kShader* mp_VertexShader;
		kShader* mp_PixelShader;

		kBlendState* mp_BlendState;
		kDepthStencilState* mp_DepthStencilState;
		kRasterizerState* mp_RasterizerState;
	public:
		kGraphicsPipline();
		~kGraphicsPipline();
		/**
		* @brief 頂点シェーダーを読み込む
		* @param[in] filename ファイル名
		*/
		bool createVertexShader(const char* filename);
		/**
		* @brief ピクセルシェーダーを読み込む
		* @param[in] filename ファイル名
		*/
		bool createPixelShader(const char* filename);
		/**
		* @brief ブレンドステートを作成する
		* @param[in] BlendStateType ブレンドタイプ
		*/
		bool createBlendState(const eBlendType BlendStateType);
		/**
		* @brief デプスステンシルステートを作成する
		* @param[in] enable 深度テストを使用するか
		* @param[in] func 深度テスト比較関数
		*/
		bool createDepthStencilState(bool enable,eDepthFunc func);
		/**
		* @brief ラスタライザステートを作成する
		* @param[in] fill ポリゴン描画モード
		* @param[in] cull カリング向き
		* @param[in] front ポリゴン正面判定
		*/
		bool createRasterizerState(eFillMode fill,eCullMode cull,bool front);
		/**
		* @brief 頂点定義を指定してパイプラインを完成させる
		* @param[in] desc 頂点定義
		* @param[in] descsize 定義数
		*/
		bool complete(const kInputElementDesc* desc,u32 descsize);
		/**
		* @brief パイプラインを指定する
		*/
		bool setPipline();
		void setShaderValue(const char* name,f32 val);
		void setShaderValue(const char* name,const f32* val,u32 len);
		void setShaderValue(const char* name,s32 val);
		void setShaderValue(const char* name,const s32* val,u32 len);
		void setShaderValue(const char* name,const math::Vector2& val);
		void setShaderValue(const char* name,const math::Vector2* val,u32 len);
		void setShaderValue(const char* name,const math::Vector3& val);
		void setShaderValue(const char* name,const math::Vector3* val,u32 len);
		void setShaderValue(const char* name,const math::Vector4& val);
		void setShaderValue(const char* name,const math::Vector4* val,u32 len);
		void setShaderValue(const char* name,const math::Matrix& val);
		void setShaderValue(const char* name,const math::Matrix* val,u32 len);
	};
}