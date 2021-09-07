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
		std::istringstream ss{ strAuthorityArea };

		if (ss.str() != "")
		{
			std::vector<std::string> tokens;
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
