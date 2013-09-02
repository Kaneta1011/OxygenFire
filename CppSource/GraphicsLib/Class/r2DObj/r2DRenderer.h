#ifndef _R_I2DOBJ_
#define _R_I2DOBJ_

#include "math\kmathf.h"
#include "GraphicsLib\Class\tRenderState\RenderState.h"
#include "GraphicsLib\Class\kGraphicsPipline\kGraphicsPipline.h"

namespace rlib
{
	class Texture;

	/*
	-------2D�`��ł͋@�킲�Ƃ̉�ʃT�C�Y�ɂ��e�������炷���ߕ����̍��W�n���g���Ă܂�------
	�E�X�V���W		�F	cpu���Ŏg�����W�n
	�E�`����W		�F	GPU���Ŏg�����W�n
	�E����ʍ��W	�F	��ʂ̎��ۂ̃T�C�Y

	---�e���W�n�̉�ʒ[�����ʒ[�܂ł̒l�͈̔͂̕\---
	(���W�n)	�[��	( x�������Ƃ�͈�) | (y�������Ƃ�͈�)
	�X�V���W	�[��	(-100 �` x �` 100) | (-100 �` y �` 100)
	�`����W	�[��	(-  1 �` x �`   1) | (-  1 �` y �`�@ 1)
	����ʍ��W	�[��	( 0 �` x �` RenderState::getScreenWidth() ) | ( 0 �` y �` RenderState::getScreenHeight() )
	*/

	//==================================================================
	//
	//		2D�ɂ�������W�ϊ����T�|�[�g����
	//		�E�X�N���[�����W����v���W�F�N�V�������W�ւ̒l�ɕϊ��֐�������܂��B
	//		�E2D�`��ł͍��W�̎w��̓v���W�F�N�V�������W�n���̗p���Ă��܂��B
	//			x,y�Ƃ��ɒl�͈̔͂�-100(��,��)�`100(�E,��)�ɂȂ��Ă܂��B
	//		�EmlInput�N���X�͊e�Q�b�^�[�ɂ��̃N���X�̊֐����g���Ă܂��̂ŁA�^�b�`�ʒu�Ȃǂ��g���ہA���p�ґ����ϊ����Ȃ��Ă����悤�ɂ��Ă��܂��B
	//
	//==================================================================
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

		/*
		�X�V���Ɉ���2D���W�l��`��p�̂��̂ɕϊ�����֐�
		*/
		static float toRenderCoord(float updateCoord){
			static const float UtoR = 0.01f;
			return updateCoord * UtoR;
		}
		/*
		�`�掞�Ɉ���2D���W�l���X�V�p�̂��̂ɕϊ�����֐�
		*/
		static float toUpdateCoord(float renderCoord){
			static const float RtoU = 100.f;
			return renderCoord * RtoU;
		}

