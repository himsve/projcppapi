using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace AspCoreWebApi.Models
{
    public class ProjInitContexts : DbContext
    {
        public ProjInitContexts(DbContextOptions<ProjInitContexts> options)
             : base(options)
        {
        }
        
        public DbSet<ProjInit> DbProjInit { get; set; }
    }

    public class ProjInitDTO
    {
        public int Id { get; set; } = 1;
        public string EpsgAutoritySource { get; set; } = "";
        public string EpsgCodeSource { get; set; } = "";
        public string EpsgAutorityTarget { get; set; } = "";
        public string EpsgCodeTarget { get; set; } = "";
        public string EpsgAutorityArea { get; set; } = "";
        public string EpsgCodeArea { get; set; } = "";
    }
}
