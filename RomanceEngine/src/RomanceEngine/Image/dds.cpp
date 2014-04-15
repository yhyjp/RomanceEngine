#include <RomanceEngine/Image/dds.h>
#include <gl/glew.h>
#include <GL/GL.h>
#include <cstdio>
#include <cstring>

namespace RomanceEngine {
namespace Image {

bool DDSImage::ReadDDS(const std::string& filename)
{
  FILE *fp;
  char magic[4];
  int mipmapFactor;
  long curr, end;
  DDSurfaceDesc ddsd;

  fp = fopen(filename.c_str(), "rb");
  if ( !fp )
  {
    printf("Error : 指定されたファイルを開けませんでした. %s\n", filename.c_str());
    return false;
  }

  // magic.
  fread(&magic, sizeof(char), 4, fp);
  if ( strncmp(magic, "DDS ", 4 ) != 0 )
  {
    printf("Error : DDSファイルではありません\n");
    fclose(fp);
    return false;
  }

  // read header.
  fread(&ddsd, sizeof(ddsd), 1, fp);

  // w,h.
  height = ddsd.height;
  width = ddsd.width;
  numMipmaps = ddsd.mipMapLevels;

  // detect format.
  switch ( ddsd.format.fourCC )
  {
  case kFOURCC_DXT1:
    format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    internalFormat = 3;
    mipmapFactor = 2;
    break;

  case kFOURCC_DXT3:
    format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    internalFormat = 4;
    mipmapFactor = 4;
    break;

  case kFOURCC_DXT5:
    format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    internalFormat = 4;
    mipmapFactor = 4;
    break;

  default:
    printf("Error : 未対応の形式です。DXT1, DXT3, DXT5のいずれかを使用してください\n");
    return false;
  }

  // calc size.
  curr = ftell(fp);
  fseek(fp, 0, SEEK_END);
  end = ftell(fp);
  fseek(fp, curr, SEEK_SET);
  imageSize = end - curr;
  imageData = new uint8_t [imageSize];

  // load image.
  fread(imageData, sizeof(uint8_t), imageSize, fp);
  
  fclose(fp);

  return true;
}

void DDSImage::DecompressDDS()
{
  int blockSize;
  int offset = 0;
  uint32_t mWidth = width, mHeight = height, mSize = 0;

  // DXT1.
  if (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
    blockSize = 8;
  // DXT3, DXT5.
  else
    blockSize = 16;

  // decompress.
  for (int i=0; i < (int)numMipmaps; i++)
  {
    mSize = ((mWidth+3)/4 ) * ( (mHeight+3)/4) * blockSize;
    glCompressedTexImage2D(GL_TEXTURE_2D, i, format, mWidth, mHeight, 0, mSize, imageData + offset);

    if (mWidth >> 1)  mWidth = (mWidth >> 1);
    else    mWidth = 1;

    if (mHeight >> 1) mHeight = (mHeight >> 1);
    else    mHeight = 1;

    offset += mSize;
  }
}

uint32_t DDSImage::Load(const std::string& filename)
{
  //　ファイルの読み込み
  if (!ReadDDS(filename))
    return 0;

  //　テクスチャを生成
  glGenTextures(1, &ID);

  //　テクスチャをバインド
  glBindTexture(GL_TEXTURE_2D, ID);

  //　拡大・縮小する方法の指定
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  //　テクスチャ環境の設定
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  //　解凍作業
  DecompressDDS();

  if (imageData)
  {
    delete [] imageData;
    imageData = NULL;
  }

  //
  glBindTexture(GL_TEXTURE_2D, 0);

  //　生成したテクスチャ番号を返す
  return ID;
}

} // RomanceEngine
} // Image
