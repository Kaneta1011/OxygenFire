#include "GimmickInfo.h"

#include "utility\textWriter.h"

//#include <iostream>
#include "utility\utility.h"

#include "Game\Gimmick\Gimmick.h"

using namespace std;
using namespace rlib;

const char* GimmickInfoBase::CHECK_END = "END@";

static const char* TAG = "GimmickInfo";

template<typename T>
void writeStringIt(T begin, T end, textWriter& w)
{
	for( T it = begin; it != end; it++ ){
		w.write(it->c_str());
		w.write(' ');
	}
	w.write(GimmickInfoBase::CHECK_END);
}

void writeVec3(klib::math::Vector3& vec, textWriter& w)
{
	w.write(vec.x).write(' ').write(vec.y).write(' ').write(vec.z);
}

GIMMICK_TYPE rlib::getGimmickType(PlacementLib::ePLACEMENT_TYPE type)
{
	static GIMMICK_TYPE table[]={
		eGIMMICK_WOOD_BOX,		//PLACEMENT_BOX,				//	箱(初期テスト用)
		eGIMMICK_GARBAGE_BAG,	//PLACEMENT_GarbageBag,			//	ゴミ袋
		eGIMMICK_DRUM,			//PLACEMENT_DRUM,				//	ドラム
		eGIMMICK_GASOLINE,		//PLACEMENT_GASOLINE,			//	ガソリン
		eGIMMICK_WOOD_BOX,		//PLACEMENT_WoodenBox,			//	木箱
		eGIMMICK_CARDBOARD,		//PLACEMENT_CARDBOARD,			//	ダンボール
		eGIMMICK_FAN,			//PLACEMENT_FAN,				//	扇風機
		eGIMMICK_CANDLE,		//PLACEMENT_CANDLE,				//	ろうそく
		eGIMMICK_2D,			//PLACEMENT_2D,					//	2D画像
	};
	return table[type];
}

//===============================================================
//
//		GimmickInfoBaseクラス
//
//===============================================================
void GimmickInfoBase::forFile(textWriter& w)
{
	w.write("name\t").write(this->name.c_str()).br();

	w.write("type\t").write(this->type).br();

	w.write("checkOn\t");
	writeStringIt( checkOn.begin(), checkOn.end(), w);
	w.br();

	w.write("checkOff\t");
	writeStringIt( checkOff.begin(), checkOff.end(), w);
	w.nestOut();
	w.br();
}

void GimmickInfoBase::load(textLoader& loader)
{
	while(true)
	{
		loader.LoadString(loader.tmpBuf);
		if( !loadParam(loader) )
		{
			if( strcmp("}",loader.tmpBuf) == 0 ){
				break;
			}else{
				LOGE(TAG, "unknown type set at setGimmickInfo!! type=%d | name=\"%s\"\n", type, name.c_str());
				break;
			}
		}
	}
}

bool GimmickInfoBase::loadParam(textLoader& loader)
{
	if( strcmp("checkOn", loader.tmpBuf) == 0 ){
		while(true){
			loader.LoadString(loader.tmpBuf);
			if( strcmp(GimmickInfoBase::CHECK_END, loader.tmpBuf)==0) break;
			this->checkOn.push_back(loader.tmpBuf);
		}
		return true;
	}else if( strcmp("checkOff", loader.tmpBuf) == 0 ){
		while(true){
			loader.LoadString(loader.tmpBuf);
			if( strcmp(GimmickInfoBase::CHECK_END, loader.tmpBuf) == 0) break;
			this->checkOff.push_back(loader.tmpBuf);
		}
		return true;
	}
	return false;
}

//===============================================================
//
//		GGimmickInfoクラス
//
//===============================================================
void GGimmickInfo::convert(PlacementData* data, int index)
{
//名前の設定
	//cout << "GGimmickInfoで名前設定の処理作ってね" << endl;
	this->name = data->spStr[index];
//typeの設定
	//cout << "GGimmickInfoでtype設定の処理作ってね" << endl;
	//this->type = eGIMMICK_DRUM;
	this->type = getGimmickType(data->spType[index]);
//他のパラメータの設定
	this->pos = data->spPos[index];
	this->angle = data->spAngle[index];
	this->scale = data->spScale[index];
}

void GGimmickInfo::forFile(textWriter& w)
{
	GimmickInfoBase::forFile(w);

	w.nestIn();w.writeNest();
	w.write("pos\t");
	writeVec3(pos, w);
	w.br().write("angle\t");
	writeVec3(angle, w);
	w.br().write("scale\t");
	writeVec3(scale, w);
	w.nestOut();
	w.br();
}

