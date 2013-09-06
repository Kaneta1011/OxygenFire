#include "kDevice.h"
#include "../kMesh/kMesh.h"

namespace klib
{
	thread::kMutex kDevice::m_Mutex;

	kDevice::LoadVertexBufferQueue kDevice::m_LoadVertexBufferQueue;
	kDevice::LoadIndexBufferQueue kDevice::m_LoadIndexBufferQueue;
	kDevice::LoadShaderQueue kDevice::m_LoadShaderQueue;
	kDevice::CreateInputLayoutQueue kDevice::m_CreateInputLayoutQueue;

	const kInputLayout* kDevice::m_IAInputLayout;
	const kObjectBuffer* kDevice::m_IAVertexBuffer;
	const kObjectBuffer* kDevice::m_IAIndexBuffer;

	const kBlendState* kDevice::m_OMBlendState;
	const kDepthStencilState* kDevice::m_OMDepthStencilState;
	const kRasterizerState* kDevice::m_RasterizerState;

	//デバイス遅延読み込みを開始する
	void kDevice::begin()
	{
		m_Mutex.lock();
		dprintf("kDevice begin");
		//キューに溜まった頂点バッファを削除する
		while(!m_LoadVertexBufferQueue.isEmpty())
		{
			LoadBufferInfo* p;
			m_LoadVertexBufferQueue.get(&p);
			delete p;
		}
		//キューに溜まったインデックスバッファを削除する
		while(!m_LoadIndexBufferQueue.isEmpty())
		{
			LoadBufferInfo* p;
			m_LoadIndexBufferQueue.get(&p);
			delete p;
		}
		//キューに溜まったシェーダーを削除する
		while(!m_LoadShaderQueue.isEmpty())
		{
			LoadShaderInfo* p;
			m_LoadShaderQueue.get(&p);
			delete p;
		}
		//キューに溜まったインプットレイアウトを削除する
		while(!m_CreateInputLayoutQueue.isEmpty())
		{
			CreateInputLayoutInfo* p;
			m_CreateInputLayoutQueue.get(&p);
			delete p;
		}
		m_LoadVertexBufferQueue.clear();
		m_LoadIndexBufferQueue.clear();
		m_LoadShaderQueue.clear();
		m_CreateInputLayoutQueue.clear();
	}
	//デバイス遅延読み込みを終了する
	void kDevice::end()
	{
		//キューに溜まった頂点バッファを作成する
		while(!m_LoadVertexBufferQueue.isEmpty())
		{
			LoadBufferInfo* p;
			m_LoadVertexBufferQueue.get(&p);
			//頂点バッファオブジェクトの作成
			glGenBuffers(1,&p->m_Buffer->m_BO);
			glBindBuffer(GL_ARRAY_BUFFER,p->m_Buffer->m_BO);
			glBufferData(GL_ARRAY_BUFFER,p->m_DataSize,p->m_Data,p->m_Type);
			p->m_Buffer->m_BufferSize=p->m_DataSize;
			p->m_Buffer->m_BufferType=GL_ARRAY_BUFFER;
			//バインド解除
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			delete p;
		}
		//キューに溜まったインデックスバッファを作成する
		while(!m_LoadIndexBufferQueue.isEmpty())
		{
			LoadBufferInfo* p;
			m_LoadIndexBufferQueue.get(&p);
			//頂点バッファオブジェクトの作成
			glGenBuffers(1,&p->m_Buffer->m_BO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,p->m_Buffer->m_BO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,p->m_DataSize,p->m_Data,p->m_Type);
			p->m_Buffer->m_BufferSize=p->m_DataSize;
			p->m_Buffer->m_BufferType=GL_ELEMENT_ARRAY_BUFFER;
			//バインド解除
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			delete p;
		}
		//キューに溜まったシェーダーをコンパイルする
		while(!m_LoadShaderQueue.isEmpty())
		{
			LoadShaderInfo* p;
			m_LoadShaderQueue.get(&p);
			//シェーダープログラムを作成
			p->m_Technique->m_Program=glCreateProgram();
			createVertexShaderFromMemory(p->m_Technique->mp_VertexShader,p->mp_VShader,p->m_VShaderLength);
			glAttachShader(p->m_Technique->m_Program,p->m_Technique->mp_VertexShader->m_ShaderID);

			createPixelShaderFromMemory(p->m_Technique->mp_PixelShader,p->mp_PShader,p->m_PShaderLength);
			glAttachShader(p->m_Technique->m_Program,p->m_Technique->mp_PixelShader->m_ShaderID);

			glLinkProgram(p->m_Technique->m_Program);
			delete p;
		}
		//キューに溜まったインプットレイアウトを作成する
		while(!m_CreateInputLayoutQueue.isEmpty())
		{
			CreateInputLayoutInfo* p;
			m_CreateInputLayoutQueue.get(&p);
			for(int i=0;i<p->m_InputLayout->m_TypeNum;i++)
			{
				dprintf("%s",p->m_SemanticName[i]);
				p->m_InputLayout->mp_Desc[i].m_Location=glGetAttribLocation(*p->m_Program,p->m_SemanticName[i]);
			}
			delete p;
		}
		dprintf("kDevice end");
		m_Mutex.unLock();
	}

