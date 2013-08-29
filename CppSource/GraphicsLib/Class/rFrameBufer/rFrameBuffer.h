#ifndef _R_FRAME_BUFFER_
#define _R_FRAME_BUFFER_

#include <GLES2\gl2.h>
#include "GraphicsLib\Class\rTexture\Texture.h"
#include "GraphicsLib\Class\r2DObj\r2DRenderer.h"

namespace rlib
{
	/*
	�t���[���o�b�t�@�I�u�W�F�N�g������킷�N���X
	�o�͌��ʂ��g�p�������Ȃ�getColorTex()��rlib::Texture�N���X���Ƃ��Ă�������
	*/
	class FrameBuffer : public r2DRenderer
	{
	public:
		/*
		�E�X�^�e�B�b�N�֐�
		�E�o�b�N�o�b�t�@�������_�[�^�[�Q�b�g�ɐݒ肷��
		*/
		static void bindScreenBuffer(float r=0.1f, float g=0.125f, float b=0.3f, float a=1.f);

	public:
		FrameBuffer();
		~FrameBuffer();

		void clear();
		/*
		�E�t���[���o�b�t�@�I�u�W�F�N�g���쐬����
		�E����
			width, height	: �摜�T�C�Y
			format			: GL_RGB�Ȃǂ̃J���[�f�[�^�̌`��.	default=GL_RGBA
			bitOrder		: format�̃f�[�^�̋�؂�.			default=GL_UNSIGNED_SHORT_4_4_4_4
			depthFormat		: �[�x�o�b�t�@�̌`��.				default=GL_DEPTH_COMPONENT16
		*/
		void init( int width, int height, GLenum format=GL_RGBA, GLenum bitOrder=GL_UNSIGNED_SHORT_4_4_4_4, GLenum depthFormat=GL_DEPTH_COMPONENT16 );

		/*
		�E�t���[���o�b�t�@�������_�[�^�[�Q�b�g�ɐݒ肷��
		�E���Ńo�b�t�@�̃N���A�ƃr���[�|�[�g�̐ݒ�����Ă��܂�
		*/
		void bind(float r=0.1f, float g=0.125f, float b=0.3f, float a=1.f);

		virtual void render();

	public:
		void		bindColorTex(unsigned int activeIndex);
		Texture&	gexColorTexture(){return this->mColorTex;}

	protected:
		virtual void initTexBuf();

	private:
		int width, height;

		GLuint mFB;
		Texture mColorTex;
		GLuint mDepth;
	};
}

#endif