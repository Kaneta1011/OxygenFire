#ifndef _GIMMICK_INFO_
#define _GIMMICK_INFO_

#include "math\kmathf.h"
#include "PlacementLib/Placement.h"
#include "utility\textLoader.h"
#include <string>
#include <list>

namespace rlib
{
	using namespace PlacementLib;

	class textWriter;
	class IGimmick;

	enum GIMMICK_TYPE
	{
		eGIMMICK_DRUM,			//ドラム缶
		eGIMMICK_GASOLINE,		//ガソリン
		eGIMMICK_GARBAGE_BAG,	//ゴミ袋
		eGIMMICK_WOOD_BOX,		//木箱
		eGIMMICK_CARDBOARD,		//ダンボール
		eGIMMICK_FAN,			//扇風機
		eGIMMICK_CANDLE,		//ろうそく
		eGIMMICK_FUSE,			//導火線
		eGIMMICK_FUSE_POINT,	//導火線の両端
		eGIMMICK_WIND,			//風
		eGIMMICK_2D,			//2D描画
		eGIMMICK_GOAL,			//ゴール
		eGIMMICK_RESET_CANDLE,	//リセットろうそく
		eGIMMICK_TYPE_NUM,
		eGIMMICK_UNKNOWN
	};
	GIMMICK_TYPE getGimmickType(PlacementLib::ePLACEMENT_TYPE type);

	struct GimmickInfoBase{
		static const char* CHECK_END;
		static void writeVec3(klib::math::Vector3& vec, textWriter& w);

		std::string name;
		GIMMICK_TYPE type;
		std::list<std::string> checkOn;
		std::list<std::string> checkOff;

		void setNameAndType(std::string& name, int type){
			this->name = name;
			this->type = (GIMMICK_TYPE)type;
		}
		virtual void forFile(textWriter& writer) = 0;

		void load(textLoader& loader);
		virtual bool loadParam(textLoader& loader) = 0;

		virtual IGimmick* makeGimmick()=0;
	};

	struct GGimmickInfo : public GimmickInfoBase
	{
		klib::math::Vector3 pos;
		klib::math::Vector3 angle;
		klib::math::Vector3 scale;

		void convert(PlacementData* data, int index);
		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual IGimmick* makeGimmick()=0;

	};

}

#endif