using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace AspCoreWebApi.Models
{
    public class ProjInit
    {
        public int Id { get; set; } = 1;
        public string EpsgAutoritySource { get; set; } = "EPSG";
        public string EpsgCodeSource { get; set; } = "";
        public string EpsgAutorityTarget { get; set; } = "EPSG";
        public string EpsgCodeTarget { get; set; } = "";
        public string EpsgAutorityArea { get; set; } = "EPSG";
        public string EpsgCodeArea { get; set; } = "";
    }
}
