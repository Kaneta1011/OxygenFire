#pragma once
#include "defines.h"
#include "math\kmathf.h"
#include "GraphicsLib\Class\tRenderState\RenderState.h"
#include "../kDevice/kDevice.h"
#include "GraphicsLib\Class\tIObject3D\Object3D.h"
#include "GraphicsLib\Class\rTexture\Texture.h"

//kMeshに存在するメッシュへのポインタをvoid*にして
//ユーザーが頂点のフォーマットを指定して
//ライブラリ側で吸収してユーザー指定の構造体を使えるようにする
//実際のメッシュは読み込み委譲クラスに作成する
//kMeshのポインタはそこへのポインタとする


namespace klib
{
	class IMeshLoadDelegate;
	class IMeshRenderDelegate;

	struct kMeshInfo
	{

		u32				NumVertex;		//頂点数
		u32				NumFace;		//ポリゴン数
		u32				MaterialCount;	//	材質数
		u16*			Index;				//頂点インデックス
		u32*			Material;			//ポリゴン材質番号
		u32*			MaterialNumFace;	//材質ごとのポリゴン数
		u16**			MaterialIndex;	//材質ごとの頂点インデックス
		//合計フレーム数
		u32				NumFrame;
		u32*			dwFrameFlag;
		u16*			motionOffsets;
		u16				NumBone;
		rlib::Texture* Diffuse[32];
		rlib::Texture* Normal[32];
		rlib::Texture* Specular[32];
		kMeshInfo()
		{
			for(int i=0;i<32;i++)
			{
				Diffuse[i]=Normal[i]=Specular[i]=NULL;
			}
			
			NumVertex=NumFace=MaterialCount=NumFrame=NumBone=0;
			Material=MaterialNumFace=dwFrameFlag=NULL;
			Index=NULL;
			MaterialIndex=NULL;
			motionOffsets=NULL;
		}
		~kMeshInfo()
		{
			//SAFE_DELETE(Index);
			//SAFE_DELETE(Material);

			//	テクスチャ解放
			for( u32 i=0 ; i<MaterialCount ; i++ ){
				SAFE_DELETE(Diffuse[i]);
				SAFE_DELETE(Normal[i]);
				SAFE_DELETE(Specular[i]);
				//SAFE_DELETE(MaterialIndex[i]);
			}
			//SAFE_DELETE_ARRAY(MaterialNumFace);
			//SAFE_DELETE_ARRAY(MaterialIndex);
		}
	};

	struct kMeshVertex
	{
		math::Vector3 m_Pos;
		math::Vector4 m_Color;
		math::Vector3	m_Normal;
		math::Vector2	m_TexCoord;
	};

	struct kSkinAnimation {


		u32			rotNum;
		u16*		rotFrame;
		math::Quaternion*	rot;

		u32			posNum;
		u16*		posFrame;
		math::Vector3*	pos;
		kSkinAnimation()
		{
			rotFrame=NULL;
			rot=NULL;
			posFrame=NULL;
			pos=NULL;
		}
		~kSkinAnimation()
		{
			SAFE_DELETE_ARRAY(rotFrame);
			SAFE_DELETE_ARRAY(rot);
			SAFE_DELETE_ARRAY(posFrame);
			SAFE_DELETE_ARRAY(pos);
		}
	};

	struct kSkinBone
	{
			math::Matrix				BoneMatrix;			//	ボーン行列
			u16								parent;				//	親ボーン

			math::Quaternion		orgPose;			//	基本姿勢
			math::Vector3			orgPos;				//	基本座標

			u16								IndexNum;			//	影響頂点数
			u32*							Index;				//	影響頂点Index
			f32*								Influence;			//	影響力
			kSkinBone()
			{
				Index=NULL;
				Influence=NULL;
			}
			~kSkinBone()
			{
				SAFE_DELETE_ARRAY(Index);
				SAFE_DELETE_ARRAY(Influence);
			}
	};

	//メッシュに必要なデータ構造体
	struct kMeshData
	{
		kMeshInfo		m_Info;
		kMeshVertex*	mp_Vertex;		//頂点データ
		kMeshData()
		{
			mp_Vertex = NULL;
		}
		virtual ~kMeshData()
		{
			dprintf("~kMeshData");
			SAFE_DELETE(mp_Vertex);
			dprintf("~kMeshData");
		}
	};

