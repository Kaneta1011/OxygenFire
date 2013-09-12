#include "Gimmick.h"

#ifndef ANDROID_REDNER
#include "GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"
#include "Game\CommonPipeline\GameCommonPipeline.h"

#include "EffectLib\Effect.h"
#include "Ueda\rTestScene.h"

#include "utility\debugMessageMng.h"
#include "utility/textLoader.h"

#include "kaneta\ActionMediate\ActionMediate.h"
#include "input\Input.h"

#include "Game\Stage\Stage.h"
#endif

//#include "PlacementLib\Placement.h"

#include "utility\utility.h"

#include "GimmickInfo.h"
#include "GimmickInfoManager.h"



//#include "Tool\Particle.h"

using namespace rlib;
using namespace klib;
using namespace PlacementLib;

//===============================================================================
//
//		GimmickManager�N���X
//		�E�V���O���g��
//
//===============================================================================
using namespace klib;

static const char* TAG = "GimmickManager";

//==================================================================
//	�M�~�b�N�̎�ނ𔻕ʂ��邽�߂̊֐�
//==================================================================
#include "GExplosion.h"
#include "Wind.h"
#include "GimmickInfo.h"

//��������?(�h�����ʁA�ؔ��A�_���{�[����)
bool GimmickManager::isExplosion(GIMMICK_TYPE type){ return IGExplosion::isExplosion(type); }
//����?
bool GimmickManager::isWindType(GIMMICK_TYPE type){ return GWind::isWindType(type); }
//���ΐ����H
bool GimmickManager::isFuse(GIMMICK_TYPE type){ return type == eGIMMICK_FUSE; }
//���ΐ��̗��[���H�v���C���[���߂Â����烁�b�Z�[�W���o���܂�
bool GimmickManager::isFusePoint(GIMMICK_TYPE type){ return type == eGIMMICK_FUSE_POINT; }
//���b�Z�[�W���o���^�C�v���H
bool GimmickManager::isShowMessegeType(GIMMICK_TYPE type){
	switch( type ){
	case eGIMMICK_FUSE_POINT:
		return true;
	default:
		return false;
	}
}
//�Ԃ���^�C�v���H
bool GimmickManager::isHitGimmick(GIMMICK_TYPE type)
{
	switch( type )
	{
	case eGIMMICK_WIND:
	case eGIMMICK_FUSE:
	case eGIMMICK_CANDLE_CHECKER:
	case eGIMMICK_UNKNOWN:
		return false;
	default:
		return true;
	}
}
//�S�[���M�~�b�N���H
bool GimmickManager::isGoalGimmick(GIMMICK_TYPE type)
{
	LOGE(TAG, "please make GimmickManager::isGoalGimmick()");
	return false;
}
//==================================================================
//
//		�}�l�[�W���̒�`
//
//==================================================================
#ifndef ANDROID_REDNER
static klib::kMesh* debugMesh = NULL;
static bool isDebugMesh = false;
#endif

GimmickManager::GimmickManager()
#ifndef ANDROID_REDNER
	:mppMeshies(NULL)
#endif
{
}

GimmickManager::~GimmickManager()
{
}

void GimmickManager::clear()
{
	LOGI(TAG, "Execute GimmickManager::clear\n");
	clearData();
#ifndef ANDROID_REDNER
	if( debugMesh ){ delete debugMesh; debugMesh = NULL; }
	if( this->mppMeshies ){
		for( int i=0; i<eMESH_TYPE_NUM; i++ )
		{
			if( this->mppMeshies[i] ){ delete this->mppMeshies[i]; this->mppMeshies[i] = NULL; }
		}
		delete [] this->mppMeshies;
		this->mppMeshies = NULL;
	}
#endif
	LOGI(TAG, "Complete GimmickManager::clear\n");
}

void GimmickManager::clearData()
{
	Iterator it = this->mData.begin();
	while( it != end() )
	{
		if( (*it) ){ delete (*it); (*it) = NULL; }
		it++;
	}
	this->mData.clear();
}

