#pragma once
#include "kMesh.h"
#include "utility\Class\kBufferAnalyze\kBufferAnalyze.h"
#include "utility\assetsLoader.h"
#include "utility\kutility.h"

namespace klib
{

	class IMeshLoadDelegate
	{
		//friend kMesh;
	protected:
		//kMeshData* m_Data;
	public:
		IMeshLoadDelegate(){}
		virtual ~IMeshLoadDelegate(){}
		virtual bool Load(const char* filename,kMeshData* data)=0;
	};

	

	

}