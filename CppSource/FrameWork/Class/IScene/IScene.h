#pragma once

namespace klib
{
	class IScene
	{
	private:
	public:
		IScene(){}
		virtual void entry()=0;
		virtual void update()=0;
		virtual void render()=0;
		virtual void exit()=0;
	};
}