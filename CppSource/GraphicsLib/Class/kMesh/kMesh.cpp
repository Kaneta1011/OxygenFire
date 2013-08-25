#include "kMesh.h"
#include "IMeshLoadDelegate.h"
#include "IMeshRenderDelegate.h"

namespace klib
{
	using namespace math;
	kMesh::kMesh( const char* filename ,IMeshLoadDelegate* load,IMeshRenderDelegate* render )
	{
		m_bChanged=false;
		mp_RenderDelegate=render;

		bool readok;

		mp_UseCount=new int;
		{
			kMeshData retMesh;
			readok=load->Load(filename,&retMesh);
			mp_MeshData=new kMeshData;
			mp_MeshData->mp_Vertex=new kMeshVertex[retMesh.m_Info.NumVertex];
			CopyMemory(&mp_MeshData->m_Info,&retMesh.m_Info,sizeof(kMeshInfo));

			for(s32 i=0;i<retMesh.m_Info.NumVertex;i++)
			{
				((kMeshVertex*)mp_MeshData->mp_Vertex)[i].m_Pos=((kMeshVertex2*)retMesh.mp_Vertex)[i].m_Pos;
				((kMeshVertex*)mp_MeshData->mp_Vertex)[i].m_Color=Vector4(1,1,1,1);
			}
		}

		if( !readok )
		{
			//	読み込み失敗
			WARNING(0,"kesh Loading error %s\n", filename);
			*mp_UseCount = 0;
		} else {
			//頂点、インデックスバッファを作成
			//ついでにマテリアルごとに分ける
			//CreateBuffer();
			mp_RenderDelegate->CreateBuffer(this);
			*mp_UseCount = 0;
		}
		setPosition(0.f,0.f,0.f);
		setScale(1.0f);
		setAngle(0.f);
		Update();

		SAFE_DELETE(load);
	}
	kMesh::~kMesh()
	{
		if( *mp_UseCount==0 )
		{
			SAFE_DELETE(mp_RenderDelegate);
			SAFE_DELETE(mp_MeshData);
			SAFE_DELETE(mp_UseCount);
		}
		else *mp_UseCount--;
	}
	//------------------------------------------------------
	//	クローンの作成
	//------------------------------------------------------

	kMesh*	kMesh::Clone()
	{
		kMesh* obj = new kMesh(*this);
		*mp_UseCount++;
		return obj;
	}

	//**************************************************************************************************
	//
	//		更新処理
	//
	//**************************************************************************************************
	void	kMesh::Update()
	{
		//位置、スケール、角度が変更されていなければ更新をしない
		//if( !m_bChanged ) return;
		Matrix MatScale;
		//	スケーリング
		MatScale.identity();
		MatScale.mulS(m_Scale);
		//	座標系設定 
		m_TransMatrix.setRZXY(m_Angle);
		m_TransMatrix.setT(m_Position);
		//	転送行列作成 
		m_TransMatrix = MatScale * m_TransMatrix;

		m_bChanged=false;
	}

	//**************************************************************************************************
	//
	//		描画
	//
	//**************************************************************************************************

	//------------------------------------------------------
	//	通常描画
	//------------------------------------------------------
	void kMesh::Render(kGraphicsPipline* shader)
	{
		mp_RenderDelegate->Render(m_TransMatrix,shader);
	}








