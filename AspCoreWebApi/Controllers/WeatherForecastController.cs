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
    //[Route("[controller]")]
    [Route("api/[controller]")]
    public class WeatherForecastController : ControllerBase
    {
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
        public async Task<ActionResult<IEnumerable<WeatherForecast>>> GetWeatherForecastItems()
        {
            return await _context.TodoItems.Select(x => x).ToListAsync();
        }

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
        /// Add new weather forecast item  
        /// </summary>
        /// <param name="todoItem">Forecast to be added</param>
        /// <returns>Result</returns>
        [HttpPost]
        public async Task<ActionResult<WeatherForecast>> PostWeatherForecastItem(WeatherForecast todoItem)
        {
            _context.TodoItems.Add(todoItem);
            await _context.SaveChangesAsync();

            return CreatedAtAction(nameof(GetTodoItem), new { id = todoItem.Id }, todoItem);
        }

        [HttpPut("{id}")]
        public async Task<IActionResult> UpdateTodoItem(long id, WeatherForecast todoItemDTO)
        {
            if (id != todoItemDTO.Id)
            {
                return BadRequest();
            }

            var todoItem = await _context.TodoItems.FindAsync(id);
            if (todoItem == null)
            {
                return NotFound();
            }

            todoItem.Summary = todoItemDTO.Summary;
            todoItem.TemperatureC = todoItemDTO.TemperatureC;
            todoItem.Date = todoItemDTO.Date;

            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException) when (!TodoItemExists(id))
            {
                return NotFound();
            }
            return NoContent();
        }


        /// <summary>
        /// Removes a forecast from the list
        /// </summary>
        /// <param name="id">Id of the forcast to be deleted</param>
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteTodoItem(long id)
        {
            var todoItem = await _context.TodoItems.FindAsync(id);
            if (todoItem == null)
            {
                return NotFound();
            }

            _context.TodoItems.Remove(todoItem);
            await _context.SaveChangesAsync();

            return NoContent();
        }

        private bool TodoItemExists(long id) => _context.TodoItems.Any(e => e.Id == id);
    };    
}
