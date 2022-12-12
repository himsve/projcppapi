#pragma once

#include "pch.h"
#include "..\ProjCppApi\ProjCppApi.h"
#include <proj.h>
#include <proj/io.hpp>
#include <proj/metadata.hpp>
#include <proj/nn.hpp>
#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Runtime::CompilerServices;
using namespace msclr::interop;

namespace ProjCppApiCore
{
	public ref class EpsgPair {
	private:
		int m_code;
		System::String ^ m_name;
	public:
		EpsgPair(void)
		{
		};
		int GetCode()
		{
			return m_code;
		};
		void SetCode(int code)
		{
			m_code = code;
		};
		System::String ^ GetName()
		{
			return m_name;
		};
		void SetName(System::String ^ name)
		{
			m_name = name;
		};
	};

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
		System::String ^ GetProjDbPath()
		{
			msclr::interop::marshal_context context;
			const char* cProjDbPath = p->GetProjDbPath();

			return gcnew String(cProjDbPath);
		}
		cli::array<EpsgPair^>^ GetAvailableEpsgCodes()
		{
			std::list<std::pair<int, string>> epsgCodes = p->GetAvailableEpsgCodes();

			const int SIZE = epsgCodes.size();
			cli::array<EpsgPair^>^ epsgCodesList = gcnew cli::array<EpsgPair^>(SIZE);

			int i = 0;

			for (auto it = epsgCodes.begin(); it != epsgCodes.end(); ++it)
			{
				epsgCodesList[i] = gcnew EpsgPair;
				epsgCodesList[i]->SetCode(it->first);

				String^ strName = gcnew String(it->second.c_str());
				epsgCodesList[i++]->SetName(strName);
			}			
			return epsgCodesList;
		}
		bool SetProjDbPath(System::String^ strProjDbPath)
		{
			msclr::interop::marshal_context context;

			const char* cProjDbPath = context.marshal_as<const char*>(strProjDbPath);

			return p->SetProjDbPath(cProjDbPath);
		}
		System::String ^ ProjGetArea(System::String^ strSourceCrs, System::String^ strTargetCrs)
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
		bool InitializeProj(System::String ^ strSourceCrs, System::String ^ strTargetCrs, System::String ^ strAuthorityAreaCode)
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