bool GGimmickInfo::loadParam(textLoader& loader)
{
	if( ! GimmickInfoBase::loadParam(loader) )
	{
		if( strcmp("pos",loader.tmpBuf) == 0 ){
			this->pos.x = loader.LoadFloat();
			this->pos.y = loader.LoadFloat();
			this->pos.z = loader.LoadFloat();
			return true;
		}else if( strcmp("angle",loader.tmpBuf) == 0 ){
			this->angle.x = loader.LoadFloat();
			this->angle.y = loader.LoadFloat();
			this->angle.z = loader.LoadFloat();
			return true;
		}else if( strcmp("scale",loader.tmpBuf) == 0 ){
			this->scale.x = loader.LoadFloat();
			this->scale.y = loader.LoadFloat();
			this->scale.z = loader.LoadFloat();
			return true;
		}
		return false;
	}
	return true;
}

Gimmick* GGimmickInfo::makeGimmick()
{
	Gimmick* set = new Gimmick();
	GimmickInfo info;
	info.type = this->type;
	info.pos = this->pos;
	info.angle = this->angle;
	info.size = this->scale;
	set->init(info);
	return set;
}

//===============================================================
//
//		GLineInfoクラス
//
//===============================================================
void GLineInfo::convert(GimmickLine* data, int index)
{
//名前の設定
	//cout << "GLineInfoで名前設定の処理作ってね" << endl;
	this->name = data->spStr[index];
//
	this->type = eGIMMICK_FUSE;
	this->start = data->spStart[index];
	this->end = data->spEnd[index];
}

void GLineInfo::forFile(textWriter& w)
{
	GimmickInfoBase::forFile(w);

	w.nestIn();w.writeNest();
	w.write("start\t");
	writeVec3(start, w);
	w.br().write("end\t");
	writeVec3(end, w);
	w.nestOut();
	w.br();
}

bool GLineInfo::loadParam(textLoader& loader)
{
	if( ! GimmickInfoBase::loadParam(loader) )
	{
		if( strcmp("start",loader.tmpBuf) == 0 ){
			this->start.x = loader.LoadFloat();
			this->start.y = loader.LoadFloat();
			this->start.z = loader.LoadFloat();
			return true;
		}else if( strcmp("end",loader.tmpBuf) == 0 ){
			this->end.x = loader.LoadFloat();
			this->end.y = loader.LoadFloat();
			this->end.z = loader.LoadFloat();
			return true;
		}
		return false;
	}
	return true;
}

Gimmick* GLineInfo::makeGimmick()
{
	Gimmick* set = new Gimmick();
	GimmickInfo info;
	info.type = this->type;
	info.pos = this->start;
	info.size.x = info.size.y = info.size.z = 0.5f;
	set->init(info);
	return set;
}

//===============================================================
//
//		GWindInfoクラス
//
//===============================================================
void GWindInfo::convert(WindData* data, int index)
{
//名前の設定
	//cout << "GWindInfoで名前設定の処理作ってね" << endl;
	this->name = data->spStr[index];
//
	this->type = eGIMMICK_WIND;
	this->pos = data->spPos[index];
	this->dir = data->spWindVec[index];
	this->scale = data->spScale[index];
}

void GWindInfo::forFile(textWriter&  w)
{
	GimmickInfoBase::forFile(w);

	w.nestIn();w.writeNest();
	w.write("pos\t");
	writeVec3(pos, w);
	w.br().write("dir\t");
	writeVec3(dir, w);
	w.br().write("scale\t");
	writeVec3(scale, w);

	w.nestOut();
	w.br();
}

bool GWindInfo::loadParam(textLoader& loader)
{
	if( ! GimmickInfoBase::loadParam(loader) )
	{
		if( strcmp("pos",loader.tmpBuf) == 0 ){
			this->pos.x = loader.LoadFloat();
			this->pos.y = loader.LoadFloat();
			this->pos.z = loader.LoadFloat();
			return true;
		}else if( strcmp("dir",loader.tmpBuf) == 0 ){
			this->dir.x = loader.LoadFloat();
			this->dir.y = loader.LoadFloat();
			this->dir.z = loader.LoadFloat();
			return true;
		}else if( strcmp("scale",loader.tmpBuf) == 0 ){
			this->scale.x = loader.LoadFloat();
			this->scale.y = loader.LoadFloat();
			this->scale.z = loader.LoadFloat();
			return true;
		}
		return false;
	}
	return true;
}

Gimmick* GWindInfo::makeGimmick()
{
	Gimmick* set = new Gimmick();
	GimmickInfo info;
	info.type = this->type;
	info.pos = this->pos;
	info.size = this->scale;
	set->init(info);
	return set;
}