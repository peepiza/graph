cmake_minimum_required(VERSION 3.14)
project(GraphVisualizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основная программа
add_executable(graph_vis 
    src/main.cpp 
    src/Graph.cpp 
    src/Config.cpp 
    src/LayoutEngine.cpp 
    src/SvgRenderer.cpp
)
target_include_directories(graph_vis PRIVATE src)

# Тесты
find_package(GTest QUIET)
if(GTEST_FOUND)
    enable_testing()
    
    add_executable(test_graph tests/test_graph.cpp src/Graph.cpp)
    target_link_libraries(test_graph GTest::gtest_main pthread)
    target_include_directories(test_graph PRIVATE src)
    
    add_executable(test_config tests/test_config.cpp src/Config.cpp)
    target_link_libraries(test_config GTest::gtest_main pthread)
    target_include_directories(test_config PRIVATE src)
    
    add_executable(test_layout tests/test_layout.cpp src/Graph.cpp src/LayoutEngine.cpp)
    target_link_libraries(test_layout GTest::gtest_main pthread)
    target_include_directories(test_layout PRIVATE src)
    
    add_executable(test_renderer tests/test_renderer.cpp src/Graph.cpp src/Config.cpp src/LayoutEngine.cpp src/SvgRenderer.cpp)
    target_link_libraries(test_renderer GTest::gtest_main pthread)
    target_include_directories(test_renderer PRIVATE src)
    
    add_test(NAME GraphTests COMMAND test_graph)
    add_test(NAME ConfigTests COMMAND test_config)
    add_test(NAME LayoutTests COMMAND test_layout)
    add_test(NAME RendererTests COMMAND test_renderer)
endif()
