using System;

namespace OpenAPI
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
            var oa = new OpenAPI.openapiClient(new System.Net.Http.HttpClient());

           var getSync = oa.GetAsync();
        }
    }
}
