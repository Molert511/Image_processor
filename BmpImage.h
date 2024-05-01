#pragma once

#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

const char SUITABLEFILETYPE[]{"BM"};
const double MAXBYTEVALUE = 255.f;
const uint32_t NORMALIZESIZE = 6;
const uint32_t NUMBEROFBITSPERPIXEL = 24;
const uint32_t ONEBYTE = 8;

#pragma pack(push, 1)
struct BITMAPFILEHEADER {
    uint16_t bfType;
    [[maybe_unused]] uint32_t bfSize;
    [[maybe_unused]] uint16_t bfReserved1;
    [[maybe_unused]] uint16_t bfReserved2;
    uint32_t bfOffBits;
    // sizes of fields exactly fit for their values
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BITMAPINFOHEADER {
    [[maybe_unused]] uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    [[maybe_unused]] uint16_t biPlanes;
    uint16_t biBitCount;
    [[maybe_unused]] uint32_t biCompression;
    [[maybe_unused]] uint32_t biSizeImage;
    [[maybe_unused]] int32_t biXPelsPerMeter;
    [[maybe_unused]] int32_t biYPelsPerMeter;
    [[maybe_unused]] uint32_t biClrUsed;
    [[maybe_unused]] uint32_t biClrImportant;
    // sizes of fields exactly fit for their values
};
#pragma pack(pop)

struct Pixel {
    double bgr_blue_value;
    double bgr_green_value;
    double bgr_red_value;
};

class BmpImage {
    friend class Crop;
    friend class Grayscale;
    friend class Negative;
    friend class Sharpening;
    friend class EdgeDetection;
    friend class GaussianBlur;
    friend void MultiplyToMatrix(const std::vector<std::vector<int32_t>> &matrix, BmpImage &image);

public:
    [[maybe_unused]] explicit BmpImage(std::string &file_name);
    void Read(std::string &file_name);
    void Write(std::string &file_name);
    ~BmpImage();

private:
    BITMAPFILEHEADER header_{};
    BITMAPINFOHEADER info_header_{};
    std::vector<std::vector<Pixel>> colors_of_pixels_;
};
