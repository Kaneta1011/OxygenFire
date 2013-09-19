#include	"EffectLib\\Particle.h"
#include	<GLES2/gl2.h>
#include	<GLES2/gl2ext.h>
#include	<iostream>
#include	"GraphicsLib\Class\tRenderState\RenderState.h"
#include	"ShaderManager.h"
#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
using namespace EffectLib;
using namespace klib::math;
using namespace RenderLib;
using namespace ShaderLib;

using namespace klib;

int ParticleTextrue::s_LimitNum = 0;
Vector3 Particle::s_WindVec = Vector3(0,0,0);

//	�V���O���g��
Particle_Singleton* Particle_Singleton::singleton = NULL;


Vertex::Vertex()
{
	pos.SetPtr(new Vector3[PARTICLE_VERTEX_MAX],true);
	tex.SetPtr(new Vector2[PARTICLE_VERTEX_MAX],true);
	color.SetPtr(new Vector4[PARTICLE_VERTEX_MAX],true);
}

ParticleData::ParticleData()
{

	initPos.SetPtr(new Vector3[PARTICLE_MAX],true);
	pos.SetPtr(new Vector3[PARTICLE_MAX],true);
	tex.SetPtr(new Vector2[PARTICLE_MAX],true);
	life.SetPtr(new int[PARTICLE_MAX],true);
	scale.SetPtr(new float[PARTICLE_MAX],true);
	scaleStart.SetPtr(new float[PARTICLE_MAX],true);
	scaleMiddle.SetPtr(new float[PARTICLE_MAX],true);
	scaleEnd.SetPtr(new float[PARTICLE_MAX],true);
	flag.SetPtr(new bool[PARTICLE_MAX],true);
	texNum.SetPtr(new int[PARTICLE_MAX],true);
	velocity.SetPtr(new Vector3[PARTICLE_MAX],true);
	size.SetPtr(new int[PARTICLE_MAX],true);
	moveFlag.SetPtr(new bool[PARTICLE_MAX],true);
	index.SetPtr(new int[PARTICLE_MAX],true);
	count.SetPtr(new int[PARTICLE_MAX],true);
	windPower.SetPtr(new float[PARTICLE_MAX],true);
	centerPowerStart.SetPtr(new float[PARTICLE_MAX],true);
	centerPowerMiddle.SetPtr(new float[PARTICLE_MAX],true);
	centerPowerEnd.SetPtr(new float[PARTICLE_MAX],true);
	color.SetPtr(new COLOR[PARTICLE_MAX],true);
	colorStart.SetPtr(new COLOR[PARTICLE_MAX],true);
	colorMiddle.SetPtr(new COLOR[PARTICLE_MAX],true);
	colorEnd.SetPtr(new COLOR[PARTICLE_MAX],true);

	Clear();
}

void ParticleData::Clear()
{
	for(int n=0;n<PARTICLE_MAX;n++)
	{
		count[n] = 0;
		flag[n] = false;
		texNum[n] = 0;
		life[n] = 0;
		moveFlag[n] = false;
	}

	useNum = 0;
}

