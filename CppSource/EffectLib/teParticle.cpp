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

int ParticleTextrue::s_LimitNum = 0;
Vector3 Particle::s_WindVec = Vector3(0,0,0);

//	シングルトン
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
}

void Particle::Destroy()
{
	
}

bool Particle::Initialize()
{
	m_spParticleData.SetPtr(new ParticleData());

	ShaderLib::ShaderManager::Init();

	m_spPipeline.SetPtr( new klib::kGraphicsPipline() );
	m_spPipeline->createVertexShader("shader/sprite.vs");
	m_spPipeline->createPixelShader("shader/sprite.fs");
	m_spPipeline->createBlendState(klib::k_BLEND_ADD);
	m_spPipeline->createDepthStencilState(false, false,klib::eLESS);
	m_spPipeline->createRasterizerState(klib::eSOLID, klib::eNONE, true);

	return true;
}

bool Particle::Setting_Texture( char* TextureName )
{
	//
	//	@for 空いてる場所を検索して相手いたらデータを入れる
	//
	for( int n=0; n<TEXTURE_MAX; n++ )
	{
		//	使用していたら次のデータに
		if( m_spTexture[n].GetRefNum() != 0 ){ continue; }

		//	過去のテクスチャに同じ物がないかチェック
		if( Check_UseTexture( TextureName ) == true ){ return false; }

		//	テクスチャn番目配列のメモリ確保
		m_spTexture[n].SetPtr( new ParticleTextrue );
		//	テクスチャメモリ確保
		m_spTexture[n]->spTexture.SetPtr( new Texture );

		//	ロード用の文字列設定
		char str1[256] ="effect/particle/";

		strcat( str1, TextureName ); 

		m_spTexture[n]->spTexture->Initilize( str1 );

		//	これから使用していくテクスチャ文字列を確保しておく
		strcpy( m_spTexture[n]->TextureName, TextureName );

		//	使用Numberを増やす
		m_TextureUseNumber++;

		//	データ取得したので終了
		break;
	}

	return true;
}