	kSkin::kSkin( const char* filename ,IMeshLoadDelegate* load,IMeshRenderDelegate* render )
	{
		m_bChanged=false;
		mp_RenderDelegate=render;

		bool readok;

		mp_UseCount=new int;
		{
			kSkinData retMesh;
			readok=load->Load(filename,&retMesh);
			dprintf("kSkin Load End");
			mp_MeshData=new kSkinData;
			mp_MeshData->mp_Vertex=new kMeshVertex[retMesh.m_Info.NumVertex];
			dprintf("kSkin Load End");
			CopyMemory(&mp_MeshData->m_Info,&retMesh.m_Info,sizeof(kMeshInfo));
			dprintf("kSkin Load End,%u",retMesh.m_Info.NumVertex);
	//**************************************************************************************************
	//		頂点コピー
	//**************************************************************************************************
			for(s32 i=0;i<retMesh.m_Info.NumVertex;i++)
			{
				((kMeshVertex*)mp_MeshData->mp_Vertex)[i].m_Pos=((kMeshVertex2*)retMesh.mp_Vertex)[i].m_Pos;
				((kMeshVertex*)mp_MeshData->mp_Vertex)[i].m_Color=Vector4(1,1,1,1);
			}
				((kSkinData*)mp_MeshData)->mp_Animation=new kSkinAnimation[retMesh.m_Info.NumBone];
				((kSkinData*)mp_MeshData)->mp_Bone=new kSkinBone[retMesh.m_Info.NumBone];
			for( int i=0 ; i<retMesh.m_Info.NumBone ; i++ ){
	//**************************************************************************************************
	//		モーションコピー
	//**************************************************************************************************
				((kSkinData*)mp_MeshData)->mp_Animation[i].rotNum = retMesh.mp_Animation[i].rotNum;
				((kSkinData*)mp_MeshData)->mp_Animation[i].rotFrame = new u16[ retMesh.mp_Animation[i].rotNum ];
				((kSkinData*)mp_MeshData)->mp_Animation[i].rot      = new math::Quaternion[ retMesh.mp_Animation[i].rotNum ];

				for( int j=0 ; j<retMesh.mp_Animation[i].rotNum ; j++ ){
					((kSkinData*)mp_MeshData)->mp_Animation[i].rotFrame[j] = retMesh.mp_Animation[i].rotFrame[j];
					((kSkinData*)mp_MeshData)->mp_Animation[i].rot[j] = retMesh.mp_Animation[i].rot[j];
				}
				//	ポジションコピー
				((kSkinData*)mp_MeshData)->mp_Animation[i].posNum   = retMesh.mp_Animation[i].posNum;
				if( ((kSkinData*)mp_MeshData)->mp_Animation[i].posNum > 0 )
				{
					((kSkinData*)mp_MeshData)->mp_Animation[i].posFrame = new u16[ retMesh.mp_Animation[i].posNum ];
					((kSkinData*)mp_MeshData)->mp_Animation[i].pos      = new math::Vector3[ retMesh.mp_Animation[i].posNum ];
				}
				for( int j=0 ; j<((kSkinData*)mp_MeshData)->mp_Animation[i].posNum ; j++ ){
					((kSkinData*)mp_MeshData)->mp_Animation[i].posFrame[j] = retMesh.mp_Animation[i].posFrame[j];
					((kSkinData*)mp_MeshData)->mp_Animation[i].pos[j] = retMesh.mp_Animation[i].pos[j];
				}

	//**************************************************************************************************
	//		ボーンコピー
	//**************************************************************************************************
				((kSkinData*)mp_MeshData)->mp_Bone[i].parent     = retMesh.mp_Bone[i].parent;		//	親
				((kSkinData*)mp_MeshData)->mp_Bone[i].BoneMatrix=retMesh.mp_Bone[i].BoneMatrix;	//	基準化行列
				((kSkinData*)mp_MeshData)->mp_Bone[i].orgPos         = retMesh.mp_Bone[i].orgPos;		//	標準位置
				((kSkinData*)mp_MeshData)->mp_Bone[i].orgPose        = retMesh.mp_Bone[i].orgPose;		//	標準姿勢
				((kSkinData*)mp_MeshData)->mp_Bone[i].IndexNum=retMesh.mp_Bone[i].IndexNum;		//ボーン影響インデックス数
				//ボーン影響インデックス
				((kSkinData*)mp_MeshData)->mp_Bone[i].Index=new u32[retMesh.mp_Bone[i].IndexNum];
				CopyMemory(((kSkinData*)mp_MeshData)->mp_Bone[i].Index,retMesh.mp_Bone[i].Index,sizeof(u32)*retMesh.mp_Bone[i].IndexNum);
				//ボーン影響度
				((kSkinData*)mp_MeshData)->mp_Bone[i].Influence=new f32[retMesh.mp_Bone[i].IndexNum];
				CopyMemory(((kSkinData*)mp_MeshData)->mp_Bone[i].Influence,retMesh.mp_Bone[i].Influence,sizeof(f32)*retMesh.mp_Bone[i].IndexNum);

			}

				dprintf("%u",((kSkinData*)mp_MeshData)->m_Info.NumBone);
				for(int i=0;i<((kSkinData*)mp_MeshData)->m_Info.NumBone;i++)
				{
					for(int j=0;j<((kSkinData*)mp_MeshData)->mp_Bone[i].IndexNum;j++)dprintf("%u",((kSkinData*)mp_MeshData)->mp_Bone[i].Index[j]);
				}
				//CopyMemory(((kSkinData*)mp_MeshData)->mp_Animation,retMesh.mp_Animation,sizeof(kSkinAnimation)*retMesh.m_Info.NumBone);
				//CopyMemory(((kSkinData*)mp_MeshData)->mp_Bone,retMesh.mp_Bone,sizeof(kSkinBone)*retMesh.m_Info.NumBone);
			mp_Original=new kMeshVertex[retMesh.m_Info.NumVertex];
				CopyMemory(mp_Original,mp_MeshData->mp_Vertex,sizeof(kMeshVertex)*retMesh.m_Info.NumVertex);
		}
		dprintf("kSkin Load End");
		if( !readok )
		{
			//	読み込み失敗
			WARNING(0,"kesh Loading error %s\n", filename);
			*mp_UseCount = 0;
		} else {
			//頂点、インデックスバッファを作成
			//ついでにマテリアルごとに分ける
			//CreateBuffer();
			dprintf("kSkin Load End");
			mp_RenderDelegate->CreateBuffer(this);
			*mp_UseCount = 0;
		}
		dprintf("kSkin Load End");
		setPosition(0.f,0.f,0.f);
		setScale(1.0f);
		setAngle(0.f);
		Update();
		dprintf("kSkin Load End");
		SAFE_DELETE(load);
	}
	kSkin::~kSkin()
	{
		if( *mp_UseCount==0 )
		{
			SAFE_DELETE(mp_RenderDelegate);
			SAFE_DELETE(mp_MeshData);
			SAFE_DELETE(mp_UseCount);
		}
		else *mp_UseCount--;
	}
	//------------------------------------------------------
	//	クローンの作成
	//------------------------------------------------------

