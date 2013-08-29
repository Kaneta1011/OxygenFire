#ifndef _R_I2DOBJ_
#define _R_I2DOBJ_

#include "math\kmathf.h"
#include "GraphicsLib\Class\tRenderState\RenderState.h"
#include "GraphicsLib\Class\kGraphicsPipline\kGraphicsPipline.h"
#include "Ueda\TmpShader\Shader.h"

namespace rlib
{
	class Texture;

	/*
	・2D描画のための基底クラス
	・2D描画を行いたいときはrlib::r2DObjクラスを使用してください
	*/
	class r2DRenderer
	{
	public:
		enum DRAW_ORIGIN_TYPE{
			DRAW_TOP_LEFT,
			DRAW_CENTER,
			DRAW_CENTER_TYPE_MAX,
		};

	public:
		r2DRenderer();

		virtual void	render()=0;
		/*
		外部からテクスチャを指定して描画する
		*/
		void			render(rlib::Texture* pTex);

	public://セッター・ゲッター
		/*
		x,yがとる値の範囲は-100(左端)～100(右端)の間、0が画面中央
		zは0～1の間
		*/
		void setPos(float x, float y, float z=0.f){this->mPos.x = x; this->mPos.y = y; this->mPos.z = z; this->mIsUpdate = true;}
		/*
		x,yがとる値の範囲は-100(左端)～100(右端)の間、0が画面中央
		zは0～1の間
		*/
		void setPos(const klib::math::Vector3& pos){this->mPos = pos;			this->mIsUpdate = true;}
		/*
		x,yともに端から端まで表示したいなら200を設定してください。
		値の範囲　:　0～100(画面半分)～200(画面全体)
		*/
		void setSize(float x, float y){this->mSize.x = x; this->mSize.y = y;	this->mIsUpdate = true;}
		/*
		x,yともに端から端まで表示したいなら200を設定してください。
		値の範囲　:　0～100(画面半分)～200(画面全体)
		*/
		void setSize(const klib::math::Vector2& size){this->mSize = size;		this->mIsUpdate = true;}
		/*
		ローカル内の描画位置の設定
		*/
		void setDrawOrigin(DRAW_ORIGIN_TYPE type){this->mCenterType = type;		this->mIsUpdate = true;}

		const klib::math::Vector3& getPos()const{return this->mPos;}
		const klib::math::Vector2& getSize()const{return this->mSize;}
		DRAW_ORIGIN_TYPE getDrawOrigin()const{ return this->mCenterType; }

	protected:
		enum eS_TYPE
		{
			S_POS,
			S_TEX,
			S_MAX,
		};

	protected://仮想関数
		//描画用バッファの初期化を行う
		//中でinitPosBufとinitTexBufを呼び出している
		//この関数はコンストラクタ内で呼び出されています
		void initBuf();
		//Posバッファの作成
		virtual void initPosBuf();
		//UV座標バッファの作成
		virtual void initTexBuf();

		virtual void update();
		virtual void innerRender(rlib::Texture* pTex);

	protected:
		void getDrawOffset(float* x, float *y);
		void setTexBuf(bool isFrameBuffer);

	protected:
		klib::math::Vector3		mPosBuf[4];
		klib::math::Vector2		mTexBuf[4];

		bool				mIsUpdate;
		DRAW_ORIGIN_TYPE	mCenterType;
		klib::math::Vector3 mPos;
		klib::math::Vector2 mSize;
	};

	/*
	2D描画で使用するレンダリングパイプラインを管理するクラス
	・とりあえず、RenderLib::Shaderを利用してますが、のちのちklib::kGraphicsPiplineへ移行したいです
	*/
	class r2DPipeline
	{
	public:
		static void init();
		static void clear();

		static klib::kGraphicsPipline& getPipeline(){return *spPipeline;};
		static RenderLib::Shader& getShader(){return *spShader;};

	private:
		static klib::kGraphicsPipline*	spPipeline;
		static RenderLib::Shader*		spShader;
	};

	//==================================================================
	//
	//		2Dにおける座標変換をサポートする
	//		・スクリーン座標からプロジェクション座標への値に変換関数があります。
	//		・2D描画では座標の指定はプロジェクション座標系を採用しています。
	//			x,yともに値の範囲は-100(左,下)～100(右,上)になってます。
	//		・mlInputクラスは各ゲッターにこのクラスの関数を使ってますので、タッチ位置などを使う際、利用者側が変換しなくていいようにしています。
	//
	//==================================================================
	class r2DHelper
	{
	public:
		/*
		スクリーン上のX座標をプロジェクション座標系へ変換する
		*/
		static float convertPosX(float screenX){
			screenX /= RenderLib::RenderState::getScreenWidth();
			screenX = screenX*2.f - 1.f;
			return screenX;
		}
		/*
		スクリーン上のY座標をプロジェクション座標系へ変換する
		*/
		static float convertPosY(float screenY){
			screenY /= RenderLib::RenderState::getScreenHeight();
			screenY = screenY*2.f - 1.f;
			return -screenY;
		}
		/*
		スクリーン座標系での位置ベクトルをプロジェクション座標系へ変換する
		*/
		static klib::math::Vector2 convertPos(const klib::math::Vector2& posVec)
		{
			return klib::math::Vector2( convertPosX(posVec.x), convertPosY(posVec.y) );
		}

		/*
		スクリーン座標系でのX成分の移動量をプロジェクション座標系へ変換する
		*/
		static float convertMoveX(float screenX){
			screenX *= getWidthUnit();
			return screenX;
		}
		/*
		スクリーン座標系でのY成分の移動量をプロジェクション座標系へ変換する
		*/
		static float convertMoveY(float screenY){
			screenY *= getHeightUnit();
			return -screenY;
		}
		/*
		スクリーン座標系での移動ベクトルをプロジェクション座標系へ変換する
		*/
		static klib::math::Vector2 convertMove(const klib::math::Vector2& moveVec)
		{
			return klib::math::Vector2( convertMoveX(moveVec.x), convertMoveY(moveVec.y) );
		}

		/*
		プロジェクション座標系の横軸における1ピクセルの割合を返す
		*/
		static float getWidthUnit(){ float rate = 1.f / RenderLib::RenderState::getScreenWidth();  return rate;}
		/*
		プロジェクション座標系の縦軸における1ピクセルの割合を返す
		*/
		static float getHeightUnit(){float rate = 1.f / RenderLib::RenderState::getScreenHeight(); return rate;}

		/*
		更新時に扱う2D座標値を描画用のものに変換する関数
		*/
		static float toRenderCoord(float updateCoord){
			static const float UtoR = 0.01f;
			return updateCoord * UtoR;
		}
		/*
		描画時に扱う2D座標値を更新用のものに変換する関数
		*/
		static float toUpdateCoord(float renderCoord){
			static const float RtoU = 100.f;
			return renderCoord * RtoU;
		}
	};

}
#endif