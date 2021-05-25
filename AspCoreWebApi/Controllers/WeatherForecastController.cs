using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using AspCoreWebApi.Models;
using Microsoft.EntityFrameworkCore;

namespace AspCoreWebApi.Controllers
{
    [Authorize]
    [ApiController]
    [Route("[controller]")]
    public class WeatherForecastController : ControllerBase
    {
        private readonly WeatherForecastContext _context;
        private readonly ILogger<WeatherForecastController> _logger;

        private static readonly string[] Summaries = new[]
        {
            "Freezing", "Bracing", "Chilly", "Cool", "Mild", "Warm", "Balmy", "Hot", "Sweltering", "Scorching"
        };

        public WeatherForecastController(WeatherForecastContext context, ILogger<WeatherForecastController> logger)
        {
            _logger = logger;
            _context = context;
        }

        /// <summary>
        /// Get all the weather forecasts
        /// </summary>
        /// <returns>List of weather forecasts</returns>
        [HttpGet]
        [ProducesResponseType(StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status500InternalServerError)]
        public async Task<ActionResult<IEnumerable<WeatherForecastDTO>>> GetWeatherForecasts()
        {
            return await _context.DbWeatherForeCasts.Select(x => WeatherForecastToDTO(x)).ToListAsync();
        }

        /// <summary>
        /// Get a weather forecast with id
        /// </summary>
        /// <param name="id"></param>
        /// <returns>Object of WeatherForecast</returns>
        [HttpGet("{id}")]
        [ProducesResponseType(StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status500InternalServerError)]
        public async Task<ActionResult<WeatherForecastDTO>> GetWeatherForecast(int id)
        {
            var weatherForecast = await _context.DbWeatherForeCasts.FindAsync(id);

            if (weatherForecast == null)
            {
                return NotFound();
            }
            return WeatherForecastToDTO(weatherForecast);
        }

        /// <summary>
        /// Add new weather forecast item  
        /// </summary>
        /// <param name="weatherForecastDTO">Forecast to be added</param>
        /// <returns>Result</returns>
        [HttpPost]
        public async Task<ActionResult<WeatherForecastDTO>> PostWeatherForecast(WeatherForecastDTO weatherForecastDTO)
        {
            var weatherForecast = new WeatherForecast
            {
                Summary = weatherForecastDTO.Summary,
                Date = weatherForecastDTO.Date,
                TemperatureC = weatherForecastDTO.TemperatureC
            };

            _context.DbWeatherForeCasts.Add(weatherForecast);
            await _context.SaveChangesAsync();

            return CreatedAtAction(nameof(GetWeatherForecast), new { id = weatherForecast.Id }, WeatherForecastToDTO(weatherForecast));
        }

        /// <summary>
        /// Update a weather forecast with id
        /// </summary>
        /// <param name="id">Id of the forecast to be updated</param>
        /// <param name="weatherForecastDTO">Forecast to be updated</param>
        /// <returns>Updated object</returns>
        [HttpPut("{id}")]
        public async Task<IActionResult> UpdateWeatherForecast(int id, WeatherForecastDTO weatherForecastDTO)
        {
            if (id != weatherForecastDTO.Id)
            {
                return BadRequest();
            }

            var weatherForecast = await _context.DbWeatherForeCasts.FindAsync(id);
            if (weatherForecast == null)
            {
                return NotFound();
            }

            weatherForecast.Summary = weatherForecastDTO.Summary;
            weatherForecast.TemperatureC = weatherForecastDTO.TemperatureC;
            weatherForecast.Date = weatherForecastDTO.Date;

            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException) when (!WeatherForecastExists(id))
            {
                return NotFound();
            }
            return NoContent();
        }

        /// <summary>
        /// Removes a forecast from the list
        /// </summary>
        /// <param name="id">Id of the forecast to be deleted</param>
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteWeatherForecast(int id)
        {
            var weatherForecast = await _context.DbWeatherForeCasts.FindAsync(id);
            if (weatherForecast == null)
            {
                return NotFound();
            }

            _context.DbWeatherForeCasts.Remove(weatherForecast);
            await _context.SaveChangesAsync();

            return NoContent();
        }

        private bool WeatherForecastExists(int id) => _context.DbWeatherForeCasts.Any(e => e.Id == id);

        private static WeatherForecastDTO WeatherForecastToDTO(WeatherForecast weatherForecast) =>
            new WeatherForecastDTO
            {
                Id = weatherForecast.Id,
                Date = weatherForecast.Date,
                TemperatureC = weatherForecast.TemperatureC,
                Summary = weatherForecast.Summary
            };
    };
}
