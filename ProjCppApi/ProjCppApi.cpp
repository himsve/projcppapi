#pragma once

#include "pch.h"
#include "ProjCppApi.h"
#include <iostream>
#include <sqlite3.h>
#include <cstdlib>

using namespace NS_PROJ::io;
using namespace NS_PROJ::metadata;

ProjCppWrapper::ProjCppWrapper::ProjCppWrapper()
{
	m_ctxt = proj_context_create();
	proj_log_level(m_ctxt, PJ_LOG_LEVEL::PJ_LOG_NONE);
}

const char* ProjCppWrapper::ProjCppWrapper::ProjGetArea(std::string strSourceCrs, std::string strTargetCrs)
{
	if (!strSourceCrs.empty())
	{
		m_src = proj_create(nullptr, strSourceCrs.c_str());

		if (m_src == nullptr)
			return nullptr;
	}
	else
		return nullptr;

	if (!strTargetCrs.empty())
	{
		m_trg = proj_create(nullptr, strTargetCrs.c_str());

		if (m_trg == nullptr)
			return nullptr;
	}
	else
		return nullptr;

	m_transformation = proj_create_crs_to_crs_from_pj(nullptr, m_src, m_trg, nullptr, nullptr);

	const char *name = nullptr;

	double w;
	double s;
	double e;
	double n;
	
	proj_get_area_of_use(m_ctxt, m_transformation, &w, &s, &e, &n, &name);

	return name;
}

bool ProjCppWrapper::ProjCppWrapper::InitializeProj(std::string strProj)
{
	if (!strProj.empty())
	{
		m_transformation = proj_create(m_ctxt, strProj.c_str());				
	}

	if (m_transformation == nullptr)
		return false;

	return true;
}

bool ProjCppWrapper::ProjCppWrapper::InitializeProj(const char* strSourceCrs, const char* strTargetCrs, const char* strAuthorityArea)
{
	if (strSourceCrs != nullptr)	
	{
		m_src = proj_create(nullptr, strSourceCrs);
		
		if (m_src == nullptr)
			return false;
	}
	else
		return false;

	if (strTargetCrs != nullptr)
	{
		m_trg = proj_create(nullptr, strTargetCrs);

		if (m_trg == nullptr)
			return false;
	}
	else
		return false;

	m_transformation = proj_create_crs_to_crs_from_pj(nullptr, m_src, m_trg, nullptr, nullptr);
	
	const char* resourcePath = proj_context_get_database_path(m_ctxt);

	if (strAuthorityArea != nullptr)
	{
		std::vector<std::string> tokens;
		std::istringstream ss { strAuthorityArea };
		std::string token;

		while (std::getline(ss, token, ':'))
			if (!token.empty())
				tokens.push_back(token);
		
		if (tokens.size() != 2)
			return false;

		const std::string& areaAuth = tokens[0];
		const std::string& areaCode = tokens[1];

		DatabaseContextPtr dbContext = DatabaseContext::create(resourcePath).as_nullable();
		ExtentPtr bboxFilter = AuthorityFactory::create(NN_NO_CHECK(dbContext), areaAuth)->createExtent(areaCode).as_nullable();
		
		if (bboxFilter)
		{
			auto geogElts = bboxFilter->geographicElements();
			if (geogElts.size() == 1)
			{
				auto bbox = std::dynamic_pointer_cast<GeographicBoundingBox>(geogElts[0].as_nullable());
				if (bbox)
				{
					m_pj_area = proj_area_create();
					proj_area_set_bbox(m_pj_area, bbox->westBoundLongitude(), bbox->southBoundLatitude(), bbox->eastBoundLongitude(), bbox->northBoundLatitude());
				}
			}
		}
	}
	
	m_transformation = proj_create_crs_to_crs_from_pj(nullptr, m_src, m_trg, m_pj_area, nullptr);
		
	if (m_transformation == nullptr)
		return false;

	return true;
}

const char* ProjCppWrapper::ProjCppWrapper::GetProjDbPath()
{
	if (m_ctxt != nullptr)
	{
		auto path = proj_context_get_database_path(m_ctxt);
	
		if (path != nullptr)
	 		return path;		
	}
	return nullptr;
}

bool ProjCppWrapper::ProjCppWrapper::SetProjDbPath(std::string strProjPathCrs)
{
	m_projPath = strProjPathCrs.c_str();
   
	if (m_projPath != nullptr && m_ctxt != nullptr)
	{
		int res = proj_context_set_database_path(m_ctxt, m_projPath, nullptr, nullptr);
		if (res == 0)
			return false;

		return true;
	}
	return false;
}

bool ProjCppWrapper::ProjCppWrapper::DestroyProj()
{
	if (m_ctxt != nullptr)
		if (proj_context_destroy(m_ctxt) != nullptr)
			return false;
 
	if (m_src != nullptr)
		if (proj_destroy(m_src) != nullptr)
			return false;

	if (m_trg != nullptr)
		if (proj_destroy(m_trg) != nullptr)
			return false;
	 
	if (m_pj_area != nullptr)
		proj_area_destroy(m_pj_area);

	if (m_transformation != nullptr)
		if (proj_destroy(m_transformation) != nullptr)
			return false;

	if (m_projPath != nullptr)
		m_projPath = nullptr;	
	 
	return true;
}

