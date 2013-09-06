#include "Stage.h"

#include "utility\utility.h"

#include "GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"

#include "Game/CommonPipeline/GameCommonPipeline.h"

using namespace rlib;

static const char* TAG = "Stage";
bool Stage::init(const char* modelPath)
{
	clear();
	LOGI(TAG, "Execute stage init");
	this->mpMesh = new klib::kMesh(modelPath, new klib::kMeshLoadIMO, new klib::kMeshGLES20Render() );
	this->mpMesh->setScale(1.f);
	this->mpMesh->Update();
	LOGI(TAG, "Complete stage init");
}

void Stage::clear()
{
	LOGI(TAG, "Execute stage clear");
	if( this->mpMesh ){ delete this->mpMesh; this->mpMesh = NULL; }
	LOGI(TAG, "Complete stage clear");
}

void Stage::update()
{
}

void Stage::render()
{
	this->mpMesh->Render(GameCommonPipeline::getPipeline());
}
