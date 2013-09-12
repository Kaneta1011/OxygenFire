#include "Wind.h"

#include "utility\textWriter.h"

using namespace rlib;

bool GWind::isWindType(GIMMICK_TYPE type)
{
	return type == eGIMMICK_WIND;
}

//===============================================================
//
//		GWindInfoクラス
//
//===============================================================
void GWindInfo::convert(WindData* data, int index)
{
//名前の設定
	//cout << "GWindInfoで名前設定の処理作ってね" << endl;
	this->name = data->spStr[index];
//
	this->type = eGIMMICK_WIND;
	this->pos = data->spPos[index];
	this->dir = data->spWindVec[index];
	this->power = this->dir.length();
	this->dir.normalize();
	this->scale = data->spScale[index];
}

void GWindInfo::forFile(textWriter&  w)
{
	GimmickInfoBase::forFile(w);

	w.nestIn();w.writeNest();
	w.write("pos\t");
	writeVec3(pos, w);
	w.br().write("dir\t");
	writeVec3(dir, w);
	w.br().write("power\t").write(this->power);
	w.br().write("scale\t");
	writeVec3(scale, w);
	w.br().write("isRender\t");
	w.write(this->isRender);

	w.nestOut();
	w.br();
}

bool GWindInfo::loadParam(textLoader& loader)
{
	if( ! GimmickInfoBase::loadParam(loader) )
	{
		if( strcmp("pos",loader.tmpBuf) == 0 ){
			this->pos.x = loader.LoadFloat();
			this->pos.y = loader.LoadFloat();
			this->pos.z = loader.LoadFloat();
			return true;
		}else if( strcmp("dir",loader.tmpBuf) == 0 ){
			this->dir.x = loader.LoadFloat();
			this->dir.y = loader.LoadFloat();
			this->dir.z = loader.LoadFloat();
			return true;
		}else if( strcmp("power",loader.tmpBuf) == 0 ){
			this->power = loader.LoadFloat();
			return true;
		}else if( strcmp("scale",loader.tmpBuf) == 0 ){
			this->scale.x = loader.LoadFloat();
			this->scale.y = loader.LoadFloat();
			this->scale.z = loader.LoadFloat();
			return true;
		}else if( strcmp("isRender",loader.tmpBuf) == 0 ){
			this->isRender = loader.LoadInt() == 1;
			return true;
		}
		return false;
	}
	return true;
}

IGimmick* GWindInfo::makeGimmick()
{
	//LOGE(TAG,"GWindInfo::makeGimmick() 作ってね\n");
	GWind* set = new GWind(*this);
	return set;
}

//================================================================================
//
//		風クラス
//
//================================================================================
GWind::GWind(GWindInfo& info):
IGimmick(&info)
{
	this->mPos = info.pos;
	this->mRate = 1.f;
	this->mMaxPower = info.power;
	this->mDirection = info.dir;
	this->mVelocity = this->mDirection * this->mMaxPower;

	this->mRange = info.scale*0.5f;
	this->mIsRender = info.isRender;

#ifndef ANDROID_REDNER
	this->mEmitter.Clear();
	if( this->mIsRender )
	{
		this->mEmitter = EffectLib::EffectManager_Singleton::getInstance()->Create( EffectLib::WIND );
		this->mEmitter->Setting_Position( getEmitterPos() );
		this->mEmitter->Setting_Velocity(this->mVelocity);
		this->mEmitter->Setting_Scale( 0.5f );
		this->mEmitter->Loop();
	}
#endif
}

Vector3 GWind::getEmitterPos()
{
	Vector3 offset = this->mVelocity;
	offset.normalize();
	offset.x *= this->mRange.x*2.f;
	offset.y *= this->mRange.y*2.f;
	offset.z *= this->mRange.z*2.f;
	return this->mPos-offset;
}

GWind::~GWind()
{
#ifndef ANDROID_REDNER
	if( this->mEmitter.IsExist() )
		this->mEmitter->End();
#endif
}

int GWind::update()
{
//風の強さを変異させる
	float delta = rand()/(float)RAND_MAX * 0.2f - 0.1f;
	this->mRate += delta;
	if( this->mRate > 1.f ) this->mRate = 1.f;
	else if( this->mRate < 0.3f ) this->mRate = 0.3f;

	this->mVelocity = this->mDirection * this->mMaxPower * this->mRate;

#ifndef ANDROID_REDNER
	if( this->mEmitter.IsExist() )
	{
		Vector3 vec = this->mVelocity * this->mRate;
		float scale = vec.lengthSq() / 25.f;
		this->mEmitter->Setting_Scale(scale);
	}
#endif
	return 0;
}

bool GWind::vs(Bullet* op)
{
	if( IGimmick::vs(op) ){
	//現在の風の強さを弱める
	}
	return false;
}

void GWind::flagOnListener(IGimmick* thiz)
{
}

void GWind::flagOffListener(IGimmick* thiz)
{
}

#ifndef ANDROID_REDNER
void GWind::render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline)
{
}
#endif
