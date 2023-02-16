#include "pch.h"
#include "CppUnitTest.h"
#include "ProjCppApi.h"
#include "proj.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/**
* The tests can also fire from "Developer PowerShell for VS2019".
* Type "VSTest.Console ProjCppApiTest" from the debug or release folder.
*/
namespace ProjCppApiTest
{	
	TEST_CLASS(ProjCppApiStressTest)
	{
	private:
		int noOfPoint = 10000000;
		double fRand(double fMin, double fMax)
		{
			double f = (double)rand() / RAND_MAX;
			return fMin + f * (fMax - fMin);
		}
		TEST_METHOD(Transform_EPSG7789_EPSG4936_EPSG1352_Stresstest)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("EPSG:7789", "EPSG:4936", "EPSG:1352");
			Assert::IsTrue(res);

			// TRYS 2987993.64255 655946.42161 5578690.43270 2020.00
			double xInput = 2987993.64255, yInput = 655946.42161, zInput = 5578690.43270;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			for (int i = 0; i < noOfPoint; i++)
			{
				xInput += fRand(-0.001, 0.001);
				yInput += fRand(-0.001, 0.001);
				zInput += fRand(-0.001, 0.001);

				res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);

				Assert::IsTrue(res);
			}
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_EPSG4937_EPSG4273_Stresstest)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("EPSG:4937", "EPSG:4273", "");
			Assert::IsTrue(res);

			double xInput = 10.0, yInput = 61.0, zInput = 0.0;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			for (int i = 0; i < noOfPoint; i++)
			{
				xInput += fRand(-0.0000001, 0.0000001);
				yInput += fRand(-0.0000001, 0.0000001);
				zInput += fRand(-0.0000001, 0.0000001);

				res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
				Assert::IsTrue(res);
			}
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_EPSG4258_EPSG25832_Stresstest)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("EPSG:4258", "EPSG:25832", "");
			Assert::IsTrue(res);

			double xInput = 10.0, yInput = 61.0, zInput = 0.0;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			for (int i = 0; i < noOfPoint; i++)
			{
				xInput += fRand(-0.0000001, 0.0000001);
				yInput += fRand(-0.0000001, 0.0000001);
				zInput += fRand(-0.0000001, 0.0000001);

				res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
				Assert::IsTrue(res);
			}
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_Utm_To_LatLon_Stresstest)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("+proj=utm +ellps=GRS80 +zone=32 +inv");
			Assert::IsTrue(res);

			double xInput = 600000.0, yInput = 6610000.0, zInput = 100.0;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			for (int i = 0; i < noOfPoint; i++)
			{
				xInput += fRand(-0.00001, 0.00001);
				yInput += fRand(-0.00001, 0.00001);
				zInput += fRand(-0.00001, 0.00001);

				res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
				Assert::IsTrue(res);
			}
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_LatLon_To_Utm_Stresstest)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("+proj=utm +ellps=GRS80 +zone=32");
			Assert::IsTrue(res);

			double xInput = 10, yInput = 60, zInput = 100.0;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			for (int i = 0; i < noOfPoint; i++)
			{
				xInput += fRand(-0.0000001, 0.0000001);
				yInput += fRand(-0.0000001, 0.0000001);
				zInput += fRand(-0.0000001, 0.0000001);

				res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
				Assert::IsTrue(res);
			}
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_LatLon_To_Geocentric_Stresstest)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("+proj=cart +ellps=GRS80");
			Assert::IsTrue(res);

			double xInput = 10, yInput = 60, zInput = 100.0;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			for (int i = 0; i < noOfPoint; i++)
			{
				xInput += fRand(-0.0000001, 0.0000001);
				yInput += fRand(-0.0000001, 0.0000001);
				zInput += fRand(-0.0000001, 0.0000001);

				res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
				Assert::IsTrue(res);
			}
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_Utm_To_Utm_Stresstest)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("+proj=pipeline +step +proj=utm +ellps=GRS80 +zone=32 +inv +step +proj=utm +zone=33");
			Assert::IsTrue(res);

			double xInput = 600000.0, yInput = 6610000.0, zInput = 100.0;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			for (int i = 0; i < noOfPoint; i++)
			{
				xInput += fRand(-0.00001, 0.00001);
				yInput += fRand(-0.00001, 0.00001);
				zInput += fRand(-0.00001, 0.00001);

				res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
				Assert::IsTrue(res);
			}
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_Vgridshift_Stresstest)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("+proj=vgridshift +grids=no_kv_HREF2018B_NN2000_EUREF89.tif");
			Assert::IsTrue(res);

			double xInput = 8.0, yInput = 60.0, zInput = 100.0;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			for (int i = 0; i < noOfPoint; i++)
			{
				xInput += fRand(-0.000001, 0.000001);
				yInput += fRand(-0.000001, 0.000001);
				zInput += fRand(-0.000001, 0.000001);

				res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
				Assert::IsTrue(res);
			}
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_Hgridshift_Stresstest)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("+proj=hgridshift +grids=no_kv_ED50_ETRS89.tif");
			Assert::IsTrue(res);

			double xInput = 8.0, yInput = 60.0, zInput = 100.0;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			for (int i = 0; i < noOfPoint; i++)
			{
				xInput += fRand(-0.000001, 0.000001);
				yInput += fRand(-0.000001, 0.000001);
				zInput += fRand(-0.000001, 0.000001);

				res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
				Assert::IsTrue(res);
			}
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_Xyzgridshift_Stresstest)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			double xInput = 3148533.4299, yInput = 555171.4330, zInput = 5500477.0872;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			bool res = wrapper.InitializeProj("+proj=xyzgridshift +grids=no_kv_NKGETRF14_EPSG7922_2000.tif");
			Assert::IsTrue(res);

			for (int i = 0; i < noOfPoint; i++)
			{
				xInput += fRand(-0.001, 0.001);
				yInput += fRand(-0.001, 0.001);
				zInput += fRand(-0.001, 0.001);

				res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
				Assert::IsTrue(res);
			}
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_TIN_Stresstest)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("+proj=tinshift +file=no_kv_ETRS89NO_NGO48_TIN.json");
			Assert::IsTrue(res);

			double xInput = 8.0, yInput = 60.0, zInput = 100.0;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			for (int i = 0; i < noOfPoint; i++)
			{
				xInput += fRand(-0.000001, 0.000001);
				yInput += fRand(-0.000001, 0.000001);
				zInput += fRand(-0.000001, 0.000001);

				res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
				Assert::IsTrue(res);
			}
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_Deformation_Stresstest)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("+proj=deformation +t_epoch=2000.0 +grids=eur_nkg_nkgrf17vel.tif");
			Assert::IsTrue(res);

			double xInput = 3582104.8458, yInput = 532590.0946, zInput = 5232755.0863;
			double epoch = 2025.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			for (int i = 0; i < noOfPoint; i++)
			{
				xInput += fRand(-0.001, 0.001);
				yInput += fRand(-0.001, 0.001);
				zInput += fRand(-0.001, 0.001);
				epoch += fRand(-0.0001, 0.0001);

				res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
				Assert::IsTrue(res);
			}
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
	};

	TEST_CLASS(ProjCppApiTest)
	{
	public:
		TEST_METHOD(ProjGetEllipsName)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;
			
			auto res = wrapper.ProjGetEllps("EPSG:4258");

			Assert::IsNotNull(res);
		}
		TEST_METHOD(ProjGetEllipsParams)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			double a = 0.0;
			double b = 0.0;
			double f = 0.0;

			auto res = wrapper.ProjGetEllpsParams("EPSG:4258", a, b, f);

			Assert::IsTrue(res);

			Assert::IsTrue(a != 0.0);
			Assert::IsTrue(b != 0.0);
			Assert::IsTrue(f != 0.0);
		}
		TEST_METHOD(ProjGetMeridianParams)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			double longitude = 0.0, longitudeUnit = 0.0;
			const char* longitudeUnitName = nullptr;

			auto res = wrapper.ProjGetMeridianParams("EPSG:25833", longitude, longitudeUnit, longitudeUnitName);
			
			Assert::IsTrue(res);

			Assert::IsTrue(longitude != 0.0);
			Assert::IsNotNull(longitudeUnitName);
		}
		TEST_METHOD(ProjGetProjectionParams)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			double falseEasting = 0.0, dScaleFactor = 0.0, dLongitudeOfNaturalOrigin = 0.0;

			auto res = wrapper.ProjGetProjectionParams("EPSG:25833", falseEasting, dScaleFactor, dLongitudeOfNaturalOrigin);

			Assert::IsTrue(res);
		}
		TEST_METHOD(ProjGetArea_EPSG7789_EPSG4936)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			const char* result = wrapper.ProjGetArea("EPSG:7789", "EPSG:4936");

			Assert::AreEqual(result, "Norway - onshore.");
			Assert::IsNotNull(result);

			bool res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(GetAvailableEpsgCodes)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			std::list<std::pair<int, string>> res = wrapper.GetAvailableEpsgCodes();			 

			auto res2 = wrapper.DestroyProj();
			Assert::IsTrue(res2);
		}
		TEST_METHOD(InitializeProj_EPSG7789_EPSG4936_EPSG1352)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("EPSG:7789", "EPSG:4936", "EPSG:1352");

			Assert::IsTrue(res);

		    res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(InitializeProj_EPSG7789_EPSG4936)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("EPSG:7789", "EPSG:4936");

			Assert::IsTrue(res);

		    res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_EPSG7789_EPSG4936_EPSG1352)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("EPSG:7789", "EPSG:4936", "EPSG:1352");
			Assert::IsTrue(res);

			// TRYS 2987993.64255 655946.42161 5578690.43270 2020.00
			double xInput = 2987993.64255, yInput = 655946.42161, zInput = 5578690.43270;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);

			Assert::IsTrue(res);
			Assert::AreEqual(xOutput, 2987994.1200, 1E-04);
			Assert::AreEqual(yOutput, 655946.0593, 1E-04);
			Assert::AreEqual(zOutput, 5578690.01005, 1E-04);
	
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_EPSG7789_EPSG4936_EPSG1080)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("EPSG:7789", "EPSG:4936", "EPSG:1080");
			Assert::IsTrue(res);

			//#ESBC
			// accept      3582104.8458    532590.0946   5232755.0863 2015.0
			// expect      3582105.2916    532589.7310   5232754.8057 2015.0
			double xInput = 3582104.8458, yInput = 532590.0946, zInput = 5232755.0863;
			double epoch = 2015.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
			Assert::IsTrue(res);
			Assert::AreEqual(xOutput, 3582105.2916, 1E-04);
			Assert::AreEqual(yOutput, 532589.7310, 1E-04);
			Assert::AreEqual(zOutput, 5232754.8057, 1E-04);
			
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_EPSG7789_EPSG4936)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;
			
			bool res = wrapper.InitializeProj("EPSG:7789", "EPSG:4936", "");
			Assert::IsTrue(res);

			// TRYS 2987993.64255 655946.42161 5578690.43270 2020.00
			double xInput = 2987993.64255, yInput = 655946.42161, zInput = 5578690.43270;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
			
			Assert::IsTrue(res);
			Assert::AreEqual(xOutput, 2987994.1200, 1E-04);
			Assert::AreEqual(yOutput, 655946.0593, 1E-04);
			Assert::AreEqual(zOutput, 5578690.01005, 1E-04);
			
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_EPSG4937_EPSG4273)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("EPSG:4937", "EPSG:4273");
			Assert::IsTrue(res);

			double xInput = 10.0, yInput = 60.0, zInput = 100.0;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
			Assert::IsTrue(res);

			Assert::AreEqual(xOutput,9.9940136662563610,1E-11);
			Assert::AreEqual(yOutput,60.003804193826781,1E-11);
			Assert::AreEqual(zOutput,100.0, 1E-03);
			
			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}
		TEST_METHOD(Transform_ProjString_LatLon_Utm)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("+proj=utm +zone=32 +ellps=GRS80");
			Assert::IsTrue(res);

			double xInput = 10.0, yInput = 60.0, zInput = 100.0;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
			Assert::IsTrue(res);
			Assert::IsTrue(xOutput != HUGE_VAL);
			Assert::IsTrue(yOutput != HUGE_VAL);
			Assert::IsTrue(zOutput != HUGE_VAL);

			res = wrapper.DestroyProj();
			Assert::IsTrue(res);
		}		
		TEST_METHOD(GetDatabasePath)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;
			
			std::string res = wrapper.GetProjDbPath();

			Assert::IsFalse(res == "");
		}
		TEST_METHOD(SetDatabasePath)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.SetProjDbPath("c:/temp/proj.db");

			if (!res)
			{
				wrapper.DestroyProj();
				Assert::IsTrue(res);
			}
			wrapper.DestroyProj();
		}
		TEST_METHOD(DestroyProj)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.DestroyProj();

			Assert::IsTrue(res);
		}
	};
}
