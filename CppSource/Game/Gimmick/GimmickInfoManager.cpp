#include "GimmickInfoManager.h"

#include "GExplosion.h"
#include "FusePoint.h"
#include "Wind.h"
#include "GGoal.h"
#include "GCandle.h"
#include "GResetCandle.h"
#include "GCandleChecker.h"
#include "G2D.h"

#include "PlacementLib/Placement.h"
#include "utility\textWriter.h"

#include "utility\utility.h"

#include <stdlib.h>

using namespace rlib;

static const char* TAG = "GimmickInfoManager";

template <typename T>
void swap(T& a, T& b)
{
	T w = a;
	a = b;
	b = w;
}

GimmickInfoManager::GimmickInfoManager()
{
}

GimmickInfoManager::~GimmickInfoManager()
{
	clear();
}

void GimmickInfoManager::clear()
{
	for( size_t i=0; i<this->mData.size(); i++ )
	{
		if( this->mData[i] ){ delete this->mData[i]; this->mData[i] = 0; }
	}
	this->mData.clear();
}

void GimmickInfoManager::loadMqo(char* mqoFilePath)
{
	clear();
	sPlacementManager->Load(mqoFilePath);
	sp<PlacementData> spBox;	sPlacementManager->GetBox(&spBox);
	sp<WindData> spWind;		sPlacementManager->GetWind(&spWind);
	sp<WindData> spGimmickWind;	sPlacementManager->GetGimmickWind(&spGimmickWind);
	sp<GimmickLine> spLine;		sPlacementManager->GetGimmickLine(&spLine);

//BOX�n�̓ǂݍ���
	for( int i=0; i<spBox->Num; i++ )
	{
		spBox->spPos[i].x *= -1.f;
		swap( spBox->spAngle[i].x, spBox->spAngle[i].y );

		GimmickInfoBase* info = NULL;
		switch( spBox->spType[i] )
		{
		//�������n
		case PLACEMENT_BOX://�e�X�g�p
		case PLACEMENT_DRUM:
		case PLACEMENT_GASOLINE:
		case PLACEMENT_GarbageBag:
		case PLACEMENT_WoodenBox:
		case PLACEMENT_CARDBOARD:
			{
				GExplosionInfo* expInfo = new GExplosionInfo();
				expInfo->convert(spBox.GetPtr(), i);
				info = expInfo;
				break;
			}
		case PLACEMENT_GOAL:
			{
				GGoalInfo* goalInfo = new GGoalInfo();
				goalInfo->convert(spBox.GetPtr(), i);

				info = goalInfo;
				break;
			}
		//��@
		case PLACEMENT_FAN:
			{
				LOGE(TAG, "��@�̕ϊ�����������Ă� GimmickInfoManager::loadMqo()\n");
				//info = ;
				break;
			}
		//�낤����
		case PLACEMENT_CANDLE:
			{
				//LOGE(TAG, "�낤�����̕ϊ�����������Ă� GimmickInfoManager::loadMqo()\n");
				GCandleInfo *set = new GCandleInfo();
				set->convert(spBox.GetPtr(), i);

				info = set;
				break;
			}
		//2D
		case PLACEMENT_2D:
			{
				G2DInfo* set = new G2DInfo();
				set->convert(spBox.GetPtr(), i);
				info = set;
				break;
			}
		case PLACEMENT_RESET_CANDLE:
			{
				GResetCandleInfo* set = new GResetCandleInfo();
				set->convert(spBox.GetPtr(), i);
				info = set;
			//���Z�b�g�낤�����̏ꍇ��GCandleCheckerInfo���Z�b�g�ō��
				GCandleCheckerInfo* checkerInfo = new GCandleCheckerInfo();
				std::string checkerName = spBox->spStr[i]+"_CHECKER";
				checkerInfo->setNameAndType(checkerName, eGIMMICK_CANDLE_CHECKER);
				checkerInfo->correctAnswer = GCandleCheckerInfo::NON_DATA;
				checkerInfo->checkOn.push_back(set->name);
				this->mData.push_back(checkerInfo);
				break;
			}
		//�G���[
		default:
			{
				LOGE(TAG, "���m��ʎ�ނ�ϊ����悤�Ƃ��܂��� GimmickInfoManager::loadMqo()\n");
				break;
			}
		}
		if( info )
			this->mData.push_back(info);
	}
//���̓ǂݍ���
	for( int i=0; i<spWind->Num; i++ )
	{
		spWind->spPos[i].x *= -1;
		spWind->spWindVec[i].x *= -1;
		swap( spWind->spAngle[i].x, spWind->spAngle[i].y );
		GWindInfo* info = new GWindInfo();
		info->convert(spWind.GetPtr(), i);

		this->mData.push_back(info);
	}
//�M�~�b�N���̓ǂݍ���
	for( int i=0; i<spGimmickWind->Num; i++ )
	{
		spGimmickWind->spPos[i].x *= -1;
		spGimmickWind->spWindVec[i].x *= -1;
		swap( spGimmickWind->spAngle[i].x, spGimmickWind->spAngle[i].y );
		GWindInfo* info = new GWindInfo();
		info->convert(spGimmickWind.GetPtr(), i);

		this->mData.push_back(info);
	}
//���ΐ��̓ǂݍ���
	for( int i=0; i<spLine->Num; i++ )
	{
		spLine->spStart[i].x *= -1;
		spLine->spEnd[i].x *= -1;
		GLineInfo* lineInfo = new GLineInfo();
		lineInfo->convert( spLine.GetPtr(), i );
		std::string spName = lineInfo->name+"Start";
		std::string epName = lineInfo->name+"End";
		lineInfo->checkOn.push_back( spName );
		lineInfo->checkOn.push_back( epName );
		this->mData.push_back(lineInfo);

	//���ΐ��̗��[�̒ǉ�
		//LOGE(TAG,"���ΐ��̗��[������Ă� GimmickInfoManager::loadMqo()\n");
		GFusePointInfo* sp = new GFusePointInfo();
		setFusePoint(sp, spName, lineInfo->start);
		this->mData.push_back(sp);
		GFusePointInfo* ep = new GFusePointInfo();
		setFusePoint(ep, epName, lineInfo->end);
		this->mData.push_back(ep);
	}
	sPlacementManager->Destroy();
	PlacementManager_Singleton::deleteInstance();
}

