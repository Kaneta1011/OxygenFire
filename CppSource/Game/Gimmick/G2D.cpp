#include "G2D.h"

#include "utility\textWriter.h"
#include "utility\utility.h"

#ifndef ANDROID_REDNER
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"

#include "GraphicsLib\Class\r2DObj\r2DObj.h"

using namespace EffectLib;

#endif

using namespace rlib;

//=======================================================================
//
//		G2DInfoクラス
//
//=======================================================================
std::string G2DInfo::NON_PATH = "NON_PATH";
void G2DInfo::forFile(textWriter&  w)
{
	GGimmickInfo::forFile(w);
	w.nestIn();w.writeNest();
	w.write("imagePath\t").write(this->imagePath.c_str());
	w.nestOut();
	w.br();
}

bool G2DInfo::loadParam(textLoader& loader)
{
	if( ! GGimmickInfo::loadParam(loader) )
	{
		if( strcmp( "imagePath", loader.tmpBuf ) == 0 ){
			loader.LoadString(loader.tmpBuf);
			imagePath = loader.tmpBuf;
			return true;
		}
		return false;
	}
	return true;
}

IGimmick* G2DInfo::makeGimmick()
{
	G2D* set = new G2D(this);
	return set;
}

//====================================================================
//
//		ろうそくクラス
//
//====================================================================
G2D::G2D(G2DInfo* info):
	IGimmickObj(info),
	mImagePath(info->imagePath)
#ifndef ANDROID_REDNER
	,mpImage(NULL)
#endif
{
	if( info->imagePath == G2DInfo::NON_PATH ){
		LOGE("G2D","G2D::G2D : please set image path... | name=\"%s\"\n", info->name.c_str() );
	}
#ifndef ANDROID_REDNER
	mpImage = new r2DObj();
	this->mpImage->load(info->imagePath.c_str());
#endif
}

G2D::~G2D()
{
#ifndef ANDROID_REDNER
	if( this->mpImage ){ delete this->mpImage; this->mpImage = NULL; }
#endif
}

int G2D::update()
{
	return MSG_NON;
}

bool G2D::vs(Bullet* op)
{
	bool ret = false;

	return ret;
}

void G2D::flagOnListener(IGimmick* thiz)
{
}

void G2D::flagOffListener(IGimmick* thiz)
{
}

#ifndef ANDROID_REDNER
void G2D::render(klib::kMesh* mesh,const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline)
{
}
#endif