	kSkin*	kSkin::Clone()
	{
		kSkin* obj = new kSkin(*this);
		*mp_UseCount++;
		return obj;
	}

	//**************************************************************************************************
	//
	//		更新処理
	//
	//**************************************************************************************************
	//------------------------------------------------------
	//	モーション
	//------------------------------------------------------
	void kSkin::animation(f32 dt)
	{
		const kSkinData* m_Skin=(const kSkinData*)mp_MeshData;
		int		param;
		u32	work;

		work = (u32)dwFrame;
		param = m_Skin->m_Info.dwFrameFlag[(s32)dwFrame];
		if( param & 0x4000 ) param = 0xFFFF;
		if( param != 0xFFFF ){
			//	アニメーションジャンプ
			if( param & 0x8000 ){
				SetMotion( param&0xFF );
			} else dwFrame = (f32)param;
		} else {
			dwFrame +=dt;
			if( dwFrame >= m_Skin->m_Info.NumFrame ) dwFrame = 0.0f;
		}

		//if( dwFrame != work ) bChanged = TRUE;

		param = m_Skin->m_Info.dwFrameFlag[(s32)dwFrame];
		if( (param!=0xFFFF) && (param&0x4000) ) Param[(param&0x0F00)>>8] = (u8)(param&0x00FF);

	}
	//*****************************************************************************
	//	フレーム更新
	//*****************************************************************************
	void	kSkin::updateSkinMesh( f32 frame )
	{
		const kSkinData* m_Skin=(const kSkinData*)mp_MeshData;
		u32			i, j;
		kSkinAnimation*	lpAnime;
		float		t;

		for( i=0 ; i<m_Skin->m_Info.NumBone ; i++ ){
			lpAnime = &m_Skin->mp_Animation[i];

			//	ポーズ設定
			if( lpAnime->rotNum == 0 ) CurPose[i] = m_Skin->mp_Bone[i].orgPose;
			else if( lpAnime->rotNum == 1 ) CurPose[i] = lpAnime->rot[0];
			else {
				//	回転キー補間
				for( j=0 ; j<lpAnime->rotNum-1 ; j++ )
				{
					//	現在位置検索
					if( (frame>=lpAnime->rotFrame[j]) && (frame<lpAnime->rotFrame[j+1]) )
					{
						//	経過フレーム計算
						t = (float)(frame-lpAnime->rotFrame[j]) / (float)(lpAnime->rotFrame[j+1] - lpAnime->rotFrame[j]);
						//	補間
						QuaternionSlerp(&CurPose[i],lpAnime->rot[j], lpAnime->rot[j+1], t);
						//CurPose[i] = QuaternionSlerp( lpAnime->rot[j], lpAnime->rot[j+1], t );
						break;
					}
				}
				if( j == lpAnime->rotNum-1 ) CurPose[i] = lpAnime->rot[lpAnime->rotNum-1];
			}
			//	座標設定
			if( lpAnime->posNum == 0 ) CurPos[i] = m_Skin->mp_Bone[i].orgPos;
			else {
				//	位置補間
				for( j=0 ; j<lpAnime->posNum-1 ; j++ )
				{
					//	現在位置検索
					if( (frame>=lpAnime->posFrame[j]) && (frame<lpAnime->posFrame[j+1]) )
					{
						//	経過フレーム計算
						t = (float)(frame-lpAnime->posFrame[j]) / (float)(lpAnime->posFrame[j+1] - lpAnime->posFrame[j]);
						//	補間
						CurPos[i] = lpAnime->pos[j] + (lpAnime->pos[j+1]-lpAnime->pos[j])*t;
						break;
					}
				}
				if( j == lpAnime->posNum-1 ) CurPos[i] = lpAnime->pos[lpAnime->posNum-1];
			}
		}
	}

