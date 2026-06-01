#include <gtest/gtest.h>
#include "Graph.h"
#include <fstream>
#include <cstdio>

class GraphTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаём корректный файл с графом
        std::ofstream f("test_correct.txt");
        f << "4\n";
        f << "1 2\n";
        f << "0 2 3\n";
        f << "0 1\n";
        f << "1\n";
        f.close();
        
        // Создаём файл с неверным индексом вершины
        std::ofstream f2("test_invalid.txt");
        f2 << "3\n";
        f2 << "1 5\n";
        f2 << "0\n";
        f2 << "0\n";
        f2.close();
        
        // Создаём пустой граф
        std::ofstream f3("test_empty.txt");
        f3 << "0\n";
        f3.close();
        
        // Создаём ориентированный граф
        std::ofstream f4("test_directed.txt");
        f4 << "3\n";
        f4 << "1\n";
        f4 << "2\n";
        f4 << "0\n";
        f4.close();
    }
    
    void TearDown() override {
        std::remove("test_correct.txt");
        std::remove("test_invalid.txt");
        std::remove("test_empty.txt");
        std::remove("test_directed.txt");
    }
};

//Проверка загрузки корректного графа
TEST_F(GraphTest, LoadsCorrectGraph) {
    Graph g;
    EXPECT_TRUE(g.loadFromFile("test_correct.txt"));
    EXPECT_EQ(g.getVerticesCount(), 4);
    EXPECT_EQ(g.getEdges().size(), 4); // (0,1), (0,2), (1,2), (1,3)
}

//Проверка количества рёбер в неориентированном графе
TEST_F(GraphTest, UndirectedEdgesNoDuplicates) {
    Graph g;
    g.loadFromFile("test_correct.txt");
    auto edges = g.getEdges();
    int edgeCount = 0;
    for (const auto& e : edges) {
        if (e.first < e.second) edgeCount++;
    }
    EXPECT_EQ(edgeCount, 4);
}

//Проверка обработки неверного индекса вершины
TEST_F(GraphTest, ThrowsOnInvalidVertexIndex) {
    Graph g;
    EXPECT_THROW(g.loadFromFile("test_invalid.txt"), std::runtime_error);
}

//Проверка обработки несуществующего файла
TEST_F(GraphTest, ThrowsOnMissingFile) {
    Graph g;
    EXPECT_THROW(g.loadFromFile("nonexistent.txt"), std::runtime_error);
}

//Проверка пустого графа
TEST_F(GraphTest, EmptyGraph) {
    Graph g;
    EXPECT_TRUE(g.loadFromFile("test_empty.txt"));
    EXPECT_EQ(g.getVerticesCount(), 0);
    EXPECT_TRUE(g.getEdges().empty());
}

//Проверка флага directed
TEST_F(GraphTest, DirectedFlag) {
    Graph g;
    EXPECT_FALSE(g.isDirected());
    g.setDirected(true);
    EXPECT_TRUE(g.isDirected());
    g.setDirected(false);
    EXPECT_FALSE(g.isDirected());
}

//Проверка ориентированного графа (рёбра с направлением)
TEST_F(GraphTest, DirectedGraphEdges) {
    Graph g;
    g.setDirected(true);
    g.loadFromFile("test_directed.txt");
    EXPECT_EQ(g.getVerticesCount(), 3);
    auto edges = g.getEdges();
    EXPECT_EQ(edges.size(), 3);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
