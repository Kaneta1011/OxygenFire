#pragma once
#include "defines.h"
#include "math\kmathf.h"
#include "GraphicsLib\Class\tRenderState\RenderState.h"
#include "../kDevice/kDevice.h"
#include "GraphicsLib\Class\tIObject3D\Object3D.h"
#include "GraphicsLib\Class\rTexture\Texture.h"

//kMesh�ɑ��݂��郁�b�V���ւ̃|�C���^��void*�ɂ���
//���[�U�[�����_�̃t�H�[�}�b�g���w�肵��
//���C�u�������ŋz�����ă��[�U�[�w��̍\���̂��g����悤�ɂ���
//���ۂ̃��b�V���͓ǂݍ��݈Ϗ��N���X�ɍ쐬����
//kMesh�̃|�C���^�͂����ւ̃|�C���^�Ƃ���


namespace klib
{
	class IMeshLoadDelegate;
	class IMeshRenderDelegate;

	struct kMeshInfo
	{

		u32				NumVertex;		//���_��
		u32				NumFace;		//�|���S����
		u32				MaterialCount;	//	�ގ���
		u16*			Index;				//���_�C���f�b�N�X
		u32*			Material;			//�|���S���ގ��ԍ�
		u32*			MaterialNumFace;	//�ގ����Ƃ̃|���S����
		u16**			MaterialIndex;	//�ގ����Ƃ̒��_�C���f�b�N�X
		//���v�t���[����
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

			//	�e�N�X�`�����
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
			math::Matrix				BoneMatrix;			//	�{�[���s��
			u16								parent;				//	�e�{�[��

			math::Quaternion		orgPose;			//	��{�p��
			math::Vector3			orgPos;				//	��{���W

			u16								IndexNum;			//	�e�����_��
			u32*							Index;				//	�e�����_Index
			f32*								Influence;			//	�e����
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

	//���b�V���ɕK�v�ȃf�[�^�\����
	struct kMeshData
	{
		kMeshInfo		m_Info;
		kMeshVertex*	mp_Vertex;		//���_�f�[�^
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
		//�{�[���f�[�^
		kSkinBone	*				mp_Bone;
		//�A�j���[�V�����f�[�^
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
		IMeshRenderDelegate* mp_RenderDelegate;			//�ǂݍ��݈Ϗ����\�b�h

		//���b�V���f�[�^
		kMeshData* mp_MeshData;

		s32				*mp_UseCount;
		math::Matrix			m_TransMatrix;	//	�]���s��
	public:


		//------------------------------------------------------
		//	������
		//------------------------------------------------------
		kMesh(){}
		kMesh( const char* filename ,IMeshLoadDelegate* load,IMeshRenderDelegate* render);
		//klibMesh2(){ UseNum = new int; *UseNum=0;}
		kMesh*	Clone();
		virtual ~kMesh();

		//------------------------------------------------------
		//	�X�V
		//------------------------------------------------------
		void Update();
		//------------------------------------------------------
		//	�`��
		//------------------------------------------------------
		void Render(kGraphicsPipline* shader);
		//------------------------------------------------------
		//	���C����
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
		//	������
		//------------------------------------------------------
		kSkin(){}
		kSkin( const char* filename ,IMeshLoadDelegate* load,IMeshRenderDelegate* render);
		//klibMesh2(){ UseNum = new int; *UseNum=0;}
		kSkin*	Clone();
		virtual ~kSkin();

		//------------------------------------------------------
		//	�X�V
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
		//	�`��
		//------------------------------------------------------
		void Render(kGraphicsPipline* shader);
		//------------------------------------------------------
		//	���C����
		//------------------------------------------------------
	};
}