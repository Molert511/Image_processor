#pragma once
#include "BmpImage.h"

class Filter {
public:
    virtual void Apply(BmpImage &image, const std::vector<std::string> &parameters);
    virtual ~Filter();
};

class Crop : public Filter {
public:
    void Apply(BmpImage &image, const std::vector<std::string> &parameters) override;
};

class Grayscale : public Filter {
public:
    void Apply(BmpImage &image, const std::vector<std::string> &parameters) override;
};

class Negative : public Filter {
public:
    void Apply(BmpImage &image, const std::vector<std::string> &parameters) override;
};

class Sharpening : public Filter {
public:
    void Apply(BmpImage &image, const std::vector<std::string> &parameters) override;
};

class EdgeDetection : public Filter {
public:
    void Apply(BmpImage &image, const std::vector<std::string> &parameters) override;
};

class GaussianBlur : public Filter {
public:
    void Apply(BmpImage &image, const std::vector<std::string> &parameters) override;
};