	struct kSkinData:public	kMeshData
	{
		//ボーンデータ
		kSkinBone	*				mp_Bone;
		//アニメーションデータ
		kSkinAnimation*		mp_Animation;

		kSkinData():kMeshData()
		{
			mp_Bone=NULL;
			mp_Animation=NULL;
		}
		~kSkinData()
		{
			dprintf("~kSkinData");
			SAFE_DELETE_ARRAY(mp_Bone);
			dprintf("~kSkinData");
			SAFE_DELETE_ARRAY(mp_Animation);
			dprintf("~kSkinData");
		}
	};

	class kMesh :public RenderLib::IObject3D
	{
	private:
	protected:
		bool m_bChanged;
		IMeshRenderDelegate* mp_RenderDelegate;			//読み込み委譲メソッド

		//メッシュデータ
		kMeshData* mp_MeshData;

		s32				*mp_UseCount;
		math::Matrix			m_TransMatrix;	//	転送行列
	public:


		//------------------------------------------------------
		//	初期化
		//------------------------------------------------------
		kMesh(){}
		kMesh( const char* filename ,IMeshLoadDelegate* load,IMeshRenderDelegate* render);
		//klibMesh2(){ UseNum = new int; *UseNum=0;}
		kMesh*	Clone();
		virtual ~kMesh();

		//------------------------------------------------------
		//	更新
		//------------------------------------------------------
		void Update();
		//------------------------------------------------------
		//	描画
		//------------------------------------------------------
		void Render(kGraphicsPipline* shader);
		//------------------------------------------------------
		//	レイ判定
		//------------------------------------------------------
		//int	RayPick( math::Vector3* out, const math::Vector3& pos, math::Vector3* vec, float* Dist );
		//int	RayPickUD( math::Vector3* out, const math::Vector3& pos, math::Vector3* vec, float* Dist );

		inline s32 getUseCount()const{return *mp_UseCount;}
		inline const kMeshData* getMeshPtr()const{return mp_MeshData;}
		inline const math::Matrix& getTransMatrixRef()const{return m_TransMatrix;}
		inline void setTransMatrix(const math::Matrix& mat){m_TransMatrix=mat;}
	};

	class kSkin:public kMesh
	{
	private:
	protected:
		kMeshVertex* mp_Original;
		math::Quaternion CurPose[256];
		math::Vector3 CurPos[256];
		math::Matrix lpBoneMatrix[256];
		math::Matrix lpMatrix[256];

		u32				Param[16];
		u16 Motion;
		f32 dwFrame;
	public:
		//------------------------------------------------------
		//	初期化
		//------------------------------------------------------
		kSkin(){}
		kSkin( const char* filename ,IMeshLoadDelegate* load,IMeshRenderDelegate* render);
		//klibMesh2(){ UseNum = new int; *UseNum=0;}
		kSkin*	Clone();
		virtual ~kSkin();

		//------------------------------------------------------
		//	更新
		//------------------------------------------------------
	private:
		void updateBoneMatrix();
		void updateSkinMesh(f32 frame);
	public:
		u16 getMotion()const{return Motion;}
		void forceMotion( int motion )
		{
			const kSkinData* m_Skin=(const kSkinData*)mp_MeshData;
			int		param;

			if( m_Skin->m_Info.motionOffsets[motion] == 65535 ) return;
			Motion  = motion;
			dwFrame = (f32)m_Skin->m_Info.motionOffsets[motion];
			//bChanged = true;

			param = m_Skin->m_Info.dwFrameFlag[(s32)dwFrame];
			if( (param!=0xFFFF) && (param&0x4000) ) Param[(param&0x0F00)>>8] = (u32)(param&0x00FF);
		}
		void SetMotion( int motion )
		{
			if(Motion==motion)return;
			forceMotion(motion);
		}
		void animation(f32 dt);
		void Update();
		//------------------------------------------------------
		//	描画
		//------------------------------------------------------
		void Render(kGraphicsPipline* shader);
		//------------------------------------------------------
		//	レイ判定
		//------------------------------------------------------
	};
}