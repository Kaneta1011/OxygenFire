#pragma once
#include "kMeshLoadIMO.h"

namespace klib
{
	class kMeshLoadIEM:public	IMeshLoadDelegate
	{
		//friend kMesh;
	protected:
		//kMeshData* m_Data;
		//*****************************************************************************
		//
		//		オブジェクト作成
		//
		//*****************************************************************************

		typedef struct tagIEMBONE {
			math::Matrix			BoneMatrix;			//	ボーン行列
			u16				parent;				//	親ボーン

			math::Quaternion		orgPose;			//	基本姿勢
			math::Vector3			orgPos;				//	基本座標

			u16				IndexNum;			//	影響頂点数
			u32*			Index;				//	影響頂点Index
			f32*			Influence;			//	影響力
		} IEMBONE, *LPIEMBONE;

		typedef struct tagIEMMOTION {
			u16				NumRotate;			//	回転キーフレーム数
			u16*			RotateFrame;		//	回転キーフレーム
			math::Quaternion*		Rotate;				//	ボーンの状態クォータニオン

			u16				NumPosition;		//	座標キーフレーム数
			u16*			PositionFrame;		//	座標キーフレーム
			math::Vector3*		Position;			//	座標
		} IEMMOTION, *LPIEMMOTION;

		typedef struct tagIEXANIME2 {
			u32			rotNum;
			u16*		rotFrame;
			math::Quaternion*	rot;

			u32			posNum;
			u16*		posFrame;
			math::Vector3*	pos;
		} IEXANIME2, *LPIEXANIME2;

		typedef struct tagIEMFILE
		{
			int				version;			//	バージョン	
			//	メッシュ情報
			u16				NumVertex;			//	頂点数
			void*			lpVertex;			//	頂点バッファ

			u16				NumFace;			//	ポリゴン数
			u16*			lpFace;				//	ポリゴンインデックス
			u32*			lpAtr;				//	ポリゴン材質

			u16				NumMaterial;		//	マテリアル数
			D3DMATERIAL9	Material[32];		//	マテリアル
			char			Texture[32][64];	//	テクスチャファイル

			//	ボーン情報
			u16				NumBone;
			LPIEMBONE		lpBone;

			//	モーション情報
			u16				MaxFrame;
			u16				NumMotion;
			u16				M_Offset[256];
			u16				FrameFlag[65535];

			LPIEMMOTION		lpMotion;
		}IEMFILE, *LPIEMFILE;

