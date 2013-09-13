#include "kMesh.h"
#include "IMeshLoadDelegate.h"
#include "IMeshRenderDelegate.h"
#include "EffectLib\Effect.h"

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
			mp_MeshData=new kMeshData;
			readok=load->Load(filename,mp_MeshData);

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
			dprintf("~kMesh");
			SAFE_DELETE(mp_RenderDelegate);
			dprintf("~kMesh");
			SAFE_DELETE(mp_MeshData);
			dprintf("~kMesh");
			SAFE_DELETE(mp_UseCount);
			dprintf("~kMesh");
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
	//		レイピック
	//
	//**************************************************************************************************
	//------------------------------------------------------
	//		レイピック
	//------------------------------------------------------
	int	kMesh::RayPick( math::Vector3* out, const math::Vector3& pos, math::Vector3* vec,float* Dist)
	{
		int		ret = -1;

		//if( vec->x == .0f && vec->z == .0f ) return RayPickUD( out, pos, vec ,Dist);

		Matrix inv=m_TransMatrix;
		inv.inverse();

		Vector3 p = pos;
		Vector3 vv = *vec;

		p.trans(inv);
		vv.trans3x3(inv);

		float neart = *Dist;
		float dist = *Dist;
		*out = p;


		//	頂点サイズ計算
		int VertexSize = this->mp_MeshData->m_Info.NumVertex;

		//	バッファロック
		kMeshVertex	*pVertices=this->mp_MeshData->mp_Vertex;
		u16		*pIndices=this->mp_MeshData->m_Info.Index;
		u32 NumFace = this->mp_MeshData->m_Info.NumFace;

		Vector3 v1, v2, v3;
		Vector3	n;
		Vector3	l1, l2, l3;
		Vector3	temp;
		Vector3	cp;

		Vector3 p1, p2, p3;

		//dprintf("%d %u",VertexSize,NumFace);
		for( u32 j=0 ; j<NumFace ; j++ )
		{
			//dprintf("%d",j);
			//	面頂点取得
			int a = pIndices[j*3+0];
			v1.x = pVertices[a].m_Pos.x;	v1.y = pVertices[a].m_Pos.y;	v1.z = pVertices[a].m_Pos.z;
			//dprintf("v1 a=%u x=%f y=%f z=%f",a,v1.x,v1.y,v1.z);

			int b = pIndices[j*3+1] ;
			v2.x = pVertices[b].m_Pos.x;	v2.y = pVertices[b].m_Pos.y;	v2.z = pVertices[b].m_Pos.z;
			//dprintf("v2 b=%u x=%f y=%f z=%f",b,v2.x,v2.y,v2.z);

			int c = pIndices[j*3+2];
			v3.x = pVertices[c].m_Pos.x;	v3.y = pVertices[c].m_Pos.y;	v3.z = pVertices[c].m_Pos.z;
			//dprintf("v3 c=%u x=%f y=%f z=%f",c,v3.x,v3.y,v3.z);

			//	距離判定
			//Vector3	ss = (v1 + v2 + v3) / 3.0f - p;
			//if( ss.LengthSq() > dist ) continue;
			l1.x = v2.x - v1.x;
			l1.y = v2.y - v1.y;
			l1.z = v2.z - v1.z;
			l2.x = v3.x - v2.x;
			l2.y = v3.y - v2.y;
			l2.z = v3.z - v2.z;

			//	外積による法線算出		
			Vector3Cross( &n, l1, l2 );
			//	内積の結果がプラスならば裏向き
			float dot = Vector3Dot( vv, n );
			if( dot >= 0 ) continue;
			//	交点算出
			p1.x = v1.x - p.x;
			p1.y = v1.y - p.y;
			p1.z = v1.z - p.z;
			float t = Vector3Dot( n, p1 ) / dot;
			if( t < .0f || t > neart ) continue;

			cp.x = vv.x*t + p.x;
			cp.y = vv.y*t + p.y;
			cp.z = vv.z*t + p.z;
			//	内点判定
			p1.x = v1.x - cp.x;
			p1.y = v1.y - cp.y;
			p1.z = v1.z - cp.z;

			Vector3Cross( &temp, p1, l1 );
			if( Vector3Dot(temp, n) < .0f ) continue;

			p2.x = v2.x - cp.x;
			p2.y = v2.y - cp.y;
			p2.z = v2.z - cp.z;
			Vector3Cross( &temp, p2, l2 );
			if( Vector3Dot(temp, n) < .0f ) continue;

			l3.x = v1.x - v3.x;
			l3.y = v1.y - v3.y;
			l3.z = v1.z - v3.z;
			p3.x = v3.x - cp.x;
			p3.y = v3.y - cp.y;
			p3.z = v3.z - cp.z;
			Vector3Cross( &temp, p3, l3 );
			if( Vector3Dot(temp, n) < .0f ) continue;

			*out = cp;
			*vec = n;
			ret = j;
			neart = t;
		}

		*Dist = neart;

		out->trans(m_TransMatrix);
		vec->trans3x3(m_TransMatrix);

		if(ret!=-1)eprintf("Hit!!!!!!%f %f %f",out->x,out->y,out->z);
		return	ret;
	}

	//------------------------------------------------------
	//		上下最適化
	//------------------------------------------------------
	int	kMesh::RayPickUD( math::Vector3* out, const math::Vector3& pos, math::Vector3* vec,float* Dist )
	{
		float	t, neart;
		float	vy;
		int		ret = -1;

		int		VertexSize;

		Matrix inv=m_TransMatrix;
		inv.inverse();

		Vector3 p = pos;
		Vector3 vv=*vec;

		p.trans(inv);
		//vv.trans3x3(inv);
		//vy*=inv._22;
		vy=vv.y;

		neart = *Dist;

		//	頂点サイズ計算
		VertexSize = this->mp_MeshData->m_Info.NumVertex;

		//	バッファロック
		kMeshVertex	*pVertices=this->mp_MeshData->mp_Vertex;
		u16		*pIndices=this->mp_MeshData->m_Info.Index;
		int NumFace = this->mp_MeshData->m_Info.NumFace;

		Vector3	l1, l2, l3;
		Vector3	p1, p2, p3;
		Vector3 v[3];
		Vector3 n;

		for( int j=0 ; j<NumFace ; j++ )
		{
			//	面頂点取得
			int a = pIndices[j*3+0];
			int b = pIndices[j*3+1];
			int c = pIndices[j*3+2];

			v[0].x = pVertices[a].m_Pos.x;	v[1].x = pVertices[b].m_Pos.x;	v[2].x = pVertices[c].m_Pos.x;
			if( v[0].x > p.x && v[1].x > p.x && v[2].x > p.x ) continue;

			v[0].z = pVertices[a].m_Pos.z;	v[1].z = pVertices[b].m_Pos.z;	v[2].z = pVertices[c].m_Pos.z;
			if( v[0].z > p.z && v[1].z > p.z && v[2].z > p.z ) continue;

			v[0].y = pVertices[a].m_Pos.y;	v[1].y = pVertices[b].m_Pos.y;	v[2].y = pVertices[c].m_Pos.y;


			//	内点判定（全外積がマイナス）		
			l1.x = v[1].x - v[0].x;
			l1.z = v[1].z - v[0].z;
			p1.x = v[0].x - p.x;
			p1.z = v[0].z - p.z;
			if( (p1.x*l1.z - p1.z*l1.x)*vy < 0 ) continue;

			l2.x = v[2].x - v[1].x;
			l2.z = v[2].z - v[1].z;
			p2.x = v[1].x - p.x;
			p2.z = v[1].z - p.z;
			if( (p2.x*l2.z - p2.z*l2.x)*vy < 0 ) continue;

			l3.x = v[0].x - v[2].x;
			l3.z = v[0].z - v[2].z;
			p3.x = v[2].x - p.x;
			p3.z = v[2].z - p.z;
			if( (p3.x*l3.z - p3.z*l3.x)*vy < 0 ) continue;

			//	外積による法線算出		
			l1.y = v[1].y - v[0].y;
			l2.y = v[2].y - v[1].y;
			Vector3Cross( &n, l1, l2 );
			//	表裏判定
			if( vy*n.y >= 0 ) continue;

			//	交点算出
			p1.y = v[0].y - p.y;
			t = Vector3Dot( n, p1 ) / (n.y*vy);
			if( t < .0f || t > neart ) continue;

			*vec = n;
			ret = j;
			neart = t;
		}

		//out->y = neart*vy + p.y;
		//out->x = p.x;
		//out->z = p.z;
		out->x  = pos.x;
		out->y  = p.x*m_TransMatrix._12+(neart*vy + p.y)*m_TransMatrix._22+p.z*m_TransMatrix._32+m_TransMatrix._42;
		out->z  = pos.z;
		*Dist = neart;

		//out->trans(m_TransMatrix);
		vec->trans3x3(m_TransMatrix);

		return	ret;
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
			//kSkinData retMesh;
			mp_MeshData=new kSkinData;
			readok=load->Load(filename,mp_MeshData);

			dprintf("%u",((kSkinData*)mp_MeshData)->m_Info.NumBone);

			mp_Original=new kMeshVertex[mp_MeshData->m_Info.NumVertex];
			CopyMemory(mp_Original,mp_MeshData->mp_Vertex,sizeof(kMeshVertex)*mp_MeshData->m_Info.NumVertex);

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
		//if( *mp_UseCount==0 )
		//{
		//	dprintf("~kSkin");
		//	SAFE_DELETE(mp_RenderDelegate);
		//	dprintf("~kSkin");
		//	SAFE_DELETE(mp_MeshData);
		//	dprintf("~kSkin");
		//	SAFE_DELETE(mp_UseCount);
		//	dprintf("~kSkin");
		//}
		//else *mp_UseCount--;
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

				MatrixVec3TransformCoord(&out,mp_Original[m_Skin->mp_Bone[i].Index[j]].m_Pos,lpMatrix[i]);
				mp_MeshData->mp_Vertex[m_Skin->mp_Bone[i].Index[j]].m_Pos+=out*m_Skin->mp_Bone[i].Influence[j];
				mp_MeshData->mp_Vertex[m_Skin->mp_Bone[i].Index[j]].m_Color=math::Vector4(1,1,1,1);
				mp_MeshData->mp_Vertex[m_Skin->mp_Bone[i].Index[j]].m_Normal=mp_Original[m_Skin->mp_Bone[i].Index[j]].m_Normal;
				mp_MeshData->mp_Vertex[m_Skin->mp_Bone[i].Index[j]].m_TexCoord=mp_Original[m_Skin->mp_Bone[i].Index[j]].m_TexCoord;
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