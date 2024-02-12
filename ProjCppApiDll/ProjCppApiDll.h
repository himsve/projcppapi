#pragma once

#ifndef DLL_EXPORT
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <proj.h>
#include "proj/io.hpp"
#include "proj/metadata.hpp"
#include "proj/nn.hpp"

using namespace std;

namespace ProjDllWrapper
{
	class DLL_API ProjDllWrapper
	{
	private:
		const char* m_projDataPath = nullptr;
		const char* m_projDllFileName = "proj_9_2.dll";
		
		PJ_CONTEXT* m_ctxt = nullptr;
		PJ* m_transformation = nullptr;
		PJ* m_src = nullptr;
		PJ* m_trg = nullptr;

		typedef PJ_CONTEXT* (PROJCONTEXTCREATE)(void);
		typedef PJ* (PROJCREATE)(PJ_CONTEXT*, const char*);
		typedef PJ* (PROJCREATECRSTOCRSFROMPJ)(PJ_CONTEXT*, PJ*, PJ*, PJ_AREA*, const char* const*);		
		typedef PJ_COORD (PROJTRANS)(PJ*, PJ_DIRECTION, PJ_COORD);
		typedef const char* (PROJCONTEXTGETDATABASEPATH)(PJ_CONTEXT*);
		//typedef AuthorityFactory::createExtent (WWWF)(const std::string&);
				
		PROJCONTEXTCREATE* pProjContextCreate;
		PROJCREATE* pProjTransformation;
		PROJCREATE* pProjSource;
		PROJCREATE* pProjTarget;
		PROJCREATECRSTOCRSFROMPJ* pProjTransCrs2Crs;
		PROJTRANS* pProjTrans;
		PROJCONTEXTGETDATABASEPATH* pProjContextGetDatabasePath;

	public:
		ProjDllWrapper(void);
		~ProjDllWrapper();

		bool InitializeProj(std::string strProj);
		bool InitializeProj(const char* strSourceCrs, const char* strTargetCrs, const char* strAuthorityArea);
		bool Transform(double xInput, double yInput, double zInput, double epoch, double& xOutput, double& yOutput, double& zOutput);
	};
}
