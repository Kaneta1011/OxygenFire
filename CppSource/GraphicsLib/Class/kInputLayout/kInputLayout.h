#pragma once
#include "defines.h"
namespace klib
{

	enum eVertexFormat
	{
		k_VF_UNKNOWN=0,

		k_VF_R32_FLOAT=4,
		k_VF_R32G32_FLOAT,
		k_VF_R32G32B32_FLOAT,
		k_VF_R32G32B32A32_FLOAT,

		k_VF_R16_FLOAT,
		k_VF_R16G16_FLOAT,
		k_VF_R16G16B16_FLOAT,
		k_VF_R16G16B16A16_FLOAT,

		k_VF_R32_SINT,
		k_VF_R32G32_SINT,
		k_VF_R32G32B32_SINT,
		k_VF_R32G32B32A32_SINT,

		k_VF_R16_SINT,
		k_VF_R16G16_SINT,
		k_VF_R16G16B16_SINT,
		k_VF_R16G16B16A16_SINT,

		k_VF_R8_SINT,
		k_VF_R8G8_SINT,
		k_VF_R8G8B8_SINT,
		k_VF_R8G8B8A8_SINT,

		k_VF_R32_UINT,
		k_VF_R32G32_UINT,
		k_VF_R32G32B32_UINT,
		k_VF_R32G32B32A32_UINT,

		k_VF_R16_UINT,
		k_VF_R16G16_UINT,
		k_VF_R16G16B16_UINT,
		k_VF_R16G16B16A16_UINT,

		k_VF_R8_UINT,
		k_VF_R8G8_UINT,
		k_VF_R8G8B8_UINT,
		k_VF_R8G8B8A8_UINT,

		k_VF_R16_SNORM,
		k_VF_R16G16_SNORM,
		k_VF_R16G16B16_SNORM,
		k_VF_R16G16B16A16_SNORM,

		k_VF_R8_SNORM,
		k_VF_R8G8_SNORM,
		k_VF_R8G8B8_SNORM,
		k_VF_R8G8B8A8_SNORM,

		k_VF_R16_UNORM,
		k_VF_R16G16_UNORM,
		k_VF_R16G16B16_UNORM,
		k_VF_R16G16B16A16_UNORM,

		k_VF_R8_UNORM,
		k_VF_R8G8_UNORM,
		k_VF_R8G8B8_UNORM,
		k_VF_R8G8B8A8_UNORM,
	};

