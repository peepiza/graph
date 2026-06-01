#include "LayoutEngine.h"
#include <cmath>

LayoutEngine::LayoutEngine(Graph* graph) : graph_(graph) {}

void LayoutEngine::layout(int width, int height) {
    circularLayout(width, height);
}

void LayoutEngine::circularLayout(int width, int height) {
    int n = graph_->getVerticesCount();
    if (n == 0) return;
    
    double radius = std::min(width, height) * 0.35;
    double centerX = width / 2.0;
    double centerY = height / 2.0;
    
    int idx = 0;
    for (int v : graph_->getVertices()) {
        double angle = 2 * M_PI * idx / n;
        positions_[v] = Point(centerX + radius * std::cos(angle),
                              centerY + radius * std::sin(angle));
        ++idx;
    }
}
