#include "Stage.h"

#include "utility\utility.h"
#include "utility\textLoader.h"

#include "GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"

#include "Game/CommonPipeline/GameCommonPipeline.h"

#include "Game\Gimmick\Gimmick.h"

using namespace rlib;

static const char* TAG = "Stage";
bool Stage::init(const char* stageInfoPath)
{
	clear();
	LOGI(TAG, "Execute stage init");
	textLoader loader(stageInfoPath);
//ステージモデルの読み込み
	loader.LoadString(loader.tmpBuf);
	loader.LoadString(loader.tmpBuf);
	LOGI(TAG, "meshPath=\"%s\"", loader.tmpBuf);
	this->mpMesh = new klib::kMesh(loader.tmpBuf, new klib::kMeshLoadIMO, new klib::kMeshGLES20Render() );
	this->mpMesh->setScale(1.f);
	this->mpMesh->Update();
//ギミック情報の読み込み
	loader.LoadString(loader.tmpBuf);
	loader.LoadString(loader.tmpBuf);
	LOGI(TAG, "gimmickInfo=\"%s\"", loader.tmpBuf);
	GIMMICK_MNG.init(loader.tmpBuf);
//プレイヤーの初期位置の読み込み
	loader.LoadString(loader.tmpBuf);
	mPlayerStartPos.x = loader.LoadFloat();
	mPlayerStartPos.y= loader.LoadFloat();
	mPlayerStartPos.z = loader.LoadFloat();
	LOGI(TAG, "player startPos(%.2f,%.2f,%.2f)", mPlayerStartPos.x, mPlayerStartPos.y, mPlayerStartPos.z);

	LOGI(TAG, "Complete stage init");
}

void Stage::clear()
{
	LOGI(TAG, "Execute stage clear");
	if( this->mpMesh ){ delete this->mpMesh; this->mpMesh = NULL; }
	GIMMICK_MNG.clear();
	LOGI(TAG, "Complete stage clear");
}

void Stage::update()
{
}

void Stage::render()
{
	this->mpMesh->Render(GameCommonPipeline::getPipeline());
}

int	Stage::rayPick( klib::math::Vector3* out, const klib::math::Vector3& pos, klib::math::Vector3* vec,float* Dist)
{
	return this->mpMesh->RayPick(out, pos, vec, Dist);
}

int	Stage::rayPickUD( klib::math::Vector3* out, const klib::math::Vector3& pos, klib::math::Vector3* vec,float* Dist )
{
	return this->mpMesh->RayPickUD(out, pos, vec, Dist);
}

