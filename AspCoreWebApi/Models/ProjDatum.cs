using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using Newtonsoft.Json.Converters;

namespace AspCoreWebApi.Models
{
    //[JsonConverter(typeof(StringEnumConverter))]
    public enum EpsgCodesEnum
    {
        [Description("EUREF8989 Lat/Lon 2D")] Euref89LatLon2D = 4258,
        [Description("ED50 Lat/Lon 2D ")] Ed50LatLon2D = 4230
    }

    public static class DatumListClass
    {
        public static List<string> DatumList
        {
            get
            {
                var list = new List<string>();
                
                list.Add("lcelkm");
                list.Add("lceld3rekm");

                return list;
            }
        }
    }     

    [Keyless]
    public class ProjDatum
    {
        public ProjDatum()
        {
        }

        public ProjDatum(int epsgCode) 
        {
            EpsgCode = epsgCode;
        }

        public ProjDatum(int epsgCode, string epsgName)
        {
            EpsgCode = epsgCode;
            EpsgName = epsgName;
        }

        public int EpsgCode { get; set; } = 0;
        public string EpsgName { get; set; } = "";
    }
}
