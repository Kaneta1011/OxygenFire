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
		static bool createVertexShaderFromMemory(kShader* out,const char* buffer,s32 length)
		{
			out->m_ShaderID = glCreateShader(GL_VERTEX_SHADER);

			glShaderSource(out->m_ShaderID, 1, &buffer, &length);
			glCompileShader(out->m_ShaderID);
			return true;
		}
		/**
		* @brief ピクセルシェーダーをシェーダー文字列から作成する
		* @param[out] out 作成したシェーダー
		* @param[in] buffer シェーダーが記述された文字列
		*/
		static bool createPixelShaderFromMemory(kShader* out,const char* buffer,s32 length)
		{
			out->m_ShaderID = glCreateShader(GL_FRAGMENT_SHADER);

			glShaderSource(out->m_ShaderID, 1, &buffer, &length);
			glCompileShader(out->m_ShaderID);
			return true;
		}
		/**
		* @brief 頂点レイアウトを作成する
		* @param[out] out 作成したレイアウト
		* @param[in] usePipline レイアウトを使用する描画パイプライン
		* @param[in] desc 頂点定義
		* @param[in] descsize 定義数
		*/
		static bool createInputLayout(kInputLayout* out,const kGraphicsPipline* usePipline,const kInputElementDesc* desc,u32 descsize)
		{
			dprintf("createInputLayout\n");
			if(!out)return false;
			//頂点フォーマット情報を定義数確保する
			out->mp_Desc=new kInputLayoutDesc[descsize];
			out->m_TypeNum=descsize;
			out->m_VertexSize=0;
			for(int i=0;i<descsize;i++)
			{
				//シェーダロケーションを取得する
				out->mp_Desc[i].m_Location=glGetAttribLocation(usePipline->m_Program,desc[i].SemanticName);
				//フォーマット情報を取得する
				out->mp_Desc[i].m_Types.dxgi_format=PixelToFormat(desc[i].Format,&out->mp_Desc[i].m_Types.component,&out->mp_Desc[i].m_Types.size,&out->mp_Desc[i].m_Types.normalize);
				dprintf("SemanticName=%s\n",desc[i].SemanticName);
				dprintf("AttribLocation=%u\n",out->mp_Desc[i].m_Location);
				dprintf("SemanticSize=%u\n",out->mp_Desc[i].m_Types.size);
				dprintf("SemanticOffset=%u\n",out->m_VertexSize);
				out->m_VertexSize+=out->mp_Desc[i].m_Types.size;
			}
			//bit単位をbyte単位に変換
			out->m_VertexSize/=8;
			dprintf("InputLayoutSize=%ubyte\n",out->m_VertexSize);
			return true;
		}
		/**
		* @brief 頂点バッファーを作成する
		* @param[out] out 作成した頂点バッファ
		* @param[in] data バッファの初期値
		* @param[in] datasize バッファサイズ
		* @param[in] type バッファタイプ
		*/
		static bool createVertexBuffer(kObjectBuffer*out,const void* data,u32 datasize,s32 type)
		{
			dprintf("createVertexBuffer\n");
			//頂点バッファオブジェクトの作成
			glGenBuffers(1,&out->m_BO);
			glBindBuffer(GL_ARRAY_BUFFER,out->m_BO);
			glBufferData(GL_ARRAY_BUFFER,datasize,data,type);
			out->m_BufferSize=datasize;
			out->m_BufferType=GL_ARRAY_BUFFER;
			//バインド解除
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			return true;
		}
		/**
		* @brief インデックスバッファーを作成する
		* @param[out] out 作成したインデックスバッファ
		* @param[in] data バッファの初期値
		* @param[in] datasize バッファサイズ
		* @param[in] type バッファタイプ
		*/
		static bool createIndexBuffer(kObjectBuffer* out,const void* data,u32 datasize,s32 type)
		{
			dprintf("createIndexBuffer\n");
			//インデックスバッファオブジェクトの作成
			glGenBuffers(1, &out->m_BO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out->m_BO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, datasize,data, type);
			out->m_BufferSize=datasize;
			out->m_BufferType=GL_ELEMENT_ARRAY_BUFFER;
			//　バインドしたものをもどす
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			return true;
		}
		/**
		* @brief バッファの内容を更新する
		* @param[in] in 更新するバッファ
		* @param[in] data 更新する値
		* @param[in] datasize 更新するサイズ
		*/
		static bool updateSubResource(const kObjectBuffer* in,const void* data,u32 datasize)
		{
			glBindBuffer( in->m_BufferType, in->m_BO );
			glBufferSubData(in->m_BufferType, 0, datasize, data);  
			return true;
		}
		/**
		* @brief 頂点レイアウトを設定する
		* @param[in] in 頂点レイアウト
		*/
		static bool IAsetInputLayout(const kInputLayout* in)
		{
			u32 offset=0;
			const kInputLayoutDesc* desc=in->mp_Desc;
			for(int i=0;i<in->m_TypeNum;i++)
			{
				//現段階ではロケーションは0のみ
				glEnableVertexAttribArray(desc[i].m_Location);
				glVertexAttribPointer(desc[i].m_Location, desc[i].m_Types.component, desc[i].m_Types.dxgi_format, desc[i].m_Types.normalize, in->m_VertexSize, (GLvoid*)offset);
				//dprintf("Location %u\n",desc[i].m_Location);
				//dprintf("Component %u\n",desc[i].m_Types.component);
				//dprintf("VertexSize %u\n",in->m_VertexSize);
				//dprintf("Offset %u\n",offset);
				offset+=desc[i].m_Types.size;
			}
			return true;
		}
		/**
		* @brief 頂点バッファを設定する
		* @param[in] in 頂点レイアウト
		*/
		static bool IAsetVertexBuffer(const kObjectBuffer* in)
		{
			glBindBuffer( GL_ARRAY_BUFFER, in->m_BO );
			return true;
		}
		/**
		* @brief インデックスバッファを設定する
		* @param[in] in 頂点レイアウト
		*/
		static bool IAsetIndexBuffer(const kObjectBuffer* in)
		{
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, in->m_BO );
			return true;
		}
	};
}