#include "Placement.h"

#include "utility\utility.h"

using namespace PlacementLib;
PlacementManager_Singleton* PlacementManager_Singleton::singleton = NULL;

void PlacementManager::Clear()
{
	m_spPlayerData.SetPtr(new Data("PLAYER"));
	m_spBoxData.SetPtr(new Data("BOX"));
	m_spWindData.SetPtr(new Data("WIND"));
	m_spGimmickWindData.SetPtr(new Data("GWIND"));
	m_spGimmickLineData.SetPtr(new Data("GimmickLine"));
}
void PlacementManager::Destroy()
{

}
void PlacementManager::Search_Num(const char* File)
{
	tl.SetPtr(new textLoader(File));
	sp<char> name(new char[STR_LENGTH],true);
	sp<char> load(new char[STR_LENGTH],true);
	
	for(;;)
	{
		if( false == tl->Search("Object") ){ break; }

		ZeroMemory(load.GetPtr(),STR_LENGTH);
		ZeroMemory(name.GetPtr(),STR_LENGTH);
		tl->LoadName(load.GetPtr());
		
		Setting_ObjectName(name,load);
		Add_ObjectNum(name,load);
	}

	tl.Clear();
}
void PlacementManager::Setting_ObjectName(sp<char> name,sp<char> load)
{
	int n = 0;
	for(n=0;load[n]!='\0';n++){
		if(load[n] == '_'){ break; }
		name[n] = load[n];
	}
}
void PlacementManager::Setting_CreateObjectName(sp<char> name,sp<char> load)
{
	int n = 0;
	for(n=0;load[n]!='\0';n++){
		if(load[n] == '_'){ break; }
		name[n] = load[n];
	}
	
	if(strcmp(name.GetPtr(),"BOX")!=0)return;

	char box[STR_LENGTH];
	char fullName[STR_LENGTH];
	ZeroMemory(box,STR_LENGTH);
	ZeroMemory(fullName,STR_LENGTH);

	bool fBox = false;

	n++;
	int i=0;
	for(;load[n]!='\0';n++){
		if(load[n] == '_'){ fBox=true; }
		if( !fBox )
		{
			box[i] = load[n];
		}
		fullName[i] = load[n];
		i++;
	}

	m_spBoxData->spStr[m_spBoxData->NowNum] = fullName;

	struct TypeTable{
		const char*		keyName;
		ePLACEMENT_TYPE type;
	};
	static TypeTable table[]={
		{"GARBAGEBAG",	PLACEMENT_GarbageBag},
		{"DRUM",		PLACEMENT_DRUM},
		{"GASOLINE",	PLACEMENT_GASOLINE},
		{"WOODBOX",		PLACEMENT_WoodenBox},
		{"CARDBOARD",	PLACEMENT_CARDBOARD},
		{"FAN",			PLACEMENT_FAN},
		{"CANDLE",		PLACEMENT_CANDLE},
		{"2D",			PLACEMENT_2D},
		{"GOAL",		PLACEMENT_GOAL},
		{"RESETCANDLE",	PLACEMENT_RESET_CANDLE},
		{"ITTOKAN",		PLACEMENT_ITTOKAN},
		{"",			PLACEMENT_WoodenBox},//不明データ　一番最後にしてください
	};
	static int tableNum = sizeof( table ) / sizeof(table[0]);
	int tableIndex;
	for( tableIndex=0; tableIndex<tableNum-1; tableIndex++ )
	{
		if(strcmp(box,table[tableIndex].keyName)==0)
		{
			m_spBoxData->spType[m_spBoxData->NowNum] = table[tableIndex].type;
			break;
		}
	}
//見つからなければ木箱として扱う
	if( tableIndex == tableNum-1 ){
		LOGE("PlacementManager", "PlacementManager:: Setting_CreateObjectName : unknwon box find...　name=%s\n", load.GetPtr());
		m_spBoxData->spType[m_spBoxData->NowNum] = table[tableIndex].type;
	}
}