void ParticleData::Setting(sp<ParticleData> data)
{

#if 0

	useNum = data->useNum;

	memcpy(&count[0],&data->count[0],sizeof(ParticleData)*PARTICLE_MAX);

#if 0
	for(int n=0;n< (PARTICLE_MAX/10);n++)
	{
		memcpy(&initPos[n*10],&data->initPos[n*10],sizeof(ParticleData)*10);

#if 0
		initPos[n] = data->initPos[n];
		pos[n] = data->pos[n];
		tex[n] = data->tex[n];
		life[n] = data->life[n];
		scale[n] = data->scale[n];
		scaleStart[n] = data->scaleStart[n];
		scaleMiddle[n] = data->scaleMiddle[n];
		scaleEnd[n] = data->scaleEnd[n];
		flag[n] = data->flag[n];
		texNum[n] = data->texNum[n];
		velocity[n] = data->velocity[n];
		size[n] = data->size[n];
		moveFlag[n] = data->moveFlag[n];
		index[n] = data->index[n];
		count[n] = data->count[n];
		windPower[n] = data->windPower[n];
		centerPowerStart[n] = data->centerPowerStart[n];
		centerPowerMiddle[n] = data->centerPowerMiddle[n];
		centerPowerEnd[n] = data->centerPowerEnd[n];
		color[n] = data->color[n];
		colorStart[n] = data->colorStart[n];
		colorMiddle[n] = data->colorMiddle[n];
		colorEnd[n] = data->colorEnd[n];

		count[n] = data->count[n];
		flag[n] = data->flag[n];
		texNum[n] = data->texNum[n];
		life[n] = data->life[n];
		moveFlag[n] = data->moveFlag[n];
#endif


	}

#if 0
	for(int n=0;n<TEXTURE_MAX;n++)
	{
		texData[n].StartNum = data->texData[n].StartNum;
		texData[n].UseNum = data->texData[n].UseNum;
	}
#endif
#endif

#endif
}


ParticleTextrue::ParticleTextrue()
{
	texNum=s_LimitNum;s_LimitNum++;
}

//-------------------------------------------------------------------------
//	Particle
//-------------------------------------------------------------------------
void Particle::Clear()
{
	m_spVertexBuf.SetPtr(new Vertex);
	m_TextureUseNumber = 0;
	m_Matrix.identity();
	offset = 0;

	m_spParticleData.SetPtr(new ParticleData,false);

	m_spParticleData2.SetPtr(new ParticleData,false);

	UpdateNumber = 0;

	//work.SetPtr(new ParticleData(),false);


	for(int n=0;n<TEXTURE_MAX;n++)
	{
		texData[n].StartNum=0;
		texData[n].UseNum=0;
	}
}

void Particle::Destroy()
{
	
}

