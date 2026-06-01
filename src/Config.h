#pragma once
#include <string>
#include <map>

class Config {
public:
    Config();
    bool loadFromArgs(int argc, char* argv[]);
    
    // Getters
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    double getVertexRadius() const { return vertexRadius_; }
    std::string getBgColor() const { return bgColor_; }
    std::string getVertexColor() const { return vertexColor_; }
    std::string getEdgeColor() const { return edgeColor_; }
    bool isDirected() const { return directed_; }
    std::string getInputFile() const { return inputFile_; }
    std::string getOutputFile() const { return outputFile_; }
    
    // Setters for builder pattern
    void setWidth(int w) { width_ = w; }
    void setHeight(int h) { height_ = h; }
    void setVertexRadius(double r) { vertexRadius_ = r; }
    void setBgColor(const std::string& c) { bgColor_ = c; }
    void setVertexColor(const std::string& c) { vertexColor_ = c; }
    void setEdgeColor(const std::string& c) { edgeColor_ = c; }
    void setDirected(bool d) { directed_ = d; }
    void setInputFile(const std::string& f) { inputFile_ = f; }
    void setOutputFile(const std::string& f) { outputFile_ = f; }
    
    void printHelp() const;
    
private:
    int width_;
    int height_;
    double vertexRadius_;
    std::string bgColor_;
    std::string vertexColor_;
    std::string edgeColor_;
    bool directed_;
    std::string inputFile_;
    std::string outputFile_;
};