bool ProjCppWrapper::ProjCppWrapper::Transform(double xInput, double yInput, double zInput, double epoch, double &xOutput, double &yOutput, double &zOutput)
{
	if (m_transformation == nullptr)
		return false;

	PJ_COORD inputCoord;
	PJ_COORD outputCoord;

	inputCoord.xyzt.x = xInput;
	inputCoord.xyzt.y = yInput;
	inputCoord.xyzt.z = zInput;
	inputCoord.xyzt.t = epoch;

	if (proj_angular_input(m_transformation, PJ_FWD))
	{
		inputCoord.lpzt.lam = proj_torad(inputCoord.lpzt.lam);
		inputCoord.lpzt.phi = proj_torad(inputCoord.lpzt.phi);
	}

	outputCoord = proj_trans(m_transformation, PJ_FWD, inputCoord);
	
	if (outputCoord.xyzt.x == HUGE_VAL || outputCoord.xyzt.y == HUGE_VAL || outputCoord.xyzt.z == HUGE_VAL)
		return false;

	xOutput = outputCoord.xyzt.x;
	yOutput = outputCoord.xyzt.y;
	zOutput = outputCoord.xyzt.z;

	return true;
}

// For testing:
//int main()
//{
	//proj_context_get_database_path()
//	//char* libvar;
//
//	//libvar = std::getenv("PROJ_LIB");
//	//size_t requiredSize = 100;	
//	//auto res = getenv_s(&requiredSize, NULL, 0, "PROJ_LIB");
//
//	//if (libvar = getenv("PROJ_LIB"))
//	//{
//	//	std::cout << "Your PATH is: " << libvar << '\n';
//	//}
//
//	const char* resourcePath1 = "C:/Users/himsve/Miniconda3/Library/share/proj";
//	const char* resourcePath2 = "C:/Users/Administrator/source/repos/PROJ/build/data";
//	const char* resourcePath3 = "C:/Users/himsve/Miniconda3/Library/share/proj";
//
//	PJ *P = nullptr;
//	PJ *Q = nullptr;
//	PJ *R = nullptr;
//
//	PJ_COORD c;
//	PJ_COORD d;
//	PJ_CONTEXT *m_ctxt = nullptr;
//	PJ_AREA *pj_area = nullptr;
//
//	double westlon = 4; double southlat = 58; double eastlon = 32; double northlat = 71;
//
//	c.lpzt.lam = 0.140; // radians
//	c.lpzt.phi = 1.05; // radians
//	c.lpzt.z = 100.0;
//	c.lpzt.t = 2000.0;
//
//	d.xyzt.x = 2987993.64255; 
//	d.xyzt.y = 655946.42161;
//	d.xyzt.z = 5578690.43270;
//	d.xyzt.t = 2020.00;
//
//	m_ctxt = proj_context_create();
//	//proj_context_set_file_finder();
//
//	const char *paths[] = { resourcePath1 }; 
//
//	proj_context_set_search_paths(m_ctxt, 1, paths);
//	proj_context_set_database_path(m_ctxt, resourcePath1, nullptr, nullptr);
//
//	// Test: Listing proj paths
//	auto getResourcePath = proj_context_get_database_path(m_ctxt);
//
//	DatabaseContextPtr dbContext = DatabaseContext::create(getResourcePath).as_nullable();
//	//DatabaseContextPtr dbContext = DatabaseContext::create(resourcePath1).as_nullable();
//	//DatabaseContextPtr dbContext = DatabaseContext::create(m_ctxt).as_nullable();
//
//	ExtentPtr bboxFilter = AuthorityFactory::create(NN_NO_CHECK(dbContext), "EPSG")->createExtent("1352").as_nullable();
//	
//	if (bboxFilter) 
//	{
//		auto geogElts = bboxFilter->geographicElements();
//		if (geogElts.size() == 1)
//		{
//			auto bbox = std::dynamic_pointer_cast<GeographicBoundingBox>(geogElts[0].as_nullable());
//			if (bbox)
//			{
//				pj_area = proj_area_create();
//				proj_area_set_bbox(pj_area, bbox->westBoundLongitude(), bbox->southBoundLatitude(), bbox->eastBoundLongitude(), bbox->northBoundLatitude());
//			}
//		}
//	}
// 
//	PJ *F =	proj_create_crs_to_crs(m_ctxt, "EPSG:4936", "EPSG:7789", pj_area);
//	d = proj_trans(F, PJ_FWD, d);
//
//	std::cout << "EUREF89: lat lon orto. height: " << c.lpzt.phi << " " << c.lpzt.lam << " " << c.lpzt.z << std::endl;
//
//	P = proj_create(m_ctxt, "+proj=vgridshift +grids=egm96_15.gtx");
//	c = proj_trans(P, PJ_INV, c);
// 	std::cout << "EUREF89: lat lon ell. height: " << c.lpzt.phi << " " << c.lpzt.lam << " " << c.lpzt.z << std::endl;
//
//	Q = proj_create(m_ctxt, "+proj=hgridshift +grids=EUREF89_NGO48_20081014.ct2 +ellps=GRS80");
//	c = proj_trans(Q, PJ_FWD, c);
//	std::cout << "NGO48: lat lon ell. height: " << c.lpzt.phi << " " << c.lpzt.lam << " " << c.lpzt.z << std::endl;
//
//	c = proj_trans(Q, PJ_INV, c);
//	std::cout << "EUREF89: lat lon ell. height: " << c.lpzt.phi << " " << c.lpzt.lam << " " << c.lpzt.z << std::endl;
//
//	R = proj_create(m_ctxt, "+proj=utm +zone=32 +ellps=GRS80");
//	c = proj_trans(R, PJ_FWD, c);
//	std::cout << "EUREF89: east north ell. height: " << c.xyz.x << " " << c.xyz.y << " " << c.xyz.z << std::endl;
//
//	if (P == 0)
//		return 1; 
//
//	if (Q == 0)
//		return 1;
//
//	if (R == 0)
//		return 1;
//	 
//	proj_destroy(P);
//	proj_destroy(Q);
//	proj_destroy(R);
//	
//	std::cout << "Press Enter to continue...";
//	std::cout.flush();
//	getwchar();
//}
