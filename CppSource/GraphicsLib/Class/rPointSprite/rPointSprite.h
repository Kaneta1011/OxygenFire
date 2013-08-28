#ifndef _R_POINT_SPRITE_
#define _R_POINT_SPRITE_

#include "math/kmathf.h"
#include "GraphicsLib\Class\rTexture\Texture.h"
#include "Ueda\TmpShader\Shader.h"

#include <GLES2\gl2.h>
#include <GLES2/gl2ext.h>

namespace rlib{
	//==============================================================================================
	//
	//	PointSprite class
	//		�E�_��񂩂�X�v���C�g��`�悷��N���X
	//		�E�`�搔��7500���炢���A���肵�ĕ`��ł��鐔�݂���
	//
	//==============================================================================================
	class PointSprite
	{
	public:
		enum DRAW_OFFSET_TYPE{
			OFFSET_CENTER,
			OFFSET_TOP_LEFT,
		};
	public:
		typedef Texture myTexture;
		static void init();
		static void clear();

		//
		//	arrayDrawPos�̒l��0.f�`1.f�͈̔͂��Ƃ�悤�ɂ��Ă�������
		//	�E��ʃT�C�Y���قȂ�@��Ԃł̌݊���ۂ��߂ɂ��Ă��܂�
		//	�@�C���[�W��UV���W�ł��Ă��銴���ł�
		//	�Esize��0.f�`100.f�̊ԂɂȂ�悤�ɂȂ��Ă܂��B
		//
		static void render(klib::math::Vector3* arrayDrawPos, int count, myTexture* texture, float size, DRAW_OFFSET_TYPE offsetType = OFFSET_CENTER);

	private:
		enum eS_TYPE
		{
			S_POS,
			S_MAX,
		};

	private:
		static float getDrawOffsetX(DRAW_OFFSET_TYPE type);
		static float getDrawOffsetY(DRAW_OFFSET_TYPE type);

	private:
		static RenderLib::Shader*	m_spSprite;

	private:
		PointSprite(){}
		PointSprite(PointSprite&){}
		void operator=(PointSprite&){}
		~PointSprite(){}
	};

}
#endif