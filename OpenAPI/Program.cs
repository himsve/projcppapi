using System;
using OpenApi;

namespace OpenApi
{
    class Program
    {
        public static bool WriteToConsole = false;

        static void Main(string[] args)
        {
            Console.WriteLine("Transformation is started");
           
            // TestOpenApiV1();
            TestOpenApiV1Test();

            Console.WriteLine("Finish!");
        }

        static void TestOpenApiV1()
        {
            using var httpClient = new System.Net.Http.HttpClient();

            var oa = new OpenApiV1.TransformerClient(httpClient);

            var x = 9d; var y = 60d;
            var rand = new Random();

            for (int i = 0; i < 100; i++)
            {
                var xrand = x + rand.NextDouble();
                var yrand = y + rand.NextDouble();

                var res = oa.GetAsync(xrand, yrand, OpenApiV1.Epsg._4258, OpenApiV1.Epsg._25832).Result;

                if (WriteToConsole)
                    Console.WriteLine($"Input: x {xrand} y {yrand}, Output: x {res.X} y {res.Y}");
            }            
        }

        static void TestOpenApiV1Test()
        { 
            using var httpClient = new System.Net.Http.HttpClient();

            var oa = new OpenApiV1Test.TransformerClient(httpClient);

            var x = 9d; var y = 60d; var z = 0d; var t = 2020.0d;
            var rand = new Random();

            for (int i = 0; i < 1000; i++)
            {
                var xrand = x + rand.NextDouble();
                var yrand = y + rand.NextDouble();
                var zrand = z + rand.NextDouble() * 100d;

                var res = oa.GetAsync(xrand, yrand, zrand, t, OpenApiV1Test.Epsg._7912, OpenApiV1Test.Epsg._4937).Result;
                
                if (WriteToConsole)
                    Console.WriteLine($"Input: x {xrand} y {yrand} z {zrand}, Output: x {res.X} y {res.Y} z {res.Z}");
            }
        }
    }
}
