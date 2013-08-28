#ifndef _R_I2DOBJ_
#define _R_I2DOBJ_

#include "math\kmathf.h"
#include "GraphicsLib\Class\tRenderState\RenderState.h"
#include "GraphicsLib\Class\kGraphicsPipline\kGraphicsPipline.h"
#include "Ueda\TmpShader\Shader.h"

namespace rlib
{
	class Texture;

	class r2DRenderer
	{
	public:
		enum DRAW_ORIGIN_TYPE{
			DRAW_TOP_LEFT,
			DRAW_CENTER,
			DRAW_CENTER_TYPE_MAX,
		};

	public:
		r2DRenderer();

		void initBuf();

		void render(rlib::Texture* pTex);

	public://�Z�b�^�[�E�Q�b�^�[
		void setPos(float x, float y, float z=0.f){this->mPos.x = x; this->mPos.y = y; this->mPos.z = z; this->mIsUpdate = true;}
		void setSize(float x, float y){this->mSize.x = x; this->mSize.y = y; this->mIsUpdate = true;}
		void setDrawCenter(DRAW_ORIGIN_TYPE type){this->mCenterType = type; this->mIsUpdate = true;}

		const klib::math::Vector3& getPos()const{return this->mPos;}
		const klib::math::Vector2& getSize()const{return this->mSize;}
		DRAW_ORIGIN_TYPE getDrawOriginType()const{ return this->mCenterType; }

	protected:
		enum eS_TYPE
		{
			S_POS,
			S_TEX,
			S_MAX,
		};

	protected:
		virtual void initPosBuf();
		virtual void initTexBuf();

		virtual void update();
		virtual void innerRender(rlib::Texture* pTex);

		void getDrawOffset(float* x, float *y);

		void setTexBuf(bool isFrameBuffer);
	protected:
		klib::math::Vector3		mPosBuf[4];
		klib::math::Vector2		mTexBuf[4];

		bool				mIsUpdate;
		DRAW_ORIGIN_TYPE	mCenterType;
		klib::math::Vector3 mPos;
		klib::math::Vector2 mSize;
	};

	class r2DPipeline
	{
	public:
		static void init();
		static void clear();

		static klib::kGraphicsPipline& getPipeline(){return *spPipeline;};
		static RenderLib::Shader& getShader(){return *spShader;};

	private:
		static klib::kGraphicsPipline*	spPipeline;
		static RenderLib::Shader*		spShader;
	};

	class r2DHelper
	{
	public:
		/*
		�X�N���[�����X���W���v���W�F�N�V�������W�n�֕ϊ�����
		*/
		static float convertPosX(float screenX){
			screenX /= RenderLib::RenderState::getScreenWidth();
			screenX = screenX*2.f - 1.f;
			return screenX;
		}
		/*
		�X�N���[�����Y���W���v���W�F�N�V�������W�n�֕ϊ�����
		*/
		static float convertPosY(float screenY){
			screenY /= RenderLib::RenderState::getScreenHeight();
			screenY = screenY*2.f - 1.f;
			return -screenY;
		}
		/*
		�X�N���[�����W�n�ł̈ʒu�x�N�g�����v���W�F�N�V�������W�n�֕ϊ�����
		*/
		static klib::math::Vector2 convertPos(const klib::math::Vector2& posVec)
		{
			return klib::math::Vector2( convertPosX(posVec.x), convertPosY(posVec.y) );
		}

		/*
		�X�N���[�����W�n�ł�X�����̈ړ��ʂ��v���W�F�N�V�������W�n�֕ϊ�����
		*/
		static float convertMoveX(float screenX){
			screenX *= getWidthUnit();
			return screenX;
		}
		/*
		�X�N���[�����W�n�ł�Y�����̈ړ��ʂ��v���W�F�N�V�������W�n�֕ϊ�����
		*/
		static float convertMoveY(float screenY){
			screenY *= getHeightUnit();
			return -screenY;
		}
		/*
		�X�N���[�����W�n�ł̈ړ��x�N�g�����v���W�F�N�V�������W�n�֕ϊ�����
		*/
		static klib::math::Vector2 convertMove(const klib::math::Vector2& moveVec)
		{
			return klib::math::Vector2( convertMoveX(moveVec.x), convertMoveY(moveVec.y) );
		}

		/*
		�v���W�F�N�V�������W�n�̉����ɂ�����1�s�N�Z���̊�����Ԃ�
		*/
		static float getWidthUnit(){ float rate = 1.f / RenderLib::RenderState::getScreenWidth();  return rate;}
		/*
		�v���W�F�N�V�������W�n�̏c���ɂ�����1�s�N�Z���̊�����Ԃ�
		*/
		static float getHeightUnit(){float rate = 1.f / RenderLib::RenderState::getScreenHeight(); return rate;}
	};

}
#endif