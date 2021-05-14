using System;
using System.Runtime.InteropServices;

namespace ProjCoreApi
{
    public class ProjCoreApi
    {
        private ProjCppApiCore.ProjCppApiCore _projCoreApi;

        public ProjCoreApi()
        {
            _projCoreApi = new ProjCppApiCore.ProjCppApiCore();
        }

        public static void Main()
        {
            ProjCppApiCore.ProjCppApiCore? o = null;

            try
            {            
                o = new ProjCppApiCore.ProjCppApiCore();
                
                var getPath = o.GetProjDbPath();

                Console.WriteLine($"GetPath: {getPath}");

                bool res = o.InitializeProj("EPSG:7789", "EPSG:4936", "EPSG:1352");
                
                if (!res)
                {
                    Console.WriteLine("Initialization failed");
                    Console.ReadKey();
                    return;
                }

                double xInput = 2987993.64255, yInput = 655946.42161, zInput = 5578690.43270;
                double epoch = 2020.0;
                double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;
                
                res = o.Transform(xInput, yInput, zInput, epoch, ref xOutput, ref yOutput, ref zOutput);

                if (!res)
                {
                    Console.WriteLine("Transformation failed");
                    Console.ReadKey();
                    return;
                }

                Console.WriteLine($"{xInput}, {yInput}, {zInput}, {epoch}, {xOutput}, {yOutput}, {zOutput}");
                
                Console.ReadKey();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message +  "\n" + ex.StackTrace);                
            }
            finally
            {
                if (o != null)
                    o.DestroyProj();
            }
        }
    }
}
