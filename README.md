#Консольный визуализатор графов

## Описание проекта

Консольное приложение на C++ для визуализации графов. Программа читает описание графа из файла (список смежности) и генерирует векторное изображение в формате SVG с круговой компоновкой вершин.

---

## Стек технологий

| Технология | Назначение |
|------------|------------|
| C++17 | Язык программирования |
| CMake 3.14+ | Система сборки |
| GoogleTest | Unit-тестирование |
| Docker | Контейнеризация |
| SVG | Формат выходного изображения |

---

## Структура проекта
graph/
├── src/ # Исходный код
│ ├── Graph.h/.cpp # Класс графа
│ ├── Config.h/.cpp # Класс конфигурации
│ ├── LayoutEngine.h/.cpp # Класс компоновки вершин
│ ├── SvgRenderer.h/.cpp # Класс генерации SVG
│ ├── Point.h # Структура точки
│ └── main.cpp # Точка входа
├── tests/ # Unit-тесты
│ ├── test_graph.cpp # 7 тестов
│ ├── test_config.cpp # 6 тестов
│ ├── test_layout.cpp # 6 тестов
│ └── test_renderer.cpp # 5 тестов
├── CMakeLists.txt # Конфигурация сборки
├── Dockerfile # Конфигурация Docker
├── .dockerignore # Исключения для Docker
├── input.txt # Тестовый граф
└── README.md # Документация

text

---

## Сборка и запуск

### Локальная сборка

```bash
cd ~/Desktop/graph
mkdir build && cd build
cmake ..
make
Запуск программы

bash
./graph_vis -i ../input.txt -o output.svg
open output.svg
Запуск всех тестов (24 теста)

bash
./test_graph && ./test_config && ./test_layout && ./test_renderer
Аргументы командной строки

Аргумент	Описание	Значение по умолчанию
-i <file>	Входной файл (обязательный)	—
-o <file>	Выходной SVG файл	output.svg
-width <px>	Ширина изображения	800
-height <px>	Высота изображения	600
-radius <px>	Радиус вершин	20
-bgcolor <color>	Цвет фона	#ffffff
-vertexcolor <color>	Цвет заливки вершин	#88aaff
-edgecolor <color>	Цвет рёбер	#333333
-directed	Ориентированный граф	false
-h, --help	Показать справку	—
Примеры:

bash
# Обычный граф
./graph_vis -i input.txt -o graph.svg

# Ориентированный граф
./graph_vis -i input.txt -o graph.svg -directed

# С настройками
./graph_vis -i input.txt -o graph.svg -width 1000 -height 700 -radius 25 -vertexcolor "#ff8888"
 Формат входного файла

Первая строка — количество вершин (N).
Следующие N строк — список смежных вершин через пробел.

Пример для графа из 4 вершин:

text
4
1 2
0 2 3
0 1
1
Визуализация:

text
    0
   / \
  1---2
  |
  3
 Результаты тестирования

Всего реализовано 24 unit-теста. Результаты запуска:

Модуль	Тестов пройдено	Тестов не пройдено
Graph	7	0
Config	6	0
LayoutEngine	6	0
SvgRenderer	5	0
Итого	24	0
text
[==========] 7 tests from GraphTest
[  PASSED  ] 7 tests.

[==========] 6 tests from ConfigTest
[  PASSED  ] 6 tests.

[==========] 6 tests from LayoutTest
[  PASSED  ] 6 tests.

[==========] 5 tests from RendererTest
[  PASSED  ] 5 tests.
 Контейнеризация (Docker)

Сборка образа

bash
docker build -t graph-visualizer .
Запуск тестов в контейнере

bash
docker run --rm graph-visualizer /usr/local/bin/test_graph
docker run --rm graph-visualizer /usr/local/bin/test_config
docker run --rm graph-visualizer /usr/local/bin/test_layout
docker run --rm graph-visualizer /usr/local/bin/test_renderer
Запуск программы в контейнере

bash
mkdir -p output
cp input.txt output/

docker run --rm -v $(pwd)/output:/workspace graph-visualizer \
    -i /workspace/input.txt -o /workspace/output.svg -directed

open output/output.svg
🏗️ Паттерны проектирования

Паттерн	Применение в проекте
Стратегия (Strategy)	Алгоритмы компоновки вершин (в перспективе: круговая, силовая)
Фабричный метод (Factory Method)	Создание объектов рендереров (SvgRenderer)
Строитель (Builder)	Конфигурация из аргументов командной строки
Шаблонный метод (Template Method)	Жизненный цикл приложения
Диаграмма классов

text
┌─────────────────┐     ┌─────────────────┐     ┌─────────────────┐
│     Graph       │────▶│  LayoutEngine   │────▶│  SvgRenderer    │
├─────────────────┤     ├─────────────────┤     ├─────────────────┤
│ - vertices_     │     │ - graph_        │     │ - config_       │
│ - edges_        │     │ - positions_    │     ├─────────────────┤
│ - directed_     │     ├─────────────────┤     │ + render()      │
├─────────────────┤     │ + layout()      │     └─────────────────┘
│ + loadFromFile()│     │ + getPositions()│
│ + getVertices() │     └─────────────────┘
│ + getEdges()    │              ▲
│ + setDirected() │              │
└─────────────────┘     ┌─────────────────┐
                        │     Config      │
                        ├─────────────────┤
                        │ - width_        │
                        │ - height_       │
                        │ - vertexRadius_ │
                        │ - directed_     │
                        ├─────────────────┤
                        │ + loadFromArgs()│
                        │ + getters       │
                        └─────────────────┘
 Примеры графов для тестирования

1. Простой граф (4 вершины)

text
4
1 2
0 2 3
0 1
1
2. Ориентированный граф (цикл)

text
4
1
2
3
0
3. Полный граф K5

text
5
1 2 3 4
0 2 3 4
0 1 3 4
0 1 2 4
0 1 2 3
4. Двудольный граф K3,3

text
6
3 4 5
3 4 5
3 4 5
0 1 2
0 1 2
0 1 2
