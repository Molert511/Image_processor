#include "Filters.h"

void Filter::Apply(BmpImage &image, const std::vector<std::string> &parameters) {
}

Filter::~Filter() {
}

void Crop::Apply(BmpImage &image, const std::vector<std::string> &parameters) {
    int32_t new_width = static_cast<int32_t>(stoi(parameters[0]));
    int32_t new_height = static_cast<int32_t>(stoi(parameters[1]));
    // changing sizes of image
    int32_t diff{};
    diff = std::max(static_cast<int32_t>(0), image.info_header_.biHeight - new_height);
    if (new_height < image.info_header_.biHeight) {
        image.info_header_.biHeight = new_height;
    }
    if (new_width < image.info_header_.biWidth) {
        image.info_header_.biWidth = new_width;
    }
    std::vector<std::vector<Pixel>> new_colors_of_pixels(image.info_header_.biHeight,
                                                         std::vector<Pixel>(image.info_header_.biWidth));
    for (int32_t i_row = 0; i_row < image.info_header_.biHeight; ++i_row) {
        for (int32_t i_column = 0; i_column < image.info_header_.biWidth; ++i_column) {
            new_colors_of_pixels[i_row][i_column] = image.colors_of_pixels_[i_row + diff][i_column];
        }
    }
    image.colors_of_pixels_ = new_colors_of_pixels;
}

void Grayscale::Apply(BmpImage &image, const std::vector<std::string> &parameters) {
    const double coef_b = 0.114f;
    const double coef_g = 0.587f;
    const double coef_r = 0.299f;
    for (int32_t i_row = 0; i_row < image.info_header_.biHeight; ++i_row) {
        for (int32_t i_column = 0; i_column < image.info_header_.biWidth; ++i_column) {
            // finding new values
            const double new_value = coef_b * image.colors_of_pixels_[i_row][i_column].bgr_blue_value +
                                     coef_g * image.colors_of_pixels_[i_row][i_column].bgr_green_value +
                                     coef_r * image.colors_of_pixels_[i_row][i_column].bgr_red_value;
            // changing pixel colors
            image.colors_of_pixels_[i_row][i_column].bgr_blue_value = new_value;
            image.colors_of_pixels_[i_row][i_column].bgr_green_value = new_value;
            image.colors_of_pixels_[i_row][i_column].bgr_red_value = new_value;
        }
    }
}

void Negative::Apply(BmpImage &image, const std::vector<std::string> &parameters) {
    for (int32_t i_row = 0; i_row < image.info_header_.biHeight; ++i_row) {
        for (int32_t i_column = 0; i_column < image.info_header_.biWidth; ++i_column) {
            // changing pixel colors
            image.colors_of_pixels_[i_row][i_column].bgr_blue_value =
                1. - image.colors_of_pixels_[i_row][i_column].bgr_blue_value;
            image.colors_of_pixels_[i_row][i_column].bgr_green_value =
                1. - image.colors_of_pixels_[i_row][i_column].bgr_green_value;
            image.colors_of_pixels_[i_row][i_column].bgr_red_value =
                1. - image.colors_of_pixels_[i_row][i_column].bgr_red_value;
        }
    }
}

