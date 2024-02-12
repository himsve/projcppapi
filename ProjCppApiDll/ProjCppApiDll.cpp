#pragma once

#include "pch.h"
#include "ProjCppApiDll.h"
#include <iostream>
#include "sqlite3.h"
#include <cstdlib>

#include <winbase.inl>
#include <wtypes.h>
#include <libloaderapi.h>

using namespace NS_PROJ::io;
using namespace NS_PROJ::metadata;

ProjDllWrapper::ProjDllWrapper::ProjDllWrapper(void)
{
	if (m_projDataPath == nullptr)
	{
		m_projDataPath = getenv("PROJ_DATA");
	}

	HINSTANCE hGetProcIDDLL = LoadLibraryA(m_projDllFileName);

	pProjContextCreate = (PROJCONTEXTCREATE*)::GetProcAddress(hGetProcIDDLL, "proj_context_create");
	pProjTransformation = (PROJCREATE*)::GetProcAddress(hGetProcIDDLL, "proj_create");
	pProjSource = (PROJCREATE*)::GetProcAddress(hGetProcIDDLL, "proj_create");
	pProjTarget = (PROJCREATE*)::GetProcAddress(hGetProcIDDLL, "proj_create");
	pProjTrans = (PROJTRANS*)::GetProcAddress(hGetProcIDDLL, "proj_trans");
	pProjTransCrs2Crs = (PROJCREATECRSTOCRSFROMPJ*)::GetProcAddress(hGetProcIDDLL, "proj_create_crs_to_crs_from_pj");
	pProjContextGetDatabasePath = (PROJCONTEXTGETDATABASEPATH*)::GetProcAddress(hGetProcIDDLL, "proj_context_get_database_path");

	//typedef DatabaseContext* (__stdcall* clientdllptr)();
	//clientdllptr = (clientdllptr*)GetProcAddress(hGetProcIDDLL, "getAuthorities()");
	
	typedef DatabaseContext* (*PFNCREATEMYCLASS)();
	typedef void (DatabaseContext::* PMYCLASSMETHOD)();
	typedef void (*PFNDELETEMYCLASS)(DatabaseContext*);
	typedef PMYCLASSMETHOD(*PFNGETCLASSMETHOD)();
	typedef void (DatabaseContext::* startInsertStatementsSession)();
	typedef PROJStringParser* (setUsePROJ4InitRules)(bool enable);
	typedef DatabaseContextPtr* (testtest)(const std::string);
	typedef GeographicBoundingBoxPtr* (createGeographicBoundingBox)(double a, double b, double c, double d);

	createGeographicBoundingBox* createObj;
	//GeographicBoundingBoxPtr* (*createGeographicBoundingBox)(double, double, double, double);

	typedef int(__cdecl* MYPROC) (LPCSTR);
	//typedef int(_cdecl* DLLPROC) (LPWSTR);
	MYPROC ProcAdd;	
	MYPROC ProcAdd3;

	// https://www.ibm.com/support/pages/part-one-how-dynamically-link-pure-c-function-dll-library-rhapsody-executable-project
	typedef GeographicBoundingBoxPtr(_cdecl* GeographicBoundingBoxCreate) (double, double, double, double);
	typedef double (__cdecl* WestBoundLongitude)();
	GeographicBoundingBoxCreate createGeographicBoundingBoxProc;
	WestBoundLongitude westBoundLongitudeProc;
  
	createGeographicBoundingBoxProc = (GeographicBoundingBoxCreate) GetProcAddress(hGetProcIDDLL, (LPCSTR)MAKELONG(397, 0));
	westBoundLongitudeProc = (WestBoundLongitude) GetProcAddress(hGetProcIDDLL, (LPCSTR)MAKELONG(901, 0));
	
	if (createGeographicBoundingBoxProc != NULL)
	{
		/*auto*/ 
		GeographicBoundingBoxPtr res1 = (createGeographicBoundingBoxProc)(1, 10, 3, 50);
		 
		//&res1->(westBoundLongitudeProc)();  //(westBoundLongitudeProc)();		
	}

	typedef DatabaseContext* (__cdecl* iklass_factory)();

	auto factory4 = reinterpret_cast<createGeographicBoundingBox*>(::GetProcAddress(hGetProcIDDLL, (LPCSTR)MAKELONG(397, 0)));
	createObj = (createGeographicBoundingBox*)::GetProcAddress(hGetProcIDDLL, (LPCSTR) MAKELONG(397, 0));	

	auto res1 = createObj(3, 30, 3, 2);

	auto factory1 = reinterpret_cast<setUsePROJ4InitRules*>(::GetProcAddress(hGetProcIDDLL, "setUsePROJ4InitRules"));
	auto factory2 = (PROJStringParser*) GetProcAddress(hGetProcIDDLL, "setUsePROJ4InitRules");	
	auto factory3 = (setUsePROJ4InitRules*) GetProcAddress(hGetProcIDDLL, "setUsePROJ4InitRules");
	
	ProcAdd = (MYPROC) GetProcAddress(hGetProcIDDLL, "?squaredEccentricity@Ellipsoid@datum@proj@osgeo@@QEBANXZ");		
	ProcAdd3 = (MYPROC) GetProcAddress(hGetProcIDDLL, (LPCSTR) MAKELONG(375, 0));
	
	auto adas2 = (testtest*)::GetProcAddress(hGetProcIDDLL, (LPCSTR) MAKELONG(375, 0));

	const std::string resourcePath = "C:/Users/himsve/Anaconda3/Library/share/proj/proj.db";

	//FARPROC pfn = ::GetProcAddress(hGetProcIDDLL, "?create@DatabaseContext@io@proj@osgeo@@SA?AV?$nn@V?$shared_ptr@VDatabaseContext@io@proj@osgeo@@@std@@@oxygen@dropbox@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBV?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@9@PEAUpj_ctx@@@Z");
	//DatabaseContextPtr* pfn = (*adas2)(resourcePath);
	
	auto wewqwdq = (*adas2)(resourcePath);
	DatabaseContextPtr* out2 = (*adas2)(resourcePath);
	
	if (!out2)
	{
		FreeLibrary(hGetProcIDDLL);
	}

	if (!adas2)
	{
		FreeLibrary(hGetProcIDDLL);
	}

	FreeLibrary(hGetProcIDDLL);

	return;
}

