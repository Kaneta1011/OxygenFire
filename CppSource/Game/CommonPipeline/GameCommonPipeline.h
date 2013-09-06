#ifndef _GAME_COMMON_PIPELINE_
#define _GAME_COMMON_PIPELINE_

#include "GraphicsLib\Class\kGraphicsPipline\kGraphicsPipline.h"
#include "GraphicsLib\Class\kBlendState\kBlendState.h"

namespace rlib
{
	class GameCommonPipeline
	{
	public:
		static void init();

		static void clear();

		static klib::kGraphicsPipline* getPipeline(){return pipeline;}

	private:
		static klib::kGraphicsPipline* pipeline;
	};
}

#endif