bool Particle::Initialize()
{
	m_spParticleData.SetPtr(new ParticleData());
	m_spParticleData2.SetPtr(new ParticleData());

	ShaderLib::ShaderManager::Init();


#if 0
	pipeline = new klib::kGraphicsPipline();
	pipeline->createVertexShader("vertex.txt");
	pipeline->createPixelShader("pixel.txt");
	pipeline->createBlendState(k_BLEND_NONE);
	pipeline->createDepthStencilState(true,eLESS_EQUAL);
	pipeline->createRasterizerState(eSOLID,eFRONT,false);

	kInputElementDesc desc[]=
	{
		{"Pos",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
		{"Tex",0,k_VF_R32G32_FLOAT,0,eVertex,0},
		{"Color",0,k_VF_R32G32B32A32_FLOAT,0,eVertex,0},
	};
	u32 descnum=sizeof(desc)/sizeof(kInputElementDesc);
	pipeline->complete(desc,descnum);
#endif

	return true;
}

bool Particle::Setting_Texture( char* TextureName )
{
	//
	//	@for �󂢂Ă�ꏊ���������đ��肢����f�[�^������
	//
	for( int n=0; n<TEXTURE_MAX; n++ )
	{
		//	�g�p���Ă����玟�̃f�[�^��
		if( m_spTexture[n].GetRefNum() != 0 ){ continue; }

		//	�ߋ��̃e�N�X�`���ɓ��������Ȃ����`�F�b�N
		if( Check_UseTexture( TextureName ) == true ){ return false; }

		//	�e�N�X�`��n�Ԗڔz��̃������m��
		m_spTexture[n].SetPtr( new ParticleTextrue );
		//	�e�N�X�`���������m��
		m_spTexture[n]->spTexture.SetPtr( new Texture );

		//	���[�h�p�̕�����ݒ�
		char str1[256] ="effect/particle/";

		strcat( str1, TextureName ); 

		m_spTexture[n]->spTexture->Initilize( str1 );

		//	���ꂩ��g�p���Ă����e�N�X�`����������m�ۂ��Ă���
		strcpy( m_spTexture[n]->TextureName, TextureName );

		//	�g�pNumber�𑝂₷
		m_TextureUseNumber++;

		//	�f�[�^�擾�����̂ŏI��
		break;
	}

	return true;
}

bool Particle::Check_UseTexture( char* CheckName )
{
	//
	//	@for �g���Ă镪�̃e�N�X�`�������[�v����
	//	���łɎg���Ă�e�N�X�`�����ƈꏏ�Ȃ玟����͎g��Ȃ�
	//
	for( int n=0; n<TEXTURE_MAX; n++ )
	{
		//	�g�p���Ă����玟�̃f�[�^��
		if( m_spTexture[n].GetRefNum() == 0 ){ continue; }

		//	��r
		if( strcmp( CheckName, m_spTexture[n]->TextureName ) == 0 )
		{
			return true;
		}
	}
	return false;
}

void Particle::Create_VBO()
{
	glGenBuffers(VBO_MAX,m_VBO);

	//	���W
	glBindBuffer(GL_ARRAY_BUFFER,m_VBO[VBO_POS]);
	glBufferData(GL_ARRAY_BUFFER,PARTICLE_VERTEX_MAX * sizeof(Vector3),
		m_spVertexBuf->pos.GetPtr(),GL_STATIC_DRAW);

	//	�e�N�X�`�����W
	glBindBuffer(GL_ARRAY_BUFFER,m_VBO[VBO_TEX]);
	glBufferData(GL_ARRAY_BUFFER,PARTICLE_VERTEX_MAX * sizeof(Vector2),
		m_spVertexBuf->tex.GetPtr(),GL_STATIC_DRAW);

	//	�F
	glBindBuffer(GL_ARRAY_BUFFER,m_VBO[VBO_COLOR]);
	glBufferData(GL_ARRAY_BUFFER,PARTICLE_VERTEX_MAX * sizeof(Vector4),
		m_spVertexBuf->color.GetPtr(),GL_STATIC_DRAW);

	//	�L����
	glEnableVertexAttribArray(VBO_POS);
	glEnableVertexAttribArray(VBO_TEX);
	glEnableVertexAttribArray(VBO_COLOR);


	glVertexAttribPointer(VBO_POS,3,GL_FLOAT,GL_FALSE,0,(const void*) offset);
	offset += 3 * sizeof(GLfloat);

	glVertexAttribPointer(VBO_POS,2,GL_FLOAT,GL_FALSE,0,(const void*) offset);
	offset += 2 * sizeof(GLfloat);
	
	glVertexAttribPointer(VBO_POS,4,GL_FLOAT,GL_FALSE,0,(const void*) offset);
}

void Particle::Update()
{
	/*
	�e�N�X�`���̏��ԂŃf�[�^���i�[���Ȃ���
	*/
	
	//sp<ParticleData> work;
	//work.SetPtr( new ParticleData(), false );


	sp<ParticleData> work;
	sp<ParticleData> particle;

	if( UpdateNumber == 0 )
	{
		work = m_spParticleData2;
		particle = m_spParticleData;
		UpdateNumber++;
	}else{
		work = m_spParticleData;
		particle = m_spParticleData2;
		UpdateNumber= 0;
	}

	

	work->Clear();

	static int useNum=0;
	useNum = 0;
	static int saveNum=0;
	saveNum = 0;

	work->useNum = particle->useNum;
	
	static float force = .0f; force = .0f;
	static float fv = .0f; fv = .0f;
	static float	rate = .0f; rate = .0f;
	static Vector3 vec;
	static COLOR nextC;


	//
	//	�œK�����\�ł������]�[��
	//
	for(int n=0;n<m_TextureUseNumber;n++)
	{
		for(int v=0;v<PARTICLE_MAX;v++)
		{
			//	�R���e�B�j���[����
			if(useNum >= particle->useNum)break;

			if(false == particle->flag[v])continue;
			if(n != particle->texNum[v])continue;

			//	���S����
			if( particle->count[v] >= particle->life[v] ){
				work->flag[useNum] = false;
				particle->count[v] = 0;
				work->useNum--;
				continue;
			}

			//======================================
			//	�f�[�^���X�V
			//======================================
			particle->count[v]++;
			particle->pos[v] += Vector3(0.0000000000001f,0,0);

			//	���x
			particle->pos[v] += particle->velocity[v];

			//	���̉e���x
			particle->pos[v] += s_WindVec * particle->windPower[v];

			//	�J�E���g�ɂ���ĕs�����x�̍X�V
			rate = (float)particle->count[v] / (float)particle->life[v];

			//	�A�j���[�V����
			if( particle->moveFlag[v] == true ){
				particle->index[v] = (int)(15.9f * rate);
			}

			//	�X�P�[���̌v�Z
			static float sv = .0f;//scaleValue
			sv = .0f;
			static float lf;
			lf = (float)particle->life[v] / 2; //life half
			static float next;
			next = 0;

			if( particle->count[v] < lf )
			{
				next = particle->scaleMiddle[v] - particle->scaleStart[v];
				sv = particle->count[v] / lf ;
				particle->scale[v] = particle->scaleStart[v] + (next*sv);
			}else{
				next = particle->scaleEnd[v] - particle->scaleMiddle[v];
				sv = (particle->count[v]-lf) / lf;
				particle->scale[v] = particle->scaleMiddle[v] + (next*sv);
			}

			//	�F�̌v�Z
			sv = .0f;//scaleValue
			lf = (float)particle->life[v] / 2; //life half



			static COLOR colorSave;
			static float hl;

			if( particle->count[v] < lf )
			{
				//red
				next = particle->colorMiddle[v].red - particle->colorStart[v].red;
				sv = particle->count[v] / lf ;
				particle->color[v].red = particle->colorStart[v].red + (next*sv);
				//green
				next = particle->colorMiddle[v].green - particle->colorStart[v].green;
				sv = particle->count[v] / lf ;
				particle->color[v].green = particle->colorStart[v].green + (next*sv);
				//blue
				next = particle->colorMiddle[v].blue - particle->colorStart[v].blue;
				sv = particle->count[v] / lf ;
				particle->color[v].blue = particle->colorStart[v].blue + (next*sv);
			}else{
				//red
				next = particle->colorEnd[v].red - particle->colorMiddle[v].red;
				sv = (particle->count[v]-lf) / lf;
				particle->color[v].red = particle->colorMiddle[v].red + (next*sv);
				//green
				next = particle->colorEnd[v].green - particle->colorMiddle[v].green;
				sv = (particle->count[v]-lf) / lf;
				particle->color[v].green = particle->colorMiddle[v].green + (next*sv);
				//red
				next = particle->colorEnd[v].blue - particle->colorMiddle[v].blue;
				sv = (particle->count[v]-lf) / lf;
				particle->color[v].blue = particle->colorMiddle[v].blue + (next*sv);
			}

			particle->color[v].alpha = 255 * (1.0f - rate);

			//particle->color[v].red = 255;

			//	������
			vec = particle->initPos[v] - particle->pos[v];

			//	���������G
			vec.normalize();

			if( particle->count[v] < lf )
			{
				next = particle->centerPowerMiddle[v] - 
					particle->centerPowerStart[v];
				fv = particle->count[v] / lf ;
				force = particle->centerPowerStart[v] + (next*fv);
			}else{
				next = particle->centerPowerEnd[v] - 
					particle->centerPowerMiddle[v];
				fv = (particle->count[v]-lf) / lf;
				force = particle->centerPowerMiddle[v] + (next*fv);
			}
			particle->pos[v] += vec*force;

			//======================================
			//	���Ԃ����ւ���
			//======================================
			work->initPos[useNum] = particle->initPos[v];
			work->pos[useNum] = particle->pos[v];
			work->tex[useNum] = particle->tex[v];
			work->life[useNum] = particle->life[v];
			work->flag[useNum] = particle->flag[v];
			work->texNum[useNum] = particle->texNum[v];
			work->scale[useNum] = particle->scale[v];
			work->scaleStart[useNum] = particle->scaleStart[v];
			work->scaleMiddle[useNum] = particle->scaleMiddle[v];
			work->scaleEnd[useNum] = particle->scaleEnd[v];
			work->velocity[useNum] = particle->velocity[v];
			work->moveFlag[useNum] = particle->moveFlag[v];
			work->size[useNum] = particle->size[v];
			work->index[useNum] = particle->index[v];
			work->count[useNum] = particle->count[v];
			work->windPower[useNum] = particle->windPower[v];
			work->centerPowerStart[useNum] = particle->centerPowerStart[v];
			work->centerPowerMiddle[useNum] = particle->centerPowerMiddle[v];
			work->centerPowerEnd[useNum] = particle->centerPowerEnd[v];
			work->color[useNum] = particle->color[v];
			work->colorStart[useNum] = particle->colorStart[v];
			work->colorMiddle[useNum] = particle->colorMiddle[v];
			work->colorEnd[useNum] = particle->colorEnd[v];

			useNum++;
		}
		texData[n].StartNum = saveNum;
		texData[n].UseNum = useNum - saveNum;

		saveNum = useNum;
	}

	//m_spParticleData->Setting(work);
}


void Particle::Render()
{
	static sp<ParticleData> particle;

	if( UpdateNumber == 1 )
	{
		particle = m_spParticleData;
	}else{
		particle = m_spParticleData2;
	}

	//glFrontFace(GL_CCW);

	static Matrix m = RenderState::getViewMatrix();
	m = RenderState::getViewMatrix();

	static Vector3 axisX(m._11,m._12,m._13);
	axisX = Vector3(m._11,m._12,m._13);
	static Vector3 axisY(m._21,m._22,m._23);
	axisY = Vector3(m._21,m._22,m._23);

	static Vector3 tl,tr,ul,ur; //	t=top; u=under; l=left; r=right;
	static Vector3 p;	//	p=pos;

	static Vector2 uv[4];float tu,tv;



	static int useNum=0;
	useNum = 0;

	for(int n=0;n<PARTICLE_MAX;n++)
	{
		if(useNum >= particle->useNum)break;

		//if( n > m_spParticleData->useNum-1 ) break;
		//axisX = Vector3(m._11,m._12,m._13);
		//axisY = Vector3(m._21,m._22,m._23);

		axisX = Vector3(m._11,m._21,m._31);
		axisY = Vector3(m._12,m._22,m._32);

		axisX *= particle->scale[n];
		axisY *= particle->scale[n];

		p=particle->pos[n];

		//	����
		tl.x = p.x - axisX.x + axisY.x;
		tl.y = p.y - axisX.y + axisY.y;
		tl.z = p.z - axisX.z + axisY.z;
		//	�E��
		tr.x = p.x + axisX.x + axisY.x;
		tr.y = p.y + axisX.y + axisY.y;
		tr.z = p.z + axisX.z + axisY.z;
		//	����
		ul.x = p.x - axisX.x - axisY.x;
		ul.y = p.y - axisX.y - axisY.y;
		ul.z = p.z - axisX.z - axisY.z;
		//	�E��
		ur.x = p.x + axisX.x - axisY.x;
		ur.y = p.y + axisX.y - axisY.y;
		ur.z = p.z + axisX.z - axisY.z;

		m_spVertexBuf->pos[(n*ONE_PARTICLE_VERTEX_NUM)+0] = tl;
		m_spVertexBuf->pos[(n*ONE_PARTICLE_VERTEX_NUM)+1] = ul;
		m_spVertexBuf->pos[(n*ONE_PARTICLE_VERTEX_NUM)+2] = tr;
		m_spVertexBuf->pos[(n*ONE_PARTICLE_VERTEX_NUM)+3] = tr;
		m_spVertexBuf->pos[(n*ONE_PARTICLE_VERTEX_NUM)+4] = ul;
		m_spVertexBuf->pos[(n*ONE_PARTICLE_VERTEX_NUM)+5] = ur;

		switch( particle->size[n] )
		{
		case TEXTURE_SIZE_1x1:
			uv[0].x = 0;	uv[0].y = 0;
			uv[1].x = 1;	uv[1].y = 0;
			uv[2].x = 0;	uv[2].y = 1;
			uv[3].x = 1;	uv[3].y = 1;
			break;
		case TEXTURE_SIZE_2x2:
			tu = ( particle->index[n] % 2 ) * 0.5f;
			tv = ( particle->index[n] / 2 ) * 0.5f;
			uv[0].x = tu;					uv[0].y = tv;
			uv[1].x = tu+0.499f;	uv[1].y = tv;
			uv[2].x = tu;					uv[2].y = tv+0.499f;
			uv[3].x = tu+0.499f;	uv[3].y = tv+0.499f;
			break;
		case TEXTURE_SIZE_4x4:
			tu = ( particle->index[n] % 4 ) * 0.25f;
			tv = ( particle->index[n] / 4 ) * 0.25f;
			uv[0].x = tu;					uv[0].y = tv;
			uv[1].x = tu+0.249f;	uv[1].y = tv;
			uv[2].x = tu;					uv[2].y = tv+0.249f;
			uv[3].x = tu+0.249f;	uv[3].y = tv+0.249f;
			break;
		}

		m_spVertexBuf->tex[(n*ONE_PARTICLE_VERTEX_NUM)+0].x = uv[0].x;
		m_spVertexBuf->tex[(n*ONE_PARTICLE_VERTEX_NUM)+0].y = uv[0].y;
		m_spVertexBuf->tex[(n*ONE_PARTICLE_VERTEX_NUM)+1].x = uv[2].x;
		m_spVertexBuf->tex[(n*ONE_PARTICLE_VERTEX_NUM)+1].y = uv[2].y;
		m_spVertexBuf->tex[(n*ONE_PARTICLE_VERTEX_NUM)+2].x = uv[1].x;
		m_spVertexBuf->tex[(n*ONE_PARTICLE_VERTEX_NUM)+2].y = uv[1].y;
		m_spVertexBuf->tex[(n*ONE_PARTICLE_VERTEX_NUM)+3].x = uv[1].x;
		m_spVertexBuf->tex[(n*ONE_PARTICLE_VERTEX_NUM)+3].y = uv[1].y;
		m_spVertexBuf->tex[(n*ONE_PARTICLE_VERTEX_NUM)+4].x = uv[2].x;
		m_spVertexBuf->tex[(n*ONE_PARTICLE_VERTEX_NUM)+4].y = uv[2].y;
		m_spVertexBuf->tex[(n*ONE_PARTICLE_VERTEX_NUM)+5].x = uv[3].x;
		m_spVertexBuf->tex[(n*ONE_PARTICLE_VERTEX_NUM)+5].y = uv[3].y;

		//	�F
		m_spVertexBuf->color[(n*ONE_PARTICLE_VERTEX_NUM)+0] =
			m_spVertexBuf->color[(n*ONE_PARTICLE_VERTEX_NUM)+1] =
			m_spVertexBuf->color[(n*ONE_PARTICLE_VERTEX_NUM)+2] =
			m_spVertexBuf->color[(n*ONE_PARTICLE_VERTEX_NUM)+3] =
			m_spVertexBuf->color[(n*ONE_PARTICLE_VERTEX_NUM)+4] =
			m_spVertexBuf->color[(n*ONE_PARTICLE_VERTEX_NUM)+5] = 
			particle->color[n].Value_0from1();

		useNum++;
	}
	//	Uniform���M
	ShaderManager::getSprite()->Send_Matrix(m_Matrix);

	glBindBuffer(GL_ARRAY_BUFFER, 0);// 0 �����Ă����Ȃ��Ƃ����Ȃ�

	glDisable(GL_DEPTH_TEST);

ShaderManager::getSprite()->Begin();

	//
	//	�e�N�X�`�����`��
	//
	for(int n=0;n<m_TextureUseNumber;n++)
	{
		//	�����p����
		if(m_spTexture[n].GetPtr() == 0)continue;
		if(texData[n].UseNum == 0)continue;

		//	Position
		glEnableVertexAttribArray(VBO_POS);
		glVertexAttribPointer(VBO_POS,3,GL_FLOAT, GL_FALSE,
			0,m_spVertexBuf->pos.GetPtr());
		//	TexCoord
		glEnableVertexAttribArray(VBO_TEX);
		glVertexAttribPointer(VBO_TEX,2,GL_FLOAT, GL_FALSE,
			0,m_spVertexBuf->tex.GetPtr());
		//	Color
		glEnableVertexAttribArray(VBO_COLOR);
		glVertexAttribPointer(VBO_COLOR,4,GL_FLOAT, GL_FALSE,
			0,m_spVertexBuf->color.GetPtr());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);

		//	�e�N�X�`�����ݒ�
		m_spTexture[n]->spTexture->Setting(rlib::Texture::ACTIVE_6);
		ShaderManager::getSprite()->SetValue_No_BeginEnd( "uTex", 6 );

		//	�`��
		glDrawArrays(GL_TRIANGLES,
			texData[n].StartNum*ONE_PARTICLE_VERTEX_NUM,
			texData[n].UseNum*ONE_PARTICLE_VERTEX_NUM);
	}

	glBindBuffer( GL_ARRAY_BUFFER, 0);
ShaderManager::getSprite()->End();
}


