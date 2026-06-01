#pragma once
#include "Graph.h"
#include "Point.h"
#include <map>

class LayoutEngine {
public:
    LayoutEngine(Graph* graph);
    void layout(int width, int height);
    std::map<int, Point> getPositions() const { return positions_; }
    
private:
    Graph* graph_;
    std::map<int, Point> positions_;
    
    void circularLayout(int width, int height);
};