void GimmickInfoManager::setFusePoint(GGimmickInfo* out, std::string& name, const klib::math::Vector3& pos)
{
	//std::cout << "���ΐ��̗��[�̖��O��ݒ肵�Ă�" << std::endl;
	out->name = name;
	out->type = eGIMMICK_FUSE_POINT;
	out->pos = pos;
	out->scale.x = out->scale.y = out->scale.z = 0.5f;
}

//=========================================================
//
//		gi�t�@�C���̓ǂݍ���
//
//=========================================================
void GimmickInfoManager::load(const char* giFilePath)
{
	clear();
	textLoader loader(giFilePath);

	if( !loader.Search("count") ){ LOGE(TAG, "not find \"count\" ..."); return ; }
	int count = loader.LoadInt();

	char buf[128];
	this->mData.resize(count);
	for( int i=0; i<count; i++ )
	{
		GimmickInfoBase** info = &this->mData[i];
		
		if( !loader.Search("index") ){ LOGE(TAG, "not find index =%d", i); return;}
		int index = loader.LoadInt();
		loader.In();

		std::string name;
		loader.LoadString(buf);
		if( strcmp( "name", buf ) == 0 ){
			loader.LoadString(buf);
			name = buf;
		}else{
			LOGE(TAG, "GimmickInfoManager::load() : miss data... [no name]\n");
			return ;
		}
		loader.LoadString(buf);
		if( strcmp( "type", buf ) == 0 ){
			int type = loader.LoadInt();
			setInfo(loader, info, type, name);

		}else{
			LOGE(TAG, "GimmickInfoManager::load : miss data... [no type]\n");
			return ;
		}
	}
}