		/*
		�X�V��̃T�C�Y�������ʏ�ŏc�������T�C�Y�ɂȂ�悤�ɂȂ�悤�ȃT�C�Y(�X�V���W�n)��Ԃ�
		�E����
			float updateRange	:�X�V���W�n�ł̃T�C�Y
			bool isLarge		:�c���̃T�C�Y�����傫���ق��ɃT�C�Y�����킹�邩�̃t���O�B�@false�Ȃ珬�����ق��ɂ��킹��
		*/
		static klib::math::Vector2 adjustSize(float updateRange, bool isLarge)
		{
			float w = updateRange * (RenderLib::RenderState::getScreenWidth() * 0.5f);
			float h = updateRange * (RenderLib::RenderState::getScreenHeight() * 0.5f);
			if( isLarge ){
				if( klib::math::kfabsf(w) < klib::math::kfabsf(h) ) w = h;
				else						h = w;
			}else{
				if( klib::math::kfabsf(w) > klib::math::kfabsf(h) ) w = h;
				else						h = w;
			}

			klib::math::Vector2 size;
			size.x = w / (RenderLib::RenderState::getScreenWidth() * 0.5f);
			size.y = h / (RenderLib::RenderState::getScreenHeight() * 0.5f);
			return size;
		}
	};

	/*
	�E2D�`��̂��߂̊��N���X
	�E2D�`����s�������Ƃ���rlib::r2DObj�N���X���g�p���Ă�������
	*/
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

		virtual void	render()=0;
		/*
		�O������e�N�X�`�����w�肵�ĕ`�悷��
		*/
		void			render(rlib::Texture* pTex);

	public://�Z�b�^�[�E�Q�b�^�[
		/*
		x,y���Ƃ�l�͈̔͂�-100(���[)�`100(�E�[)�̊ԁA0����ʒ���
		z��0�`1�̊�
		*/
		void setPos(float x, float y, float z=0.f){this->mPos.x = x; this->mPos.y = y; this->mPos.z = z; this->mIsUpdate = true;}
		/*
		x,y���Ƃ�l�͈̔͂�-100(���[)�`100(�E�[)�̊ԁA0����ʒ���
		z��0�`1�̊�
		*/
		void setPos(const klib::math::Vector3& pos){this->mPos = pos;			this->mIsUpdate = true;}
		/*
		x,y�Ƃ��ɒ[����[�܂ŕ\���������Ȃ�200��ݒ肵�Ă��������B
		�l�͈̔́@:�@0�`100(��ʔ���)�`200(��ʑS��)
		*/
		void setSize(float x, float y){this->mSize.x = x; this->mSize.y = y;	this->mIsUpdate = true;}
		/*
		x,y�Ƃ��ɒ[����[�܂ŕ\���������Ȃ�200��ݒ肵�Ă��������B
		�l�͈̔́@:�@0�`100(��ʔ���)�`200(��ʑS��)
		*/
		void setSize(const klib::math::Vector2& size){this->mSize = size;		this->mIsUpdate = true;}
		/*
		x,y�Ƃ��ɒ[����[�܂ŕ\���������Ȃ�200��ݒ肵�Ă��������B
		�l�͈̔́@:�@0�`100(��ʔ���)�`200(��ʑS��)
		*/
		void setSize(float size,bool isLarge){this->mSize = rlib::r2DHelper::adjustSize(size,isLarge);	this->mIsUpdate = true;}

		/*
		���[�J�����̕`��ʒu�̐ݒ�
		*/
		void setDrawOrigin(DRAW_ORIGIN_TYPE type){this->mCenterType = type;		this->mIsUpdate = true;}
		/*
		�F�̐ݒ�
		*/
		void setColor(float r, float g, float b, float a){
			this->mColor.x = r; this->mColor.y = g; this->mColor.z = b; this->mColor.w = a;
			this->mIsUpdate = true;
		}

		const klib::math::Vector3& getPos()const{return this->mPos;}
		const klib::math::Vector2& getSize()const{return this->mSize;}
		DRAW_ORIGIN_TYPE getDrawOrigin()const{ return this->mCenterType; }
		const klib::math::Vector4& getColor()const{return this->mColor; }

	protected:
		enum eS_TYPE
		{
			S_POS,
			S_TEX,
			S_MAX,
		};

	protected://���z�֐�
		//�`��p�o�b�t�@�̏��������s��
		//����initPosBuf��initTexBuf���Ăяo���Ă���
		//���̊֐��̓R���X�g���N�^���ŌĂяo����Ă��܂�
		void initBuf();
		//Pos�o�b�t�@�̍쐬
		virtual void initPosBuf();
		//UV���W�o�b�t�@�̍쐬
		virtual void initTexBuf();

		virtual void update();
		virtual void innerRender(rlib::Texture* pTex);

	protected:
		void getDrawOffset(float* x, float *y);
		void setTexBuf(bool isFrameBuffer);

	protected:
		GLuint					mBuf[S_MAX];
		klib::math::Vector3		mPosBuf[4];
		klib::math::Vector2		mTexBuf[4];

		bool				mIsUpdate;
		DRAW_ORIGIN_TYPE	mCenterType;
		klib::math::Vector3 mPos;
		klib::math::Vector2 mSize;
		klib::math::Vector4 mColor;
	};

	/*
	2D�`��Ŏg�p���郌���_�����O�p�C�v���C�����Ǘ�����N���X
	�E�Ƃ肠�����ARenderLib::Shader�𗘗p���Ă܂����A�̂��̂�klib::kGraphicsPipline�ֈڍs�������ł�
	*/
	class r2DPipeline
	{
	public:
		static void init();
		static void clear();

		static klib::kTechnique& getPipeline(){return *spPipeline;};

	private:
		static klib::kTechnique*	spPipeline;
	};

}
#endif