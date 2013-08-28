#include "kGraphicsPipline.h"
#include "../kDevice/kDevice.h"
#include "../../../utility/Class/kBufferAnalyze/kBufferAnalyze.h"
#include "../../../utility\assetsLoader.h"
#include "../../../utility\kutility.h"

namespace klib
{
		kGraphicsPipline::kGraphicsPipline()
		{
			//シェーダープログラムを作成
			m_Program=glCreateProgram();
			//頂点レイアウトを確保
			mp_InputLayout=new kInputLayout;
			//頂点シェーダーを確保
			mp_VertexShader=new kShader;
			//ピクセルシェーダーを確保
			mp_PixelShader=new kShader;

			//ブレンドステートを確保
			mp_BlendState=new kBlendState;
			//深度ステートを確保
			mp_DepthStencilState=new kDepthStencilState;
			//ラスタライザステートを確保
			mp_RasterizerState=new kRasterizerState;
		}
		kGraphicsPipline::~kGraphicsPipline()
		{
			SAFE_DELETE(mp_InputLayout);
			SAFE_DELETE(mp_VertexShader);
			SAFE_DELETE(mp_PixelShader);

			SAFE_DELETE(mp_BlendState);
			SAFE_DELETE(mp_DepthStencilState);
			SAFE_DELETE(mp_RasterizerState);
			//シェーダープログラムを解放する
			glDeleteProgram(m_Program);
		}
		bool kGraphicsPipline::createVertexShader(const char* filename)
		{
			dprintf("createVertexShader\n");

			char* buffer;
			s32 buffersize;
			//アセットからシェーダーコードを読み込み
			AssetsLoader::load(&buffer, &buffersize, filename);
			//コードからシェーダーをコンパイルする
			kDevice::createVertexShaderFromMemory(mp_VertexShader,buffer,buffersize);
			delete[] buffer;

			//	プログラムにアタッチ
			glAttachShader(m_Program,mp_VertexShader->m_ShaderID);
			return true;
		}
		bool kGraphicsPipline::createPixelShader(const char* filename)
		{
			dprintf("createPixelShader\n");

			char* buffer;
			s32 buffersize;
			//アセットからシェーダーコードを読み込み
			AssetsLoader::load(&buffer, &buffersize, filename);
			//コードからシェーダーをコンパイルする
			kDevice::createPixelShaderFromMemory(mp_PixelShader,buffer,buffersize);
			delete[] buffer;

			//	プログラムにアタッチ
			glAttachShader(m_Program,mp_PixelShader->m_ShaderID);
			return true;
		}
		bool kGraphicsPipline::createBlendState(const eBlendType BlendStateType)
		{
			kDevice::createBlendStateAll(mp_BlendState,BlendStateType);
			return true;
		}
		bool kGraphicsPipline::createDepthStencilState(bool enable,eDepthFunc func)
		{
			kDevice::createDepthStencilState(mp_DepthStencilState,enable,func);
			return true;
		}
		bool kGraphicsPipline::createRasterizerState(eFillMode fill,eCullMode cull,bool front)
		{
			kDevice::createRasterizerState(mp_RasterizerState,fill,cull,front);
		}
		bool kGraphicsPipline::complete(const kInputElementDesc* desc,u32 descsize)
		{
			dprintf("complete\n");
			//プログラムをリンクする
			glLinkProgram(m_Program);
			//頂点レイアウトを定義から作成する
			kDevice::createInputLayout(mp_InputLayout,this,desc,descsize);
			return true;
		}
		bool kGraphicsPipline::setPipline()
		{

			glUseProgram(m_Program);

			kDevice::IAsetInputLayout(mp_InputLayout);
			kDevice::OMsetBlendState(mp_BlendState);
			kDevice::OMsetDepthStencilState(mp_DepthStencilState);
			kDevice::RSsetState(mp_RasterizerState);

			return true;
		}
		void kGraphicsPipline::setShaderValue(const char* name,f32 val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform1f(ID,val);//値設定
		}
		void kGraphicsPipline::setShaderValue(const char* name,const f32* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform1fv(ID,len,val);//値設定
		}
		void kGraphicsPipline::setShaderValue(const char* name,s32 val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform1i(ID,val);//値設定
		}
		void kGraphicsPipline::setShaderValue(const char* name,const s32* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform1iv(ID,len,val);//値設定
		}
		void kGraphicsPipline::setShaderValue(const char* name,const math::Vector2& val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform2f(ID,val.x,val.y);//値設定
		}
		void kGraphicsPipline::setShaderValue(const char* name,const math::Vector2* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform2fv(ID,len,(f32*)val);//値設定
		}
		void kGraphicsPipline::setShaderValue(const char* name,const math::Vector3& val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform3f(ID,val.x,val.y,val.z);//値設定
		}
		void kGraphicsPipline::setShaderValue(const char* name,const math::Vector3* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform3fv(ID,len,(f32*)val);//値設定
		}
		void kGraphicsPipline::setShaderValue(const char* name,const math::Vector4& val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform4f(ID,val.x,val.y,val.z,val.w);//値設定
		}
		void kGraphicsPipline::setShaderValue(const char* name,const math::Vector4* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform4fv(ID,len,(f32*)val);//値設定
		}
		void kGraphicsPipline::setShaderValue(const char* name,const math::Matrix& val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniformMatrix4fv(ID,1,GL_FALSE,(f32*)&val);//値設定
		}
		void kGraphicsPipline::setShaderValue(const char* name,const math::Matrix* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniformMatrix4fv(ID,len,GL_FALSE,(f32*)val);//値設定
		}
}