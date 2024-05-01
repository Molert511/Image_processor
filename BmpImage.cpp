#include "BmpImage.h"

[[maybe_unused]] BmpImage::BmpImage(std::string &file_name) {
    Read(file_name);
}
void BmpImage::Read(std::string &file_name) {
    // open binary file
    std::ifstream binary_file(file_name, std::ios::binary);
    if (binary_file.is_open()) {
        // there we sequentially write the bytes into the fields of structure BITMAPFILEHEADER
        binary_file.read(reinterpret_cast<char *>(&header_), sizeof(header_));
        if (header_.bfType != ((SUITABLEFILETYPE[1] << ONEBYTE) | SUITABLEFILETYPE[0])) {
            throw std::runtime_error("It is not a BMP file!");
        }
        // there we sequentially write the bytes into the fields of structure BITMAPINFOHEADER
        binary_file.read(reinterpret_cast<char *>(&info_header_), sizeof(info_header_));
        if (info_header_.biBitCount != NUMBEROFBITSPERPIXEL) {
            throw std::runtime_error("It is not a BMP file!");
        }

        // finding start-pointer of image
        binary_file.seekg(header_.bfOffBits, std::ios::beg);
        // changing fields of BITMAPFILEHEADER and BITMAPINFOHEADER if there are extra data
        header_.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        info_header_.biSize = sizeof(BITMAPINFOHEADER);
        header_.bfSize = header_.bfOffBits;

        // recording the image
        int32_t padding_width = ((4 - (info_header_.biWidth * 3) % 4) % 4);  // padding size in pixels
        uint8_t padding_buffer[3] = {0, 0, 0};                               // buffer for padding (like a bin)
        colors_of_pixels_.assign(info_header_.biHeight, std::vector<Pixel>(info_header_.biWidth));
        for (int32_t i_row = 0; i_row < info_header_.biHeight; ++i_row) {
            for (int32_t i_column = 0; i_column < info_header_.biWidth; ++i_column) {
                uint8_t blue_color_value = 0;
                uint8_t green_color_value = 0;
                uint8_t red_color_value = 0;
                binary_file.read(reinterpret_cast<char *>(&blue_color_value), 1);
                binary_file.read(reinterpret_cast<char *>(&green_color_value), 1);
                binary_file.read(reinterpret_cast<char *>(&red_color_value), 1);
                colors_of_pixels_[i_row][i_column].bgr_blue_value =
                    static_cast<double>(blue_color_value) / MAXBYTEVALUE;
                colors_of_pixels_[i_row][i_column].bgr_green_value =
                    static_cast<double>(green_color_value) / MAXBYTEVALUE;
                colors_of_pixels_[i_row][i_column].bgr_red_value = static_cast<double>(red_color_value) / MAXBYTEVALUE;
            }
            binary_file.read(reinterpret_cast<char *>(padding_buffer), padding_width);
        }
    } else {
        throw std::runtime_error("There is no file on this path or we do not have permission to open it!");
    }
    binary_file.close();
}
void BmpImage::Write(std::string &file_name) {
    // open binary file
    std::ofstream binary_file(file_name, std::ios::binary);
    if (binary_file.is_open()) {
        // write metadata
        binary_file.write(reinterpret_cast<char *>(&header_), sizeof(header_));
        binary_file.write(reinterpret_cast<char *>(&info_header_), sizeof(info_header_));

        // write the bytes of pixels
        int32_t padding_width = ((4 - (info_header_.biWidth * 3) % 4) % 4);  // padding size in pixels
        uint8_t padding_buffer[3] = {0, 0, 0};                               // buffer with elements for paddings
        for (int32_t i_row = 0; i_row < info_header_.biHeight; ++i_row) {
            for (int32_t i_column = 0; i_column < info_header_.biWidth; ++i_column) {
                uint8_t blue_color_value = 0;
                blue_color_value =
                    static_cast<uint8_t>(colors_of_pixels_[i_row][i_column].bgr_blue_value * MAXBYTEVALUE);
                uint8_t green_color_value = 0;
                green_color_value =
                    static_cast<uint8_t>(colors_of_pixels_[i_row][i_column].bgr_green_value * MAXBYTEVALUE);
                uint8_t red_color_value = 0;
                red_color_value = static_cast<uint8_t>(colors_of_pixels_[i_row][i_column].bgr_red_value * MAXBYTEVALUE);
                binary_file.write(reinterpret_cast<char *>(&blue_color_value), 1);
                binary_file.write(reinterpret_cast<char *>(&green_color_value), 1);
                binary_file.write(reinterpret_cast<char *>(&red_color_value), 1);
            }
            binary_file.write(reinterpret_cast<char *>(padding_buffer), padding_width);
        }
    } else {
        throw std::runtime_error("There is no file on this path. Or we do not have permission to open it!");
    }
    binary_file.close();
}

BmpImage::~BmpImage() {
}