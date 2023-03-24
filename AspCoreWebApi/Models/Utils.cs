using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;

namespace AspCoreWebApi.Models
{
    public class Enum<T> where T : Enum
    {
        public static IEnumerable<T> GetAllValuesAsIEnumerable()
        {
            return Enum.GetValues(typeof(T)).Cast<T>();
        }
    }
}
