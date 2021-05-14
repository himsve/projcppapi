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
    public class ProjController : ControllerBase
    {
        private readonly ProjInitContexts _context;
        private readonly ILogger<ProjController> _logger;

        public ProjController(ProjInitContexts context, ILogger<ProjController> logger)
        {
            _logger = logger;
            _context = context;
        }

        [HttpGet]
        [ProducesResponseType(StatusCodes.Status200OK)]
        [ProducesResponseType(StatusCodes.Status500InternalServerError)]
        public async Task<ActionResult<IEnumerable<ProjInitDTO>>> GetProjInit()
        {
            return await _context.DbProjInit.Select(x => ProjInfoToDTO(x)).ToListAsync();
        }

        private static ProjInitDTO ProjInfoToDTO(ProjInit projInfo) =>
          new ProjInitDTO
          {
              EpsgAutorityArea = projInfo.EpsgAutorityArea,
              EpsgCodeArea = projInfo.EpsgCodeArea,
              EpsgAutoritySource = projInfo.EpsgAutoritySource,
              EpsgCodeSource = projInfo.EpsgCodeSource,
              EpsgAutorityTarget = projInfo.EpsgAutorityTarget,
              EpsgCodeTarget= projInfo.EpsgCodeTarget
          };
    }
}
