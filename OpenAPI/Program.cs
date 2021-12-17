using System;

namespace OpenAPI
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Transformation is started");

            using var httpClient = new System.Net.Http.HttpClient();
            var oa = new OpenAPI.TransformerClient(httpClient);
            
            for (int i = 0; i < 100; i++)
            {                
                var res = oa.GetAsync(7d + (double)(i /100000d), 60d + (double)(i / 100000d), OpenAPI.Epsg._4258, OpenAPI.Epsg._25832).Result;
            }
            Console.WriteLine("Finish!");
        }
    }
}
