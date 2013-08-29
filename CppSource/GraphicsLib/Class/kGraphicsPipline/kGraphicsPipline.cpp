#include "kGraphicsPipline.h"
#include "../kDevice/kDevice.h"
#include "../../../utility/Class/kBufferAnalyze/kBufferAnalyze.h"
#include "../../../utility\assetsLoader.h"
#include "../../../utility\kutility.h"
#include "../r2DObj/r2DObj.h"



namespace klib
{
	
		kTechnique::kTechnique()
		{
			//シェーダープログラムを作成
			m_Program=glCreateProgram();
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
		kTechnique::~kTechnique()
		{
			SAFE_DELETE(mp_VertexShader);
			SAFE_DELETE(mp_PixelShader);

			SAFE_DELETE(mp_BlendState);
			SAFE_DELETE(mp_DepthStencilState);
			SAFE_DELETE(mp_RasterizerState);
			//シェーダープログラムを解放する
			glDeleteProgram(m_Program);
			dprintf("delete kTechnique");
		}
		bool kTechnique::createVertexShader(const char* filename)
		{
			dprintf("createVertexShade\n");

			char* buffer;
			s32 buffersize;
			//アセットからシェーダーコードを読み込み
			AssetsLoader::load(&buffer, &buffersize, filename);
			//コードからシェーダーをコンパイルする
			if(!kDevice::createVertexShaderFromMemory(mp_VertexShader,buffer,buffersize))
			{
				eprintf("filename=%s",filename);
				eprintf("%s",buffer);
			}
			delete[] buffer;

			//	プログラムにアタッチ
			glAttachShader(m_Program,mp_VertexShader->m_ShaderID);
			return true;
		}
		bool kTechnique::createPixelShader(const char* filename)
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
		bool kTechnique::createBlendState(const eBlendType BlendStateType)
		{
			kDevice::createBlendStateAll(mp_BlendState,BlendStateType);
			return true;
		}
		bool kTechnique::createDepthStencilState(bool enable,eDepthFunc func)
		{
			kDevice::createDepthStencilState(mp_DepthStencilState,enable,func);
			return true;
		}
		bool kTechnique::createRasterizerState(eFillMode fill,eCullMode cull,bool front)
		{
			kDevice::createRasterizerState(mp_RasterizerState,fill,cull,front);
		}

		bool kTechnique::complete()
		{
			dprintf("complete\n");
			//プログラムをリンクする
			glLinkProgram(m_Program);
			return true;
		}
		bool kTechnique::setTechnique()
		{

			glUseProgram(m_Program);

			kDevice::OMsetBlendState(mp_BlendState);
			kDevice::OMsetDepthStencilState(mp_DepthStencilState);
			kDevice::RSsetState(mp_RasterizerState);

			return true;
		}
		void kTechnique::setShaderValue(const char* name,f32 val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform1f(ID,val);//値設定
		}
		void kTechnique::setShaderValue(const char* name,const f32* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform1fv(ID,len,val);//値設定
		}
		void kTechnique::setShaderValue(const char* name,s32 val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform1i(ID,val);//値設定
		}
		void kTechnique::setShaderValue(const char* name,const s32* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform1iv(ID,len,val);//値設定
		}
		void kTechnique::setShaderValue(const char* name,const math::Vector2& val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform2f(ID,val.x,val.y);//値設定
		}
		void kTechnique::setShaderValue(const char* name,const math::Vector2* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform2fv(ID,len,(f32*)val);//値設定
		}
		void kTechnique::setShaderValue(const char* name,const math::Vector3& val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform3f(ID,val.x,val.y,val.z);//値設定
		}
		void kTechnique::setShaderValue(const char* name,const math::Vector3* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform3fv(ID,len,(f32*)val);//値設定
		}
		void kTechnique::setShaderValue(const char* name,const math::Vector4& val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform4f(ID,val.x,val.y,val.z,val.w);//値設定
		}
		void kTechnique::setShaderValue(const char* name,const math::Vector4* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniform4fv(ID,len,(f32*)val);//値設定
		}
		void kTechnique::setShaderValue(const char* name,const math::Matrix& val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniformMatrix4fv(ID,1,GL_FALSE,(f32*)&val);//値設定
		}
		void kTechnique::setShaderValue(const char* name,const math::Matrix* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//シェーダでの名前を関連付ける
			glUniformMatrix4fv(ID,len,GL_FALSE,(f32*)val);//値設定
		}

		void kTechnique::setTexture(const char* name,u32 index,const rlib::r2DObj* tex)
		{
			GLenum texIndex = GL_TEXTURE0 + index;
			glActiveTexture(texIndex);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex->getTextureID());
			int loc = glGetUniformLocation(m_Program,name);
		  if( loc >= 0 )
		  {
			  glUniform1i(loc, index);
		  } else {
			  eprintf("Failure SetValue_No_BeginEnd name=%s", name);
		  }
		}

		void kTechnique::setTexture(const char* name,u32 index,const rlib::Texture* tex)
		{
			GLenum texIndex = GL_TEXTURE0 + index;
			glActiveTexture(texIndex);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex->getID());
			int loc = glGetUniformLocation(m_Program,name);
		  if( loc >= 0 )
		  {
			  glUniform1i(loc, index);
		  } else {
			  eprintf("Failure SetValue_No_BeginEnd name=%s", name);
		  }
		}

		kGraphicsPipline::kGraphicsPipline():kTechnique()
		{
			//頂点レイアウトを確保
			mp_InputLayout=new kInputLayout;
		}
		kGraphicsPipline::~kGraphicsPipline()
		{
			SAFE_DELETE(mp_InputLayout);
			dprintf("delete kGraphicsPipline");
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

}