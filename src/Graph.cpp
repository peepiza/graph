#include "Graph.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

bool Graph::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + path);
    }
    
    int n;
    file >> n;
    if (n < 0) throw std::runtime_error("Invalid vertex count");
    
    vertices_.clear();
    edges_.clear();
    
    // Добавляем вершины
    for (int i = 0; i < n; ++i) {
        vertices_.push_back(i);
    }
    
    // Читаем список смежности и создаём рёбра
    std::string line;
    std::getline(file, line); // дочитываем после n
    
    for (int i = 0; i < n; ++i) {
        if (!std::getline(file, line)) {
            throw std::runtime_error("Not enough lines");
        }
        std::istringstream iss(line);
        int neighbor;
        while (iss >> neighbor) {
            if (neighbor < 0 || neighbor >= n) {
                throw std::runtime_error("Vertex index out of range");
            }
            if (directed_) {
                edges_.push_back({i, neighbor});
            } else {
                // Для неориентированного графа добавляем ребро только один раз
                if (i < neighbor) {
                    edges_.push_back({i, neighbor});
                }
            }
        }
    }
    
    return true;
}