		//------------------------------------------------------
		//		ＩＥＭから３Dオブジェクト作成
		//------------------------------------------------------
		bool CreateFromIEM( char* path, LPIEMFILE lpIem ,kSkinData* data)
		{
			u32		i, j;

			//dwFrame = 0;

			//読み込んだデータ廃棄する前にをコピー
			data->m_Info.NumVertex = lpIem->NumVertex;
			data->m_Info.NumFace = lpIem->NumFace;

			//頂点コピー
			data->mp_Vertex=new kMeshVertex[lpIem->NumVertex];

			//バージョンによって読み込みを変える
			if( lpIem->version < 4 )
			{
				for(s32 i=0;i<(s32)lpIem->NumVertex;i++)
				{
					MESHVERTEX* work=(MESHVERTEX*)lpIem->lpVertex;
					((kMeshVertex*)(data->mp_Vertex))[i].m_Pos=math::Vector3(work[i].x,work[i].y,work[i].z);
					((kMeshVertex*)(data->mp_Vertex))[i].m_Normal=math::Vector3(work[i].nx,work[i].ny,work[i].nz);
					((kMeshVertex*)(data->mp_Vertex))[i].m_Color=math::Vector4(1,1,1,1);
					((kMeshVertex*)(data->mp_Vertex))[i].m_TexCoord=math::Vector2(work[i].tu,work[i].tv);
				}
			} else {
				for(s32 i=0;i<(s32)lpIem->NumVertex;i++)
				{
					MESHVERTEX2* work=(MESHVERTEX2*)lpIem->lpVertex;
					((kMeshVertex*)(data->mp_Vertex))[i].m_Pos=math::Vector3(work[i].x,work[i].y,work[i].z);
					((kMeshVertex*)(data->mp_Vertex))[i].m_Normal=math::Vector3(work[i].nx,work[i].ny,work[i].nz);
					//data->m_Vertex[i].color=work[i].color;
					((kMeshVertex*)(data->mp_Vertex))[i].m_Color=math::Vector4(1,1,1,1);
					((kMeshVertex*)(data->mp_Vertex))[i].m_TexCoord=math::Vector2(work[i].tu,work[i].tv);
				}
			}

			//インデックスコピー
			data->m_Info.Index=new u32[data->m_Info.NumFace*3];
			for(int i=0;i<data->m_Info.NumFace*3;i++)
			{
				data->m_Info.Index[i]=(u32)lpIem->lpFace[i];
			}
			//マテリアルコピー
			data->m_Info.Material=new u32[data->m_Info.NumFace];
			CopyMemory( data->m_Info.Material, lpIem->lpAtr, sizeof(u32)*(data->m_Info.NumFace) );

			//	マテリアル設定
			data->m_Info.MaterialCount = (u32)lpIem->NumMaterial;
			data->m_Info.MaterialIndex=new u32*[lpIem->NumMaterial];
			data->m_Info.MaterialNumFace=new u32[lpIem->NumMaterial];

			u32* workMI=new u32[data->m_Info.NumFace*3];
			for( u32 i=0 ; i<data->m_Info.MaterialCount ; i++ ){
				dprintf("Material %d",i);
				//lpTexture[i]  = NULL;
				//lpNormal[i]   = NULL;
				//lpSpecular[i] = NULL;
				//lpHeight[i] = NULL;
				//材質毎のポリゴン数初期化
				data->m_Info.MaterialNumFace[i]=0;
				//材質内のポリゴン数とインデックスを取得
				for(u32 j=0;j<data->m_Info.NumFace;j++)
				{
					if(data->m_Info.Material[j]==i)
					{
						//dprintf("	%d,%d,%d",data->m_Info.Index[j*3+1],data->m_Info.Index[j*3+2],data->m_Info.Index[j*3+3]);
						workMI[data->m_Info.MaterialNumFace[i]*3+0]=data->m_Info.Index[j*3+0];
						workMI[data->m_Info.MaterialNumFace[i]*3+1]=data->m_Info.Index[j*3+1];
						workMI[data->m_Info.MaterialNumFace[i]*3+2]=data->m_Info.Index[j*3+2];
						data->m_Info.MaterialNumFace[i]++;
					}
				}
				//インデックスバッファ作成
				if(data->m_Info.MaterialNumFace[i]==0)
				{
					data->m_Info.MaterialIndex[i]=NULL;
				}
				else
				{
					data->m_Info.MaterialIndex[i]=new u32[data->m_Info.MaterialNumFace[i]*3];
					CopyMemory( data->m_Info.MaterialIndex[i], workMI, sizeof(u32)*(data->m_Info.MaterialNumFace[i]*3) );
					dprintf("Material NumFace %u",data->m_Info.MaterialNumFace[i]);
				}
			}
			delete[] workMI;

			////	テクスチャ設定
			//lpTexture  = new klib2DObj* [ MaterialCount ];
			//lpNormal   = new klib2DObj* [ MaterialCount ];
			//lpSpecular = new klib2DObj* [ MaterialCount ];
			//lpHeight   = new klib2DObj* [ MaterialCount ];

			//for( i=0 ; i<MaterialCount ; i++ ){
			//	lpTexture[i]  = NULL;
			//	lpNormal[i]   = NULL;
			//	lpSpecular[i] = NULL;
			//	lpHeight[i] = NULL;
			//	if( lpIem->Texture[i][0] == '\0' ) continue;
			//	//	テクスチャ読み込み
			//	char	temp[256];
			//	sprintf_s( temp,256, "%s%s", path, lpIem->Texture[i] );
			//	lpTexture[i] = new klib2DObj(temp);

			//	sprintf_s( temp,256, "%sN%s", path, lpIem->Texture[i] );
			//	lpNormal[i] = new klib2DObj(temp);

			//	sprintf_s( temp,256, "%sS%s", path, lpIem->Texture[i] );
			//	lpSpecular[i] = new klib2DObj(temp);

			//	sprintf_s( temp,256, "%sH%s", path, lpIem->Texture[i] );
			//	lpHeight[i] = new klib2DObj(temp);

			//}

			//
			//	ボーン情報
			//

			data->m_Info.NumBone = lpIem->NumBone;
			data->mp_Bone=new kSkinBone[lpIem->NumBone];
			//dprintf("BoneInfo %u",data->m_Info.NumBone);

			//合計フレーム数
			data->m_Info.NumFrame = lpIem->MaxFrame;
			data->m_Info.motionOffsets=new u16[256];
			CopyMemory( data->m_Info.motionOffsets, lpIem->M_Offset, 2*256 );
			//パラメーター
			data->m_Info.dwFrameFlag = new u32[lpIem->MaxFrame];
			for(int i=0;i<lpIem->MaxFrame;i++)
			{
				data->m_Info.dwFrameFlag[i] =  lpIem->FrameFlag[i];
			}

			data->mp_Animation=new kSkinAnimation[lpIem->NumBone];

			for( i=0 ; i<lpIem->NumBone ; i++ ){
				data->mp_Bone[i].parent     = lpIem->lpBone[i].parent;		//	親
				data->mp_Bone[i].BoneMatrix= lpIem->lpBone[i].BoneMatrix;	//	基準化行列
				data->mp_Bone[i].orgPos         = lpIem->lpBone[i].orgPos;		//	標準位置
				data->mp_Bone[i].orgPose        = lpIem->lpBone[i].orgPose;		//	標準姿勢
				data->mp_Bone[i].IndexNum=lpIem->lpBone[i].IndexNum;		//ボーン影響インデックス数
				//ボーン影響インデックス
				data->mp_Bone[i].Index=new u32[lpIem->lpBone[i].IndexNum];
				CopyMemory(data->mp_Bone[i].Index,lpIem->lpBone[i].Index,sizeof(u32)*lpIem->lpBone[i].IndexNum);
				//ボーン影響度
				data->mp_Bone[i].Influence=new f32[lpIem->lpBone[i].IndexNum];
				CopyMemory(data->mp_Bone[i].Influence,lpIem->lpBone[i].Influence,sizeof(f32)*lpIem->lpBone[i].IndexNum);

				//dprintf("BoneInfo parent %u",data->mp_Bone[i].parent);
				//dprintf("BoneInfo IndexNum %u",data->mp_Bone[i].IndexNum);
				for(int j=0;j<data->mp_Bone[j].IndexNum;j++)
				{
					//dprintf("BoneInfo Index %u",data->mp_Bone[i].Index[j]);
					//dprintf("BoneInfo Influence %f",data->mp_Bone[i].Influence[j]);
				}


				//	クォータニオンコピー
				data->mp_Animation[i].rotNum = lpIem->lpMotion[i].NumRotate;
				data->mp_Animation[i].rotFrame = new u16[ data->mp_Animation[i].rotNum ];
				data->mp_Animation[i].rot      = new math::Quaternion[ data->mp_Animation[i].rotNum ];
				for( j=0 ; j<data->mp_Animation[i].rotNum ; j++ ){
					data->mp_Animation[i].rotFrame[j] = lpIem->lpMotion[i].RotateFrame[j];
					data->mp_Animation[i].rot[j] = lpIem->lpMotion[i].Rotate[j];
				}
				//	ポジションコピー
				data->mp_Animation[i].posNum   = lpIem->lpMotion[i].NumPosition;
				if( data->mp_Animation[i].posNum > 0 )
				{
					data->mp_Animation[i].posFrame = new u16[ data->mp_Animation[i].posNum ];
					data->mp_Animation[i].pos      = new math::Vector3[ data->mp_Animation[i].posNum ];
				}
				for( j=0 ; j<data->mp_Animation[i].posNum ; j++ ){
					data->mp_Animation[i].posFrame[j] = lpIem->lpMotion[i].PositionFrame[j];
					data->mp_Animation[i].pos[j] = lpIem->lpMotion[i].Position[j];
				}
			}

			return true;
		}

