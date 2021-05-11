using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace AspCoreWebApi.Controllers
{
    [Authorize]
    [ApiController]
    [Route("[controller]")]
    public class WeatherForecastController : ControllerBase
    {
        private static readonly string[] Summaries = new[]
        {
            "Freezing", "Bracing", "Chilly", "Cool", "Mild", "Warm", "Balmy", "Hot", "Sweltering", "Scorching"
        };

        private readonly ILogger<WeatherForecastController> _logger;

        public WeatherForecastController(ILogger<WeatherForecastController> logger)
        {
            _logger = logger;
        }

        /// <summary>
        /// Get all the weather forecasts
        /// </summary>
        /// <returns>List of weather forecasts</returns>
        [HttpGet]
        public IEnumerable<WeatherForecast> Get()
        {
            return weathers;
        }

        [HttpGet("{id}")]
        [ProducesResponseType(StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status500InternalServerError)]
        public WeatherForecast Get(int id)
        {
            return weathers.FirstOrDefault(x => x.Id == id);
        }

        /// <summary>
        /// Adds a forecast to the list
        /// </summary>
        /// <param name="value">Forecast to be added</param>
        [HttpPost]
        public void Post([FromBody] WeatherForecast value)
        {
            weathers.Add(value);
        }

        /// <summary>
        /// Removes a forecast from the list
        /// </summary>
        /// <param name="id">Id of the forcast to be deleted</param>
        [HttpDelete("{id}")]
        public void Delete(int id)
        {
            weathers.RemoveAll(m => m.Id == id);
        }

        private static List<WeatherForecast> weathers
        {
            get
            {
                var ind = 0;
                var rng = new Random();

                var w = Enumerable.Range(1, 10).Select(index => new WeatherForecast
                {
                    Id = ind++,
                    Date = DateTime.Now.AddDays(index),
                    TemperatureC = rng.Next(-20, 55),
                    Summary = Summaries[rng.Next(Summaries.Length)]
                }).ToList();

                return w;
            }
        }
    };    
}