ProjDllWrapper::ProjDllWrapper::~ProjDllWrapper()
{
}

bool ProjDllWrapper::ProjDllWrapper::InitializeProj(std::string strProj)
{
	if (m_ctxt == nullptr)
	{	
		m_ctxt = (*pProjContextCreate)();
	}
	if (!m_ctxt)
		return false;

	if (m_transformation == nullptr)
	{	
		m_transformation = (*pProjTransformation)(m_ctxt, strProj.c_str());
	}
	if (!m_transformation)
		return false;

	return true;
}

bool ProjDllWrapper::ProjDllWrapper::InitializeProj(const char* strSourceCrs, const char* strTargetCrs, const char* strAuthorityArea)
{
	if (m_ctxt == nullptr)
	{
		m_ctxt = (*pProjContextCreate)();
	}
	if (!m_ctxt)
		return false;

	if (m_src == nullptr)
	{
		m_src = (*pProjSource)(m_ctxt, strSourceCrs);
	}
	if (!m_src)
		return false;

	if (m_trg == nullptr)
	{
		m_trg = (*pProjTarget)(m_ctxt, strTargetCrs);
	}
	if (!m_trg)
		return false;

	m_transformation = (*pProjTransCrs2Crs)(m_ctxt, m_src, m_trg, nullptr, nullptr);

	const char* resourcePath = (*pProjContextGetDatabasePath)(m_ctxt);

	if (strAuthorityArea != nullptr)
	{
		std::istringstream stream{ strAuthorityArea };

		if (stream.str() != "")
		{
			std::vector<std::string> tokens;
			std::string token;

			while (std::getline(stream, token, ':'))
				if (!token.empty())
					tokens.push_back(token);

			if (tokens.size() != 2)
				return false;

			const std::string& areaAuth = tokens[0];
			const std::string& areaCode = tokens[1];
			 
			
			//DatabaseContextPtr dbContext = DatabaseContext::create(resourcePath).as_nullable();
			//ExtentPtr bboxFilter = AuthorityFactory::create(NN_NO_CHECK(dbContext), areaAuth)->createExtent(areaCode).as_nullable();
		}
	}
	return true;
}

bool ProjDllWrapper::ProjDllWrapper::Transform(double xInput, double yInput, double zInput, double epoch, double& xOutput, double& yOutput, double& zOutput)
{
	if (m_transformation == nullptr)
		return false;
		
	PJ_COORD inputCoord;
	PJ_COORD outputCoord;

	inputCoord.xyzt.x = xInput;
	inputCoord.xyzt.y = yInput;
	inputCoord.xyzt.z = zInput;
	inputCoord.xyzt.t = epoch;	
	
	outputCoord = (*pProjTrans)(m_transformation, PJ_FWD, inputCoord);
	 
	xOutput = outputCoord.xyzt.x;
	yOutput = outputCoord.xyzt.y;
	zOutput = outputCoord.xyzt.z;
 
	return true;
}
