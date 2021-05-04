using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ProjNetApi
{
    public class ProjNetApi
    {
        public static void Main()
        {
            ProjCppApiNet.ProjCppApiNet o = null;

            try
            {
                o = new ProjCppApiNet.ProjCppApiNet();

                bool res = o.InitializeProj("EPSG:7789", "EPSG:4936", "EPSG", "1352");

                if (!res)
                    Console.WriteLine("Transformation failed");

                double xInput = 2987993.64255, yInput = 655946.42161, zInput = 5578690.43270;
                double epoch = 2020.0;
                double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

                o.Transform(xInput, yInput, zInput, epoch, ref xOutput, ref yOutput, ref zOutput);

                Console.WriteLine($"{xInput}, {yInput}, {zInput}, {epoch}, {xOutput}, {yOutput}, {zOutput}");

                Console.ReadKey();

                o.DestroyProj();
            }
            catch (Exception ex)
            { 
                Console.WriteLine(ex.Message);
            }
            finally
            {
                o.DestroyProj();
            }
        }
    }
}
