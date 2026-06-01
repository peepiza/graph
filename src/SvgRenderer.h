#pragma once
#include "Config.h"
#include "Graph.h"
#include "Point.h"
#include <map>
#include <string>

class SvgRenderer {
public:
    SvgRenderer(const Config& config);
    std::string render(Graph* graph, const std::map<int, Point>& positions);
    
private:
    Config config_;
};
