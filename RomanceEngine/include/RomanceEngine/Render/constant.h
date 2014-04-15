#ifndef __RM_RENDER_CONSTANT__37F3D42B3B4B410EBA62FD84E0BB5B4C__
#define __RM_RENDER_CONSTANT__37F3D42B3B4B410EBA62FD84E0BB5B4C__

#include <cstdint>

namespace RomanceEngine {
namespace Render {
// #ifdef GL ‚Æ‚©‚µ‚Ä‚¢‚­.  

// DataType.
enum
{
  kRM_BYTE                          = 0x1400,
  kRM_UNSIGNED_BYTE                 = 0x1401,
  kRM_SHORT                         = 0x1402,
  kRM_UNSIGNED_SHORT                = 0x1403,
  kRM_INT                           = 0x1404,
  kRM_UNSIGNED_INT                  = 0x1405,
  kRM_FLOAT                         = 0x1406,
  kRM_2_BYTES                       = 0x1407,
  kRM_3_BYTES                       = 0x1408,
  kRM_4_BYTES                       = 0x1409,
  kRM_DOUBLE                        = 0x140A
};

// BeginMode.
enum
{
  kRM_POINTS                         = 0x0000,
  kRM_LINES                          = 0x0001,
  kRM_LINE_LOOP                      = 0x0002,
  kRM_LINE_STRIP                     = 0x0003,
  kRM_TRIANGLES                      = 0x0004,
  kRM_TRIANGLE_STRIP                 = 0x0005,
  kRM_TRIANGLE_FAN                   = 0x0006,
  kRM_QUADS                          = 0x0007,
  kRML_QUAD_STRIP                    = 0x0008,
  kRM_POLYGON                        = 0x0009
};

} // RomanceEngine
} // Render

#endif //__RM_RENDER_CONSTANT__37F3D42B3B4B410EBA62FD84E0BB5B4C__