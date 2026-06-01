#!/bin/bash

echo "========================================="
echo "   ВИЗУАЛИЗАТОР ГРАФОВ - ЗАПУСК"
echo "========================================="

cd ~/Desktop/graph

echo ""
echo "1. ОЧИСТКА И СБОРКА ПРОЕКТА"
echo "-----------------------------------------"
rm -rf build
mkdir build && cd build
cmake .. && make

echo ""
echo "2. ЗАПУСК ТЕСТОВ"
echo "-----------------------------------------"
./test_graph
./test_config
./test_layout
./test_renderer

echo ""
echo "3. ЗАПУСК ПРОГРАММЫ"
echo "-----------------------------------------"
./graph_vis -i ../input.txt -o ../output.svg -directed

echo ""
echo "4. ОТКРЫТИЕ РЕЗУЛЬТАТА В SAFARI"
echo "-----------------------------------------"
open -a Safari ../output.svg

echo ""
echo "========================================="
echo "   ГОТОВО!"
echo "========================================="