	//*****************************************************************************
	//	ボーン行列更新
	//*****************************************************************************
	void kSkin::updateBoneMatrix()
	{
		u32			i;
		const kSkinData* m_Skin=(const kSkinData*)mp_MeshData;
		//	ボーン更新
		for( i=0 ; i<m_Skin->m_Info.NumBone ; i++ )
		{
			CurPose[i].toMatrix( &lpBoneMatrix[i] );
			//	位置情報コピー
			lpBoneMatrix[i]._41 = CurPos[i].x;
			lpBoneMatrix[i]._42 = CurPos[i].y;
			lpBoneMatrix[i]._43 = CurPos[i].z;

			if( m_Skin->mp_Bone[i].parent != 0xFFFF ) lpBoneMatrix[i] *= lpBoneMatrix[ m_Skin->mp_Bone[i].parent ];
		}
		for( i=0 ; i<m_Skin->m_Info.NumBone ; i++ )
		{
			lpMatrix[i] = m_Skin->mp_Bone[i].BoneMatrix * lpBoneMatrix[i];
		}
	}
	void	kSkin::Update()
	{
		updateSkinMesh( (f32)dwFrame );
		updateBoneMatrix();
		//位置、スケール、角度が変更されていなければ更新をしない
		//if( !m_bChanged ) return;
		Matrix MatScale;
		//	スケーリング
		MatScale.identity();
		MatScale.mulS(m_Scale);
		//	座標系設定 
		m_TransMatrix.setRZXY(m_Angle);
		m_TransMatrix.setT(m_Position);
		//	転送行列作成 
		m_TransMatrix = MatScale * m_TransMatrix;

		m_bChanged=false;

		//座標更新用変数
		const kSkinData* m_Skin=(kSkinData*)mp_MeshData;
		ZeroMemory(mp_MeshData->mp_Vertex,sizeof(kMeshVertex)*mp_MeshData->m_Info.NumVertex);
		for(int i=0;i<(s32)m_Skin->m_Info.NumBone;i++)
		{
			//頂点に影響しているボーン行列を重みをつけて掛けていく
			for(int j=0;j<m_Skin->mp_Bone[i].IndexNum;j++)
			{
				math::Vector3 out;
				//dprintf("%u",m_Skin->mp_Bone[i].Index[j]);
				MatrixVec3TransformCoord(&out,mp_Original[m_Skin->mp_Bone[i].Index[j]].m_Pos,lpMatrix[i]);
				mp_MeshData->mp_Vertex[m_Skin->mp_Bone[i].Index[j]].m_Pos+=out*m_Skin->mp_Bone[i].Influence[j];
				mp_MeshData->mp_Vertex[m_Skin->mp_Bone[i].Index[j]].m_Color=math::Vector4(1,1,1,1);
			}
		}

	}

	//**************************************************************************************************
	//
	//		描画
	//
	//**************************************************************************************************

	//------------------------------------------------------
	//	通常描画
	//------------------------------------------------------
	void kSkin::Render(kGraphicsPipline* shader)
	{
		mp_RenderDelegate->Render(m_TransMatrix,shader);
	}
};