void GimmickManager::init(const char* giFilePath)
{
#ifndef ANDROID_REDNER
	debugMesh = new klib::kMesh("gimmick/unitBox.IMO", new klib::kMeshLoadIMO, new klib::kMeshGLES20Render() );
#endif

	LOGI(TAG, "Execute GimmickManager init\n");

	GimmickInfoManager infoMng;
	infoMng.load(giFilePath);
	std::vector<GimmickInfoBase*>& infos = infoMng.getDatas();
	LOGI(TAG, "data count = %d\n", infos.size() );
#ifndef ANDROID_REDNER
	TEST_POS_NUM = 0;
#endif
	size_t makeNum = infos.size();
	this->mData.resize(makeNum);
	for( size_t i=0; i<makeNum; i++ )
	{
		if( infos[i] ){
			this->mData[i] = infos[i]->makeGimmick();
#ifndef ANDROID_REDNER
			if( isShowMessegeType( this->mData[i]->getType() ) ){
				klib::testpos[TEST_POS_NUM] = this->mData[i]->getPos();
				TEST_POS_NUM++;
			}
#endif
		}
	}
	LOGI(TAG, "start listener set!\n");

//���X�i�[�̓o�^
	for( size_t i=0; i<makeNum; i++ )
	{
		IGimmick* g = this->mData[i];
		GimmickInfoBase* info = infos[i];
		if( info == NULL ){ LOGE(TAG, "null pointer found... set Listenier loop index = %d\n", i); continue;}

	//flagOn�C�x���g�̓o�^
		for( std::list<std::string>::iterator onIt = info->checkOn.begin();
				onIt != info->checkOn.end();
				onIt++ )
		{
			for( size_t n=0; n<this->mData.size(); n++ )
			{//�f�[�^�̒����猟��
				IGimmick* check = this->mData[n];
				if( check == NULL ) continue;

				if( (*onIt) == check->getName() )
				{
					check->addOnListener( g );
				}
			}
		}
	//flagOff�C�x���g�̓o�^
		for( std::list<std::string>::iterator offIt = info->checkOff.begin();
				offIt != info->checkOff.end();
				offIt++ )
		{
			for( size_t n=0; n<this->mData.size(); n++ )
			{//�f�[�^�̒����猟��
				IGimmick* check = this->mData[n];
				if( check == NULL ) continue;

				if( (*offIt) == check->getName() )
				{
					check->addOffListener( g );
				}
			}
		}
	}

	infoMng.clear();
#ifdef ANDROID_REDNER
	for( unsigned int i=0; i<this->mData.size(); i++ )
	{
		IGimmick* g = this->mData[i];
		if( g == NULL ){
			LOGE(TAG,"null pointer found... show debugMessage loop at init. index=%d\n", i);
			continue;
		}
		g->debugMessage();
	}
#endif

#ifndef ANDROID_REDNER
	LOGI(TAG, "start mesh scale file!\n");
	textLoader loader("gimmick/meshScale.txt");
	mMeshScales.resize(eGIMMICK_TYPE_NUM);
	for( int i=0; i<eGIMMICK_TYPE_NUM; i++ )
	{
		loader.LoadString(loader.tmpBuf);
		mMeshScales[i].x = loader.LoadFloat();
		mMeshScales[i].y = loader.LoadFloat();
		mMeshScales[i].z = loader.LoadFloat();
	}
	LOGI(TAG, "finish mesh scale file!\n");
	loadMeshes();
#endif

	LOGI(TAG, "box num = %d\n", this->mData.size() );
	LOGI(TAG,"OK Placement read\n");
	LOGI(TAG, "Complete GimmickManager init\n");
}

