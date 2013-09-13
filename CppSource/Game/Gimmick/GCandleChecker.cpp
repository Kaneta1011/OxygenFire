#include "GCandleChecker.h"

#include "utility\textWriter.h"
#include "utility\utility.h"

#include "GCandle.h"

#ifndef ANDROID_REDNER
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"
using namespace EffectLib;

#endif

using namespace rlib;


//=======================================================================
//
//		GCandleInfoクラス
//
//=======================================================================
std::string GCandleCheckerInfo::NON_DATA = "NON_DATA";
void GCandleCheckerInfo::forFile(textWriter&  w)
{
	GimmickInfoBase::forFile(w);

	w.nestIn();w.writeNest();
	w.write("correctAnswer\t").write(this->correctAnswer.c_str());
	w.nestOut();
	w.br();
}

bool GCandleCheckerInfo::loadParam(textLoader& loader)
{
	if( ! GimmickInfoBase::loadParam(loader) )
	{
		if( strcmp( "correctAnswer", loader.tmpBuf ) == 0 ){
			loader.LoadString(loader.tmpBuf);
			this->correctAnswer = loader.tmpBuf;
			return true;
		}
		return false;
	}
	return true;
}

IGimmick* GCandleCheckerInfo::makeGimmick()
{
	GCandleChecker* set = new GCandleChecker(this);
	return set;
}

//====================================================================
//
//		ろうそくチェッカークラス
//
//====================================================================
GCandleChecker::GCandleChecker(GCandleCheckerInfo* info):
	IGimmick(info),
	mNowCheckIndex(0),
	mCorrectAnswer(info->correctAnswer)
{
	if( this->mCorrectAnswer == GCandleCheckerInfo::NON_DATA ){
		LOGI("GCandleChecker", "GCandleChecker Constructer : please set correct answer!!!\n");
	}
}

GCandleChecker::~GCandleChecker()
{
}

int GCandleChecker::update()
{
	return MSG_NON;
}

bool GCandleChecker::vs(Bullet* op)
{
	bool ret = false;
	return ret;
}

void GCandleChecker::flagOnListener(IGimmick* thiz)
{
	if( thiz->getType() == eGIMMICK_RESET_CANDLE ){
		this->mNowCheckIndex = 0;
		LOGI("GCandleChecker","reset | name=\"%s\"\n", getName().c_str() );
	}else if( thiz->getType() == eGIMMICK_CANDLE ){
	//すでに間違えているか？ または 正解したか？
		if( this->mNowCheckIndex >= this->mCorrectAnswer.size() ) return;

		GCandle* candle = (GCandle*)thiz;
		char id = candle->getID();
		LOGI("GCandleChecker","check id=%c | nextID=%c\n", id, this->mCorrectAnswer[this->mNowCheckIndex]);
		if( id == this->mCorrectAnswer[this->mNowCheckIndex] ){
			this->mNowCheckIndex++;
			LOGI("GCandleChecker", "OK!!\n");
			if( this->mNowCheckIndex == this->mCorrectAnswer.size() ){
				flagOn();
				LOGI("GCandleChecker", "Complete!!\n");
			}
		}else{
			this->mNowCheckIndex = this->mCorrectAnswer.size()+1;
			LOGI("GCandleChecker", "Failure...\n");
		}
	}
}

void GCandleChecker::flagOffListener(IGimmick* thiz)
{
}

#ifndef ANDROID_REDNER
void GCandleChecker::render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)
{
}
#endif