void GimmickInfoManager::setInfo(textLoader& loader, GimmickInfoBase** out, int type, std::string& name)
{
	switch( type )
	{
	case eGIMMICK_DRUM:			//�h������
	case eGIMMICK_GASOLINE:		//�K�\����
	case eGIMMICK_GARBAGE_BAG:	//�S�~��
	case eGIMMICK_WOOD_BOX:		//�ؔ�
	case eGIMMICK_CARDBOARD:	//�_���{�[��
	case eGIMMICK_FAN:			//��@
	case eGIMMICK_CANDLE:		//�낤����
	case eGIMMICK_FUSE_POINT:	//���ΐ��̗��[
	case eGIMMICK_2D:			//2D�`��
	case eGIMMICK_GOAL:
	case eGIMMICK_RESET_CANDLE:
		setGimmickInfo(loader, out, type, name);
		break;
	case eGIMMICK_CANDLE_CHECKER:
		{
			GCandleCheckerInfo* info = new GCandleCheckerInfo();
			info->setNameAndType(name, type);
			info->load(loader);
			*out = info;
			break;
		}
	case eGIMMICK_FUSE:			//���ΐ�
		setLineInfo(loader, out, type, name);
		break;
	case eGIMMICK_WIND:			//��
		setWindInfo(loader, out, type, name);
		break;
	default:
		LOGE(TAG,"GimmickInfoManager::setInfo : unknown type set!! type=%d | name=\"%s\"\n", type, name.c_str());
		break;
	}
}

void GimmickInfoManager::setGimmickInfo(textLoader& loader, GimmickInfoBase** out, int type, std::string& name)
{
	//LOGE(TAG,"BOX�n�̍쐬�����������Ă� GimmickInfoManager::setGimmickInfo()\n");
	GGimmickInfo* set = 0;
	switch( type )
	{
	//������
	case eGIMMICK_DRUM:			//�h������
	case eGIMMICK_GASOLINE:		//�K�\����
	case eGIMMICK_GARBAGE_BAG:	//�S�~��
	case eGIMMICK_WOOD_BOX:		//�ؔ�
	case eGIMMICK_CARDBOARD:	//�_���{�[��
		{
			GExplosionInfo* info = new GExplosionInfo();
			info->setNameAndType(name, type);

			info->load(loader);
			
			set = info;
			break;
		}
	case eGIMMICK_FAN:			//��@
		{
			LOGE(TAG, "GimmickInfoManager::setGimmickInfo() : Please create a fan!\n");
			//set = ;
			break;
		}
	case eGIMMICK_CANDLE:		//�낤����
		{
			GCandleInfo* info = new GCandleInfo();
			info->setNameAndType(name, type);
			info->load(loader);
			set = info;
			break;
		}
	case eGIMMICK_RESET_CANDLE:
		{
			GResetCandleInfo* info = new GResetCandleInfo();
			info->setNameAndType(name, type);
			info->load(loader);
			set = info;
			break;
		}
	case eGIMMICK_FUSE_POINT:	//���ΐ��̗��[
		{
			GFusePointInfo* info = new GFusePointInfo();
			info->setNameAndType(name, type);
			info->load(loader);

			set = info;
			break;
		}
	case eGIMMICK_2D:			//2D�`��
		{
			G2DInfo* info = new G2DInfo();
			info->setNameAndType(name, type);
			info->load(loader);
			set = info;
			break;
		}
	case eGIMMICK_GOAL:
		{
			GGoalInfo* info = new GGoalInfo();
			info->setNameAndType(name, type);
			info->load(loader);

			set = info;
			break;
		}
	default:
		LOGE(TAG, "GimmickInfoManager::setGimmickInfo() : Try creating a gimmick type of unknown...\n");
		break;
	}
	*out = set;
}

void GimmickInfoManager::setWindInfo(textLoader& loader, GimmickInfoBase** out, int type, std::string& name)
{
	GWindInfo* set = new GWindInfo();
	set->name = name;
	set->type = (GIMMICK_TYPE)type;

	set->load(loader);

	*out = set;
}

void GimmickInfoManager::setLineInfo(textLoader& loader, GimmickInfoBase** out, int type, std::string& name)
{
	GLineInfo* set = new GLineInfo();
	set->name = name;
	set->type = (GIMMICK_TYPE)type;

	set->load(loader);

	*out = set;
}

//=========================================================
//
//		�f�[�^��������
//
//=========================================================
void GimmickInfoManager::write(const char* giFilePath)
{
	textWriter w;
	w.open(giFilePath);

	w.write("count ").write((int)this->mData.size()).br();

	std::vector<GimmickInfoBase*>::iterator it = this->mData.begin();
	for( int i=0; it!= this->mData.end(); it++, i++ )
	{
		w.write("index ").write(i).write(' ').write("{");
		w.nestIn();
		w.br();
		if( *it )
			(*it)->forFile(w);
		w.nestOut();

		w.write("}").br();
	}
}
