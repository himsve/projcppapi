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

namespace ProjCppWrapper
{
	class DLL_API ProjCppWrapper
	{
	private:
		PJ_CONTEXT *m_ctxt = nullptr;

		PJ *m_src = nullptr;
		PJ *m_trg = nullptr;
		PJ *m_transformation = nullptr;

		PJ_AREA* m_pj_area = nullptr;
	public:
		ProjCppWrapper();

		const char* ProjGetArea();
		bool DestroyProj();
		bool InitializeProj(std::string strProj);
		bool InitializeProj(std::string strSourceCrs, std::string strTargetCrs, std::string strAuthorityArea = "");
		bool Transform(double xInput, double yInput, double zInput, double epoch, double &xOutput, double &yOutput, double &zOutput);
	};
}