void Particle::Setting(const Vector3& Pos,char* File,float ScaleStart,
	float ScaleMiddle,float ScaleEnd,int Life,
	const Vector3& Velocity,bool MoveFlag,int Size,int Index,float WindPower,
	float CenterPowerStart,float CenterPowerMiddle,float CenterPowerEnd,
	COLOR ColorStart,COLOR ColorMiddle,COLOR ColorEnd)
{
	sp<ParticleData> particle;

	if( UpdateNumber == 0 )
	{
		particle = m_spParticleData;
	}else{
		particle = m_spParticleData2;
	}

	for(int n=0;n<PARTICLE_MAX;n++)
	{
		if(true == particle->flag[n])continue;

		//	Data�i�[
		particle->useNum++;
		particle->pos[n] = Pos;
		particle->initPos[n] = Pos;
		particle->flag[n] = true;
		particle->scaleStart[n] = ScaleStart;
		particle->scaleMiddle[n] = ScaleMiddle;
		particle->scaleEnd[n] = ScaleEnd;
		particle->life[n] = Life;
		particle->velocity[n] = Velocity;
		particle->moveFlag[n] = MoveFlag;
		particle->size[n] = Size;
		particle->index[n] = Index;
		particle->windPower[n] = WindPower;
		particle->centerPowerStart[n] = CenterPowerStart;
		particle->centerPowerMiddle[n] = CenterPowerMiddle;
		particle->centerPowerEnd[n] = CenterPowerEnd;
		particle->colorStart[n] = ColorStart;
		particle->colorMiddle[n] = ColorMiddle;
		particle->colorEnd[n] = ColorEnd;

		//	�ǂ̃e�N�X�`�����T��
		for(int t=0;t<m_TextureUseNumber;t++)
		{
			if(m_spTexture[t].GetRefNum() == 0)continue;

			if(strcmp( m_spTexture[t]->TextureName, File ) == 0)
			{
				particle->texNum[n] = m_spTexture[t]->texNum;
				return;
			}
		}
		break;
	}
}

