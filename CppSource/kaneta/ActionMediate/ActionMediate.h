#pragma once

#include "templateLib\kVector.h"
#include "math\kmathf.h"
#include "templateLib\kIAssociative.h"
#define TEST_ACTION_POS 100
extern int TEST_POS_NUM;

//ëOï˚êÈåæ
namespace rlib
{
	class r2DObj;
	class IGimmick;
}
namespace klib
{
	class kPlayer;
	class kGraphicsPipline;
}

namespace klib
{
	extern klib::math::Vector3 testpos[TEST_ACTION_POS];
	struct TouchEvent
	{
		f32 m_FontTime;
		f32 m_RingTime;
		math::Vector3 m_Pos;
	};
	class ActionMediate
	{

		typedef ktl::kVector<rlib::IGimmick*> PointerList;
	private:

		static ktl::kIAssociative<rlib::IGimmick*,TouchEvent> m_Table;

		static PointerList m_PointerList;


		static rlib::r2DObj* m_Font0;
		static rlib::r2DObj* m_FireMask;
		static rlib::r2DObj* m_Ring;

		static kGraphicsPipline* m_AlphaBord;
		static kGraphicsPipline* m_AddBord;

	public:
		static void init();
		static bool release();
		static bool update(kPlayer* m_Player);
		static void render();
	};
}