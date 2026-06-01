#include <gtest/gtest.h>
#include "LayoutEngine.h"
#include "Graph.h"
#include <fstream>
#include <cstdio>

class LayoutTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::ofstream f("test_graph.txt");
        f << "4\n";
        f << "1 2\n";
        f << "0 2 3\n";
        f << "0 1\n";
        f << "1\n";
        f.close();
        
        graph.loadFromFile("test_graph.txt");
    }
    
    void TearDown() override {
        std::remove("test_graph.txt");
    }
    
    Graph graph;
};

//Корректное создание объекта LayoutEngine
TEST_F(LayoutTest, ConstructorWorks) {
    LayoutEngine engine(&graph);
    SUCCEED();
}

//Количество позиций равно количеству вершин графа
TEST_F(LayoutTest, LayoutReturnsCorrectNumberOfPositions) {
    LayoutEngine engine(&graph);
    engine.layout(800, 600);
    auto positions = engine.getPositions();
    EXPECT_EQ(positions.size(), 4);
}

//Каждая вершина имеет свои координаты
TEST_F(LayoutTest, AllVerticesHavePositions) {
    LayoutEngine engine(&graph);
    engine.layout(800, 600);
    auto positions = engine.getPositions();
    
    for (int v : graph.getVertices()) {
        EXPECT_TRUE(positions.find(v) != positions.end());
    }
}

//Координаты вершин находятся в пределах холста 
TEST_F(LayoutTest, PositionsWithinBounds) {
    LayoutEngine engine(&graph);
    engine.layout(800, 600);
    auto positions = engine.getPositions();
    
    for (const auto& [id, pos] : positions) {
        EXPECT_GE(pos.x, 50);
        EXPECT_LE(pos.x, 750);
        EXPECT_GE(pos.y, 50);
        EXPECT_LE(pos.y, 550);
    }
}

//Корректная обработка пустого графа
TEST_F(LayoutTest, EmptyGraph) {
    Graph empty;
    LayoutEngine engine(&empty);
    engine.layout(800, 600);
    auto positions = engine.getPositions();
    EXPECT_TRUE(positions.empty());
}

//Одна вершина размещается в пределах экрана
TEST_F(LayoutTest, SingleVertex) {
    Graph single;
    std::ofstream f("single.txt");
    f << "1\n";
    f << "\n";
    f.close();
    single.loadFromFile("single.txt");
    
    LayoutEngine engine(&single);
    engine.layout(800, 600);
    auto positions = engine.getPositions();
    
    EXPECT_EQ(positions.size(), 1);
    // Проверяем, что вершина находится в пределах экрана
    EXPECT_GE(positions[0].x, 50);
    EXPECT_LE(positions[0].x, 750);
    EXPECT_GE(positions[0].y, 50);
    EXPECT_LE(positions[0].y, 550);
    
    std::remove("single.txt");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