void MultiplyToMatrix(const std::vector<std::vector<int32_t>> &matrix, BmpImage &image) {
    std::vector<std::vector<Pixel>> copy_of_colors_of_pixels(image.colors_of_pixels_);
    for (int32_t i_row = 0; i_row < image.info_header_.biHeight; ++i_row) {
        for (int32_t i_column = 0; i_column < image.info_header_.biWidth; ++i_column) {
            // finding new values
            double final_value_b{0};
            double final_value_g{0};
            double final_value_r{0};
            if (i_row - 1 >= 0 && i_column - 1 >= 0) {
                final_value_b += matrix[0][0] * copy_of_colors_of_pixels[i_row - 1][i_column - 1].bgr_blue_value;
                final_value_g += matrix[0][0] * copy_of_colors_of_pixels[i_row - 1][i_column - 1].bgr_green_value;
                final_value_r += matrix[0][0] * copy_of_colors_of_pixels[i_row - 1][i_column - 1].bgr_red_value;
            } else if (i_row - 1 >= 0) {
                final_value_b += matrix[0][0] * copy_of_colors_of_pixels[i_row - 1][i_column].bgr_blue_value;
                final_value_g += matrix[0][0] * copy_of_colors_of_pixels[i_row - 1][i_column].bgr_green_value;
                final_value_r += matrix[0][0] * copy_of_colors_of_pixels[i_row - 1][i_column].bgr_red_value;
            } else if (i_column - 1 >= 0) {
                final_value_b += matrix[0][0] * copy_of_colors_of_pixels[i_row][i_column - 1].bgr_blue_value;
                final_value_g += matrix[0][0] * copy_of_colors_of_pixels[i_row][i_column - 1].bgr_green_value;
                final_value_r += matrix[0][0] * copy_of_colors_of_pixels[i_row][i_column - 1].bgr_red_value;
            } else {
                final_value_b += matrix[0][0] * copy_of_colors_of_pixels[i_row][i_column].bgr_blue_value;
                final_value_g += matrix[0][0] * copy_of_colors_of_pixels[i_row][i_column].bgr_green_value;
                final_value_r += matrix[0][0] * copy_of_colors_of_pixels[i_row][i_column].bgr_red_value;
            }

            if (i_row - 1 >= 0) {
                final_value_b += matrix[0][1] * copy_of_colors_of_pixels[i_row - 1][i_column].bgr_blue_value;
                final_value_g += matrix[0][1] * copy_of_colors_of_pixels[i_row - 1][i_column].bgr_green_value;
                final_value_r += matrix[0][1] * copy_of_colors_of_pixels[i_row - 1][i_column].bgr_red_value;
            } else {
                final_value_b += matrix[0][1] * copy_of_colors_of_pixels[i_row][i_column].bgr_blue_value;
                final_value_g += matrix[0][1] * copy_of_colors_of_pixels[i_row][i_column].bgr_green_value;
                final_value_r += matrix[0][1] * copy_of_colors_of_pixels[i_row][i_column].bgr_red_value;
            }

            if (i_row - 1 >= 0 && i_column + 1 < image.info_header_.biWidth) {
                final_value_b += matrix[0][2] * copy_of_colors_of_pixels[i_row - 1][i_column + 1].bgr_blue_value;
                final_value_g += matrix[0][2] * copy_of_colors_of_pixels[i_row - 1][i_column + 1].bgr_green_value;
                final_value_r += matrix[0][2] * copy_of_colors_of_pixels[i_row - 1][i_column + 1].bgr_red_value;
            } else if (i_row - 1 >= 0) {
                final_value_b += matrix[0][2] * copy_of_colors_of_pixels[i_row - 1][i_column].bgr_blue_value;
                final_value_g += matrix[0][2] * copy_of_colors_of_pixels[i_row - 1][i_column].bgr_green_value;
                final_value_r += matrix[0][2] * copy_of_colors_of_pixels[i_row - 1][i_column].bgr_red_value;
            } else if (i_column + 1 < image.info_header_.biWidth) {
                final_value_b += matrix[0][2] * copy_of_colors_of_pixels[i_row][i_column + 1].bgr_blue_value;
                final_value_g += matrix[0][2] * copy_of_colors_of_pixels[i_row][i_column + 1].bgr_green_value;
                final_value_r += matrix[0][2] * copy_of_colors_of_pixels[i_row][i_column + 1].bgr_red_value;
            } else {
                final_value_b += matrix[0][2] * copy_of_colors_of_pixels[i_row][i_column].bgr_blue_value;
                final_value_g += matrix[0][2] * copy_of_colors_of_pixels[i_row][i_column].bgr_green_value;
                final_value_r += matrix[0][2] * copy_of_colors_of_pixels[i_row][i_column].bgr_red_value;
            }

            if (i_column - 1 >= 0) {
                final_value_b += matrix[1][0] * copy_of_colors_of_pixels[i_row][i_column - 1].bgr_blue_value;
                final_value_g += matrix[1][0] * copy_of_colors_of_pixels[i_row][i_column - 1].bgr_green_value;
                final_value_r += matrix[1][0] * copy_of_colors_of_pixels[i_row][i_column - 1].bgr_red_value;
            } else {
                final_value_b += matrix[1][0] * copy_of_colors_of_pixels[i_row][i_column].bgr_blue_value;
                final_value_g += matrix[1][0] * copy_of_colors_of_pixels[i_row][i_column].bgr_green_value;
                final_value_r += matrix[1][0] * copy_of_colors_of_pixels[i_row][i_column].bgr_red_value;
            }

            final_value_b += matrix[1][1] * copy_of_colors_of_pixels[i_row][i_column].bgr_blue_value;
            final_value_g += matrix[1][1] * copy_of_colors_of_pixels[i_row][i_column].bgr_green_value;
            final_value_r += matrix[1][1] * copy_of_colors_of_pixels[i_row][i_column].bgr_red_value;

            if (i_column + 1 < image.info_header_.biWidth) {
                final_value_b += matrix[1][2] * copy_of_colors_of_pixels[i_row][i_column + 1].bgr_blue_value;
                final_value_g += matrix[1][2] * copy_of_colors_of_pixels[i_row][i_column + 1].bgr_green_value;
                final_value_r += matrix[1][2] * copy_of_colors_of_pixels[i_row][i_column + 1].bgr_red_value;
            } else {
                final_value_b += matrix[1][2] * copy_of_colors_of_pixels[i_row][i_column].bgr_blue_value;
                final_value_g += matrix[1][2] * copy_of_colors_of_pixels[i_row][i_column].bgr_green_value;
                final_value_r += matrix[1][2] * copy_of_colors_of_pixels[i_row][i_column].bgr_red_value;
            }

            if (i_row + 1 < image.info_header_.biHeight && i_column - 1 >= 0) {
                final_value_b += matrix[2][0] * copy_of_colors_of_pixels[i_row + 1][i_column - 1].bgr_blue_value;
                final_value_g += matrix[2][0] * copy_of_colors_of_pixels[i_row + 1][i_column - 1].bgr_green_value;
                final_value_r += matrix[2][0] * copy_of_colors_of_pixels[i_row + 1][i_column - 1].bgr_red_value;
            } else if (i_row + 1 < image.info_header_.biHeight) {
                final_value_b += matrix[2][0] * copy_of_colors_of_pixels[i_row + 1][i_column].bgr_blue_value;
                final_value_g += matrix[2][0] * copy_of_colors_of_pixels[i_row + 1][i_column].bgr_green_value;
                final_value_r += matrix[2][0] * copy_of_colors_of_pixels[i_row + 1][i_column].bgr_red_value;
            } else if (i_column - 1 >= 0) {
                final_value_b += matrix[2][0] * copy_of_colors_of_pixels[i_row][i_column - 1].bgr_blue_value;
                final_value_g += matrix[2][0] * copy_of_colors_of_pixels[i_row][i_column - 1].bgr_green_value;
                final_value_r += matrix[2][0] * copy_of_colors_of_pixels[i_row][i_column - 1].bgr_red_value;
            } else {
                final_value_b += matrix[2][0] * copy_of_colors_of_pixels[i_row][i_column].bgr_blue_value;
                final_value_g += matrix[2][0] * copy_of_colors_of_pixels[i_row][i_column].bgr_green_value;
                final_value_r += matrix[2][0] * copy_of_colors_of_pixels[i_row][i_column].bgr_red_value;
            }

            if (i_row + 1 < image.info_header_.biHeight) {
                final_value_b += matrix[2][1] * copy_of_colors_of_pixels[i_row + 1][i_column].bgr_blue_value;
                final_value_g += matrix[2][1] * copy_of_colors_of_pixels[i_row + 1][i_column].bgr_green_value;
                final_value_r += matrix[2][1] * copy_of_colors_of_pixels[i_row + 1][i_column].bgr_red_value;
            } else {
                final_value_b += matrix[2][1] * copy_of_colors_of_pixels[i_row][i_column].bgr_blue_value;
                final_value_g += matrix[2][1] * copy_of_colors_of_pixels[i_row][i_column].bgr_green_value;
                final_value_r += matrix[2][1] * copy_of_colors_of_pixels[i_row][i_column].bgr_red_value;
            }

            if (i_row + 1 < image.info_header_.biHeight && i_column + 1 < image.info_header_.biWidth) {
                final_value_b += matrix[2][2] * copy_of_colors_of_pixels[i_row + 1][i_column + 1].bgr_blue_value;
                final_value_g += matrix[2][2] * copy_of_colors_of_pixels[i_row + 1][i_column + 1].bgr_green_value;
                final_value_r += matrix[2][2] * copy_of_colors_of_pixels[i_row + 1][i_column + 1].bgr_red_value;
            } else if (i_row + 1 < image.info_header_.biHeight) {
                final_value_b += matrix[2][2] * copy_of_colors_of_pixels[i_row + 1][i_column].bgr_blue_value;
                final_value_g += matrix[2][2] * copy_of_colors_of_pixels[i_row + 1][i_column].bgr_green_value;
                final_value_r += matrix[2][2] * copy_of_colors_of_pixels[i_row + 1][i_column].bgr_red_value;
            } else if (i_column + 1 < image.info_header_.biWidth) {
                final_value_b += matrix[2][2] * copy_of_colors_of_pixels[i_row][i_column + 1].bgr_blue_value;
                final_value_g += matrix[2][2] * copy_of_colors_of_pixels[i_row][i_column + 1].bgr_green_value;
                final_value_r += matrix[2][2] * copy_of_colors_of_pixels[i_row][i_column + 1].bgr_red_value;
            } else {
                final_value_b += matrix[2][2] * copy_of_colors_of_pixels[i_row][i_column].bgr_blue_value;
                final_value_g += matrix[2][2] * copy_of_colors_of_pixels[i_row][i_column].bgr_green_value;
                final_value_r += matrix[2][2] * copy_of_colors_of_pixels[i_row][i_column].bgr_red_value;
            }
            image.colors_of_pixels_[i_row][i_column].bgr_blue_value =
                std::min(static_cast<double>(1), std::max(static_cast<double>(0), final_value_b));
            image.colors_of_pixels_[i_row][i_column].bgr_green_value =
                std::min(static_cast<double>(1), std::max(static_cast<double>(0), final_value_g));
            image.colors_of_pixels_[i_row][i_column].bgr_red_value =
                std::min(static_cast<double>(1), std::max(static_cast<double>(0), final_value_r));
        }
    }
}