		//*****************************************************************************************************************************
		//	iEMファイル読み込み
		//*****************************************************************************************************************************
		s32		LoadiEM( LPIEMFILE lpIem, const char* filename )
		{

			char* buffer;
			s32 buffersize;
			AssetsLoader::load(&buffer, &buffersize, filename);
			utility::kBufferAnalyze analyze(buffer,(u32)buffersize);

			u32		FileID;
			s32		version, i;

			//	ID
			analyze.read(&FileID,4);
			char* iem=(char*)&FileID;
			dprintf("%c%c%c%c",iem[0],iem[1],iem[2],iem[3]);
			if( FileID == '1MEI' ) version = 1;
			else if( FileID == '2MEI' ) version = 2;
			else if( FileID == '3MEI' ) version = 3;
			else if( FileID == '4MEI' ) version = 4;
			else version = -1;

			lpIem->version = version;
			//	頂点	
			analyze.read(&lpIem->NumVertex, sizeof(u16));
			if( version < 4 )
			{
				lpIem->lpVertex = new MESHVERTEX[ lpIem->NumVertex ];
				analyze.read(lpIem->lpVertex,   sizeof(MESHVERTEX)*lpIem->NumVertex);
			} else {
				lpIem->lpVertex = new MESHVERTEX2[ lpIem->NumVertex ];
				analyze.read(lpIem->lpVertex,   sizeof(MESHVERTEX2)*lpIem->NumVertex);
			}
			//	ポリゴン	
			analyze.read(&lpIem->NumFace, sizeof(u16));
			dprintf("NumFace %u",lpIem->NumFace);
			lpIem->lpFace = new u16[ lpIem->NumFace*3 ];
			lpIem->lpAtr  = new u32[ lpIem->NumFace ];
			analyze.read(lpIem->lpFace,   sizeof(u16)*lpIem->NumFace*3);
			analyze.read(lpIem->lpAtr,    sizeof(u32)*lpIem->NumFace);
			//	マテリアル	
			analyze.read(&lpIem->NumMaterial, sizeof(u16));
			dprintf("NumMaterial %u",lpIem->NumMaterial);
			analyze.read(lpIem->Material,     sizeof(D3DMATERIAL9)*lpIem->NumMaterial);
			analyze.read(lpIem->Texture,      sizeof(char)*lpIem->NumMaterial*64);

			//	ボーン
			analyze.read(&lpIem->NumBone, sizeof(u16));
			dprintf("NumBone %u",lpIem->NumBone);
			lpIem->lpBone = new IEMBONE[ lpIem->NumBone ];
			for( i=0 ; i<lpIem->NumBone ; i++ ){
				analyze.read(&lpIem->lpBone[i], sizeof(IEMBONE));

				lpIem->lpBone[i].Index = new u32[lpIem->lpBone[i].IndexNum];
				analyze.read(lpIem->lpBone[i].Index, sizeof(u32)*lpIem->lpBone[i].IndexNum);

				lpIem->lpBone[i].Influence = new f32[lpIem->lpBone[i].IndexNum];
				analyze.read(lpIem->lpBone[i].Influence, sizeof(f32)*lpIem->lpBone[i].IndexNum);
			}

			//	モーション
			analyze.read(&lpIem->NumMotion, sizeof(u16));
			analyze.read(&lpIem->MaxFrame,  sizeof(u16));
			analyze.read(&lpIem->M_Offset,  sizeof(u16)*256);
			analyze.read(&lpIem->FrameFlag, sizeof(u16)*lpIem->MaxFrame);
			dprintf("NumMotion %u",lpIem->NumMotion);
			dprintf("MaxFrame %u",lpIem->MaxFrame);
			lpIem->lpMotion = new IEMMOTION[ lpIem->NumBone ];

			for( i=0 ; i<lpIem->NumBone ; i++ ){
				analyze.read(&lpIem->lpMotion[i], sizeof(IEMMOTION));

				lpIem->lpMotion[i].Rotate       = new math::Quaternion[lpIem->lpMotion[i].NumRotate];
				lpIem->lpMotion[i].RotateFrame  = new u16[lpIem->lpMotion[i].NumRotate];
				analyze.read(lpIem->lpMotion[i].Rotate, sizeof(math::Quaternion)*lpIem->lpMotion[i].NumRotate);
				analyze.read(lpIem->lpMotion[i].RotateFrame, sizeof(u16)*lpIem->lpMotion[i].NumRotate);

				lpIem->lpMotion[i].Position      = new math::Vector3[lpIem->lpMotion[i].NumPosition];
				lpIem->lpMotion[i].PositionFrame = new u16[lpIem->lpMotion[i].NumPosition];
				analyze.read(lpIem->lpMotion[i].Position, sizeof(math::Vector3)*lpIem->lpMotion[i].NumPosition);
				analyze.read(lpIem->lpMotion[i].PositionFrame, sizeof(u16)*lpIem->lpMotion[i].NumPosition);
			}

			delete[] buffer;

			return version;
		}

