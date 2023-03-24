#pragma once

#include "pch.h"
#include "ProjCppApi.h"
#include <iostream>
#include "sqlite3.h"
#include <cstdlib>

using namespace NS_PROJ::io;
using namespace NS_PROJ::metadata;

ProjCppWrapper::ProjCppWrapper::ProjCppWrapper()
{	
	m_ctxt = proj_context_create();

	proj_log_level(m_ctxt, PJ_LOG_LEVEL::PJ_LOG_NONE);
}

ProjCppWrapper::ProjCppWrapper::~ProjCppWrapper()
{
	if (m_availableEpsgCodes != nullptr)
	{
		m_availableEpsgCodes->clear();
		delete m_availableEpsgCodes;
	}
}

std::list<std::pair<int, string>> ProjCppWrapper::ProjCppWrapper::GetAvailableEpsgCodes()
{
	if (m_availableEpsgCodes != nullptr)
		return *m_availableEpsgCodes;

	m_availableEpsgCodes = new std::list<std::pair<int, string>>();
	
	if (m_ctxt != nullptr)
	{
		PROJ_CRS_LIST_PARAMETERS crsParam;

		crsParam.bbox_valid = true;
		crsParam.east_lon_degree = 32.0;
		crsParam.north_lat_degree = 82.0;
		crsParam.west_lon_degree = 5.0;
		crsParam.south_lat_degree = 57.0;

		crsParam.crs_area_of_use_contains_bbox = 1;
		crsParam.celestial_body_name = "Earth";

		crsParam.allow_deprecated = false;

		const PJ_TYPE types[] = 
		{
			PJ_TYPE_VERTICAL_CRS,
			PJ_TYPE_GEOCENTRIC_CRS,
			PJ_TYPE_GEOGRAPHIC_3D_CRS,
			PJ_TYPE_GEOGRAPHIC_2D_CRS,
			PJ_TYPE_PROJECTED_CRS,
			PJ_TYPE_COMPOUND_CRS,
			PJ_TYPE_GEOGRAPHIC_CRS,
			PJ_TYPE_GEODETIC_CRS
		};
		crsParam.types = types;
		crsParam.typesCount = 8;

		int result_count = 0;
		auto listCrs2 = proj_get_crs_info_list_from_database(m_ctxt, "EPSG", &crsParam, &result_count);		

		for (int i = 0; i < result_count; i++)
		{
			auto code = listCrs2[i]->code;
			auto name = listCrs2[i]->name;

			pair<int, string> p;

			p.first = atoi(code);
			p.second = name;

			m_availableEpsgCodes->push_back(p);
		}
	}
	return *m_availableEpsgCodes;
}

const char* ProjCppWrapper::ProjCppWrapper::ProjGetEllps(std::string strSourceCrs)
{
	if (!strSourceCrs.empty())
	{
		m_src = proj_create(nullptr, strSourceCrs.c_str());

		if (m_src == nullptr)
			return nullptr;
	}
	else
		return nullptr;

	auto ellipsoid = proj_get_ellipsoid(m_ctxt, m_src);
	  
	auto ellipsoid_name = proj_get_name(ellipsoid);

	return ellipsoid_name;
}

bool ProjCppWrapper::ProjCppWrapper::ProjGetEllpsParams(std::string strSourceCrs, double& aOut, double& bOut, double& fOut)
{
	if (!strSourceCrs.empty())
	{
		m_src = proj_create(nullptr, strSourceCrs.c_str());

		if (m_src == nullptr)
			return false;
	}
	else
		return false;

	auto ellipsoid = proj_get_ellipsoid(m_ctxt, m_src);

	double a = 0.0;
	double b = 0.0;
	double inv_flattening = 0.0;

	int res = proj_ellipsoid_get_parameters(m_ctxt, ellipsoid, &a, &b, nullptr, &inv_flattening);	  

	if (res != 1)
		return false;

	aOut = a;
	bOut = b;
	fOut = inv_flattening;

	return true;
}

bool ProjCppWrapper::ProjCppWrapper::ProjGetMeridianParams(std::string strSourceCrs, double &dLongitude, double &dLlongitudeUnit, const char* &strLongitudeUnitName)
{
	if (!strSourceCrs.empty())
	{
		m_src = proj_create(nullptr, strSourceCrs.c_str());

		if (m_src == nullptr)
			return false;
	}
	else
		return false;

	auto meridian = proj_get_prime_meridian(m_ctxt, m_src);

	double longitude = -1;
	double longitudeUnit = 0;
	const char* longitudeUnitName = nullptr;

	int res = proj_prime_meridian_get_parameters(m_ctxt, meridian, &longitude, &longitudeUnit, &longitudeUnitName);

	if (res != 1)
		return false;

	dLlongitudeUnit = longitude;
	dLongitude = longitudeUnit;
	strLongitudeUnitName = longitudeUnitName;

	return true;
}

