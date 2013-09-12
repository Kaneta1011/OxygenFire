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
//		GimmickManagerクラス
//		・シングルトン
//
//===============================================================================
using namespace klib;

static const char* TAG = "GimmickManager";

//==================================================================
//	ギミックの種類を判別するための関数
//==================================================================
#include "GExplosion.h"
#include "Wind.h"
#include "GimmickInfo.h"

//爆発物か?(ドラム缶、木箱、ダンボール等)
bool GimmickManager::isExplosion(GIMMICK_TYPE type){ return IGExplosion::isExplosion(type); }
//風か?
bool GimmickManager::isWindType(GIMMICK_TYPE type){ return GWind::isWindType(type); }
//導火線か？
bool GimmickManager::isFuse(GIMMICK_TYPE type){ return type == eGIMMICK_FUSE; }
//導火線の両端か？プレイヤーが近づいたらメッセージを出します
bool GimmickManager::isFusePoint(GIMMICK_TYPE type){ return type == eGIMMICK_FUSE_POINT; }
//メッセージを出すタイプか？
bool GimmickManager::isShowMessegeType(GIMMICK_TYPE type){
	switch( type ){
	case eGIMMICK_FUSE_POINT:
		return true;
	default:
		return false;
	}
}
//ぶつかるタイプか？
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
//ゴールギミックか？
bool GimmickManager::isGoalGimmick(GIMMICK_TYPE type)
{
	LOGE(TAG, "please make GimmickManager::isGoalGimmick()");
	return false;
}
//==================================================================
//
//		マネージャの定義
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

//リスナーの登録
	for( size_t i=0; i<makeNum; i++ )
	{
		IGimmick* g = this->mData[i];
		GimmickInfoBase* info = infos[i];
		if( info == NULL ){ LOGE(TAG, "null pointer found... set Listenier loop index = %d\n", i); continue;}

	//flagOnイベントの登録
		for( std::list<std::string>::iterator onIt = info->checkOn.begin();
				onIt != info->checkOn.end();
				onIt++ )
		{
			for( size_t n=0; n<this->mData.size(); n++ )
			{//データの中から検索
				IGimmick* check = this->mData[n];
				if( check == NULL ) continue;

				if( (*onIt) == check->getName() )
				{
					check->addOnListener( g );
				}
			}
		}
	//flagOffイベントの登録
		for( std::list<std::string>::iterator offIt = info->checkOff.begin();
				offIt != info->checkOff.end();
				offIt++ )
		{
			for( size_t n=0; n<this->mData.size(); n++ )
			{//データの中から検索
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
	case eGIMMICK_DRUM:			index = eMESH_DRUM; break;//ドラム缶
	case eGIMMICK_GASOLINE:		index = eMESH_GASOLINE; break;	//ガソリン
	case eGIMMICK_GARBAGE_BAG:	index = eMESH_CARD_BOARD; break;	//ゴミ袋
	case eGIMMICK_WOOD_BOX:		index = eMESH_WOOD_BOX; break;	//木箱
	case eGIMMICK_CARDBOARD:	index = eMESH_CARD_BOARD; break;	//ダンボール
	case eGIMMICK_ITTOKAN:		index = eMESH_ITTOKAN;		break;
	case eGIMMICK_FAN:			index = eMESH_FAN; break;	//扇風機
	case eGIMMICK_RESET_CANDLE:	index = eMESH_RESET_CANDLE; break;	//リセットろうそく
	case eGIMMICK_CANDLE_CHECKER:	break;
	case eGIMMICK_CANDLE:		break;	//ろうそく
	case eGIMMICK_FUSE:			break;	//導火線
	case eGIMMICK_FUSE_POINT:	index = eMESH_DRUM; break;	//導火線の両端
	case eGIMMICK_WIND:			break;	//風
	case eGIMMICK_2D:			break;	//2D描画
	case eGIMMICK_GOAL:			break;	//ゴールの扉
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
				i--;//ループのつじつまを合わせるためのもの
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
//エラーチェック
	if( index >= this->mData.size() ){
		LOGE(TAG, "out of range index=%d... remove()", index);
		return ;
	}
//リスナーの削除
	IGimmick*  g = this->mData[index];
	for( unsigned int n=0; n<this->mData.size(); n++ )
	{
		IGimmick* check = this->mData[n];
		check->removeOnListener(g);
		check->removeOffListener(g);
	}

//コンテナから削除
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
	//距離チェック
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

	//距離チェック
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

	//距離チェック
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
