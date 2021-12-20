using System;
using OpenApi;

namespace OpenApi
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Transformation is started");

            using var httpClient = new System.Net.Http.HttpClient();
            
            var oa = new OpenApiV1.TransformerClient(httpClient);

            var x = 9d; var y = 60d;
            var rand = new Random();
            
            for (int i = 0; i < 1000; i++)
            {
                var xrand = x + rand.NextDouble();
                var yrand = y + rand.NextDouble();
               
                var res = oa.GetAsync(xrand, yrand, OpenApiV1.Epsg._4258, OpenApiV1.Epsg._25832).Result;

               Console.WriteLine($"Input: x {xrand} y {yrand}, Output: x {res.X} y {res.Y}");
            }            
            Console.WriteLine("Finish!");
        }
    }
}
