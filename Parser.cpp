#include "Parser.h"

// struct to get information and class by name of filter
struct FilterCollection {
    std::map<std::string, Filter *> filter_map{{"-crop", new Crop{}},          {"-gs", new Grayscale()},
                                               {"-neg", new Negative()},       {"-sharp", new Sharpening()},
                                               {"-edge", new EdgeDetection()}, {"-blur", new GaussianBlur()}};
    std::map<std::string, int32_t> count_of_parameters{{"-crop", 2},  {"-gs", 0},   {"-neg", 0},
                                                       {"-sharp", 0}, {"-edge", 1}, {"-blur", 1}};
};

Parser::Parser(int32_t argc, char *argv[]) {
    // parsing
    if (argc < 2) {
        throw "Write input file and output file!";
    }
    std::vector<std::string> input_data;
    std::string input_file{argv[1]};   // name of input file
    std::string output_file{argv[2]};  // name of output file
    BmpImage image(input_file);        // reading input file
    FilterCollection filter;
    for (int32_t i = 3; i < argc; ++i) {
        input_data.push_back(argv[i]);
        if (filter.filter_map.find(input_data.back()) == filter.filter_map.end()) {
            throw "There no such filter!";
        }
        // reading parameters
        std::vector<std::string> parameters;
        for (int32_t ind_param = i + 1; ind_param <= i + filter.count_of_parameters[argv[i]]; ++ind_param) {
            if (ind_param >= argc) {
                throw "Input all parameters!";
            }
            parameters.push_back(argv[ind_param]);
        }
        (*filter.filter_map[argv[i]]).Apply(image, parameters);
        i += filter.count_of_parameters[argv[i]];
    }
    // cleaning "collection of filters"
    for (std::pair<std::string, Filter *> elem : filter.filter_map) {
        delete elem.second;
    }
    // writing final image
    image.Write(output_file);
}
