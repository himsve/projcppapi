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
            ProjCppApiCore.ProjCppApiCore o = null;

            try
            {
                o = new ProjCppApiCore.ProjCppApiCore();

                var getPath = o.GetProjDbPath();

                Console.WriteLine($"GetPath: {getPath}");

                Console.WriteLine("Enter source EPSG code: ");
                string sourceEpsg = Console.ReadLine();

                Console.WriteLine("Enter target EPSG code: ");
                string targetEpsg = Console.ReadLine();

                Console.WriteLine("Enter area EPSG code: ");
                string areaEpsg = Console.ReadLine();

                bool res = o.InitializeProj(sourceEpsg, targetEpsg, areaEpsg);

                if (!res)
                {
                    Console.WriteLine("Initialization failed");
                    return;
                }

                Console.WriteLine("Enter source coordinate: ");
                var inputCoord = Console.ReadLine().Split(new char[] { ' ', ';' }, StringSplitOptions.RemoveEmptyEntries);

                double xInput = 2987993.64255, yInput = 655946.42161, zInput = 5578690.43270, epoch = 2020.0;

                if (inputCoord.Length >= 2)
                {
                    if (!double.TryParse(inputCoord[0], out xInput) || !double.TryParse(inputCoord[1], out yInput))
                    {
                        Console.WriteLine("Input parsing failed");
                        return;
                    }
                }
                if (inputCoord.Length >= 3)
                    if (!double.TryParse(inputCoord[2], out zInput))
                    {
                        Console.WriteLine("Input parsing failed");
                        return;
                    }

                if (inputCoord.Length >= 4)
                    if (!double.TryParse(inputCoord[3], out epoch))
                    {
                        Console.WriteLine("Input parsing failed");
                        return;
                    }

                double xOutput = 0.0, yOutput = 0.0, zOutput = 0.0;

                res = o.Transform(xInput, yInput, zInput, epoch, ref xOutput, ref yOutput, ref zOutput);

                if (!res)
                {
                    Console.WriteLine("Transformation failed");
                    return;
                }

                Console.WriteLine($"Target coordinate: {xOutput} {yOutput} {zOutput}");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            finally
            {
                Console.ReadKey();
                o.DestroyProj();
            }
        }
    }
}
