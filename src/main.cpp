#include "Graph.h"
#include "Config.h"
#include "LayoutEngine.h"
#include "SvgRenderer.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    try {
        Config config;
        if (!config.loadFromArgs(argc, argv)) {
            return 1;
        }
        
        Graph graph;
        graph.setDirected(config.isDirected());
        graph.loadFromFile(config.getInputFile());
        
        LayoutEngine engine(&graph);
        engine.layout(config.getWidth(), config.getHeight());
        
        SvgRenderer renderer(config);
        std::string svg = renderer.render(&graph, engine.getPositions());
        
        std::ofstream file(config.getOutputFile());
        if (!file.is_open()) {
            throw std::runtime_error("Cannot write to " + config.getOutputFile());
        }
        file << svg;
        
        std::cout << "Graph saved to " << config.getOutputFile() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
