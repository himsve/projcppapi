using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;

namespace AspCoreWebApi.Models
{
    [Keyless]
    public class ProjTransform
    {
        public int EpsgCodeSource { get; set; } = 0;
        public int EpsgCodeTarget { get; set; } = 0;
        public int EpsgCodeArea { get; set; } = 0;
        public double XInput { get; set; }
        public double YInput { get; set; }
        public double ZInput { get; set; }
        public double XOutput { get; set; }
        public double YOutput { get; set; }
        public double ZOutput { get; set; }
        public double Epoch { get; set; }
    }
}
