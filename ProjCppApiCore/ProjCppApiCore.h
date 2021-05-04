#ifndef PROJCPPAPICLR_H
#define PROJCPPAPICLR_H

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

namespace ProjCppApiCore
{
	public ref class ProjCppApiCore
	{
	private:
		// Pointer to unmanaged code
		ProjCppWrapper::ProjCppWrapper *p;
	public:
		ProjCppApiCore(void)
		{
			p = new ProjCppWrapper::ProjCppWrapper();
		};
		bool DestroyProj()
		{
			return p->DestroyProj();
		};	
		System::String ^ ProjGetArea()
		{
			return gcnew String(p->ProjGetArea());
		};
		bool InitializeProj(System::String ^ strProj)
		{
			msclr::interop::marshal_context context;

			const char* cProj = context.marshal_as<const char*>(strProj);
						
			return p->InitializeProj(cProj);	
		};
		bool InitializeProj(System::String ^ strSourceCrs, System::String ^ strTargetCrs, [System::Runtime::InteropServices::Optional] System::String ^ strAuthority, [System::Runtime::InteropServices::Optional] System::String ^ strAreaCode)
		{
			msclr::interop::marshal_context context;

			const char* sSourceCrs = context.marshal_as<const char*>(strSourceCrs);
			const char* sTargetCrs = context.marshal_as<const char*>(strTargetCrs);
			const char* sAuthority = context.marshal_as<const char*>(strAuthority);
			const char* sAreaCode = context.marshal_as<const char*>(strAreaCode);

			return p->InitializeProj(sSourceCrs, sTargetCrs, sAuthority, sAreaCode);
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
