#ifndef __RM_BITMAP__3AE5665C1FB644A4BA840989C5BE32BB__
#define __RM_BITMAP__3AE5665C1FB644A4BA840989C5BE32BB__

#include <cstdint>
#include <string>

namespace RomanceEngine {
namespace Image {

class Bitmap
{
private:
  struct FileHeader
  {
    uint16_t type_;      // 'BM'.
    uint32_t size_;      // �t�@�C���T�C�Y(byte).
    uint16_t reserved1_; // always 0.
    uint16_t reserved2_; // always 0.
    uint32_t offBits_;   // �摜�f�[�^�܂ł̃I�t�Z�b�g.
  };

  // for OS/2.
  struct CoreHeader
  {
    uint32_t size_;     // always 12.
    uint16_t width_;    // �摜�̕�px.
    uint16_t height_;   // �摜�̍���px�i���Ȃ�ォ�牺�j.
    uint32_t planes_;   // always 1.
    uint32_t bitCount_; // 1��f�ӂ�̃f�[�^�T�C�Y.
  };

  // for Windows Bitmap.
  struct InfoHeader
  {
    uint32_t size_;           // always 40.
     int32_t width_;          // �摜�̕�px.
     int32_t height_;         // �摜�̍���px�i���Ȃ�ォ�牺�j.
    uint16_t planes_;         // always 1.
    uint16_t bitCount_;       // 1��f�ӂ�̃f�[�^�T�C�Y.
    uint32_t compression_;    // ���k�`��
    uint32_t sizeImage_;      // �摜�f�[�^���̃T�C�Y byte.
     int32_t xPixelPerMeter_; // �������𑜓x.
     int32_t yPixelPerMeter_; // �c�����𑜓x.
    uint32_t colorUsed_;      // �i�[����Ă���p���b�g��.
    uint32_t colorImportant_; // �d�v�ȃp���b�g�̃C���f�b�N�X.
  };
  
  struct RGBTriple
  {
    uint8_t blue_;
    uint8_t green_;
    uint8_t red_;
  };

  struct RGBQuad
  {
    uint8_t blue_;
    uint8_t green_;
    uint8_t red_;
    uint8_t reserved_;
  };


public:
  Bitmap();
  Bitmap(const std::string& fileName);
  Bitmap(const int width, const int height);

  ~Bitmap();

  void save(const std::string& fileName);

  int32_t getWidth() const { return infoHeader_.width_; }
  int32_t getHeight() const { return infoHeader_.height_; }
  void setPixel(const int32_t x, const int32_t y, const uint8_t r, const uint8_t g, const uint8_t b);
  void getPixel(const int32_t x, const int32_t y, uint8_t& resR, uint8_t& resG, uint8_t& resB);

private:
  void load(const std::string& fileName);
  int getExtraByte() const { return (4-getWidth()*3%4)%4; }

private:
  FileHeader fileHeader_;
  InfoHeader infoHeader_;
  uint8_t* image_;
};


} // Image
} // RomanceEngine

#endif // __RM_BITMAP__3AE5665C1FB644A4BA840989C5BE32BB__
