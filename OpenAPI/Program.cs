using System;
using OpenApi;

namespace OpenApi
{
    class Program
    {
        public static bool WriteToConsole = true;

        static void Main(string[] args)
        {
            Console.WriteLine("Transformation is started");
           
            TestOpenApiV1();
            
            Console.WriteLine("Transformation completed!");
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
                var z = 200d + rand.NextDouble()*50d;
                var t = 2020d;

                //var res = oa.GetAsync(xrand, yrand, z, t, OpenApiV1.Epsg._4258, OpenApiV1.Epsg._25832).Result;
                var res = oa.GetAsync(xrand, yrand, z, t, OpenApiV1.Epsg._7912, OpenApiV1.Epsg._4937).Result;                

                if (WriteToConsole)
                    Console.WriteLine($"Input: x {xrand} y {yrand} z {z} t {t}, Output: x {res.X} y {res.Y} z {res.Z}");
            }            
        }        
    }
}
