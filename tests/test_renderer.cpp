#include <gtest/gtest.h>
#include "SvgRenderer.h"
#include "Config.h"
#include "Graph.h"
#include "LayoutEngine.h"
#include <fstream>
#include <cstdio>

class RendererTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::ofstream f("test_graph.txt");
        f << "2\n";
        f << "1\n";
        f << "0\n";
        f.close();
        
        graph.loadFromFile("test_graph.txt");
        
        LayoutEngine engine(&graph);
        engine.layout(800, 600);
        positions = engine.getPositions();
    }
    
    void TearDown() override {
        std::remove("test_graph.txt");
    }
    
    Graph graph;
    std::map<int, Point> positions;
};

//Проверка создания рендерера
TEST_F(RendererTest, ConstructorWorks) {
    Config cfg;
    SvgRenderer renderer(cfg);
    SUCCEED();
}

//Проверка, что рендер возвращает непустую строку
TEST_F(RendererTest, RenderReturnsNonEmptyString) {
    Config cfg;
    SvgRenderer renderer(cfg);
    std::string svg = renderer.render(&graph, positions);
    EXPECT_FALSE(svg.empty());
}

//Проверка наличия SVG тегов
TEST_F(RendererTest, RenderContainsSvgTags) {
    Config cfg;
    SvgRenderer renderer(cfg);
    std::string svg = renderer.render(&graph, positions);
    
    EXPECT_TRUE(svg.find("<svg") != std::string::npos);
    EXPECT_TRUE(svg.find("</svg>") != std::string::npos);
}

//Проверка наличия вершин в SVG
TEST_F(RendererTest, RenderContainsVertices) {
    Config cfg;
    SvgRenderer renderer(cfg);
    std::string svg = renderer.render(&graph, positions);
    
    EXPECT_TRUE(svg.find("<circle") != std::string::npos);
    EXPECT_TRUE(svg.find("<text") != std::string::npos);
}

//Проверка цвета фона
TEST_F(RendererTest, RenderRespectsBgColor) {
    Config cfg;
    cfg.setBgColor("#ff0000");
    SvgRenderer renderer(cfg);
    std::string svg = renderer.render(&graph, positions);
    
    EXPECT_TRUE(svg.find("fill=\"#ff0000\"") != std::string::npos);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
