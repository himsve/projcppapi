using AspCoreWebApi.Models;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using ProjCppApiCore;

namespace AspCoreWebApi.Controllers
{
    [Authorize]
    [ApiController]
    [Route("[controller]")]
    public class ProjInitController : ControllerBase
    {
        private readonly ProjContexts _context;
        private readonly ILogger<ProjInitController> _logger;
        private ProjCppApiCore.ProjCppApiCore _projAppApiCore;

        public ProjInitController(ProjContexts context, ILogger<ProjInitController> logger)
        {
            _logger = logger;
            _context = context;
            _projAppApiCore ??= new ProjCppApiCore.ProjCppApiCore();
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

            _projAppApiCore.InitializeProj(
                projInit.EpsgAutoritySource + ":" + projInit.EpsgCodeSource,
                projInit.EpsgAutorityTarget + ":" + projInit.EpsgCodeTarget,
                projInit.EpsgAutorityArea + ":" + projInit.EpsgCodeArea);

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

                _projAppApiCore.InitializeProj(
                    projInit.EpsgAutoritySource + ":" + projInit.EpsgCodeSource,
                    projInit.EpsgAutorityTarget + ":" + projInit.EpsgCodeTarget,
                    projInit.EpsgAutorityArea + ":" + projInit.EpsgCodeArea);

                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException) when (!ProjInitExists(id))
            {
                return NotFound();
            }
            return Ok();
            return NoContent(); // TODO: Check this return value
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
              EpsgCodeTarget = projInfo.EpsgCodeTarget
          };
    }

    [Authorize]
    [ApiController]
    [Route("[controller]")]
    public class ProjTransController : ControllerBase
    {
        private readonly ProjContexts _context;
        private readonly ILogger<ProjTransController> _logger;
        private ProjCppApiCore.ProjCppApiCore _projAppApiCore; //

        public ProjTransController(ProjContexts context, ILogger<ProjTransController> logger, ProjCppApiCore.ProjCppApiCore testObject)
        {
            _logger = logger;
            _context = context;
        }

        /// <summary>
        /// Get proj transformed coordinates
        /// </summary>
        /// <returns>Transformed coordinates</returns>
        [HttpGet]
        [ProducesResponseType(StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status500InternalServerError)]
        public async Task<ActionResult<IEnumerable<ProjTransformDTO>>> GetProjTransformed()
        {
            return await _context.DbProjTransform.Select(x => ProjTransformToDTO(x)).ToListAsync();
        }

        /// <summary>
        /// Get proj transformed coordinates with id
        /// </summary>
        /// <returns>Transformed coordinates</returns>
        [HttpGet("{id}")]
        [ProducesResponseType(StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status500InternalServerError)]
        public async Task<ActionResult<ProjTransformDTO>> GetProjTransformed(int id)
        {
            var projTrans = await _context.DbProjTransform.FindAsync(id);

            if (projTrans == null)
            {
                return NotFound();
            }
            return ProjTransformToDTO(projTrans);
        }

        /// <summary>
        /// Add new transformed coordinate item  
        /// </summary>
        /// <param name="projTransformDTO">Transformed item to be added</param>
        /// <returns>Result</returns>
        [HttpPost]
        public async Task<ActionResult<ProjTransformDTO>> PostProjTransformed(ProjTransformDTO projTransformDTO)
        {
            var projTrans = new ProjTransform
            {
                Id = projTransformDTO.Id,
                XInput = projTransformDTO.XInput,
                YInput = projTransformDTO.YInput,
                ZInput = projTransformDTO.ZInput,
                Epoch = projTransformDTO.Epoch
            };

            _context.DbProjTransform.Add(projTrans);
           
            await _context.SaveChangesAsync();

            return CreatedAtAction(nameof(GetProjTransformed), new { id = projTrans.Id }, ProjTransformToDTO(projTrans));
        }

        /// <summary>
        /// Update a transformed item with id
        /// </summary>
        /// <param name="id">Id of the forecast to be updated</param>
        /// <param name="projTransformDTO">Transformed item to be updated</param>
        /// <returns>Updated object</returns>
        [HttpPut("{id}")]
        public async Task<IActionResult> UpdateProjTransformed(int id, ProjTransformDTO projTransformDTO)
        {
            if (id != projTransformDTO.Id)
            {
                return BadRequest();
            }

            var projTrans = await _context.DbProjTransform.FindAsync(id);
            if (projTrans == null)
            {
                return NotFound();
            }

            projTrans.XInput = projTransformDTO.XInput;
            projTrans.YInput = projTransformDTO.YInput;
            projTrans.ZInput = projTransformDTO.ZInput;
            projTrans.XOutput = projTransformDTO.XOutput;
            projTrans.YOutput = projTransformDTO.YOutput;
            projTrans.ZOutput = projTransformDTO.ZOutput;
            projTrans.Epoch = projTransformDTO.Epoch;

            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException) when (!ProjTransformationExists(id))
            {
                return NotFound();
            }
            return NoContent();
        }

        /// <summary>
        /// Removes a forecast from the list
        /// </summary>
        /// <param name="id">Id of the forecast to be deleted</param>
         /// <returns>Result</returns>
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteProjTransformed(int id)
        {
            var projTrans = await _context.DbProjTransform.FindAsync(id);
            if (projTrans == null)
            {
                return NotFound();
            }
            _context.DbProjTransform.Remove(projTrans);
           
            await _context.SaveChangesAsync();

            return NoContent();
        }

        private bool ProjTransformationExists(int id) => _context.DbProjTransform.Any(e => e.Id == id);

        private static ProjTransformDTO ProjTransformToDTO(ProjTransform projTransform) =>
        new ProjTransformDTO
        {
            Id = projTransform.Id,
            XInput = projTransform.XInput,
            YInput = projTransform.YInput,
            ZInput = projTransform.ZInput,
            XOutput = projTransform.XOutput,
            YOutput = projTransform.YOutput,
            ZOutput = projTransform.ZOutput,
            Epoch = projTransform.Epoch
        };
    }
}
