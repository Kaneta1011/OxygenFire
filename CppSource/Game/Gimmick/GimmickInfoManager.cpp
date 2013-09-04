#include "GimmickInfoManager.h"

#include "PlacementLib/Placement.h"
#include "utility\textWriter.h"

#include "utility\utility.h"

using namespace rlib;

static const char* TAG = "GimmickInfoManager";

GimmickInfoManager::GimmickInfoManager()
{
}

GimmickInfoManager::~GimmickInfoManager()
{
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
		GGimmickInfo* info = new GGimmickInfo;
		info->convert( spBox.GetPtr(), i );

		this->mData.push_back(info);
	}
//���̓ǂݍ���
	for( int i=0; i<spWind->Num; i++ )
	{
		GWindInfo* info = new GWindInfo();
		info->convert(spWind.GetPtr(), i);

		this->mData.push_back(info);
	}
//�M�~�b�N���̓ǂݍ���
	for( int i=0; i<spGimmickWind->Num; i++ )
	{
		GWindInfo* info = new GWindInfo();
		info->convert(spGimmickWind.GetPtr(), i);

		this->mData.push_back(info);
	}
//���ΐ��̓ǂݍ���
	for( int i=0; i<spLine->Num; i++ )
	{
		GLineInfo* lineInfo = new GLineInfo();
		lineInfo->convert( spLine.GetPtr(), i );
		std::string spName = lineInfo->name+"Start";
		std::string epName = lineInfo->name+"End";
		lineInfo->checkOn.push_back( spName );
		lineInfo->checkOn.push_back( epName );
		this->mData.push_back(lineInfo);

	//���ΐ��̗��[�̒ǉ�
		GGimmickInfo* sp = new GGimmickInfo();
		setFusePoint(sp, spName, lineInfo->start);
		this->mData.push_back(sp);
		GGimmickInfo* ep = new GGimmickInfo();
		setFusePoint(ep, epName, lineInfo->start);
		this->mData.push_back(ep);
	}
	sPlacementManager->Destroy();
}

void GimmickInfoManager::setFusePoint(GGimmickInfo* out, std::string& name, const klib::math::Vector3& pos)
{
	//std::cout << "���ΐ��̗��[�̖��O��ݒ肵�Ă�" << std::endl;
	out->name = name;
	out->type = eGIMMICK_FUSE_POINT;
	out->pos = pos;
	out->scale.x = out->scale.y = out->scale.z = 0.5f;
}

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
			LOGE(TAG, "miss data line... [no name]");
			return ;
		}
		loader.LoadString(buf);
		if( strcmp( "type", buf ) == 0 ){
			int type = loader.LoadInt();
			setInfo(loader, info, type, name);

		}else{
			LOGE(TAG, "miss data line... [no type]");
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
	case eGIMMICK_CARDBOARD:		//�_���{�[��
	case eGIMMICK_FAN:			//��@
	case eGIMMICK_CANDLE:		//�낤����
	case eGIMMICK_FUSE_POINT:	//���ΐ��̗��[
	case eGIMMICK_2D:			//2D�`��
		setGimmickInfo(loader, out, type, name);
		break;
	case eGIMMICK_FUSE:			//���ΐ�
		setLineInfo(loader, out, type, name);
		break;
	case eGIMMICK_WIND:			//��
		setWindInfo(loader, out, type, name);
		break;
	default:
		LOGE(TAG,"unknown type set!! type=%d | name=\"%s\"\n", type, name.c_str());
		break;
	}
}

void GimmickInfoManager::setGimmickInfo(textLoader& loader, GimmickInfoBase** out, int type, std::string& name)
{
	GGimmickInfo* set = new GGimmickInfo();
	set->name = name;
	set->type = (GIMMICK_TYPE)type;

	set->load(loader);

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
		(*it)->forFile(w);
		w.nestOut();

		w.write("}").br();
	}
}
