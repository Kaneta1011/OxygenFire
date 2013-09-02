#include "PlacementLib/Placement.h"
using namespace PlacementLib;
PlacementManager_Singleton* PlacementManager_Singleton::singleton = NULL;

void PlacementManager::Clear()
{
	m_spPlayerData.SetPtr(new Data("PLAYER"));
	m_spBoxData.SetPtr(new Data("BOX"));
	m_spWindData.SetPtr(new Data("WIND"));
}
void PlacementManager::Destroy()
{

}
void PlacementManager::Search_Num(char* File)
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
	for(int n=0;load[n]!='\0';n++){
		if(load[n] == '_'){ break; }
		name[n] = load[n];
	}
}
void PlacementManager::Add_ObjectNum(sp<char> name,sp<char> load)
{
	if(strcmp(m_spPlayerData->Name,name.GetPtr())==0){m_spPlayerData->Num++;}
	else if(strcmp(m_spBoxData->Name,name.GetPtr())==0){m_spBoxData->Num++;}
	else if(strcmp(m_spWindData->Name,name.GetPtr())==0){m_spWindData->Num++;}
}
void PlacementManager::Load(char* Filename)
{
	Search_Num(Filename);
	Create_AllData();
	Setting_AllData(Filename);
}
void PlacementManager::Setting_AllData(char* File)
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
		Setting_Data(name,load);
	}

	tl.Clear();
}
void PlacementManager::Setting_Data(sp<char> name,sp<char> load)
{
	if(strcmp(m_spPlayerData->Name,name.GetPtr())==0)
	{
		Setting_Data(m_spPlayerData);
	}else if(strcmp(m_spBoxData->Name,name.GetPtr())==0)
	{
		Setting_Data(m_spBoxData);
	}else if(strcmp(m_spWindData->Name,name.GetPtr())==0)
	{
		Setting_Data(m_spWindData);
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
void PlacementManager::Setting_WindData(sp<Data> spData)
{
	if( strcmp(spData->Name,"WIND") == 0 )
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
	//	Box
	m_spBoxData->spPos.SetPtr(new Vector3[m_spBoxData->Num],true);
	m_spBoxData->spScale.SetPtr(new Vector3[m_spBoxData->Num],true);
	m_spBoxData->spAngle.SetPtr(new Vector3[m_spBoxData->Num],true);
	//	Wind
	m_spWindData->spPos.SetPtr(new Vector3[m_spWindData->Num],true);
	m_spWindData->spScale.SetPtr(new Vector3[m_spWindData->Num],true);
	m_spWindData->spAngle.SetPtr(new Vector3[m_spWindData->Num],true);
	m_spWindData->spWindVec.SetPtr(new Vector3[m_spWindData->Num],true);
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
void PlacementManager::GetWind(sp<WindData>* spData)
{
	int n = m_spWindData->Num;

	(*spData).SetPtr(new WindData);
	(*spData)->Num = n;
	(*spData)->spPos.SetPtr(new Vector3[n],true);
	(*spData)->spScale.SetPtr(new Vector3[n],true);
	(*spData)->spAngle.SetPtr(new Vector3[n],true);
	(*spData)->spWindVec.SetPtr(new Vector3[n],true);
	
	for(int n=0;n<m_spWindData->Num;n++)
	{
		(*spData)->spPos[n] = m_spWindData->spPos[n];
		(*spData)->spScale[n] = m_spWindData->spScale[n];
		(*spData)->spAngle[n] = m_spWindData->spAngle[n];
		(*spData)->spWindVec[n] = m_spWindData->spWindVec[n];
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
	
	for(int n=0;n<spData->Num;n++)
	{
		(*spUserData)->spPos[n] = spData->spPos[n];
		(*spUserData)->spScale[n] = spData->spScale[n];
		(*spUserData)->spAngle[n] = spData->spAngle[n];
	}
}
