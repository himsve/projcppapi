using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace AspCoreWebApi.Models
{
    public class ProjInit
    {
        public string EpsgAutoritySource { get; set; } = "";
        public string EpsgCodeSource { get; set; } = "";
        public string EpsgAutorityTarget { get; set; } = "";
        public string EpsgCodeTarget { get; set; } = "";
        public string EpsgAutorityArea { get; set; } = "";
        public string EpsgCodeArea { get; set; } = "";
    }
}