void Sharpening::Apply(BmpImage &image, const std::vector<std::string> &parameters) {
    const std::vector<std::vector<int32_t>> sharpening_matrix = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    MultiplyToMatrix(sharpening_matrix, image);
}

void EdgeDetection::Apply(BmpImage &image, const std::vector<std::string> &parameters) {
    double threshold = std::stof(parameters[0]);
    Grayscale().Apply(image, parameters);
    const std::vector<std::vector<int32_t>> edge_detection_matrix = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    MultiplyToMatrix(edge_detection_matrix, image);
    for (int32_t i_row = 0; i_row < image.info_header_.biHeight; ++i_row) {
        for (int32_t i_column = 0; i_column < image.info_header_.biWidth; ++i_column) {
            // checking if values of pixels bigger or less than threshold
            if (image.colors_of_pixels_[i_row][i_column].bgr_blue_value > threshold) {
                image.colors_of_pixels_[i_row][i_column].bgr_blue_value = 1;
                image.colors_of_pixels_[i_row][i_column].bgr_green_value = 1;
                image.colors_of_pixels_[i_row][i_column].bgr_red_value = 1;
            } else {
                image.colors_of_pixels_[i_row][i_column].bgr_blue_value = 0;
                image.colors_of_pixels_[i_row][i_column].bgr_green_value = 0;
                image.colors_of_pixels_[i_row][i_column].bgr_red_value = 0;
            }
        }
    }
}

