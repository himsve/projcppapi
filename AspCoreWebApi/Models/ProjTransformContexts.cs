using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace AspCoreWebApi.Models
{
    public class ProjTransformContexts : DbContext
    {
        public ProjTransformContexts(DbContextOptions<ProjTransformContexts> options)
            : base(options)
        {
        
        
        }

        public DbSet<ProjTransform> DbProjTransform { get; set; }
    }


    public class ProjTransformDTO
    {
        public double XInput { get; set; }
        public double YInput { get; set; }
        public double ZInput { get; set; }
        public double XOutput { get; set; }
        public double YOutput { get; set; }
        public double ZOutput { get; set; }
    }
}