void PlacementManager::Add_ObjectNum(sp<char> name,sp<char> load)
{
	if(strcmp(m_spPlayerData->Name,name.GetPtr())==0){m_spPlayerData->Num++;}
	else if(strcmp(m_spBoxData->Name,name.GetPtr())==0){m_spBoxData->Num++;}
	else if(strcmp(m_spWindData->Name,name.GetPtr())==0){m_spWindData->Num++;}
	else if(strcmp(m_spGimmickWindData->Name,name.GetPtr())==0){m_spGimmickWindData->Num++;}
	else if(strcmp(m_spGimmickLineData->Name,name.GetPtr())==0){m_spGimmickLineData->Num++;}
}
void PlacementManager::Load(char* Filename)
{
	Search_Num(Filename);
	Create_AllData();
	Setting_AllData(Filename);
}
void PlacementManager::Setting_AllData(const char* File)
{
	tl.SetPtr(new textLoader(File));
	sp<char> name(new char[STR_LENGTH],true);
	sp<char> load(new char[STR_LENGTH],true);

	for(;;)
	{
		if( false == tl->Search("Object") ){ break; }

		ZeroMemory(load.GetPtr(),STR_LENGTH);
		ZeroMemory(name.GetPtr(),STR_LENGTH);
		tl->LoadName(load.GetPtr());
		
		Setting_CreateObjectName(name,load);
		Setting_Data(name,load);
	}

	tl.Clear();
}
void PlacementManager::Setting_Data(sp<char> name,sp<char> load)
{
	char fullName[STR_LENGTH];
	ZeroMemory(fullName,STR_LENGTH);

	//	Player
	if(strcmp(m_spPlayerData->Name,name.GetPtr())==0)
	{
		for(int n=0;load[n]!='\0';n++){fullName[n] = load[n];}
		m_spPlayerData->spStr[m_spPlayerData->NowNum] = fullName;
		Setting_Data(m_spPlayerData);
	//	Box
	}else if(strcmp(m_spBoxData->Name,name.GetPtr())==0)
	{
		Setting_Data_ScaleHalf(m_spBoxData);
	//	Wind
	}else if(strcmp(m_spWindData->Name,name.GetPtr())==0)
	{
		for(int n=0;load[n]!='\0';n++){fullName[n] = load[n];}
		m_spWindData->spStr[m_spWindData->NowNum] = fullName;
		Setting_Data_ScaleHalf(m_spWindData);
	}
	//	GWind
	else if(strcmp(m_spGimmickWindData->Name,name.GetPtr())==0)
	{
		for(int n=0;load[n]!='\0';n++){fullName[n] = load[n];}
		m_spGimmickWindData->spStr[m_spGimmickWindData->NowNum] = fullName;
		Setting_Data(m_spGimmickWindData);
	}
	//	GLine
	else if(strcmp(m_spGimmickLineData->Name,name.GetPtr())==0)
	{
		for(int n=0;load[n]!='\0';n++){fullName[n] = load[n];}
		m_spGimmickLineData->spStr[m_spGimmickLineData->NowNum] = fullName;
		Setting_Data_GimmickLine(m_spGimmickLineData);
	}
}