#if 1
void Particle::Setting_Single(
	eSINGLE_TYPE Type,
	int Life,
	const Vector3& Pos,
	const Vector3& Move,
	float Scale,
	//	�F�̐ݒ�
	COLOR StartColor,
	COLOR MiddleColor,
	COLOR EndColor
		)
{
	sp<ParticleData> particle;

	if( UpdateNumber == 0 )
	{
		particle = m_spParticleData;
	}else{
		particle = m_spParticleData2;
	}

	char* File;

	switch( Type )
	{
	case SINGLE_NORMAL:
		File = "light.png";

		for(int n=0;n<PARTICLE_MAX;n++)
		{
			if(true == particle->flag[n])continue;

			//	Data�i�[
			particle->useNum++;
			particle->pos[n] = Pos;
			particle->initPos[n] = Pos;
			particle->flag[n] = true;
			particle->scaleStart[n] = Scale;
			particle->scaleMiddle[n] = Scale;
			particle->scaleEnd[n] = Scale;
			particle->life[n] = Life;
			particle->velocity[n] = Move;
			particle->moveFlag[n] = false;
			particle->size[n] = 1;
			particle->index[n] = 0;
			particle->windPower[n] = 0;
			particle->centerPowerStart[n] = 0;
			particle->centerPowerMiddle[n] = 0;
			particle->centerPowerEnd[n] = 0;
			particle->colorStart[n] = StartColor;
			particle->colorMiddle[n] = MiddleColor;
			particle->colorEnd[n] = EndColor;

			//	�ǂ̃e�N�X�`�����T��
			for(int t=0;t<m_TextureUseNumber;t++)
			{
				if(m_spTexture[t].GetRefNum() == 0)continue;

				if(strcmp( m_spTexture[t]->TextureName, File ) == 0)
				{
					particle->texNum[n] = m_spTexture[t]->texNum;
					return;
				}
			}
			break;
		}
		break;

	case SINGLE_FLAME:

		File = "flame.png";

		for(int n=0;n<PARTICLE_MAX;n++)
		{
			if(true == particle->flag[n])continue;

			//	Data�i�[
			particle->useNum++;
			particle->pos[n] = Pos;
			particle->initPos[n] = Pos;
			particle->flag[n] = true;
			particle->scaleStart[n] = Scale;
			particle->scaleMiddle[n] = Scale;
			particle->scaleEnd[n] = Scale;
			particle->life[n] = Life;
			particle->velocity[n] = Move;
			particle->moveFlag[n] = true;
			particle->size[n] = TEXTURE_SIZE_4x4;
			particle->index[n] = 0;
			particle->windPower[n] = 0;
			particle->centerPowerStart[n] = 0;
			particle->centerPowerMiddle[n] = 0;
			particle->centerPowerEnd[n] = 0;
			particle->colorStart[n] = StartColor;
			particle->colorMiddle[n] = MiddleColor;
			particle->colorEnd[n] = EndColor;

			//	�ǂ̃e�N�X�`�����T��
			for(int t=0;t<m_TextureUseNumber;t++)
			{
				if(m_spTexture[t].GetRefNum() == 0)continue;

				if(strcmp( m_spTexture[t]->TextureName, File ) == 0)
				{
					particle->texNum[n] = m_spTexture[t]->texNum;
					return;
				}
			}
			break;
		}
		break;
	}
}
#endif