void MakeKernel(std::vector<double> &kernel, double sigma) {  // function to make linear kernel
    int32_t radius = static_cast<int32_t>(NORMALIZESIZE * sigma + 1);
    if (radius % 2 != 1) {
        radius += 1;
    }
    kernel.assign(radius, 0);
    for (int x = -(radius / 2); x <= radius / 2; x++) {
        double value = (1. / std::sqrt(2 * M_PI * sigma * sigma)) *
                       std::pow(M_E, -((x * x) / (2 * sigma * sigma)));  // gaussian function by one variable
        kernel[x + radius / 2] = value;
    }
}

void GaussianBlur::Apply(BmpImage &image, const std::vector<std::string> &parameters) {
    double sigma = std::stof(parameters[0]);
    std::vector<double> kernel;
    MakeKernel(kernel, sigma);
    std::vector<std::vector<Pixel>> copy_of_colors_of_pixels(image.colors_of_pixels_);
    // firstly we use kernel like a horizontal mask
    for (int32_t i_row = 0; i_row < image.info_header_.biHeight; ++i_row) {
        for (int32_t i_column = 0; i_column < image.info_header_.biWidth; ++i_column) {
            // finding new values
            double new_value_b = 0;
            double new_value_g = 0;
            double new_value_r = 0;
            for (int32_t i = -static_cast<int32_t>(kernel.size()) / 2; i <= static_cast<int32_t>(kernel.size() / 2);
                 ++i) {
                int32_t curcol = i_column + i;
                if (curcol >= 0 && curcol < image.info_header_.biWidth) {
                    new_value_b +=
                        kernel[i + kernel.size() / 2] * copy_of_colors_of_pixels[i_row][curcol].bgr_blue_value;
                    new_value_g +=
                        kernel[i + kernel.size() / 2] * copy_of_colors_of_pixels[i_row][curcol].bgr_green_value;
                    new_value_r +=
                        kernel[i + kernel.size() / 2] * copy_of_colors_of_pixels[i_row][curcol].bgr_red_value;
                } else if (curcol < 0) {
                    new_value_b += kernel[i + kernel.size() / 2] * copy_of_colors_of_pixels[i_row][0].bgr_blue_value;
                    new_value_g += kernel[i + kernel.size() / 2] * copy_of_colors_of_pixels[i_row][0].bgr_green_value;
                    new_value_r += kernel[i + kernel.size() / 2] * copy_of_colors_of_pixels[i_row][0].bgr_red_value;
                } else {
                    new_value_b += kernel[i + kernel.size() / 2] *
                                   copy_of_colors_of_pixels[i_row][image.info_header_.biWidth - 1].bgr_blue_value;
                    new_value_g += kernel[i + kernel.size() / 2] *
                                   copy_of_colors_of_pixels[i_row][image.info_header_.biWidth - 1].bgr_green_value;
                    new_value_r += kernel[i + kernel.size() / 2] *
                                   copy_of_colors_of_pixels[i_row][image.info_header_.biWidth - 1].bgr_red_value;
                }
            }
            // changing value of pixels
            image.colors_of_pixels_[i_row][i_column].bgr_blue_value = new_value_b;
            image.colors_of_pixels_[i_row][i_column].bgr_green_value = new_value_g;
            image.colors_of_pixels_[i_row][i_column].bgr_red_value = new_value_r;
        }
    }
    // secondly we use kernel like a vertical mask
    std::vector<std::vector<Pixel>> second_copy_of_colors_of_pixels(image.colors_of_pixels_);
    for (int32_t i_row = 0; i_row < image.info_header_.biHeight; ++i_row) {
        for (int32_t i_column = 0; i_column < image.info_header_.biWidth; ++i_column) {
            // finding new values
            double new_value_b = 0;
            double new_value_g = 0;
            double new_value_r = 0;
            for (int32_t i = -static_cast<int32_t>(kernel.size()) / 2; i <= static_cast<int32_t>(kernel.size() / 2);
                 ++i) {
                int32_t currow = i_row + i;
                if (currow >= 0 && currow < image.info_header_.biHeight) {
                    new_value_b += kernel[i + kernel.size() / 2] *
                                   second_copy_of_colors_of_pixels[currow][i_column].bgr_blue_value;
                    new_value_g += kernel[i + kernel.size() / 2] *
                                   second_copy_of_colors_of_pixels[currow][i_column].bgr_green_value;
                    new_value_r +=
                        kernel[i + kernel.size() / 2] * second_copy_of_colors_of_pixels[currow][i_column].bgr_red_value;
                } else if (currow < 0) {
                    new_value_b +=
                        kernel[i + kernel.size() / 2] * second_copy_of_colors_of_pixels[0][i_column].bgr_blue_value;
                    new_value_g +=
                        kernel[i + kernel.size() / 2] * second_copy_of_colors_of_pixels[0][i_column].bgr_green_value;
                    new_value_r +=
                        kernel[i + kernel.size() / 2] * second_copy_of_colors_of_pixels[0][i_column].bgr_red_value;
                } else {
                    new_value_b +=
                        kernel[i + kernel.size() / 2] *
                        second_copy_of_colors_of_pixels[image.info_header_.biHeight - 1][i_column].bgr_blue_value;
                    new_value_g +=
                        kernel[i + kernel.size() / 2] *
                        second_copy_of_colors_of_pixels[image.info_header_.biHeight - 1][i_column].bgr_green_value;
                    new_value_r +=
                        kernel[i + kernel.size() / 2] *
                        second_copy_of_colors_of_pixels[image.info_header_.biHeight - 1][i_column].bgr_red_value;
                }
            }
            // changing values of pixels
            image.colors_of_pixels_[i_row][i_column].bgr_blue_value = new_value_b;
            image.colors_of_pixels_[i_row][i_column].bgr_green_value = new_value_g;
            image.colors_of_pixels_[i_row][i_column].bgr_red_value = new_value_r;
        }
    }
}