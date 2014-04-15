// DDS Image.
#ifndef __RM_IMAGE_DDS__D74BFAD7671B4890B8290100B48EC9AE__
#define __RM_IMAGE_DDS__D74BFAD7671B4890B8290100B48EC9AE__

#include <cstdint>
#include <string>

namespace RomanceEngine {
namespace Image {
  
const uint32_t kFOURCC_DXT1 = (('D'<<0) | ('X'<<8) | ('T'<<16) | ('1'<<24));
const uint32_t kFOURCC_DXT3 = (('D'<<0) | ('X'<<8) | ('T'<<16) | ('3'<<24));
const uint32_t kFOURCC_DXT5 = (('D'<<0) | ('X'<<8) | ('T'<<16) | ('5'<<24));

struct DDPixelFormat
{
  uint32_t size;
  uint32_t flgas;
  uint32_t fourCC;
  uint32_t bpp;
  uint32_t redMask;
  uint32_t greenMask;
  uint32_t blueMask;
  uint32_t alphaMask;
};

struct DDSCaps
{
  uint32_t caps;
  uint32_t caps2;
  uint32_t caps3;
  uint32_t caps4;
};

struct DDColorKey
{
  uint32_t lowVal;
  uint32_t highVal;
};

struct DDSurfaceDesc
{
  uint32_t size;
  uint32_t flags;
  uint32_t height;
  uint32_t width;
  uint32_t pitch;
  uint32_t depth;
  uint32_t mipMapLevels;
  uint32_t alphaBitDepth;
  uint32_t reserved;
  uint32_t surface;

  DDColorKey ckDestOverlay;
  DDColorKey ckDestBlt;
  DDColorKey ckSrcOverlay;
  DDColorKey ckSrcBlt;

  DDPixelFormat format;
  DDSCaps caps;

  uint32_t textureStage;
};

class DDSImage
{
public:
  uint32_t ID;
  uint32_t imageSize;
  uint8_t *imageData;
  uint32_t format;
  uint32_t internalFormat;
  uint32_t width;
  uint32_t height;
  uint32_t bpp;
  uint32_t numMipmaps;
  bool released_;

  DDSImage();
  ~DDSImage();
  void release();
  uint32_t Load(const std::string& filename);

private:
  void DecompressDDS();
  bool ReadDDS(const std::string& filename);
};


} // RomanceEngine
} // Image


#endif // __RM_IMAGE_DDS__D74BFAD7671B4890B8290100B48EC9AE__
