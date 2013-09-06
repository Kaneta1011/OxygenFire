#pragma once

#include "templateLib\kVector.h"
#include "math\kmathf.h"

//ëOï˚êÈåæ
namespace rlib
{
	class r2DObj;
}
namespace klib
{
	class kPlayer;
	class kGraphicsPipline;
}

namespace klib
{
#define TEST_ACTION_POS 100
	extern int TEST_POS_NUM;
	extern klib::math::Vector3 testpos[TEST_ACTION_POS];

	struct TouchEvent
	{
		f32 m_FontTime;
		f32 m_RingTime;
	};
	class ActionMediate
	{
		typedef ktl::kVector<TouchEvent> TimeTable;
		typedef ktl::kVector<s32> IndexList;
	private:
		static kPlayer* m_Player;
		static IndexList m_IndexList;
		static TimeTable m_TimeTable;

		static rlib::r2DObj* m_Font0;
		static rlib::r2DObj* m_FireMask;
		static rlib::r2DObj* m_Ring;

		static kGraphicsPipline* m_AlphaBord;
		static kGraphicsPipline* m_AddBord;

	public:
		static void init(kPlayer* player);
		static bool update();
		static void render();
	};
}