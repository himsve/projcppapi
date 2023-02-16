using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;

namespace AspCoreWebApi.Models
{
    public class ProjContexts : DbContext
    {
        private ProjCppApiCore.ProjCppApiCore _projAppApiCore;

        public ProjContexts(DbContextOptions<ProjContexts> options)
             : base(options)
        {
            _projAppApiCore ??= new ProjCppApiCore.ProjCppApiCore();
        }

        public DbSet<ProjDatum> DbProjDatum { get; set; }
        public DbSet<ProjTransform> DbProjTransform { get; set; }

        // Testing
        public ProjCppApiCore.ProjCppApiCore ProjObject
        {
            get => _projAppApiCore ??= new ProjCppApiCore.ProjCppApiCore();
        }
        
        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<ProjDatum>(entity =>
            {
                if (!Database.IsInMemory())
                {
                    entity.HasNoKey();
                    entity.Ignore(e => e.EpsgCode);
                }
                else
                {
                    entity.HasKey(e => e.EpsgCode);
                }
            });
            base.OnModelCreating(modelBuilder);
        }
    }

    [Keyless]
    public class ProjDatumDTO
    {
        public int EpsgCode { get; set; } = 0;
        public string EpsgName { get; set; } = "";
    }

    [Keyless]
    public class ProjTransformDTO
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
