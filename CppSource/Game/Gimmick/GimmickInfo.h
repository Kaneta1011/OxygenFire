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
	class Gimmick;

	enum GIMMICK_TYPE
	{
		eGIMMICK_DRUM,			//�h������
		eGIMMICK_GASOLINE,		//�K�\����
		eGIMMICK_GARBAGE_BAG,	//�S�~��
		eGIMMICK_WOOD_BOX,		//�ؔ�
		eGIMMICK_CARDBOARD,		//�_���{�[��
		eGIMMICK_FAN,			//��@
		eGIMMICK_CANDLE,		//�낤����
		eGIMMICK_FUSE,			//���ΐ�
		eGIMMICK_FUSE_POINT,	//���ΐ��̗��[
		eGIMMICK_WIND,			//��
		eGIMMICK_2D,			//2D�`��
		eGIMMICK_TYPE_NUM,
	};
	GIMMICK_TYPE getGimmickType(PlacementLib::ePLACEMENT_TYPE type);

	struct GimmickInfoBase{
		static const char* CHECK_END;
		std::string name;
		GIMMICK_TYPE type;
		std::list<std::string> checkOn;
		std::list<std::string> checkOff;

		virtual void forFile(textWriter& writer) = 0;

		void load(textLoader& loader);
		virtual bool loadParam(textLoader& loader) = 0;

		virtual Gimmick* makeGimmick()=0;
	};

	struct GGimmickInfo : public GimmickInfoBase
	{
		klib::math::Vector3 pos;
		klib::math::Vector3 angle;
		klib::math::Vector3 scale;

		void convert(PlacementData* data, int index);
		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual Gimmick* makeGimmick();

	};

	struct GLineInfo : public GimmickInfoBase
	{
		klib::math::Vector3 start;
		klib::math::Vector3 end;

		void convert(GimmickLine* data, int index);
		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual Gimmick* makeGimmick();
	};

	struct GWindInfo : public GimmickInfoBase
	{
		klib::math::Vector3 pos;
		klib::math::Vector3 dir;
		klib::math::Vector3 scale;

		void convert(WindData* data, int index);
		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);
		
		virtual Gimmick* makeGimmick();
	};

}

#endif