#ifndef ANDROID_REDNER
void GimmickManager::loadMeshes()
{
	this->mppMeshies = new klib::kMesh*[eMESH_TYPE_NUM];
	for( int i=0; i<eMESH_TYPE_NUM; i++ ){
		this->mppMeshies[i] = NULL;
	}

	this->mppMeshies[eMESH_DRUM] = new klib::kMesh("gimmick/drum/drum.IMO", new klib::kMeshLoadIMO, new klib::kMeshGLES20Render() );
	this->mppMeshies[eMESH_GASOLINE] = new klib::kMesh("gimmick/gasoline/gaso.IMO", new klib::kMeshLoadIMO, new klib::kMeshGLES20Render() );
	this->mppMeshies[eMESH_WOOD_BOX] = new klib::kMesh("gimmick/wood_box/kibako128.IMO", new klib::kMeshLoadIMO, new klib::kMeshGLES20Render() );
	//this->mpMeshies[eMESH_GABERAGE_BOX] = new klib::kMesh("Placement/gomibukuro.IMO", new klib::kMeshLoadIMO, new klib::kMeshGLES20Render() );
	this->mppMeshies[eMESH_CARD_BOARD] = new klib::kMesh("gimmick/danbo/danbo.IMO", new klib::kMeshLoadIMO, new klib::kMeshGLES20Render() );
	this->mppMeshies[eMESH_RESET_CANDLE] = new klib::kMesh("gimmick/candle/resetCandleS2.IMO", new klib::kMeshLoadIMO, new klib::kMeshGLES20Render() );
	this->mppMeshies[eMESH_FAN] = new klib::kMesh("gimmick/fan/fan.IMO", new klib::kMeshLoadIMO, new klib::kMeshGLES20Render() );
	this->mppMeshies[eMESH_ITTOKAN] = new klib::kMesh("gimmick/ittokan/ittokan.IMO", new klib::kMeshLoadIMO, new klib::kMeshGLES20Render() );
	LOGI(TAG, "Successed gimmick meshes | count = %d", eMESH_TYPE_NUM);
}
#endif

#ifndef ANDROID_REDNER
klib::kMesh* GimmickManager::getMesh( int type, klib::math::Vector3* outUnitScale )
{
	if( isDebugMesh ){ outUnitScale->x = outUnitScale->y = outUnitScale->z = 0.02f; return debugMesh; }

	*outUnitScale =  mMeshScales[type];
	int index = 0;
	switch(type)
	{
	case eGIMMICK_DRUM:			index = eMESH_DRUM; break;//�h������
	case eGIMMICK_GASOLINE:		index = eMESH_GASOLINE; break;	//�K�\����
	case eGIMMICK_GARBAGE_BAG:	index = eMESH_CARD_BOARD; break;	//�S�~��
	case eGIMMICK_WOOD_BOX:		index = eMESH_WOOD_BOX; break;	//�ؔ�
	case eGIMMICK_CARDBOARD:	index = eMESH_CARD_BOARD; break;	//�_���{�[��
	case eGIMMICK_ITTOKAN:		index = eMESH_ITTOKAN;		break;
	case eGIMMICK_FAN:			index = eMESH_FAN; break;	//��@
	case eGIMMICK_RESET_CANDLE:	index = eMESH_RESET_CANDLE; break;	//���Z�b�g�낤����
	case eGIMMICK_CANDLE_CHECKER:	break;
	case eGIMMICK_CANDLE:		break;	//�낤����
	case eGIMMICK_FUSE:			break;	//���ΐ�
	case eGIMMICK_FUSE_POINT:	index = eMESH_DRUM; break;	//���ΐ��̗��[
	case eGIMMICK_WIND:			break;	//��
	case eGIMMICK_2D:			break;	//2D�`��
	case eGIMMICK_GOAL:			break;	//�S�[���̔�
	default:
		LOGE(TAG, "unknwon type = %d!! GimmickManager::getMesh()", type);
	}
	return this->mppMeshies[index];
}
#endif


int GimmickManager::update()
{
	//LOGI(TAG,"gimmick update");
#ifndef ANDROID_REDNER
	static int oldTouchCount = -1;
	if( mlInput::getNowTouchCount() == 3 && oldTouchCount != 3 ){
		isDebugMesh = !isDebugMesh;
	}
	oldTouchCount = mlInput::getNowTouchCount();
#endif
	for( unsigned int i=0; i<this->mData.size(); i++ )
	{
		IGimmick* g = this->mData[i];

		if( g ){
			int msg = g->update();
			if( msg == IGimmick::MSG_DEAD )
			{
				remove( i );
				i--;//���[�v�̂��܂����킹�邽�߂̂���
			}
		}else{
			LOGE(TAG, "error null gimmick!!\n");
		}
	}

#ifdef ANDROID_REDNER
	for( unsigned int i=0; i<this->mData.size(); i++ )
	{
		IGimmick* g = this->mData[i];
		if( g == NULL ){
			LOGE(TAG,"null pointer found... show debugMessage loop at update. index=%d\n", i);
			continue;
		}
		g->debugMessage();
	}
#endif

#ifndef ANDROID_REDNER
	DEBUG_MSG("gimmick num = %d", this->mData.size());
#endif
	return MSG_NON;
}

