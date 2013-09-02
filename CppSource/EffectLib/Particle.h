#ifndef __TXPARTICLE_H__
#define __TXPARTICLE_H__
#include	"math\\kmathf.h"
#include	"GraphicsLib\Class\rTexture\Texture.h"
#include	"StandardLib\\SmartPointer.h"
#include	"EffectLib\\Item.h"
#include	"GraphicsLib\Class\kGraphicsPipline\kGraphicsPipline.h"

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
struct ParticleData
{
	struct TexData
	{
		int StartNum;
		int	UseNum;
		TexData():StartNum(0),UseNum(0){}
	};

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
	TexData	texData[TEXTURE_MAX];
	int			useNum;
	ParticleData();
};

//	ParticleTextrue
struct ParticleTextrue
{
	sp<Texture>		spTexture;
	char					TextureName[TEXTURE_NAME_SIZE];
	//	�e�N�X�`���̔ԍ��擾
	static int	s_LimitNum;
	int					texNum;
	ParticleTextrue();
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
	//�g���N�����i
	void Setting(const Vector3& Pos,char* File,float Scale){}
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

	//	Data
	sp<ParticleTextrue>	m_spTexture[TEXTURE_MAX];
	int									m_TextureUseNumber;
	Matrix							m_Matrix;
	sp<ParticleData>		m_spParticleData;
	sp<Vertex>					m_spVertexBuf;
	static Vector3			s_WindVec;

	sp<klib::kTechnique> m_spPipeline;
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