#pragma once
#include <vector>
#include <string>
#include <utility>
#include <list>

class Graph {
public:
    bool loadFromFile(const std::string& path);
    int getVerticesCount() const { return vertices_.size(); }
    const std::list<int>& getVertices() const { return vertices_; }
    const std::list<std::pair<int,int>>& getEdges() const { return edges_; }
    void setDirected(bool directed) { directed_ = directed; }
    bool isDirected() const { return directed_; }
    
private:
    std::list<int> vertices_;
    std::list<std::pair<int,int>> edges_;
    bool directed_ = false;
};