bool Particle::Check_UseTexture( char* CheckName )
{
	//
	//	@for 使ってる分のテクスチャ分ループして
	//	すでに使ってるテクスチャ名と一緒なら次からは使わない
	//
	for( int n=0; n<TEXTURE_MAX; n++ )
	{
		//	使用していたら次のデータに
		if( m_spTexture[n].GetRefNum() == 0 ){ continue; }

		//	比較
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

	//	座標
	glBindBuffer(GL_ARRAY_BUFFER,m_VBO[VBO_POS]);
	glBufferData(GL_ARRAY_BUFFER,PARTICLE_VERTEX_MAX * sizeof(Vector3),
		m_spVertexBuf->pos.GetPtr(),GL_STATIC_DRAW);

	//	テクスチャ座標
	glBindBuffer(GL_ARRAY_BUFFER,m_VBO[VBO_TEX]);
	glBufferData(GL_ARRAY_BUFFER,PARTICLE_VERTEX_MAX * sizeof(Vector2),
		m_spVertexBuf->tex.GetPtr(),GL_STATIC_DRAW);

	//	色
	glBindBuffer(GL_ARRAY_BUFFER,m_VBO[VBO_COLOR]);
	glBufferData(GL_ARRAY_BUFFER,PARTICLE_VERTEX_MAX * sizeof(Vector4),
		m_spVertexBuf->color.GetPtr(),GL_STATIC_DRAW);

	//	有効化
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
	テクスチャの順番でデータを格納しなおす
	*/
	sp<ParticleData> work;
	work.SetPtr(new ParticleData);

	int useNum=0;
	int saveNum=0;

	work->useNum = m_spParticleData->useNum;
	
	float force = .0f;
	float fv = .0f;
	float	rate = .0f;
	Vector3 vec;
	COLOR nextC;


	//
	//	最適化結構できそうゾーン
	//
	for(int n=0;n<m_TextureUseNumber;n++)
	{
		for(int v=0;v<PARTICLE_MAX;v++)
		{
			//	コンティニュー条件
			if(useNum >= m_spParticleData->useNum)break;
			if(false == m_spParticleData->flag[v])continue;
			if(n != m_spParticleData->texNum[v])continue;

			//	死亡条件
			if( m_spParticleData->count[v] >= m_spParticleData->life[v] ){
				work->flag[useNum] = false;
				m_spParticleData->count[v] = 0;
				work->useNum--;
				continue;
			}

			//======================================
			//	データを更新
			//======================================
			m_spParticleData->count[v]++;
			m_spParticleData->pos[v] += Vector3(0.0000000000001f,0,0);

			//	速度
			m_spParticleData->pos[v] += m_spParticleData->velocity[v];

			//	風の影響度
			m_spParticleData->pos[v] += s_WindVec * m_spParticleData->windPower[v];

			//	カウントによって不透明度の更新
			rate = (float)m_spParticleData->count[v] / (float)m_spParticleData->life[v];

			//	アニメーション
			if( m_spParticleData->moveFlag[v] == true ){
				m_spParticleData->index[v] = (int)(15.9f * rate);
			}

			//	スケールの計算
			float sv = .0f;//scaleValue
			float lf = (float)m_spParticleData->life[v] / 2; //life half
			float next=0;

			if( m_spParticleData->count[v] < lf )
			{
				next = m_spParticleData->scaleMiddle[v] - m_spParticleData->scaleStart[v];
				sv = m_spParticleData->count[v] / lf ;
				m_spParticleData->scale[v] = m_spParticleData->scaleStart[v] + (next*sv);
			}else{
				next = m_spParticleData->scaleEnd[v] - m_spParticleData->scaleMiddle[v];
				sv = (m_spParticleData->count[v]-lf) / lf;
				m_spParticleData->scale[v] = m_spParticleData->scaleMiddle[v] + (next*sv);
			}

			//	色の計算
			sv = .0f;//scaleValue
			lf = (float)m_spParticleData->life[v] / 2; //life half

			if( m_spParticleData->count[v] < lf )
			{
				nextC.red = m_spParticleData->colorMiddle[v].red - 
					m_spParticleData->colorStart[v].red;
				nextC.green = m_spParticleData->colorMiddle[v].green - 
					m_spParticleData->colorStart[v].green;
				nextC.blue = m_spParticleData->colorMiddle[v].blue - 
					m_spParticleData->colorStart[v].blue;
				sv = m_spParticleData->count[v] / lf ;
				m_spParticleData->color[v] = 
					m_spParticleData->colorStart[v].red + (nextC.red*sv);
				m_spParticleData->color[v] = 
					m_spParticleData->colorStart[v].green + (nextC.green*sv);
				m_spParticleData->color[v] = 
					m_spParticleData->colorStart[v].blue + (nextC.blue*sv);
			}else{
				nextC = m_spParticleData->colorEnd[v].red - 
					m_spParticleData->colorMiddle[v].red;
				nextC = m_spParticleData->colorEnd[v].green - 
					m_spParticleData->colorMiddle[v].green;
				nextC = m_spParticleData->colorEnd[v].blue - 
					m_spParticleData->colorMiddle[v].blue;
				sv = (m_spParticleData->count[v]-lf) / lf;
				m_spParticleData->color[v].red = 
					m_spParticleData->colorMiddle[v].red + (nextC.red*sv);
				m_spParticleData->color[v].green = 
					m_spParticleData->colorMiddle[v].green + (nextC.green*sv);
				m_spParticleData->color[v].blue = 
					m_spParticleData->colorMiddle[v].blue + (nextC.blue*sv);
			}
			m_spParticleData->color[v].alpha = 255 * (1.0f - rate);

			//	中央力
			vec = m_spParticleData->initPos[v] - m_spParticleData->pos[v];

			//	原因発見；
			vec.normalize();

			if( m_spParticleData->count[v] < lf )
			{
				next = m_spParticleData->centerPowerMiddle[v] - 
					m_spParticleData->centerPowerStart[v];
				fv = m_spParticleData->count[v] / lf ;
				force = m_spParticleData->centerPowerStart[v] + (next*fv);
			}else{
				next = m_spParticleData->centerPowerEnd[v] - 
					m_spParticleData->centerPowerMiddle[v];
				fv = (m_spParticleData->count[v]-lf) / lf;
				force = m_spParticleData->centerPowerMiddle[v] + (next*fv);
			}
			m_spParticleData->pos[v] += vec*force;

			//======================================
			//	順番を入れ替える
			//======================================
			work->initPos[useNum] = m_spParticleData->initPos[v];
			work->pos[useNum] = m_spParticleData->pos[v];
			work->tex[useNum] = m_spParticleData->tex[v];
			work->life[useNum] = m_spParticleData->life[v];
			work->flag[useNum] = m_spParticleData->flag[v];
			work->texNum[useNum] = m_spParticleData->texNum[v];
			work->scale[useNum] = m_spParticleData->scale[v];
			work->scaleStart[useNum] = m_spParticleData->scaleStart[v];
			work->scaleMiddle[useNum] = m_spParticleData->scaleMiddle[v];
			work->scaleEnd[useNum] = m_spParticleData->scaleEnd[v];
			work->velocity[useNum] = m_spParticleData->velocity[v];
			work->moveFlag[useNum] = m_spParticleData->moveFlag[v];
			work->size[useNum] = m_spParticleData->size[v];
			work->index[useNum] = m_spParticleData->index[v];
			work->count[useNum] = m_spParticleData->count[v];
			work->windPower[useNum] = m_spParticleData->windPower[v];
			work->centerPowerStart[useNum] = m_spParticleData->centerPowerStart[v];
			work->centerPowerMiddle[useNum] = m_spParticleData->centerPowerMiddle[v];
			work->centerPowerEnd[useNum] = m_spParticleData->centerPowerEnd[v];
			work->color[useNum] = m_spParticleData->color[v];
			work->colorStart[useNum] = m_spParticleData->colorStart[v];
			work->colorMiddle[useNum] = m_spParticleData->colorMiddle[v];
			work->colorEnd[useNum] = m_spParticleData->colorEnd[v];

			useNum++;
		}
		work->texData[n].StartNum = saveNum;
		work->texData[n].UseNum = useNum - saveNum;

		saveNum = useNum;
	}
	m_spParticleData.Clear();
	m_spParticleData.SetPtr(new ParticleData);
	m_spParticleData = work;
	work.Clear();
}

void Particle::Render()
{
	Matrix m = RenderState::getViewMatrix();

	Vector3 axisX(m._11,m._12,m._13);
	Vector3 axisY(m._21,m._22,m._23);

	Vector3 tl,tr,ul,ur; //	t=top; u=under; l=left; r=right;
	Vector3 p;	//	p=pos;

	Vector2 uv[4];float tu,tv;

	for(int n=0;n<PARTICLE_MAX;n++)
	{
		//if( n > m_spParticleData->useNum-1 ) break;
		//axisX = Vector3(m._11,m._12,m._13);
		//axisY = Vector3(m._21,m._22,m._23);

		axisX = Vector3(m._11,m._21,m._31);
		axisY = Vector3(m._12,m._22,m._32);

		axisX *= m_spParticleData->scale[n];
		axisY *= m_spParticleData->scale[n];

		p=m_spParticleData->pos[n];

		//	左上
		tl.x = p.x - axisX.x + axisY.x;
		tl.y = p.y - axisX.y + axisY.y;
		tl.z = p.z - axisX.z + axisY.z;
		//	右上
		tr.x = p.x + axisX.x + axisY.x;
		tr.y = p.y + axisX.y + axisY.y;
		tr.z = p.z + axisX.z + axisY.z;
		//	左下
		ul.x = p.x - axisX.x - axisY.x;
		ul.y = p.y - axisX.y - axisY.y;
		ul.z = p.z - axisX.z - axisY.z;
		//	右下
		ur.x = p.x + axisX.x - axisY.x;
		ur.y = p.y + axisX.y - axisY.y;
		ur.z = p.z + axisX.z - axisY.z;

		m_spVertexBuf->pos[(n*ONE_PARTICLE_VERTEX_NUM)+0] = tl;
		m_spVertexBuf->pos[(n*ONE_PARTICLE_VERTEX_NUM)+1] = ul;
		m_spVertexBuf->pos[(n*ONE_PARTICLE_VERTEX_NUM)+2] = tr;
		m_spVertexBuf->pos[(n*ONE_PARTICLE_VERTEX_NUM)+3] = tr;
		m_spVertexBuf->pos[(n*ONE_PARTICLE_VERTEX_NUM)+4] = ul;
		m_spVertexBuf->pos[(n*ONE_PARTICLE_VERTEX_NUM)+5] = ur;

		switch( m_spParticleData->size[n] )
		{
		case TEXTURE_SIZE_1x1:
			uv[0].x = 0;	uv[0].y = 0;
			uv[1].x = 1;	uv[1].y = 0;
			uv[2].x = 0;	uv[2].y = 1;
			uv[3].x = 1;	uv[3].y = 1;
			break;
		case TEXTURE_SIZE_2x2:
			tu = ( m_spParticleData->index[n] % 2 ) * 0.5f;
			tv = ( m_spParticleData->index[n] / 2 ) * 0.5f;
			uv[0].x = tu;					uv[0].y = tv;
			uv[1].x = tu+0.499f;	uv[1].y = tv;
			uv[2].x = tu;					uv[2].y = tv+0.499f;
			uv[3].x = tu+0.499f;	uv[3].y = tv+0.499f;
			break;
		case TEXTURE_SIZE_4x4:
			tu = ( m_spParticleData->index[n] % 4 ) * 0.25f;
			tv = ( m_spParticleData->index[n] / 4 ) * 0.25f;
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

		//	色
		m_spVertexBuf->color[(n*ONE_PARTICLE_VERTEX_NUM)+0] =
			m_spVertexBuf->color[(n*ONE_PARTICLE_VERTEX_NUM)+1] =
			m_spVertexBuf->color[(n*ONE_PARTICLE_VERTEX_NUM)+2] =
			m_spVertexBuf->color[(n*ONE_PARTICLE_VERTEX_NUM)+3] =
			m_spVertexBuf->color[(n*ONE_PARTICLE_VERTEX_NUM)+4] =
			m_spVertexBuf->color[(n*ONE_PARTICLE_VERTEX_NUM)+5] = 
			m_spParticleData->color[n].Value_0from1();
	}
	//	Uniform送信
	ShaderManager::getSprite()->Send_Matrix(m_Matrix);

	glBindBuffer(GL_ARRAY_BUFFER, 0);// 0 を入れておかないといけない

	glDisable(GL_DEPTH_TEST);

ShaderManager::getSprite()->Begin();

	//
	//	テクスチャ分描画
	//
	for(int n=0;n<m_TextureUseNumber;n++)
	{
		//	無視用条件
		if(m_spTexture[n].GetPtr() == 0)continue;
		if(m_spParticleData->texData[n].UseNum == 0)continue;

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

		//	テクスチャ情報設定
		m_spTexture[n]->spTexture->Setting(rlib::Texture::ACTIVE_0);
		ShaderManager::getSprite()->SetValue_No_BeginEnd( "uTex", 0 );

		//	描画
		glDrawArrays(GL_TRIANGLES,
			m_spParticleData->texData[n].StartNum*ONE_PARTICLE_VERTEX_NUM,
			m_spParticleData->texData[n].UseNum*ONE_PARTICLE_VERTEX_NUM);
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
	for(int n=0;n<PARTICLE_MAX;n++)
	{
		if(true == m_spParticleData->flag[n])continue;

		//	Data格納
		m_spParticleData->useNum++;
		m_spParticleData->pos[n] = Pos;
		m_spParticleData->initPos[n] = Pos;
		m_spParticleData->flag[n] = true;
		m_spParticleData->scaleStart[n] = ScaleStart;
		m_spParticleData->scaleMiddle[n] = ScaleMiddle;
		m_spParticleData->scaleEnd[n] = ScaleEnd;
		m_spParticleData->life[n] = Life;
		m_spParticleData->velocity[n] = Velocity;
		m_spParticleData->moveFlag[n] = MoveFlag;
		m_spParticleData->size[n] = Size;
		m_spParticleData->index[n] = Index;
		m_spParticleData->windPower[n] = WindPower;
		m_spParticleData->centerPowerStart[n] = CenterPowerStart;
		m_spParticleData->centerPowerMiddle[n] = CenterPowerMiddle;
		m_spParticleData->centerPowerEnd[n] = CenterPowerEnd;
		m_spParticleData->colorStart[n] = ColorStart;
		m_spParticleData->colorMiddle[n] = ColorMiddle;
		m_spParticleData->colorEnd[n] = ColorEnd;

		//	どのテクスチャか探す
		for(int t=0;t<m_TextureUseNumber;t++)
		{
			if(m_spTexture[t].GetRefNum() == 0)continue;

			if(strcmp( m_spTexture[t]->TextureName, File ) == 0)
			{
				m_spParticleData->texNum[n] = m_spTexture[t]->texNum;
				return;
			}
		}
		break;
	}
}



//========================================================
//	使うかわからないもの
//========================================================

#if 0

//
//	インデックスレンダー
//

	LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");

	
ShaderManager::getSprite()->Begin();


	LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");

	//
	//	テクスチャ分描画
	//
	for(int n=0;n<m_TextureUseNumber;n++)
	{
		//	無視用条件
		if(m_spTexture[n].GetPtr() == 0)continue;
		if(m_spParticleData->texData[n].UseNum == 0)continue;

			LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		//	テクスチャ情報設定
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

		//	描画
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