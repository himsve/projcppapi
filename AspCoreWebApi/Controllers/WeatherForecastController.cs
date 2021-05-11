using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using AspCoreWebApi.Models;

namespace AspCoreWebApi.Controllers
{
    [Authorize]
    [ApiController]
    [Route("[controller]")]
    public class WeatherForecastController : ControllerBase
    {
        private List<WeatherForecast> _weathersList;
        private readonly TodoContext _context;
        private readonly ILogger<WeatherForecastController> _logger;

        private static readonly string[] Summaries = new[]
        {
            "Freezing", "Bracing", "Chilly", "Cool", "Mild", "Warm", "Balmy", "Hot", "Sweltering", "Scorching"
        };

        public WeatherForecastController(TodoContext context, ILogger<WeatherForecastController> logger)
        {
            _logger = logger;
            _context = context;
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

        /*
        [HttpGet("{id}")]
        [ProducesResponseType(StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status500InternalServerError)]
        public WeatherForecast Get(int id)
        {
            return weathers.FirstOrDefault(x => x.Id == id);
        }
        */

        // GET: api/TodoItems/5
        [HttpGet("{id}")]
        [ProducesResponseType(StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status500InternalServerError)]
        public async Task<ActionResult<WeatherForecast>> GetTodoItem(long id)
        {
            var todoItem = await _context.TodoItems.FindAsync(id);

            if (todoItem == null)
            {
                return NotFound();
            }
            return todoItem;
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

        private List<WeatherForecast> weathers
        {
            get
            {
                if (_weathersList == null)
                {
                    _weathersList = new List<WeatherForecast>();

                    var ind = 0;
                    var rng = new Random();

                    _weathersList = Enumerable.Range(1, 10).Select(index => new WeatherForecast
                    {
                        Id = ind++,
                        Date = DateTime.Now.AddDays(index),
                        TemperatureC = rng.Next(-20, 55),
                        Summary = Summaries[rng.Next(Summaries.Length)]
                    }).ToList();
                }

                return _weathersList;
            }
            set
            {
                _weathersList = value;
            }
        }
    };    
}
