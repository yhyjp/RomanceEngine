#include <RomanceEngine/Image/bitmap.h>
#include <cstdio>
#include <cassert>

namespace RomanceEngine {
namespace Image {

Bitmap::Bitmap()
  : image_(0)
{}

Bitmap::Bitmap(const std::string& fileName)
  : image_(0)
{
  load(fileName);
}

Bitmap::Bitmap(const int width, const int height)
  : image_(0)
{
  fileHeader_.type_ = (('B'<<0) | ('M'<<8));
  fileHeader_.offBits_ = (2+4+2+2+4) + (40);
  fileHeader_.size_ = fileHeader_.offBits_ + width * height * 3;
  fileHeader_.reserved1_ = fileHeader_.reserved2_ = 0;

  infoHeader_.size_ = 40;
  infoHeader_.width_ = width;
  infoHeader_.height_ = height;
  infoHeader_.planes_ = 1;
  infoHeader_.bitCount_ = 24;
  infoHeader_.compression_ = 0;
  infoHeader_.sizeImage_ = (width*3 + getExtraByte())*height;
  infoHeader_.xPixelPerMeter_ = 0;
  infoHeader_.yPixelPerMeter_ = 0;
  infoHeader_.colorUsed_ = 0;
  infoHeader_.colorImportant_ = 0;

  image_ = new uint8_t[infoHeader_.sizeImage_];
  for (int i=0; i < height; i++)
  {
    for (int j=0; j < width; ++j)
    {
      int p = i*(width*3 + getExtraByte()) + j*3;
      image_[p+0] = 0;
      image_[p+1] = 0;
      image_[p+2] = 0;
    }
    for (int j=0; j < getExtraByte(); ++j)
    {
      int p = i*(width*3 + getExtraByte()) + width*3 + j;
      image_[p] = 0;
    }
  }

}

Bitmap::~Bitmap()
{
  if (image_)
  {
    delete [] image_;
  }
}

void Bitmap::load(const std::string& fileName)
{
  FILE *fp = fopen(fileName.c_str(), "rb");
  if (!fp)
  {
    printf("ファイルが存在しません: %s\n", fileName.c_str());
    return;
  }

  // fileHeader
  fread(&fileHeader_.type_, 2, 1, fp);
  fread(&fileHeader_.size_, 4, 1, fp);
  fread(&fileHeader_.reserved1_, 2, 1, fp);
  fread(&fileHeader_.reserved2_, 2, 1, fp);
  fread(&fileHeader_.offBits_, 4, 1, fp);

  // infoHeader.
  fread(&infoHeader_.size_, 4, 1, fp);
  fread(&infoHeader_.width_, 4, 1, fp);
  fread(&infoHeader_.height_, 4, 1, fp);
  fread(&infoHeader_.planes_, 2, 1, fp);
  fread(&infoHeader_.bitCount_, 2, 1, fp);
  fread(&infoHeader_.compression_, 4, 1, fp);
  fread(&infoHeader_.sizeImage_, 4, 1, fp);
  fread(&infoHeader_.xPixelPerMeter_, 4, 1, fp);
  fread(&infoHeader_.yPixelPerMeter_, 4, 1, fp);
  fread(&infoHeader_.colorUsed_, 4, 1, fp);
  fread(&infoHeader_.colorImportant_, 4, 1, fp);

  if (infoHeader_.bitCount_ != 24)
  {
    printf("24bit bitmap 以外対応していません.\n");
    fclose(fp);
    return;
  }

  image_ = new uint8_t[infoHeader_.sizeImage_];
  fread(image_, 1, infoHeader_.sizeImage_, fp);

  fclose(fp);
}

void Bitmap::save(const std::string& fileName)
{
  assert(image_);

  FILE *fp = fopen(fileName.c_str(), "wb");
  if (!fp)
  {
    printf("ファイルが作れません: %s\n", fileName.c_str());
    return;
  }

  // fileHeader
  fwrite(&fileHeader_.type_, 2, 1, fp);
  fwrite(&fileHeader_.size_, 4, 1, fp);
  fwrite(&fileHeader_.reserved1_, 2, 1, fp);
  fwrite(&fileHeader_.reserved2_, 2, 1, fp);
  fwrite(&fileHeader_.offBits_, 4, 1, fp);

  // infoHeader.
  fwrite(&infoHeader_.size_, 4, 1, fp);
  fwrite(&infoHeader_.width_, 4, 1, fp);
  fwrite(&infoHeader_.height_, 4, 1, fp);
  fwrite(&infoHeader_.planes_, 2, 1, fp);
  fwrite(&infoHeader_.bitCount_, 2, 1, fp);
  fwrite(&infoHeader_.compression_, 4, 1, fp);
  fwrite(&infoHeader_.sizeImage_, 4, 1, fp);
  fwrite(&infoHeader_.xPixelPerMeter_, 4, 1, fp);
  fwrite(&infoHeader_.yPixelPerMeter_, 4, 1, fp);
  fwrite(&infoHeader_.colorUsed_, 4, 1, fp);
  fwrite(&infoHeader_.colorImportant_, 4, 1, fp);

  fwrite(image_, 1, infoHeader_.sizeImage_, fp);

  fclose(fp);
}

void Bitmap::setPixel(const int32_t x, const int32_t y, const uint8_t r, const uint8_t g, const uint8_t b)
{
  assert(image_);
  assert(0<=x && x<getWidth() && 0<=y && y<getHeight());
  const int p = (getWidth()*3 + getExtraByte())*y + x*3;
  image_[p+0] = b;
  image_[p+1] = g;
  image_[p+2] = r;
}

void Bitmap::getPixel(const int32_t x, const int32_t y, uint8_t& resR, uint8_t& resG, uint8_t& resB)
{
  assert(image_);
  assert(0<=x && x<getWidth() && 0<=y && y<getHeight());
  const int p = (getWidth()*3 + getExtraByte())*y + x*3;
  resB = image_[p+0];
  resG = image_[p+1];
  resR = image_[p+2];
}


} // Image
} // RomanceEngine
