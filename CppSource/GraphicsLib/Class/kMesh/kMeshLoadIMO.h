#pragma once
#include "IMeshLoadDelegate.h"
#include "utility\kutility.h"

namespace klib
{
	//ihopファイル読み込み用
	typedef struct tagLVERTEX {
		float	x, y, z;
		DWORD	color;
		float	tu, tv;
	} LVERTEX, *LPLVERTEX;

	typedef struct tagMESHVERTEX {
		float	x, y, z;
		float	nx, ny, nz;
		float	tu, tv;
	} MESHVERTEX, *LPMESHVERTEX;

	typedef struct tagLNVERTEX {
		float	x, y, z;
		float	nx, ny, nz;
		DWORD	color;
		float	tu, tv;
	} MESHVERTEX2, *LPMESHVERTEX2;

	typedef struct _D3DMATERIAL9 {
		math::Vector4   Diffuse;        /* Diffuse color RGBA */
		math::Vector4   Ambient;        /* Ambient color RGB */
		math::Vector4   Specular;       /* Specular 'shininess' */
		math::Vector4   Emissive;       /* Emissive color RGB */
		float           Power;          /* Sharpness if specular highlight */
	} D3DMATERIAL9;

	class kMeshLoadIMO	:public	IMeshLoadDelegate
	{
	private:

		//	IMOオブジェクト
		typedef struct tagIMOOBJ {
			DWORD		id;	
			int			type;
			DWORD		NumVertex;
			LPLVERTEX	lpVertex;

			DWORD		NumFace;
			LPWORD		lpFace;
			LPDWORD		lpAtr;

			DWORD			NumMaterial;	//	マテリアル数
			D3DMATERIAL9	Material[32];	//	マテリアル
			char			Texture[32][32];//	テクスチャファイル

		} IMOOBJ, *LPIMOOBJ;
		//------------------------------------------------------
		//	ＩＭＯ読み込み
		//------------------------------------------------------
		bool LoadIMO(const char* filename ,kMeshData* m_Data)
		{

			char* buffer;
			s32 buffersize;
			AssetsLoader::load(&buffer, &buffersize, filename);
			utility::kBufferAnalyze analyze(buffer,(u32)buffersize);

			IMOOBJ		imo;
			//	ファイル読み込み
			u32	dum;

			s32		version = 1;

			analyze.read(&imo,sizeof(IMOOBJ));
			if( imo.id == '2OMI' ) version = 2;

			char* cha=(char*)&imo;

			dprintf("filesize%d",buffersize);

			dprintf("%c%c%c%c",cha[0],cha[1],cha[2],cha[3]);

			dprintf("NumVertex%u\n",imo.NumVertex);
			dprintf("NumFace%u\n",imo.NumFace);
			dprintf("NumMaterial%u\n",imo.NumMaterial);

			////	メッシュ作成

			MESHVERTEX2*	workV = new MESHVERTEX2[imo.NumVertex];
			u16*		workF = new u16[imo.NumFace*3];
			u32*		workM=new u32[imo.NumFace];

			//	頂点読み込み
			analyze.read(workV,sizeof(MESHVERTEX2)*imo.NumVertex);
			//	インデックス読み込み
			analyze.read(workF, sizeof(u16)*imo.NumFace*3);
			//マテリアル読み込み
			analyze.read(workM, sizeof(u32)*imo.NumFace);


			//	パス分割
			char	workpath[MAX_PATH];
			CopyMemory( workpath, filename, strlen(filename)+1 );
			for( u32 i=strlen(filename) ; i>0 ; i-- ){
				if( utility::IsDBCSLeadByte(workpath[i-2]) ){
					i--;
					continue;
				}
				if( workpath[i-1] == '\\' || workpath[i-1] == '/' ){
					workpath[i] = '\0';
					break;
				}
				workpath[i-1] = '\0';
			}

			dprintf("%s",workpath);

			//データコピー
			//マテリアル数
			m_Data->m_Info.MaterialCount = imo.NumMaterial;
			//頂点数
			m_Data->m_Info.NumVertex=imo.NumVertex;
			//ポリゴン数
			m_Data->m_Info.NumFace=imo.NumFace;
			//頂点コピー
			m_Data->mp_Vertex=new kMeshVertex[imo.NumVertex];
			for(u32 i=0;i<imo.NumVertex;i++)
			{
				((kMeshVertex*)(m_Data->mp_Vertex))[i].m_Pos=math::Vector3(workV[i].x,workV[i].y,workV[i].z);
				((kMeshVertex*)(m_Data->mp_Vertex))[i].m_Normal=math::Vector3(workV[i].nx,workV[i].ny,workV[i].nz);
				//Vertex[i].color=math::Vector4(workV[i].color);
				((kMeshVertex*)(m_Data->mp_Vertex))[i].m_TexCoord=math::Vector2(workV[i].tu,workV[i].tv);
			}
			//インデックスコピー
			m_Data->m_Info.Index=new u16[imo.NumFace*3];
			for(int i=0;i<imo.NumFace*3;i++)
			{
				m_Data->m_Info.Index[i]=(u16)workF[i];
			}
			//マテリアルコピー
			m_Data->m_Info.Material=new u32[imo.NumFace];
			CopyMemory( m_Data->m_Info.Material, workM, sizeof(u32)*(imo.NumFace) );

			//マテリアル毎のポリゴン数インデックス作成
			u16* workMI=new u16[m_Data->m_Info.NumFace*3];
			m_Data->m_Info.MaterialIndex=new u16*[m_Data->m_Info.MaterialCount];
			m_Data->m_Info.MaterialNumFace=new u32[m_Data->m_Info.MaterialCount];
			//テクスチャ読み込み
			//lpTexture  = new klib2DObj* [ imo.NumMaterial ];
			//lpNormal   = new klib2DObj* [ imo.NumMaterial ];
			//lpSpecular = new klib2DObj* [ imo.NumMaterial ];
			//lpHeight   = new klib2DObj* [ imo.NumMaterial ];

			char temp[256];
			for( u32 i=0 ; i<m_Data->m_Info.MaterialCount ; i++ ){
				dprintf("Material %d",i);
				//lpTexture[i]  = NULL;
				//lpNormal[i]   = NULL;
				//lpSpecular[i] = NULL;
				//lpHeight[i] = NULL;
				//材質毎のポリゴン数初期化
				m_Data->m_Info.MaterialNumFace[i]=0;
				//材質内のポリゴン数とインデックスを取得
				for(u32 j=0;j<m_Data->m_Info.NumFace;j++)
				{
					if(m_Data->m_Info.Material[j]==i)
					{
						//dprintf("	%d,%d,%d",m_Data->m_Info.Index[j*3+1],m_Data->m_Info.Index[j*3+2],m_Data->m_Info.Index[j*3+3])
							workMI[m_Data->m_Info.MaterialNumFace[i]*3+0]=m_Data->m_Info.Index[j*3+0];
						workMI[m_Data->m_Info.MaterialNumFace[i]*3+1]=m_Data->m_Info.Index[j*3+1];
						workMI[m_Data->m_Info.MaterialNumFace[i]*3+2]=m_Data->m_Info.Index[j*3+2];
						m_Data->m_Info.MaterialNumFace[i]++;
					}
				}
				//インデックスバッファ作成
				if(m_Data->m_Info.MaterialNumFace[i]==0)
				{
					m_Data->m_Info.MaterialIndex[i]=NULL;
				}
				else
				{
					m_Data->m_Info.MaterialIndex[i]=new u16[m_Data->m_Info.MaterialNumFace[i]*3];
					CopyMemory( m_Data->m_Info.MaterialIndex[i], workMI, sizeof(u16)*(m_Data->m_Info.MaterialNumFace[i]*3) );
				}
				if( imo.Texture[i][0] == '\0' ) continue;
				dprintf("IMO Texture Load %s",workpath);
				//	テクスチャ読み込み
				sprintf( temp, "%s%s", workpath, imo.Texture[i] );
				utility::pathYenToSlash(temp);
				m_Data->m_Info.Diffuse[i] = new rlib::Texture;
				m_Data->m_Info.Diffuse[i]->Initilize(temp);

				sprintf( temp, "%sN%s", workpath, imo.Texture[i] );
				utility::pathYenToSlash(temp);
				m_Data->m_Info.Normal[i] = new rlib::Texture;
				m_Data->m_Info.Normal[i]->Initilize(temp);

				sprintf( temp, "%sS%s", workpath, imo.Texture[i] );
				utility::pathYenToSlash(temp);
				m_Data->m_Info.Specular[i] = new rlib::Texture;
				m_Data->m_Info.Specular[i]->Initilize(temp);

				//sprintf( temp, "%sH%s", workpath, imo.Texture[i] );
				//lpHeight[i] = new klib2DObj(temp);

			}

			///////////////////////////////////////
			delete[] workV;
			delete[] workF;
			delete[] workM;
			delete[] workMI;
			delete[] buffer;

			return true;
		}
	protected:
	public:
		kMeshLoadIMO(){}
		virtual ~kMeshLoadIMO(){}
		virtual bool Load(const char* filename,kMeshData* data)
		{
			return LoadIMO(filename,data);
		}
	};
}