	// DXInputLayout.cpp
#ifdef WIN32
	static struct _FormatType {
		DXGI_FORMAT   dxgi_format;
		u32			component;
		u32			size;
		bool			normalize;
	} formatTable[]= {
		{    DXGI_FORMAT_UNKNOWN,                  0,			0,    false,    },
		{    DXGI_FORMAT_UNKNOWN,                  0,			0,   false,    },
		{    DXGI_FORMAT_UNKNOWN,                  0,			0,   false,    },
		{    DXGI_FORMAT_UNKNOWN,                  0,			0,   false,    },

		{    DXGI_FORMAT_R32_FLOAT,							1,			32,   false,	},
		{    DXGI_FORMAT_R32G32_FLOAT,					2,			64,   false,	},
		{    DXGI_FORMAT_R32G32B32_FLOAT,				3,			96,   false,	},
		{    DXGI_FORMAT_R32G32B32A32_FLOAT,		4,			128,   false,	},

		{    DXGI_FORMAT_R16_FLOAT,							1,			16,   false,	},  // half float
		{    DXGI_FORMAT_R16G16_FLOAT,					2,			32,   false,	},  // half float
		{    DXGI_FORMAT_UNKNOWN,							0,			0,   false,	},  // half float
		{    DXGI_FORMAT_R16G16B16A16_FLOAT,		4,			64,   false,	},  // half float

		{    DXGI_FORMAT_R32_SINT,							1,			32,   false,	},  // signed int 32
		{    DXGI_FORMAT_R32G32_SINT,						2,			64,   false,	},  // signed int 32
		{    DXGI_FORMAT_R32G32B32_SINT,				3,			96,   false,	},  // signed int 32
		{    DXGI_FORMAT_R32G32B32A32_SINT,			4,			128,   false,	},  // signed int 32

		{    DXGI_FORMAT_R16_SINT,							1,			16,   false,	},  // signed int 16
		{    DXGI_FORMAT_R16G16_SINT,						2,			32,   false,	},  // signed int 16
		{    DXGI_FORMAT_UNKNOWN,							0,			0,   false,	},  // signed int 16
		{    DXGI_FORMAT_R16G16B16A16_SINT,			4,			64,   false,	},  // signed int 16

		{    DXGI_FORMAT_R8_SINT,								1,			8,   false,	},  // signed byte 8
		{    DXGI_FORMAT_R8G8_SINT,							2,			16,   false,	},  // signed byte 8
		{    DXGI_FORMAT_UNKNOWN,							0,			0,   false,	},  // signed byte 8
		{    DXGI_FORMAT_R8G8B8A8_SINT,					4,			32,   false,	},  // signed byte 8

		{    DXGI_FORMAT_R32_UINT,							1,			32,   false,	},  // unsigned int 32
		{    DXGI_FORMAT_R32G32_UINT,						2,			64,   false,	},  // unsigned int 32
		{    DXGI_FORMAT_R32G32B32_UINT,				3,			96,   false,	},  // unsigned int 32
		{    DXGI_FORMAT_R32G32B32A32_UINT,			4,			128,   false,	},  // unsigned int 32

		{    DXGI_FORMAT_R16_UINT,							1,			16,   false,	},
		{    DXGI_FORMAT_R16G16_UINT,						2,			32,   false,	},
		{    DXGI_FORMAT_UNKNOWN,							0,			0,   false,	},
		{    DXGI_FORMAT_R16G16B16A16_UINT,			4,			64,   false,	},

		{    DXGI_FORMAT_R8_UINT,								1,			8,   false,	},
		{    DXGI_FORMAT_R8G8_UINT,							2,			16,   false,	},
		{    DXGI_FORMAT_UNKNOWN,							0,			0,   false,	},
		{    DXGI_FORMAT_R8G8B8A8_UINT,					4,			32,   false,	},

		{    DXGI_FORMAT_R16_SNORM,						1,			16,   true,	},
		{    DXGI_FORMAT_R16G16_SNORM,					2,			32,   true,	},
		{    DXGI_FORMAT_UNKNOWN,							0,			0,   true,	},
		{    DXGI_FORMAT_R16G16B16A16_SNORM,		4,			64,   true,	},

		{    DXGI_FORMAT_R8_SNORM,							1,			8,   true,	},
		{    DXGI_FORMAT_R8G8_SNORM,						2,			16,   true,	},
		{    DXGI_FORMAT_UNKNOWN,							0,			0,   true,	},
		{    DXGI_FORMAT_R8G8B8A8_SNORM,            4,			32,   true,	},

		{    DXGI_FORMAT_R16_UNORM,						1,			16,   true,	},
		{    DXGI_FORMAT_R16G16_UNORM,					2,			32,   true,	},
		{    DXGI_FORMAT_UNKNOWN,							0,			0,   true,	},
		{    DXGI_FORMAT_R16G16B16A16_UNORM,	4,			64,   true,	},

		{    DXGI_FORMAT_R8_UNORM,							1,			8,   true,	},
		{    DXGI_FORMAT_R8G8_UNORM,						2,			16,   true,	},
		{    DXGI_FORMAT_UNKNOWN,							0,			0,   true,	},
		{    DXGI_FORMAT_R8G8B8A8_UNORM,			4,			32,   true,	},
	};
#else
	static struct _FormatType {
		GLenum	dxgi_format;
		u32			component;
		u32			size;
		bool			normalize;
	} formatTable[]= {

		{    0,                  0,			0,    false,    },
		{    0,                  0,			0,   false,    },
		{    0,                  0,			0,   false,    },
		{    0,                  0,			0,   false,    },

		{    GL_FLOAT,           1,			32,   false,	},
		{    GL_FLOAT,           2,			64,   false,	},
		{    GL_FLOAT,           3,			96,   false,	},
		{    GL_FLOAT,           4,			128,   false,	},

		{    0,                  0,			0,   false,	},  // half float
		{    0,                  0,			0,   false,	},  // half float
		{    0,                  0,			0,   false,	},  // half float
		{    0,                  0,			0,   false,	},  // half float

		{    0,                  0,			0,   false,	},  // signed int 32
		{    0,                  0,			0,   false,	},  // signed int 32
		{    0,                  0,			0,   false,	},  // signed int 32
		{    0,                  0,			0,   false,	},  // signed int 32

		{    GL_SHORT,	         1,			16,   false,	},  // signed int 16
		{    GL_SHORT,	         2,			32,   false,	},  // signed int 16
		{    GL_SHORT,	         3,			48,   false,	},  // signed int 16
		{    GL_SHORT,	         4,			64,   false,	},  // signed int 16

		{    GL_BYTE,	         1,			8,   false,	},  // signed byte 8
		{    GL_BYTE,	         2,			16,   false,	},  // signed byte 8
		{    GL_BYTE,	         3,			24,   false,	},  // signed byte 8
		{    GL_BYTE,	         4,			32,   false,	},  // signed byte 8

		{    0,		         0,			0,   false,	},  // unsigned int 32
		{    0,		         0,			0,   false,	},  // unsigned int 32
		{    0,		         0,			0,   false,	},  // unsigned int 32
		{    0,		         0,			0,   false,	},  // unsigned int 32

		{    GL_UNSIGNED_SHORT,  1,			16,   false,	},
		{    GL_UNSIGNED_SHORT,  2,			32,   false,	},
		{    GL_UNSIGNED_SHORT,  3,			48,   false,	},
		{    GL_UNSIGNED_SHORT,  4,			64,   false,	},

		{    GL_UNSIGNED_BYTE,   1,			8,   false,	},
		{    GL_UNSIGNED_BYTE,   2,			16,   false,	},
		{    GL_UNSIGNED_BYTE,   3,			24,   false,	},
		{    GL_UNSIGNED_BYTE,   4,			32,   false,	},

		{    GL_SHORT,           1,			16,   true,	},
		{    GL_SHORT,           2,			32,   true,	},
		{    GL_SHORT,           3,			48,   true,	},
		{    GL_SHORT,           4,			64,   true,	},

		{    GL_BYTE,            1,			8,   true,	},
		{    GL_BYTE,            2,			16,   true,	},
		{    GL_BYTE,            3,			24,   true,	},
		{    GL_BYTE,            4,			32,   true,	},

		{    GL_UNSIGNED_SHORT,  1,			16,   true,	},
		{    GL_UNSIGNED_SHORT,  2,			32,   true,	},
		{    GL_UNSIGNED_SHORT,  3,			48,   true,	},
		{    GL_UNSIGNED_SHORT,  4,			64,   true,	},

		{    GL_UNSIGNED_BYTE,   1,			8,   true,	},
		{    GL_UNSIGNED_BYTE,   2,			16,   true,	},
		{    GL_UNSIGNED_BYTE,   3,			24,   true,	},
		{    GL_UNSIGNED_BYTE,   4,			32,   true,	},
	};

inline GLenum PixelToFormat( eVertexFormat func, u32* component,u32* size, bool* normal )
{
    const _FormatType*   fp= &formatTable[ func ];
    if( component ){
        *component= fp->component;
    }
    if( size ){
        *size= fp->size;
    }
    if( normal ){
        *normal= fp->normalize;
    }
    return  fp->dxgi_format;
}


#endif