	static bool isShaderError(GLenum shaderType,GLuint shader)
	{
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			//if(infoLen==0)return false;
			char* buf = new char[infoLen];
			glGetShaderInfoLog(shader, infoLen, NULL, buf);
			eprintf("Could not compile shader %d:\n%s\n",shaderType, buf);
			delete[] buf;

			glDeleteShader(shader);
			shader = 0;
		}
		else
		{
			dprintf("Shader compile success\n");
			return false;
		}
		return true;
	}

	bool kDevice::createVertexShaderFromMemory(kShader* out,const char* buffer,s32 length)
	{
		//シェーダを作成する
		out->m_ShaderID = glCreateShader(GL_VERTEX_SHADER);
		//シェーダソースをバインドする
		glShaderSource(out->m_ShaderID, 1, &buffer, &length);
		//バインドされたソースをコンパイルしてIDにバインドする
		glCompileShader(out->m_ShaderID);
		if(isShaderError(GL_VERTEX_SHADER,out->m_ShaderID))return false;
		return true;
	}

	bool kDevice::createPixelShaderFromMemory(kShader* out,const char* buffer,s32 length)
	{
		//シェーダを作成する
		out->m_ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		//シェーダソースをバインドする
		glShaderSource(out->m_ShaderID, 1, &buffer, &length);
		//バインドされたソースをコンパイルしてIDにバインドする
		glCompileShader(out->m_ShaderID);
		if(isShaderError(GL_FRAGMENT_SHADER,out->m_ShaderID))return false;
		return true;
	}

	bool kDevice::createShaderMemory(kTechnique* out,const char* vertexBuffer,s32 vertexLength,const char* pixelBuffer,s32 pixelLength)
	{
		LoadShaderInfo* info=new LoadShaderInfo(out,vertexBuffer,vertexLength,pixelBuffer,pixelLength);

		m_LoadShaderQueue.put(info);

		return true;
	}

	bool kDevice::createInputLayout(kInputLayout* out,const kGraphicsPipline* usePipline,const kInputElementDesc* desc,u32 descsize)
	{
		dprintf("createInputLayout\n");
		if(!out)return false;
		//頂点フォーマット情報を定義数確保する
		out->mp_Desc=new kInputLayoutDesc[descsize];
		out->m_TypeNum=descsize;
		out->m_VertexSize=0;
		CreateInputLayoutInfo* info=new CreateInputLayoutInfo;
		info->m_InputLayout=out;
		info->m_Program=&usePipline->m_Program;
		for(int i=0;i<descsize;i++)
		{
			info->m_SemanticName[i]=desc[i].SemanticName;
			//シェーダロケーションを取得する
			//out->mp_Desc[i].m_Location=glGetAttribLocation(usePipline->m_Program,desc[i].SemanticName);
			//フォーマット情報を取得する
			out->mp_Desc[i].m_Types.dxgi_format=PixelToFormat(desc[i].Format,&out->mp_Desc[i].m_Types.component,&out->mp_Desc[i].m_Types.size,&out->mp_Desc[i].m_Types.normalize);
			dprintf("	SemanticName=%s\n",desc[i].SemanticName);
			dprintf("	AttribLocation=%u\n",out->mp_Desc[i].m_Location);
			dprintf("	SemanticSize=%u\n",out->mp_Desc[i].m_Types.size);
			dprintf("	SemanticOffset=%u\n",out->m_VertexSize);
			out->mp_Desc[i].m_Types.size/=8;
			out->m_VertexSize+=out->mp_Desc[i].m_Types.size;
		}

		m_CreateInputLayoutQueue.put(info);
		//bit単位をbyte単位に変換
		//out->m_VertexSize/=8;
		dprintf("InputLayoutSize=%ubyte\n",out->m_VertexSize);
		dprintf("%u",sizeof(kMeshVertex));
		return true;
	}

	bool kDevice::createVertexBuffer(kObjectBuffer*out,const void* data,u32 datasize,s32 type)
	{
		dprintf("createVertexBuffer\n");
		LoadBufferInfo* p=new LoadBufferInfo;
		p->m_Buffer=out;
		p->m_Type=type;
		p->m_Data=data;
		p->m_DataSize=datasize;

		m_LoadVertexBufferQueue.put(p);
		////頂点バッファオブジェクトの作成
		//glGenBuffers(1,&out->m_BO);
		//glBindBuffer(GL_ARRAY_BUFFER,out->m_BO);
		//glBufferData(GL_ARRAY_BUFFER,datasize,data,type);
		//out->m_BufferSize=datasize;
		//out->m_BufferType=GL_ARRAY_BUFFER;
		////バインド解除
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		return true;
	}

	bool kDevice::createIndexBuffer(kObjectBuffer* out,const void* data,u32 datasize,s32 type)
	{
		dprintf("createIndexBuffer size=%u\n",datasize);
		LoadBufferInfo* p=new LoadBufferInfo;
		p->m_Buffer=out;
		p->m_Type=type;
		p->m_Data=data;
		p->m_DataSize=datasize;

		m_LoadIndexBufferQueue.put(p);
		////インデックスバッファオブジェクトの作成
		//glGenBuffers(1, &out->m_BO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out->m_BO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, datasize,data, type);
		//out->m_BufferSize=datasize;
		//out->m_BufferType=GL_ELEMENT_ARRAY_BUFFER;
		////　バインドしたものをもどす
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return true;
	}
	void kDevice::createBlendState( kBlendState* out,const eBlendType* BlendStateType, u32 BlendStateTypeLength )
	{
		DYNAMIC_ASSERT_LIMIT(BlendStateTypeLength,0,8,"CreateBlendState Error");

		ZeroMemory( &out->m_Desc, sizeof( kBlendState::kBlendDesc ) );
		//GLESでは使っていない
		out->m_Desc.AlphaToCoverageEnable = false;

		switch( BlendStateType[0] )
		{
			//ブレンド無し
		case k_BLEND_NONE:
			out->m_Desc.RenderTarget.BlendEnable=false;
			out->m_Desc.RenderTarget.SrcBlend=GL_ONE;
			out->m_Desc.RenderTarget.DstBlend=GL_ZERO;
			out->m_Desc.RenderTarget.BlendOp=GL_FUNC_ADD;
			break;
			//加算合成
		case k_BLEND_ADD:
			out->m_Desc.RenderTarget.BlendEnable=true;
			out->m_Desc.RenderTarget.SrcBlend=GL_ONE;
			out->m_Desc.RenderTarget.DstBlend=GL_ONE;
			out->m_Desc.RenderTarget.BlendOp=GL_FUNC_ADD;
			break;
		case k_BLEND_ALPHA:
			out->m_Desc.RenderTarget.BlendEnable=true;
			out->m_Desc.RenderTarget.SrcBlend=GL_SRC_ALPHA;
			out->m_Desc.RenderTarget.DstBlend=GL_ZERO;
			out->m_Desc.RenderTarget.BlendOp=GL_FUNC_ADD;
			break;
		}   
	}

	void kDevice::createBlendStateAll( kBlendState* out,const eBlendType BlendStateType)
	{
		ZeroMemory( &out->m_Desc, sizeof( kBlendState::kBlendDesc ) );
		//GLESでは使っていない
		out->m_Desc.AlphaToCoverageEnable = false;

		switch( BlendStateType )
		{
			//ブレンド無し
		case k_BLEND_NONE:
			dprintf("k_BLEND_NONE create");
			out->m_Desc.RenderTarget.BlendEnable=false;
			out->m_Desc.RenderTarget.SrcBlend=GL_ONE;
			out->m_Desc.RenderTarget.DstBlend=GL_ZERO;
			out->m_Desc.RenderTarget.BlendOp=GL_FUNC_ADD;
			break;
			//加算合成
		case k_BLEND_ADD:
			dprintf("k_BLEND_ADD create");
			out->m_Desc.RenderTarget.BlendEnable=true;
			out->m_Desc.RenderTarget.SrcBlend=GL_SRC_ALPHA;
			out->m_Desc.RenderTarget.DstBlend=GL_ONE;
			out->m_Desc.RenderTarget.BlendOp=GL_FUNC_ADD;
			break;
		case k_BLEND_ALPHA:
			out->m_Desc.RenderTarget.BlendEnable=true;
			out->m_Desc.RenderTarget.SrcBlend=GL_SRC_ALPHA;
			out->m_Desc.RenderTarget.DstBlend=GL_ONE_MINUS_SRC_ALPHA;
			out->m_Desc.RenderTarget.BlendOp=GL_FUNC_ADD;
			break;
		}   
	}
	bool kDevice::createDepthStencilState(kDepthStencilState* out,bool enable,bool writeenable,eDepthFunc func)
	{
		out->m_Desc.DepthEnable=enable;
		out->m_Desc.DepthWriteEnable=writeenable;
		out->m_Desc.DepthFunc=getDepthFunc(func);
		return true;
	}
	bool kDevice::createRasterizerState(kRasterizerState* out,eFillMode fill,eCullMode cull,bool front)
	{
		out->m_Desc.FillMode=getFillMode(fill);
		out->m_Desc.CullMode=getCullMode(cull);
		//GLESでは使っていない
		out->m_Desc.FrontCounterClockwise=front;
		return true;
	}


	bool kDevice::updateSubResource(const kObjectBuffer* in,const void* data,u32 datasize)
	{
		glBindBuffer( in->m_BufferType, in->m_BO );
		glBufferSubData(in->m_BufferType, 0, datasize, data);  
		glBindBuffer( in->m_BufferType, 0 );
		return true;
	}

	bool kDevice::IAsetInputLayout(const kInputLayout* in)
	{
		m_IAInputLayout=in;
		u32 offset=0;
		const kInputLayoutDesc* desc=m_IAInputLayout->mp_Desc;
		//セマンティクスの定義の数だけ頂点オフセットを指定する
		for(int i=0;i<m_IAInputLayout->m_TypeNum;i++)
		{
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

	bool kDevice::IAsetVertexBuffer(const kObjectBuffer* in)
	{
		m_IAVertexBuffer=in;
		glBindBuffer( GL_ARRAY_BUFFER, m_IAVertexBuffer->m_BO );
		return true;
	}

	bool kDevice::IAsetIndexBuffer(const kObjectBuffer* in)
	{
		m_IAIndexBuffer=in;
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IAIndexBuffer->m_BO );
		return true;
	}

	bool kDevice::OMsetBlendState(const kBlendState* in)
	{
		m_OMBlendState=in;
		//αブレンドを有効/無効にする
		if(m_OMBlendState->m_Desc.RenderTarget.BlendEnable){glEnable(GL_BLEND);/*dprintf("Blend On")*/}
		else {glDisable(GL_BLEND);/*dprintf("Blend OFF")*/}
		//ブレンド計算式を指定する
		glBlendEquation(m_OMBlendState->m_Desc.RenderTarget.BlendOp);
		//ブレンド係数を指定する
		glBlendFunc(m_OMBlendState->m_Desc.RenderTarget.SrcBlend,m_OMBlendState->m_Desc.RenderTarget.DstBlend);
		return true;
	}
	bool kDevice::OMsetDepthStencilState(const kDepthStencilState* in)
	{
		m_OMDepthStencilState=in;
		//深度テストを有効/無効にする
		if(m_OMDepthStencilState->m_Desc.DepthEnable)glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
		//深度バッファへの書き込みを有効/無効にする
		if(m_OMDepthStencilState->m_Desc.DepthWriteEnable)glDepthMask(GL_TRUE);
		else glDepthMask(GL_FALSE);
		//深度比較関数を指定する
		glDepthFunc(m_OMDepthStencilState->m_Desc.DepthFunc);
		return true;
	}
	bool kDevice::RSsetState(const kRasterizerState* in)
	{
		m_RasterizerState=in;
		//表裏カリングを有効/無効にする
		if(m_RasterizerState->m_Desc.CullMode)glEnable(GL_CULL_FACE); 
		else glDisable(GL_CULL_FACE); 
		//カリング面を指定する
		glCullFace(m_RasterizerState->m_Desc.CullMode);
	}
	bool kDevice::drawIndexed(u32 indexnum)
	{
		//ポリゴン描画方式を決めてインデックスバッファで描画
		glDrawElements( m_RasterizerState->m_Desc.FillMode, indexnum, GL_UNSIGNED_SHORT, NULL );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}

}