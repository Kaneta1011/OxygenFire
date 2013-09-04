#ifndef _GIMMICK_INFO_MANAGER_
#define _GIMMICK_INFO_MANAGER_

#include "GimmickInfo.h"
#include <vector>
#include <string>

namespace rlib
{

	class GimmickInfoManager
	{
	public:
		GimmickInfoManager();
		~GimmickInfoManager();

		void clear();

		void loadMqo(char* mqoFilePath);
		void load(const char* giFilePath);

		void write(const char* giFilePath);

	public:
		std::vector<GimmickInfoBase*>& getDatas(){return this->mData;}
		const std::vector<GimmickInfoBase*>& getDatas()const{return this->mData;}

	private:
		void setFusePoint(GGimmickInfo* out, std::string& name, const klib::math::Vector3& pos);

		void setInfo(textLoader& loader, GimmickInfoBase** out, int type, std::string& name);
		void setGimmickInfo(textLoader& loader, GimmickInfoBase** out, int type, std::string& name);
		void setWindInfo(textLoader& loader, GimmickInfoBase** out, int type, std::string& name);
		void setLineInfo(textLoader& loader, GimmickInfoBase** out, int type, std::string& name);

	private:
		std::vector<GimmickInfoBase*> mData;

	};

}
#endif