		//*****************************************************************************************************************************
		//	オブジェクト読み込み
		//*****************************************************************************************************************************
		bool	LoadObject( const char* filename ,kSkinData* data)
		{
			dprintf("LoadObject Start");
			IEMFILE		iem;
			char		workpath[MAX_PATH];

			dprintf("LoadiEM Start");
			s32 version = LoadiEM( &iem, filename );
			dprintf("LoadiEM End");
			if( version <= 0 )
			{
				return false;
			}

			CopyMemory( workpath, filename, strlen(filename)+1 );
			for( int i=strlen(filename) ; i>0 ; i-- ){
				if( utility::IsDBCSLeadByte(workpath[i-2]) ){
					i--;
					continue;
				}
				if( workpath[i-1] == '\\' || workpath[i-1] == '/' ){
					workpath[i] = '\0';
					break;
				}
			}
			dprintf("CreateFromIEM Start");
			CreateFromIEM( workpath, &iem ,data);
			dprintf("CreateFromIEM End");

			for( int i=0 ; i<iem.NumBone ; i++ ){
				delete[]	iem.lpBone[i].Index;
				delete[]	iem.lpBone[i].Influence;
				delete[]	iem.lpMotion[i].Rotate;
				delete[]	iem.lpMotion[i].RotateFrame;
				delete[]	iem.lpMotion[i].Position;
				delete[]	iem.lpMotion[i].PositionFrame;
			}
			delete[]	iem.lpVertex;
			delete[]	iem.lpFace;
			delete[]	iem.lpAtr;
			delete[]	iem.lpBone;
			delete[]	iem.lpMotion;

			//bLoad = true;
			dprintf("LoadObject End");
			return true;
		}
	public:
		kMeshLoadIEM(){}
		virtual ~kMeshLoadIEM(){}
		virtual bool Load(const char* filename,kMeshData* data)
		{
			return LoadObject(filename,(kSkinData*)data);
		}
	};
}