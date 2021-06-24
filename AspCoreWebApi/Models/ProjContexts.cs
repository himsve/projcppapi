using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace AspCoreWebApi.Models
{
    public class ProjContexts : DbContext
    {
        public ProjContexts(DbContextOptions<ProjContexts> options)
             : base(options)
        {
        }
        
        public DbSet<ProjInit> DbProjInit { get; set; }
        public DbSet<ProjTransform> DbProjTransform { get; set; }
    }

    public class ProjInitDTO
    {
        public int Id { get; set; } = 1;
        public string EpsgAutoritySource { get; set; } = "EPSG";
        public string EpsgCodeSource { get; set; } = "";
        public string EpsgAutorityTarget { get; set; } = "EPSG";
        public string EpsgCodeTarget { get; set; } = "";
        public string EpsgAutorityArea { get; set; } = "EPSG";
        public string EpsgCodeArea { get; set; } = "";
    }

    public class ProjTransformDTO
    {
        public long Id { get; set; } = 0;
        public double XInput { get; set; }
        public double YInput { get; set; }
        public double ZInput { get; set; }
        public double XOutput { get; set; }
        public double YOutput { get; set; }
        public double ZOutput { get; set; }
        public double Epoch { get; set; }
    }
}
