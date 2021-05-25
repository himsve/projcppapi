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
    public class ProjInitController : ControllerBase
    {
        private readonly ProjContexts _context;
        private readonly ILogger<ProjInitController> _logger;

        public ProjInitController(ProjContexts context, ILogger<ProjInitController> logger)
        {
            _logger = logger;
            _context = context;
        }

        /// <summary>
        /// Get proj transformation parameters
        /// </summary>
        /// <returns>Proj parameters</returns>
        [HttpGet]
        [ProducesResponseType(StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status500InternalServerError)]
        public async Task<ActionResult<ProjInitDTO>> GetProjInit()
        {
            var projInit = await _context.DbProjInit.FirstOrDefaultAsync();

            if (projInit == null)
            {
                return NotFound();
            }
            return ProjInfoToDTO(projInit);
        }

        /// <summary>
        /// Set proj transformation parameters
        /// </summary> 
        /// <param name="projInitDTO">ProjInit to be added</param>
        /// <returns>Result</returns>
        [HttpPost]
        public async Task<ActionResult<ProjInitDTO>> PostProjInit(ProjInitDTO projInitDTO)
        {
            var projInit = await _context.DbProjInit.FirstOrDefaultAsync();

            if (projInit == null)
            {
                projInit = new ProjInit
                {
                    EpsgAutorityArea = projInitDTO.EpsgAutorityArea,
                    EpsgAutoritySource = projInitDTO.EpsgAutoritySource,
                    EpsgAutorityTarget = projInitDTO.EpsgAutorityTarget,
                    EpsgCodeArea = projInitDTO.EpsgCodeArea,
                    EpsgCodeSource = projInitDTO.EpsgCodeSource,
                    EpsgCodeTarget = projInitDTO.EpsgCodeTarget
                };
                _context.DbProjInit.Add(projInit);
            }

            await _context.SaveChangesAsync();

            return CreatedAtAction(nameof(GetProjInit), new { id = projInit.Id }, ProjInfoToDTO(projInit));
        }

        /// <summary>
        /// Update proj transformation parameters
        /// </summary>
        /// <returns>Proj parameters</returns>
        [HttpPut]
        public async Task<IActionResult> UpdateProjInit(int id, ProjInitDTO projInitDTO)
        {
            if (id != projInitDTO.Id)
                return BadRequest();

            var projInit = await _context.DbProjInit.FindAsync(id);
           
            if (projInit == null)
                return NotFound();

            projInit.Id = projInitDTO.Id;
            projInit.EpsgAutoritySource = projInitDTO.EpsgAutoritySource;
            projInit.EpsgAutorityArea = projInitDTO.EpsgAutorityArea;
            projInit.EpsgAutorityTarget = projInitDTO.EpsgAutorityTarget;
            projInit.EpsgCodeTarget = projInitDTO.EpsgCodeTarget;
            projInit.EpsgCodeSource = projInitDTO.EpsgCodeSource;
            projInit.EpsgCodeArea = projInitDTO.EpsgCodeArea;

            try
            {
                _context.Entry(projInit).State = EntityState.Modified;
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException) when (!ProjInitExists(id))
            {
                return NotFound();
            }
            return Ok();
            return NoContent();
        }

        private bool ProjInitExists(int id) => _context.DbProjInit.Any(e => e.Id == id);

        private static ProjInitDTO ProjInfoToDTO(ProjInit projInfo) =>
          new ProjInitDTO
          {
              Id = projInfo.Id,
              EpsgAutorityArea = projInfo.EpsgAutorityArea,
              EpsgCodeArea = projInfo.EpsgCodeArea,
              EpsgAutoritySource = projInfo.EpsgAutoritySource,
              EpsgCodeSource = projInfo.EpsgCodeSource,
              EpsgAutorityTarget = projInfo.EpsgAutorityTarget,
              EpsgCodeTarget= projInfo.EpsgCodeTarget
          };

    }

    [Authorize]
    [ApiController]
    [Route("[controller]")]
    public class ProjTransController : ControllerBase
    {
        private readonly ProjContexts _context;
        private readonly ILogger<ProjTransController> _logger;

        public ProjTransController(ProjContexts context, ILogger<ProjTransController> logger)
        {
            _logger = logger;
            _context = context;
        }

        [HttpGet]
        /// <summary>
        /// Get proj transformed coordinates
        /// </summary>
        /// <returns>Transformed coordinates</returns>
        public async Task<ActionResult<ProjTransformDTO>> GetProjTransformed()
        {
            var projTransform = await _context.DbProjTransform.FirstOrDefaultAsync();
            if (projTransform == null)
            {
                return NotFound();
            }
            return ProjTransformToDTO(projTransform);
        }

        private static ProjTransformDTO ProjTransformToDTO(ProjTransform projTransform) =>
            new ProjTransformDTO
            {
                Id = projTransform.Id,
                XInput = projTransform.XInput,
                YInput = projTransform.YInput,
                ZInput = projTransform.ZInput,
                XOutput = projTransform.XOutput,
                YOutput = projTransform.YOutput,
                ZOutput = projTransform.ZOutput
          };
    }
}
