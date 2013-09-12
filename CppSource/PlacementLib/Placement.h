#ifndef __tePlacement_H__
#define __tePlacement_H__
#include	"math\\kmathf.h"
#include	"utility/textLoader.h"
#include	<iostream>
#include	"StandardLib/SmartPointer.h"
#include	<string>

namespace PlacementLib{
using namespace klib::math;
const static int STR_LENGTH = 128;

//==========================================================
//	�����̍\���̂��󂯎M�ɂ��ăf�[�^��������Ċe��ݒ�
//	*��{�I�ɏ�c�N�̂ݎg�p���邱�ƂɂȂ�
enum ePLACEMENT_TYPE
{
	PLACEMENT_BOX,			//	��(�����e�X�g�p)
	PLACEMENT_GarbageBag,	//	�S�~��
	PLACEMENT_DRUM,			//	�h����
	PLACEMENT_GASOLINE,		//	�K�\����
	PLACEMENT_WoodenBox,	//	�ؔ�
	PLACEMENT_CARDBOARD,	//	�_���{�[��
	PLACEMENT_FAN,			//	��@
	PLACEMENT_CANDLE,		//	�낤����
	PLACEMENT_2D,			//	2D�摜
	PLACEMENT_GOAL,			//	�S�[��
	PLACEMENT_RESET_CANDLE,	//�@���Z�b�g�낤����
	PLACEMENT_ITTOKAN,		//	�����Ƃ���
	PLACEMENT_TYPE_NUM,
};

struct PlacementData
{
	int Num;
	sp<std::string> spStr;
	sp<ePLACEMENT_TYPE> spType;
	sp<Vector3> spPos;
	sp<Vector3> spScale;
	sp<Vector3> spAngle;
};
struct GimmickLine
{
	int Num;
	sp<std::string> spStr;
	sp<Vector3> spStart;
	sp<Vector3> spEnd;
};
struct WindData
{
	int Num;
	sp<std::string> spStr;
	sp<Vector3> spPos;
	sp<Vector3> spScale;
	sp<Vector3> spAngle;
	sp<Vector3> spWindVec;	//1�b�Ԃɐi�ދ����p�̃x�N�g��
};
//==========================================================
struct Data;
/* -------------------------------------------------------
	class PlacementManager

	�P�DLoad(char* MQO_Filename);
	�֐��������ăI�u�W�F�N�g�̑S�����擾

	�Q�D��{�z�u��(Character�┠�Ȃ�)
	int Num = �I�u�W�F�N�g�̍ő吔
	Vector3 spPos = ���W�̔z��
	Vector3 spScale = �T�C�Y�̔z��
	Vector3 spAngle = �A���O���̔z��
	���̏���GetBox(sp<PlacementData>* spData);
	�֐��Ŏ擾�ł���̂ł��Ƃ͎󂯎��肪
	Num �Ń��[�v�܂킵�Ċe��I�u�W�F�N�g��
	���W�A�X�P�[���A�A���O�����擾���Ă�������

	�R�DDelete();
	�X�e�[�W�̏I���ȂǃI�u�W�F�N�g�̏�񂪂���Ȃ��Ȃ�Ƃ�
	���̊֐������ł��������B�����炵���X�e�[�W�ŐV��������
	�Ăэ��ނ悤�ɂ��Ă�������

 ------------------------------------------------------- */
class PlacementManager
{
public:
	//	MQO�̔z�u�f�[�^��S�Ď�荞��
	void Load(char* MQO_Filename);
	//	�f�[�^�̉��
	void Delete();
	//	�f�[�^��]��
	void GetPlayer(sp<PlacementData>* spData);	//����g���Ɖ�������
	std::string GetPlayerStr();

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	�E��c�N�p
	��{�I�g�����͐����������ǂ킩��Ȃ��Ƃ��͍l�����ɂ����ɕ�����!

	*/
	void GetBox(sp<PlacementData>* spData);
	void GetWind(sp<WindData>* spData);
	void GetGimmickWind(sp<WindData>* spData);
	void GetGimmickLine(sp<GimmickLine>* spData);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	�E�g���N�p
	�v���C���[�͒P�̂₩��ʂŎ擾�����
	�悵�͂�N�̃v���C���[�͂���ŏ����ݒ�OK
	*/
	Vector3 GetPlayerPos();			
	Vector3 GetPlayerScale();
	Vector3 GetPlayerAngle();
	/*	�g����( ���ꂾ�� )
	pos = sPlacementManager->GetPlayerPos();
	scale = sPlacementManager->GetPlayerScale();
	angle = sPlacementManager->GetPlayerAngle();
	*/
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++






	//=============================================
	//=============================================
	//			���@���̉��ȍ~�͋C�ɂ��Ȃ���OK
	//=============================================
	//=============================================
	PlacementManager(){Clear();}void Clear();
	~PlacementManager(){Destroy();}void Destroy();
private:
	void Create_AllData();
	void Create_Data(sp<PlacementData>* spUserData,sp<Data> spData); 
	void Setting_AllData(const char* File);
	void Setting_Data(sp<char> Name,sp<char> Load);
	void Setting_Data(sp<Data> spData);
	void Setting_Data_ScaleHalf(sp<Data> spData);
	void Setting_Data_GimmickLine(sp<Data> spData);
	void Setting_WindData(sp<Data> spData);
	void Setting_ObjectName(sp<char> Name,sp<char> Load);
	void Setting_CreateObjectName(sp<char> Name,sp<char> Load);
	void Add_ObjectNum(sp<char> Name,sp<char> Load);
	void Search_Num(const char* File);
	sp<Data> m_spPlayerData;
	sp<Data> m_spBoxData;
	sp<Data> m_spWindData;
	sp<Data> m_spGimmickWindData;
	sp<Data> m_spGimmickLineData;
	sp<textLoader> tl;
};


struct Data
{
	int Num;
	int NowNum;
	char Name[STR_LENGTH];
	sp<std::string> spStr;
	sp<ePLACEMENT_TYPE> spType; 
	sp<Vector3> spPos;
	sp<Vector3> spScale;
	sp<Vector3> spAngle;
	sp<Vector3>	spWindVec;
	sp<Vector3> spStart;
	sp<Vector3> spEnd;
	Data(char* Name):Num(0),NowNum(0){
		strcpy(this->Name,Name);
	}
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	Particle_Singleton
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class PlacementManager_Singleton : public PlacementManager
{
protected:
	static PlacementManager_Singleton* singleton;

	PlacementManager_Singleton(){}
	virtual ~PlacementManager_Singleton(){}
public:
	static PlacementManager_Singleton* getInstance()
	{
		if( singleton == NULL ){
			singleton = new PlacementManager_Singleton;
		}
		return singleton;
	}
	static void deleteInstance(){
		if( singleton != NULL ){
			delete singleton;

			singleton = NULL;
		}
	}
};

#define sPlacementManager ( PlacementManager_Singleton::getInstance() )

}//PlacementLib

#endif