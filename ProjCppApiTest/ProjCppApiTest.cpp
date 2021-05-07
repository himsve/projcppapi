#include "pch.h"
#include "CppUnitTest.h"
#include "ProjCppApi.h"

#include "proj.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ProjCppApiTest
{
	TEST_CLASS(ProjCppApiTest)
	{
	public:		
		TEST_METHOD(ProjGetArea_EPSG7789_EPSG4936)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			const char* res = wrapper.ProjGetArea("EPSG:7789", "EPSG:4936");

			Assert::AreEqual(res, "Norway - onshore.");
			Assert::IsNotNull(res);

			wrapper.DestroyProj();
		}
		TEST_METHOD(InitializeProj_EPSG7789_EPSG4936_EPSG1352)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("EPSG:7789", "EPSG:4936", "EPSG:1352");

			Assert::IsTrue(res);

			wrapper.DestroyProj();
		}
		TEST_METHOD(InitializeProj_EPSG7789_EPSG4936)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("EPSG:7789", "EPSG:4936");

			Assert::IsTrue(res);

			wrapper.DestroyProj();
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
			Assert::AreEqual(xOutput, 2987994.11660, 1E-04);
			Assert::AreEqual(yOutput, 655946.05964, 1E-04);
			Assert::AreEqual(zOutput, 5578690.00490, 1E-04);

			wrapper.DestroyProj();
		}
		TEST_METHOD(Transform_EPSG7789_EPSG4936_EPSG1352_OUTSIDE)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("EPSG:7789", "EPSG:4936", "EPSG:1352");
			Assert::IsTrue(res);

			//#ESBC
    		// accept      3582104.8458    532590.0946   5232755.0863 2015.0
			double xInput = 3582104.8458, yInput = 532590.0946, zInput = 5232755.0863;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
			Assert::IsFalse(res);

			wrapper.DestroyProj();
		}
		TEST_METHOD(Transform_EPSG7789_EPSG4936_EPSG1352_STRESSTEST)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("EPSG:7789", "EPSG:4936", "EPSG:1352");
			Assert::IsTrue(res);

			// TRYS 2987993.64255 655946.42161 5578690.43270 2020.00
			double xInput = 2987993.64255, yInput = 655946.42161, zInput = 5578690.43270;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			for (int i = 0; i < 10000; i++)
			{
				xInput += 1.0;
				yInput += 1.0;
				zInput += 1.0;

				res = wrapper.Transform(xInput++, yInput++, zInput++, epoch, xOutput, yOutput, zOutput);
			
				Assert::IsTrue(res);
			}
			wrapper.DestroyProj();
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

			wrapper.DestroyProj();
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
			Assert::AreEqual(xOutput, 2987994.11660, 1E-04);
			Assert::AreEqual(yOutput, 655946.05964, 1E-04);
			Assert::AreEqual(zOutput, 5578690.00490, 1E-04);

			wrapper.DestroyProj();
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

			wrapper.DestroyProj();
		}
		TEST_METHOD(Transform_EPSG4937_EPSG4273_STRESSTEST)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.InitializeProj("EPSG:4937", "EPSG:4273", "");
			Assert::IsTrue(res);

			double xInput = 5.0, yInput = 59.0, zInput = 0.0;
			double epoch = 2020.0;
			double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

			for (int i = 0; i < 1000; i++)
			{
				xInput += 0.00001;
				yInput += 0.00001;
				zInput += 0.01;

				res = wrapper.Transform(xInput, yInput, zInput, epoch, xOutput, yOutput, zOutput);
				Assert::IsTrue(res);
			}
			wrapper.DestroyProj();
		}
		TEST_METHOD(Transform_ProjString_LatLon_UTM32)
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

			wrapper.DestroyProj();
		}
		
		TEST_METHOD(DestroyProj)
		{
			ProjCppWrapper::ProjCppWrapper wrapper;

			bool res = wrapper.DestroyProj();

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
	};
}
