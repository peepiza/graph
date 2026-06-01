#include "SvgRenderer.h"
#include <sstream>

SvgRenderer::SvgRenderer(const Config& config) : config_(config) {}

std::string SvgRenderer::render(Graph* graph, const std::map<int, Point>& positions) {
    std::ostringstream svg;
    
    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
        << "width=\"" << config_.getWidth() << "\" "
        << "height=\"" << config_.getHeight() << "\">\n";
    svg << "<rect width=\"100%\" height=\"100%\" fill=\"" << config_.getBgColor() << "\" />\n";
    
    // Рисуем рёбра
    for (const auto& edge : graph->getEdges()) {
        auto it1 = positions.find(edge.first);
        auto it2 = positions.find(edge.second);
        if (it1 != positions.end() && it2 != positions.end()) {
            svg << "<line x1=\"" << it1->second.x << "\" y1=\"" << it1->second.y << "\" "
                << "x2=\"" << it2->second.x << "\" y2=\"" << it2->second.y << "\" "
                << "stroke=\"" << config_.getEdgeColor() << "\" stroke-width=\"2\" />\n";
        }
    }
    
    // Рисуем вершины
    for (const auto& [id, pos] : positions) {
        svg << "<circle cx=\"" << pos.x << "\" cy=\"" << pos.y << "\" "
            << "r=\"" << config_.getVertexRadius() << "\" "
            << "fill=\"" << config_.getVertexColor() << "\" stroke=\"#000\" stroke-width=\"1.5\" />\n";
        svg << "<text x=\"" << pos.x << "\" y=\"" << pos.y + 5 << "\" "
            << "text-anchor=\"middle\" font-family=\"Arial\" font-size=\"14\" fill=\"#000\">"
            << id << "</text>\n";
    }
    
    svg << "</svg>\n";
    return svg.str();
}