void GimmickManager::remove(unsigned int index)
{
//�G���[�`�F�b�N
	if( index >= this->mData.size() ){
		LOGE(TAG, "out of range index=%d... remove()", index);
		return ;
	}
//���X�i�[�̍폜
	IGimmick*  g = this->mData[index];
	for( unsigned int n=0; n<this->mData.size(); n++ )
	{
		IGimmick* check = this->mData[n];
		check->removeOnListener(g);
		check->removeOffListener(g);
	}

//�R���e�i����폜
	if( this->mData[index] ){ delete this->mData[index]; this->mData[index]=NULL; }
	Iterator it = this->mData.begin();
	for( int i=0; i<index; i++ ) it++;
	this->mData.erase(it);
}

std::list<IGimmick*> GimmickManager::getNearGimmick(const klib::math::Vector3& pos, float range)
{
	std::list<IGimmick*> result;

	float rangeSq = range*range;
	for( size_t i=0; i<this->mData.size(); i++ )
	{
		IGimmick* g = this->mData[i];
	//�����`�F�b�N
		Vector3 dir = g->getPos() - pos;
		if( dir.lengthSq() < rangeSq )
		{
			result.push_back(g);
		}
	}
	return result;
}

std::list<IGimmick*> GimmickManager::getNearShowMessageGimmick(const klib::math::Vector3& pos, float range)
{
	std::list<IGimmick*> result;

	float rangeSq = range*range;
	for( size_t i=0; i<this->mData.size(); i++ )
	{
		IGimmick* g = this->mData[i];
		if( !isShowMessegeType(g->getType()) ) continue;

	//�����`�F�b�N
		Vector3 dir = g->getPos() - pos;
		if( dir.lengthSq() < rangeSq )
		{
			result.push_back(g);
		}
	}
	return result;
}

klib::math::Vector3 GimmickManager::collision(const klib::math::Vector3& pos, float range)
{
	Vector3 result = pos;
	float y = pos.y;
	for( size_t i=0; i<this->mData.size(); i++ )
	{
		IGimmick* g = this->mData[i];
		if( !isHitGimmick(g->getType()) ) continue;

	//�����`�F�b�N
		Vector3 dir = g->getPos() - result;
		float rangeSq = g->getRange().x + range;
		rangeSq *= rangeSq;
		if( dir.lengthSq() < rangeSq )
		{
			if( g->getType() == eGIMMICK_GOAL && g->isFlag() ){
#ifndef ANDROID_REDNER
				STAGE.onClearFlag();
#endif
			}else{
				float len = g->getRange().x + range;
				dir.normalize();
				result = g->getPos() - dir*len;
				result.y = y;
			}
		}
	}
	return result;
}

klib::math::Vector3 GimmickManager::calWindPower(const klib::math::Vector3& pos, float range)
{
	Vector3 result;
	for( size_t i=0; i<this->mData.size(); i++ )
	{
		IGimmick* g = this->mData[i];
		if( !isWindType(g->getType()) ) continue;

		if( g->vsPoint(pos) )
		{
			result += g->getVelocity() * 0.01666f;
		}
	}
	return result;
}

#ifndef ANDROID_REDNER
void GimmickManager::render()
{
	Iterator it = this->mData.begin();
	while( it != end() )
	{
		if( *it ){
			klib::math::Vector3 scale;
			klib::kMesh* mesh = getMesh((*it)->getType(), &scale);
			if( mesh ){
				if( isDebugMesh ){
					mesh->setPosition((*it)->getPos());
					mesh->setAngle((*it)->getAngle());
					Vector3 s = (*it)->getRange();
					s.x *= scale.x;
					s.y *= scale.y;
					s.z *= scale.z;
					mesh->setScale(s);
					mesh->Update();
					mesh->Render(GameCommonPipeline::getPipeline());
				}else{
					(*it)->render( mesh, scale, GameCommonPipeline::getPipeline());
				}
			}
		}
		it++;
	}
}
#endif
