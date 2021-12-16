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
    public class ProjDatumController : ControllerBase
    {
        private readonly ProjContexts _context;
        private readonly ILogger<ProjDatumController> _logger;
        private ProjCppApiCore.ProjCppApiCore _projAppApiCore;

        public ProjDatumController(ProjContexts context, ILogger<ProjDatumController> logger)
        {
            _logger = logger;
            _context = context;
            _projAppApiCore ??= new ProjCppApiCore.ProjCppApiCore();
            
            foreach (var epsg in _projAppApiCore.GetAvailableEpsgCodes())
                _context.DbProjDatum.Add(new ProjDatum(epsg));
            
            _context.SaveChanges();
        }

        /// <summary>
        /// Get proj transformation parameters
        /// </summary>
        /// <returns>Proj parameters</returns>
        [HttpGet]
        [ProducesResponseType(StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status500InternalServerError)]
        public async Task<ActionResult<IEnumerable <ProjDatumDTO>>> GetProjEpsg()
        {
            return await _context.DbProjDatum.Select(x => ProjInfoToDTO(x)).ToListAsync();

            /*
            var projInit = await _context.DbProjDatum.FirstOrDefaultAsync();

            if (projInit == null)
            {
                return NotFound();
            }
            */
            //return ProjInfoToDTO(projInit);
        }

        private bool ProjInitExists(int epsg) => _context.DbProjDatum.Any(e => e.EpsgCode == epsg);

        private static ProjDatumDTO ProjInfoToDTO(ProjDatum projDatum) =>
          new ProjDatumDTO
          {
              EpsgCode = projDatum.EpsgCode
          };
    }


    [Authorize]
    [ApiController]
    [Route("[controller]")]
    public class ProjTransController : ControllerBase
    {
        private readonly ProjContexts _context;
        private readonly ILogger<ProjTransController> _logger;

        // Testing:
        private ProjCppApiCore.ProjCppApiCore _projAppApiCore;
        private DbSet<ProjDatum> _projDatumDbSet;

        public ProjTransController(ProjContexts context, ILogger<ProjTransController> logger /*, ProjCppApiCore.ProjCppApiCore testObject*/)
        {
            _logger = logger;
            _context = context;
            _projAppApiCore ??= new ProjCppApiCore.ProjCppApiCore();

            // Testing:
            _projDatumDbSet = _context.DbProjDatum;

            //  var projDatum = _projDatumDbSet.FindAsync(4258, 4230);
            //  Console.WriteLine(projDatum.Result);
        }

        /// <summary>
        /// Get proj transformed coordinates
        /// </summary>
        /// <returns>Transformed coordinates</returns>        
        [HttpGet]
        [ProducesResponseType(StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status500InternalServerError)]
        public async Task<ActionResult<ProjTransformDTO>> GetProjTransformed(EpsgCodesEnum EpsgSourceTull, EpsgCodesEnum EpsgTarget)
        {
            if (_projDatumDbSet == null)
                return NoContent();

            ProjTransform projTrans = null;

            if (_context.DbProjTransform.Any(x => x.EpsgCodeSource == (int)EpsgSourceTull && x.EpsgCodeTarget == (int)EpsgTarget))
                projTrans = await _context.DbProjTransform.FindAsync(EpsgSourceTull/*, EpsgTarget*/);

            if (projTrans == null)
            {
                projTrans = new ProjTransform()
                {
                    EpsgCodeSource = (int)EpsgSourceTull,
                    EpsgCodeTarget = (int)EpsgTarget
                };
                await _context.DbProjTransform.AddAsync(projTrans);
                await _context.SaveChangesAsync();

                // return NotFound();
            }
            return ProjTransformToDTO(projTrans);
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
            if (false) // (id != projTransformDTO.Id)
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

        private bool ProjTransformationExists(int epsg) => _context.DbProjTransform.Any(e => e.EpsgCodeSource == epsg);

        private static ProjTransformDTO ProjTransformToDTO(ProjTransform projTransform) =>
            new ProjTransformDTO
            {
                Id = projTransform.Id,
                EpsgCodeSource = projTransform.EpsgCodeSource,
                EpsgCodeTarget = projTransform.EpsgCodeTarget,
                EpsgCodeArea = projTransform.EpsgCodeArea, 
                
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
