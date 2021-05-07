#ifndef PROJCPPAPINET_H
#define PROJCPPAPINET_H

#pragma once
#include "pch.h"
#include <ProjCppApi.h>
#include <proj.h>
#include <proj/io.hpp>
#include <proj/metadata.hpp>
#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Runtime::CompilerServices;
using namespace msclr::interop;

namespace ProjCppApiNet
{
	public ref class ProjCppApiNet
	{
	private:
		// Pointer to unmanaged code
		ProjCppWrapper::ProjCppWrapper *p;
	public:
		ProjCppApiNet(void)
		{
			p = new ProjCppWrapper::ProjCppWrapper();
		};
		bool DestroyProj()
		{
			return p->DestroyProj();
		}; 
		System::String^ GetProjDbPath()
		{
			return gcnew String(p->GetProjDbPath().c_str());
		}
		bool SetProjDbPath(System::String^ strProjDbPath)
		{
			msclr::interop::marshal_context context;

			const char* cProjDbPath = context.marshal_as<const char*>(strProjDbPath);

			return p->SetProjDbPath(cProjDbPath);
		}
		System::String^ ProjGetArea(System::String^ strSourceCrs, System::String^ strTargetCrs)
		{
			msclr::interop::marshal_context context;

			const char* cSourceCrs = context.marshal_as<const char*>(strSourceCrs);
			const char* cTargetCrs = context.marshal_as<const char*>(strTargetCrs);

			return gcnew String(p->ProjGetArea(cSourceCrs, cTargetCrs));
		};
		bool InitializeProj(System::String ^ strProj)
		{
			msclr::interop::marshal_context context;

			const char* cProj = context.marshal_as<const char*>(strProj);
						
			return p->InitializeProj(cProj);	
		};
		bool InitializeProj(System::String ^ strSourceCrs, System::String ^ strTargetCrs, [System::Runtime::InteropServices::Optional] System::String ^ strAuthority, [System::Runtime::InteropServices::Optional] System::String ^ strAuthorityAreaCode)
		{
			msclr::interop::marshal_context context;

			const char* sSourceCrs = context.marshal_as<const char*>(strSourceCrs);
			const char* sTargetCrs = context.marshal_as<const char*>(strTargetCrs);
			const char* sAuthorityAreaCode = context.marshal_as<const char*>(strAuthorityAreaCode);

			return p->InitializeProj(sSourceCrs, sTargetCrs, sAuthorityAreaCode);
		};
		bool Transform(double xInput, double yInput, double zInput, double epoch, double % xOutput, double % yOutput, double % zOutput)
		{
			double x = xOutput;
			double y = yOutput;
			double z = zOutput;

			if (!p->Transform(xInput, yInput, zInput, epoch, x, y, z))
				return false;
			
			xOutput = x;
			yOutput = y;
			zOutput = z;

			return true;
		};
	};
};
#endif
