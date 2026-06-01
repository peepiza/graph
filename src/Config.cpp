#include "Config.h"
#include <iostream>
#include <cstring>
#include <cstdlib>

Config::Config()
    : width_(800)
    , height_(600)
    , vertexRadius_(20.0)
    , bgColor_("#ffffff")
    , vertexColor_("#88aaff")
    , edgeColor_("#333333")
    , directed_(false)
    , outputFile_("output.svg") {}

bool Config::loadFromArgs(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-i") == 0 && i+1 < argc) {
            inputFile_ = argv[++i];
        }
        else if (strcmp(argv[i], "-o") == 0 && i+1 < argc) {
            outputFile_ = argv[++i];
        }
        else if (strcmp(argv[i], "-width") == 0 && i+1 < argc) {
            width_ = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-height") == 0 && i+1 < argc) {
            height_ = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-radius") == 0 && i+1 < argc) {
            vertexRadius_ = std::stod(argv[++i]);
        }
        else if (strcmp(argv[i], "-bgcolor") == 0 && i+1 < argc) {
            bgColor_ = argv[++i];
        }
        else if (strcmp(argv[i], "-vertexcolor") == 0 && i+1 < argc) {
            vertexColor_ = argv[++i];
        }
        else if (strcmp(argv[i], "-edgecolor") == 0 && i+1 < argc) {
            edgeColor_ = argv[++i];
        }
        else if (strcmp(argv[i], "-directed") == 0) {
            directed_ = true;
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printHelp();
            return false;
        }
    }
    
    if (inputFile_.empty()) {
        std::cerr << "Error: missing -i argument\n";
        printHelp();
        return false;
    }
    return true;
}

void Config::printHelp() const {
    std::cout << "Usage: graph_vis -i <input> [options]\n"
              << "  -i <file>          Input file (required)\n"
              << "  -o <file>          Output SVG (default: output.svg)\n"
              << "  -width <px>        Width (default: 800)\n"
              << "  -height <px>       Height (default: 600)\n"
              << "  -radius <px>       Vertex radius (default: 20)\n"
              << "  -bgcolor <color>   Background color (default: #ffffff)\n"
              << "  -vertexcolor <c>   Vertex color (default: #88aaff)\n"
              << "  -edgecolor <c>     Edge color (default: #333333)\n"
              << "  -directed          Directed graph\n"
              << "  -h, --help         Show this help\n";
}
