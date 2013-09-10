#include "GimmickInfo.h"

#include "utility\textWriter.h"

//#include <iostream>
#include "utility\utility.h"

#include "IGimmick.h"
#include "Wind.h"
#include "Fuse.h"

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

void GimmickInfoBase::writeVec3(klib::math::Vector3& vec, textWriter& w)
{
	w.write(vec.x).write(' ').write(vec.y).write(' ').write(vec.z);
}

GIMMICK_TYPE rlib::getGimmickType(PlacementLib::ePLACEMENT_TYPE type)
{
	static GIMMICK_TYPE table[]={
		eGIMMICK_DRUM,			//PLACEMENT_BOX,				//	��(�����e�X�g�p)
		eGIMMICK_GARBAGE_BAG,	//PLACEMENT_GarbageBag,			//	�S�~��
		eGIMMICK_DRUM,			//PLACEMENT_DRUM,				//	�h����
		eGIMMICK_GASOLINE,		//PLACEMENT_GASOLINE,			//	�K�\����
		eGIMMICK_WOOD_BOX,		//PLACEMENT_WoodenBox,			//	�ؔ�
		eGIMMICK_CARDBOARD,		//PLACEMENT_CARDBOARD,			//	�_���{�[��
		eGIMMICK_FAN,			//PLACEMENT_FAN,				//	��@
		eGIMMICK_CANDLE,		//PLACEMENT_CANDLE,				//	�낤����
		eGIMMICK_2D,			//PLACEMENT_2D,					//	2D�摜
		eGIMMICK_GOAL,			//PLACEMENT_GOAL,				//	�S�[��
		eGIMMICK_RESET_CANDLE,	//PLACEMENT_RESET_CANDLE		//���Z�b�g�낤����
	};
	static const int tableNum = sizeof(table)/sizeof(table[0]);

	if( type >= tableNum){
		LOGE(TAG, "GimmickInfo.cpp : getGimmickType() unknwon type... type=%d\n", type);
		return table[0];
	}else{
		return table[type];
	}
}

//===============================================================
//
//		GimmickInfoBase�N���X
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
//		GGimmickInfo�N���X
//
//===============================================================
void GGimmickInfo::convert(PlacementData* data, int index)
{
//���O�̐ݒ�
	//cout << "GGimmickInfo�Ŗ��O�ݒ�̏�������Ă�" << endl;
	this->name = data->spStr[index];
//type�̐ݒ�
	//cout << "GGimmickInfo��type�ݒ�̏�������Ă�" << endl;
	//this->type = eGIMMICK_DRUM;
	this->type = getGimmickType(data->spType[index]);
//���̃p�����[�^�̐ݒ�
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