bool ProjCppWrapper::ProjCppWrapper::ProjGetProjectionParams(
	std::string strSourceCrs,
	double& dFalseEasting,
	double& dScaleFactor,
	double& dLongitudeOfNaturalOrigin)
{
	if (!strSourceCrs.empty())
	{
		m_src = proj_create(nullptr, strSourceCrs.c_str());

		if (m_src == nullptr)
			return false;
	}
	else
		return false;

	auto conv = proj_crs_get_coordoperation(m_ctxt, m_src);

	const char* methodName = nullptr;
	const char* methodAuthorityName = nullptr;
	const char* methodCode = nullptr;

	auto res = proj_coordoperation_get_method_info(m_ctxt, conv, &methodName, &methodAuthorityName, &methodCode);
	
	if (res != 1)
		return false;

	const char* name = nullptr;
	const char* nameAuthorityName = nullptr;
	const char* nameCode = nullptr;

	double falseEasting = 0.0;
	double scaleFactor = 0.0;
	double longitudeOfNaturalOrigin = 0.0;

	const char* valueString = nullptr;
	double valueUnitConvFactor = 0;
	const char* valueUnitName = nullptr;
	const char* unitAuthName = nullptr;
	const char* unitCode = nullptr;
	const char* unitCategory = nullptr;

	if (proj_coordoperation_get_param(
		m_ctxt, conv, 3, &name, &nameAuthorityName, &nameCode, &falseEasting,
		&valueString, &valueUnitConvFactor, &valueUnitName, &unitAuthName,
		&unitCode, &unitCategory) != 1)
		return false;	
	 
	if (proj_coordoperation_get_param(
		m_ctxt, conv, 2, &name, &nameAuthorityName, &nameCode, &scaleFactor,
		&valueString, &valueUnitConvFactor, &valueUnitName, &unitAuthName,
		&unitCode, &unitCategory) != 1)
		return false;

	if (proj_coordoperation_get_param(
		m_ctxt, conv, 1, &name, &nameAuthorityName, &nameCode, &longitudeOfNaturalOrigin,
		&valueString, &valueUnitConvFactor, &valueUnitName, &unitAuthName,
		&unitCode, &unitCategory) != 1)
		return false;

	dFalseEasting = falseEasting;
	dScaleFactor = scaleFactor;
	dLongitudeOfNaturalOrigin = longitudeOfNaturalOrigin;

	return true;
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

DLL_API
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

bool ProjCppWrapper::ProjCppWrapper::SetProjDbPath(const char* strProjPath)
{
	m_projPath = strProjPath;

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

	auto type = proj_get_type(m_src);
	if (type == PJ_TYPE_GEOGRAPHIC_3D_CRS || type == PJ_TYPE_GEOGRAPHIC_2D_CRS)
	{
		inputCoord.xyzt.x = yInput ;
		inputCoord.xyzt.y = xInput;
	}
	else
	{
		inputCoord.xyzt.x = xInput;
		inputCoord.xyzt.y = yInput;
		inputCoord.xyzt.z = zInput;
		inputCoord.xyzt.t = epoch;
	}	

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

extern "C"  __declspec(dllexport) double Foo()
{
	return 1000;
}

extern "C" __declspec(dllexport) const char* GetProjDbPath()
{ 
	ProjCppWrapper::ProjCppWrapper m_ProjCppWrapper;
	return m_ProjCppWrapper.GetProjDbPath();
}

extern "C" __declspec(dllexport) bool SetProjDbPath(const char* dbPath)
{
	ProjCppWrapper::ProjCppWrapper m_ProjCppWrapper;

	std::cout << "Set PATH is: " << dbPath << '\n';

	return m_ProjCppWrapper.SetProjDbPath(dbPath);
}

extern "C" __declspec(dllexport) void ProjTransform(
	const char* strSourceCrs, const char* strTargetCrs,
	double x, double y, double z, double e,
	double& xOutput, double& yOutput, double& zOutput)
{
	ProjCppWrapper::ProjCppWrapper m_ProjCppWrapper;

	if (!m_ProjCppWrapper.InitializeProj(strSourceCrs, strTargetCrs))
		return;

	double xOut = 0.0, yOut = 0.0, zOut = 0.0;
	m_ProjCppWrapper.Transform(x, y, z, e, xOut, yOut, zOut);

	xOutput = xOut;
	yOutput = yOut;
	zOutput = zOut;
}