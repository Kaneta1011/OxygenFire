#ifndef __TXPARTICLE_H__
#define __TXPARTICLE_H__
#include	"math\\kmathf.h"
#include	"GraphicsLib\Class\rTexture\Texture.h"
#include	"StandardLib\\SmartPointer.h"
#include	"EffectLib\\Item.h"
#include	"GraphicsLib\Class\kGraphicsPipline\kGraphicsPipline.h"
#include	"GraphicsLib\Class\kInputLayout\kInputLayout.h"


#include	<GLES2/gl2.h>
#include	<GLES2/gl2ext.h>
typedef unsigned long       DWORD;
typedef unsigned char       BYTE;

namespace EffectLib{

using namespace rlib;
using namespace klib::math;


//	Constant
const static int TEXTURE_MAX = 128;
static const int TEXTURE_NAME_SIZE = 128;
const static int PARTICLE_MAX = 2000;
const static int ONE_PARTICLE_VERTEX_NUM = 6;
const static int PARTICLE_VERTEX_MAX = 
	PARTICLE_MAX * ONE_PARTICLE_VERTEX_NUM;

//-----------------------------------------------
//	struct
//-----------------------------------------------
struct Vertex
{
	sp<Vector3> pos;
	sp<Vector2> tex;
	sp<Vector4>	color;
	Vertex();
};

struct TexData
{
	int StartNum;
	int	UseNum;
	TexData():StartNum(0),UseNum(0){}
};

struct ParticleData
{
	sp<int>				count;
	sp<Vector3>		initPos;
	sp<Vector3>		pos;
	sp<Vector2>		tex;
	sp<int>				life;
	sp<float>			scale;
	sp<float>			scaleStart;
	sp<float>			scaleMiddle;
	sp<float>			scaleEnd;
	sp<bool>			flag;
	sp<int>				texNum;
	sp<Vector3>		velocity;
	sp<bool>			moveFlag;
	sp<int>				index;
	sp<int>				size;
	sp<float>			windPower;
	sp<float>			centerPowerStart;
	sp<float>			centerPowerMiddle;
	sp<float>			centerPowerEnd;
	sp<COLOR>			color;
	sp<COLOR>			colorStart;
	sp<COLOR>			colorMiddle;
	sp<COLOR>			colorEnd;
	int			useNum;

	void Setting(sp<ParticleData> data);

	void Clear();
	ParticleData();
};

//	ParticleTextrue
struct ParticleTextrue
{
	sp<Texture>		spTexture;
	char					TextureName[TEXTURE_NAME_SIZE];
	//	テクスチャの番号取得
	static int	s_LimitNum;
	int					texNum;
	ParticleTextrue();
};


class particle 
{
public:
	void create( const Vector3& pos , const Vector3& vec , int life, sp<Texture> tex )
	{
		this->pos = pos;
		this->vec = vec;
		this->life = life;

	}

	void setting_vec(const Vector3& vec)
	{
		this->vec = vec;
	}

	void update()
	{
		pos += vec;
	}

	static void render()
	{

	}
	particle():pos(Vector3(0,0,0)),vec(Vector3(0,0,0)),life(0){}
private:
	Vector3 pos;
	Vector3 vec;
	int life;
};

//-------------------------------------------------------------------------
//	Particle
//-------------------------------------------------------------------------
class Particle
{
public:
	//	Initialize
	bool Initialize();
	//	Update
	void Update();
	//	Render
	void Render();
	//	Setting
	void Setting(const Vector3& Pos,char* File,float ScaleStart,
		float ScaleMiddle,float ScaleEnd,int Life,
		const Vector3& Velocity,bool MoveFlag,int Size,int Index,
		float WindPower,float CenterPowerStart,float CenterPowerMiddle,
		float CenterPowerEnd,COLOR ColorStart,COLOR ColorMiddle,COLOR ColorEnd);



	//========== 単体パーティクル用 =============
	enum eSINGLE_TYPE
	{
		SINGLE_NORMAL,
	};
	void Setting_Single(
		eSINGLE_TYPE Type,
		int Life,
		const Vector3& Pos,
		const Vector3& Move,
		float Scale,
		//	色の設定
		COLOR StartColor,
		COLOR MiddleColor,
		COLOR EndColor );
	//========== 単体パーティクル用 =============

	bool Setting_Texture( char* TextureName );
	//	BasicSet
	Particle(){Clear();}void Clear();
	~Particle(){Destroy();}void Destroy();
private:
	//	Constant
	enum eVBO_TYPE
	{
		VBO_POS,
		VBO_TEX,
		VBO_COLOR,
		VBO_MAX,
	};
	enum eTEXTURE_SIZE_TYPE
	{
		TEXTURE_SIZE_1x1 = 1,
		TEXTURE_SIZE_2x2 = 4,
		TEXTURE_SIZE_4x4 = 16,
	};

	//	Check
	bool Check_UseTexture( char* CheckName );

	//	Create
	void Create_VBO();
	GLuint m_VBO[VBO_MAX];
	GLuint offset;

	TexData	texData[TEXTURE_MAX];

	//	Data
	int UpdateNumber;

	sp<ParticleTextrue>	m_spTexture[TEXTURE_MAX];
	int									m_TextureUseNumber;
	Matrix							m_Matrix;
	sp<ParticleData>		m_spParticleData;
	sp<ParticleData>		m_spParticleData2;

	sp<Vertex>					m_spVertexBuf;
	static Vector3			s_WindVec;

	//sp<ParticleData> work;
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	Particle_Singleton
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Particle_Singleton : public Particle
{
protected:
	static Particle_Singleton* singleton;

	Particle_Singleton(){}
	virtual ~Particle_Singleton(){}
public:
	static Particle_Singleton* getInstance()
	{
		if( singleton == NULL ){
			singleton = new Particle_Singleton;
		}
		return singleton;
	}
	static void deleteInstance(){
		if( singleton != NULL ){
			delete singleton;

			singleton = NULL;
		}
	}
};

#define sParticle ( Particle_Singleton::getInstance() )

}//Renderlib




#endif






#if 0 



void Render(const math::Matrix& mat,kGraphicsPipline* shader)
{
	const kMeshData* m_MeshData=m_Mesh->getMeshPtr();
	//モデル行列作成(テスト用)
	math::Matrix wvp = 
		mat * 
		RenderLib::RenderState::getViewMatrix() *
		RenderLib::RenderState::getProjectionMatrix();
	//シェーダに行列転送
	shader->setShaderValue("WVP",wvp);
	//頂点バッファを更新する
	kDevice::updateSubResource(mp_VBO,((kMeshVertex*)m_MeshData->mp_Vertex),sizeof(kMeshVertex)*m_MeshData->m_Info.NumVertex);

	for(int i=0;i<m_MeshData->m_Info.MaterialCount;i++)
	{
		if(!mp_IBO[i])continue;
		shader->setTexture("colorTex",0,m_MeshData->m_Info.Diffuse[i]);
		//頂点バッファをパイプラインにセットする
		kDevice::IAsetVertexBuffer(mp_VBO);
		//インデックスバッファをパイプラインにセットする
		kDevice::IAsetIndexBuffer(mp_IBO[i]);
		//シェーダーをパイプラインにセットする
		shader->setPipline();
//glEnable(GL_BLEND);
//glBlendFunc(GL_ONE,GL_ONE);

//glEnable(GL_TEXTURE_2D);
		//描画
		kDevice::drawIndexed(m_MeshData->m_Info.MaterialNumFace[i]*3);
	}

}



#endif