void PlacementManager::Setting_Data(sp<Data> spData)
{
	int n = spData->NowNum;
	tl->Search("scale");
	spData->spScale[n].x = tl->LoadFloat();
	spData->spScale[n].y = tl->LoadFloat();
	spData->spScale[n].z = tl->LoadFloat();
	tl->Search("rotation");
	spData->spAngle[n].x = tl->LoadFloat();
	spData->spAngle[n].y = tl->LoadFloat();
	spData->spAngle[n].z = tl->LoadFloat();
	tl->Search("translation");
	spData->spPos[n].x = tl->LoadFloat();
	spData->spPos[n].y = tl->LoadFloat();
	spData->spPos[n].z = tl->LoadFloat();

	Setting_WindData(spData);

	spData->NowNum++;
}
void PlacementManager::Setting_Data_GimmickLine(sp<Data> spData)
{
	int n = spData->NowNum;

	Vector3 s[2];//start
	Vector3 e[2];//end
	Vector3 m;

	tl->Search("vertex");
	tl->In();

	s[0].x=tl->LoadFloat();s[0].y=tl->LoadFloat();s[0].z=tl->LoadFloat();
	s[1].x=tl->LoadFloat();s[1].y=tl->LoadFloat();s[1].z=tl->LoadFloat();

	e[0].x=tl->LoadFloat();e[0].y=tl->LoadFloat();e[0].z=tl->LoadFloat();
	e[1].x=tl->LoadFloat();e[1].y=tl->LoadFloat();e[1].z=tl->LoadFloat();

	//	Start
	m = s[1] - s[0];//middle
	m = m / 2;
	spData->spStart[n] = s[0] + m;

	//	End
	m = e[1] - e[0];//middle
	m = m / 2;
	spData->spEnd[n] = e[0] + m;

	spData->NowNum++;
}
void PlacementManager::Setting_Data_ScaleHalf(sp<Data> spData)
{
	int n = spData->NowNum;
	tl->Search("scale");
	spData->spScale[n].x = tl->LoadFloat() / 2;
	spData->spScale[n].y = tl->LoadFloat() / 2;
	spData->spScale[n].z = tl->LoadFloat() / 2;
	tl->Search("rotation");
	spData->spAngle[n].x = tl->LoadFloat();
	spData->spAngle[n].y = tl->LoadFloat();
	spData->spAngle[n].z = tl->LoadFloat();
	tl->Search("translation");
	spData->spPos[n].x = tl->LoadFloat();
	spData->spPos[n].y = tl->LoadFloat();
	spData->spPos[n].z = tl->LoadFloat();

	Setting_WindData(spData);

	spData->NowNum++;
}
void PlacementManager::Setting_WindData(sp<Data> spData)
{
	if( strcmp(spData->Name,"WIND") == 0 ||
		  strcmp(spData->Name,"GWIND") == 0 )
	{
		float pow = 0;
		Vector3 target;
		Vector3 pos;
		Vector3 windVec;

		tl->Search("Object");
		tl->In();
		tl->Search("facet");
		pow = tl->LoadFloat();
		
		tl->Search("vertex");
		tl->In();

		for(int n=0;n<24;n++)
		{
			tl->LoadFloat();
		}
		target.x=tl->LoadFloat();
		target.y=tl->LoadFloat();
		target.z=tl->LoadFloat();
		pos.x=tl->LoadFloat();
		pos.y=tl->LoadFloat();
		pos.z=tl->LoadFloat();
		
		windVec=target-pos;
		windVec.normalize();
		windVec.x=windVec.x*pow;
		windVec.y=windVec.y*pow;
		windVec.z=windVec.z*pow;

		spData->spWindVec[spData->NowNum] = windVec;
	}
}
void PlacementManager::Create_AllData()
{
	//	Player
	m_spPlayerData->spPos.SetPtr(new Vector3[m_spPlayerData->Num],true);
	m_spPlayerData->spScale.SetPtr(new Vector3[m_spPlayerData->Num],true);
	m_spPlayerData->spAngle.SetPtr(new Vector3[m_spPlayerData->Num],true);
	m_spPlayerData->spStr.SetPtr(new std::string[m_spPlayerData->Num],true);
	//	Box
	m_spBoxData->spPos.SetPtr(new Vector3[m_spBoxData->Num],true);
	m_spBoxData->spScale.SetPtr(new Vector3[m_spBoxData->Num],true);
	m_spBoxData->spAngle.SetPtr(new Vector3[m_spBoxData->Num],true);
	m_spBoxData->spType.SetPtr(new ePLACEMENT_TYPE[m_spBoxData->Num],true);
	m_spBoxData->spStr.SetPtr(new std::string[m_spBoxData->Num],true);
	//	Wind
	m_spWindData->spPos.SetPtr(new Vector3[m_spWindData->Num],true);
	m_spWindData->spScale.SetPtr(new Vector3[m_spWindData->Num],true);
	m_spWindData->spAngle.SetPtr(new Vector3[m_spWindData->Num],true);
	m_spWindData->spWindVec.SetPtr(new Vector3[m_spWindData->Num],true);
	m_spWindData->spStr.SetPtr(new std::string[m_spWindData->Num],true);
	//	GimmickWind
	m_spGimmickWindData->spPos.SetPtr(new Vector3[m_spGimmickWindData->Num],true);
	m_spGimmickWindData->spScale.SetPtr(new Vector3[m_spGimmickWindData->Num],true);
	m_spGimmickWindData->spAngle.SetPtr(new Vector3[m_spGimmickWindData->Num],true);
	m_spGimmickWindData->spWindVec.SetPtr(new Vector3[m_spGimmickWindData->Num],true);
	m_spGimmickWindData->spStr.SetPtr(new std::string[m_spGimmickWindData->Num],true);
	//	GimmickLine
	m_spGimmickLineData->spPos.SetPtr(new Vector3[m_spGimmickLineData->Num],true);
	m_spGimmickLineData->spStart.SetPtr(new Vector3[m_spGimmickLineData->Num],true);
	m_spGimmickLineData->spEnd.SetPtr(new Vector3[m_spGimmickLineData->Num],true);
	m_spGimmickLineData->spStr.SetPtr(new std::string[m_spGimmickLineData->Num],true);
}
void PlacementManager::Delete()
{
	m_spPlayerData.Clear();
	m_spBoxData.Clear();
	m_spWindData.Clear();
}
void PlacementManager::GetBox(sp<PlacementData>* spData)
{
	Create_Data(spData,m_spBoxData);
}
void PlacementManager::GetPlayer(sp<PlacementData>* spData)
{
	Create_Data(spData,m_spPlayerData);
}
std::string PlacementManager::GetPlayerStr()
{
	return m_spPlayerData->spStr[0];
}
Vector3 PlacementManager::GetPlayerPos()
{
	return m_spPlayerData->spPos[0];
}
Vector3 PlacementManager::GetPlayerScale()
{
	return m_spPlayerData->spScale[0];
}
Vector3 PlacementManager::GetPlayerAngle()
{
	return m_spPlayerData->spAngle[0];
}
void PlacementManager::GetWind(sp<WindData>* spData)
{
	int n = m_spWindData->Num;

	(*spData).SetPtr(new WindData);
	(*spData)->Num = n;
	(*spData)->spPos.SetPtr(new Vector3[n],true);
	(*spData)->spScale.SetPtr(new Vector3[n],true);
	(*spData)->spAngle.SetPtr(new Vector3[n],true);
	(*spData)->spWindVec.SetPtr(new Vector3[n],true);
	(*spData)->spStr.SetPtr(new std::string[n],true);
	
	for(int n=0;n<m_spWindData->Num;n++)
	{
		(*spData)->spPos[n] = m_spWindData->spPos[n];
		(*spData)->spScale[n] = m_spWindData->spScale[n];
		(*spData)->spAngle[n] = m_spWindData->spAngle[n];
		(*spData)->spWindVec[n] = m_spWindData->spWindVec[n];
		(*spData)->spStr[n] = m_spWindData->spStr[n];
	}
}
void PlacementManager::GetGimmickWind(sp<WindData>* spData)
{
	int n = m_spGimmickWindData->Num;

	(*spData).SetPtr(new WindData);
	(*spData)->Num = n;
	(*spData)->spPos.SetPtr(new Vector3[n],true);
	(*spData)->spScale.SetPtr(new Vector3[n],true);
	(*spData)->spAngle.SetPtr(new Vector3[n],true);
	(*spData)->spWindVec.SetPtr(new Vector3[n],true);
	(*spData)->spStr.SetPtr(new std::string[n],true);
	
	for(int n=0;n<m_spGimmickWindData->Num;n++)
	{
		(*spData)->spPos[n] = m_spGimmickWindData->spPos[n];
		(*spData)->spScale[n] = m_spGimmickWindData->spScale[n];
		(*spData)->spAngle[n] = m_spGimmickWindData->spAngle[n];
		(*spData)->spWindVec[n] = m_spGimmickWindData->spWindVec[n];
		(*spData)->spStr[n] = m_spGimmickWindData->spStr[n];
	}
}
void PlacementManager::GetGimmickLine(sp<GimmickLine>* spData)
{
	int n = m_spGimmickWindData->Num;

	(*spData).SetPtr(new GimmickLine);
	(*spData)->Num = n;
	(*spData)->spStart.SetPtr(new Vector3[n],true);
	(*spData)->spEnd.SetPtr(new Vector3[n],true);
	(*spData)->spStr.SetPtr(new std::string[n],true);
	
	for(int n=0;n<m_spGimmickWindData->Num;n++)
	{
		(*spData)->spStart[n] = m_spGimmickLineData->spStart[n];
		(*spData)->spEnd[n] = m_spGimmickLineData->spEnd[n];
		(*spData)->spStr[n] = m_spGimmickLineData->spStr[n];
	}
}
void PlacementManager::Create_Data(sp<PlacementData>* spUserData,sp<Data> spData)
{
	int n = spData->Num;

	(*spUserData).SetPtr(new PlacementData);
	(*spUserData)->Num = n;
	(*spUserData)->spPos.SetPtr(new Vector3[n],true);
	(*spUserData)->spScale.SetPtr(new Vector3[n],true);
	(*spUserData)->spAngle.SetPtr(new Vector3[n],true);
	(*spUserData)->spStr.SetPtr(new std::string[n],true);
	(*spUserData)->spType.SetPtr(new ePLACEMENT_TYPE[n], true);

	for(int n=0;n<spData->Num;n++)
	{
		(*spUserData)->spPos[n] = spData->spPos[n];
		(*spUserData)->spScale[n] = spData->spScale[n];
		(*spUserData)->spAngle[n] = spData->spAngle[n];
		(*spUserData)->spStr[n] = spData->spStr[n];
		(*spUserData)->spType[n] = spData->spType[n];
	}
}