	///@brief 頂点データのタイプ(GLESでは使用しない)
	enum eInputClassification
	{
		eVertex	= 0,
		eInstance	= 1
	};
	/**
	* @class 頂点定義用フォーマット情報
	*/
	struct kInputElementDesc
	{
		///@brief セマンティクス名
		const char* SemanticName;
		///@brief セマンティクスインデックス(使用していない)
		u32 SemanticIndex;
		///@brief セマンティクスフォーマット
		eVertexFormat Format;
		///@brief インプットスロット(使用していない)
		u32 InputSlot;
		///@brief インプットスロット(使用していない)
		eInputClassification InputSlotClass;
		///@brief インプットスロット(使用していない)
		u32 InstanceDataStepRate;
	} ;
	/**
	* @class 描画に使用する頂点フォーマット情報
	*/
	struct kInputLayoutDesc
	{
		///@brief セマンティクス位置
		u32 m_Location;
		///@brief ライブラリ固有のフォーマット情報
		_FormatType m_Types;
	};

	/**
	* @class 頂点フォーマット管理クラス
	*/
	class kInputLayout
	{
		friend class kDevice;
		friend class kGraphicsPipline;
	private:
		///@brief 頂点フォーマット
		kInputLayoutDesc* mp_Desc;
		///@brief 頂点フォーマット数
		u32 m_TypeNum;
		///@brief 頂点バイト数
		u32 m_VertexSize;
	public:
		kInputLayout()
		{
			mp_Desc=NULL;
			m_TypeNum=0;
			m_VertexSize=0;
		}
		~kInputLayout()
		{
			SAFE_DELETE_ARRAY(mp_Desc);
		}
	};
}