#pragma once

namespace klib
{
	class IScene
	{
	private:
	public:
		IScene(){}
		/**
		* @brief 初期化処理
		* シーンが変更またはプッシュされた時に呼び出される
		*/
		virtual void entry()=0;
		/**
		* @brief 更新処理
		*/
		virtual void update()=0;
		/**
		* @brief 描画処理
		*/
		virtual void render()=0;
		/**
		* @brief 終了処理
		* シーンがクリアまたはポップされた時に呼び出される
		*/
		virtual void exit()=0;
	};
}