//========================================================
//	�g�����킩��Ȃ�����
//========================================================

#if 0

//
//	�C���f�b�N�X�����_�[
//

	LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");

	
ShaderManager::getSprite()->Begin();


	LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");

	//
	//	�e�N�X�`�����`��
	//
	for(int n=0;n<m_TextureUseNumber;n++)
	{
		//	�����p����
		if(m_spTexture[n].GetPtr() == 0)continue;
		if(m_spParticleData->texData[n].UseNum == 0)continue;

			LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		//	�e�N�X�`�����ݒ�
		m_spTexture[n]->spTexture->Setting(ACTIVE_0);

		ShaderManager::getSprite()->SetValue_No_BeginEnd( "uTex", 0 );



	LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");

		ShaderManager::getSprite()->Location(VBO_POS,"Pos");
		ShaderManager::getSprite()->Location(VBO_TEX,"Tex");
		ShaderManager::getSprite()->Location(VBO_COLOR,"Color");


	LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");

		ShaderManager::getSprite()->Link();


	LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");

		//	�`��
		glDrawElements(GL_TRIANGLES,
			m_spParticleData->texData[n].UseNum*ONE_PARTICLE_VERTEX_NUM,
			GL_UNSIGNED_SHORT,NULL);


			LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");


	break;
	}


	LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
ShaderManager::getSprite()->End();

LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");






#endif