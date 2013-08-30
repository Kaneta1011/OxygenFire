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
			mp_MeshData=new kMeshData;
			readok=load->Load(filename,mp_MeshData);

		}

		if( !readok )
		{
			//	�ǂݍ��ݎ��s
			WARNING(0,"kesh Loading error %s\n", filename);
			*mp_UseCount = 0;
		} else {
			//���_�A�C���f�b�N�X�o�b�t�@���쐬
			//���łɃ}�e���A�����Ƃɕ�����
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
	//	�N���[���̍쐬
	//------------------------------------------------------

	kMesh*	kMesh::Clone()
	{
		kMesh* obj = new kMesh(*this);
		*mp_UseCount++;
		return obj;
	}

	//**************************************************************************************************
	//
	//		�X�V����
	//
	//**************************************************************************************************
	void	kMesh::Update()
	{
		//�ʒu�A�X�P�[���A�p�x���ύX����Ă��Ȃ���΍X�V�����Ȃ�
		//if( !m_bChanged ) return;
		Matrix MatScale;
		//	�X�P�[�����O
		MatScale.identity();
		MatScale.mulS(m_Scale);
		//	���W�n�ݒ� 
		m_TransMatrix.setRZXY(m_Angle);
		m_TransMatrix.setT(m_Position);
		//	�]���s��쐬 
		m_TransMatrix = MatScale * m_TransMatrix;

		m_bChanged=false;
	}

	//**************************************************************************************************
	//
	//		�`��
	//
	//**************************************************************************************************

	//------------------------------------------------------
	//	�ʏ�`��
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
			//	�ǂݍ��ݎ��s
			WARNING(0,"kesh Loading error %s\n", filename);
			*mp_UseCount = 0;
		} else {
			//���_�A�C���f�b�N�X�o�b�t�@���쐬
			//���łɃ}�e���A�����Ƃɕ�����
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
	//	�N���[���̍쐬
	//------------------------------------------------------

	kSkin*	kSkin::Clone()
	{
		kSkin* obj = new kSkin(*this);
		*mp_UseCount++;
		return obj;
	}

	//**************************************************************************************************
	//
	//		�X�V����
	//
	//**************************************************************************************************
	//------------------------------------------------------
	//	���[�V����
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
			//	�A�j���[�V�����W�����v
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
	//	�t���[���X�V
	//*****************************************************************************
	void	kSkin::updateSkinMesh( f32 frame )
	{
		const kSkinData* m_Skin=(const kSkinData*)mp_MeshData;
		u32			i, j;
		kSkinAnimation*	lpAnime;
		float		t;

		for( i=0 ; i<m_Skin->m_Info.NumBone ; i++ ){
			lpAnime = &m_Skin->mp_Animation[i];

			//	�|�[�Y�ݒ�
			if( lpAnime->rotNum == 0 ) CurPose[i] = m_Skin->mp_Bone[i].orgPose;
			else if( lpAnime->rotNum == 1 ) CurPose[i] = lpAnime->rot[0];
			else {
				//	��]�L�[���
				for( j=0 ; j<lpAnime->rotNum-1 ; j++ )
				{
					//	���݈ʒu����
					if( (frame>=lpAnime->rotFrame[j]) && (frame<lpAnime->rotFrame[j+1]) )
					{
						//	�o�߃t���[���v�Z
						t = (float)(frame-lpAnime->rotFrame[j]) / (float)(lpAnime->rotFrame[j+1] - lpAnime->rotFrame[j]);
						//	���
						QuaternionSlerp(&CurPose[i],lpAnime->rot[j], lpAnime->rot[j+1], t);
						//CurPose[i] = QuaternionSlerp( lpAnime->rot[j], lpAnime->rot[j+1], t );
						break;
					}
				}
				if( j == lpAnime->rotNum-1 ) CurPose[i] = lpAnime->rot[lpAnime->rotNum-1];
			}
			//	���W�ݒ�
			if( lpAnime->posNum == 0 ) CurPos[i] = m_Skin->mp_Bone[i].orgPos;
			else {
				//	�ʒu���
				for( j=0 ; j<lpAnime->posNum-1 ; j++ )
				{
					//	���݈ʒu����
					if( (frame>=lpAnime->posFrame[j]) && (frame<lpAnime->posFrame[j+1]) )
					{
						//	�o�߃t���[���v�Z
						t = (float)(frame-lpAnime->posFrame[j]) / (float)(lpAnime->posFrame[j+1] - lpAnime->posFrame[j]);
						//	���
						CurPos[i] = lpAnime->pos[j] + (lpAnime->pos[j+1]-lpAnime->pos[j])*t;
						break;
					}
				}
				if( j == lpAnime->posNum-1 ) CurPos[i] = lpAnime->pos[lpAnime->posNum-1];
			}
		}
	}

	//*****************************************************************************
	//	�{�[���s��X�V
	//*****************************************************************************
	void kSkin::updateBoneMatrix()
	{
		u32			i;
		const kSkinData* m_Skin=(const kSkinData*)mp_MeshData;
		//	�{�[���X�V
		for( i=0 ; i<m_Skin->m_Info.NumBone ; i++ )
		{
			CurPose[i].toMatrix( &lpBoneMatrix[i] );
			//	�ʒu���R�s�[
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
		//�ʒu�A�X�P�[���A�p�x���ύX����Ă��Ȃ���΍X�V�����Ȃ�
		//if( !m_bChanged ) return;
		Matrix MatScale;
		//	�X�P�[�����O
		MatScale.identity();
		MatScale.mulS(m_Scale);
		//	���W�n�ݒ� 
		m_TransMatrix.setRZXY(m_Angle);
		m_TransMatrix.setT(m_Position);
		//	�]���s��쐬 
		m_TransMatrix = MatScale * m_TransMatrix;

		m_bChanged=false;

		//���W�X�V�p�ϐ�
		const kSkinData* m_Skin=(kSkinData*)mp_MeshData;
		ZeroMemory(mp_MeshData->mp_Vertex,sizeof(kMeshVertex)*mp_MeshData->m_Info.NumVertex);
		for(int i=0;i<(s32)m_Skin->m_Info.NumBone;i++)
		{
			//���_�ɉe�����Ă���{�[���s����d�݂����Ċ|���Ă���
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
	//		�`��
	//
	//**************************************************************************************************

	//------------------------------------------------------
	//	�ʏ�`��
	//------------------------------------------------------
	void kSkin::Render(kGraphicsPipline* shader)
	{
		mp_RenderDelegate->Render(m_TransMatrix,shader);
	}
};