# Консольный визуализатор графов

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

## Сборка и запуск

### Локальная сборка

```bash
cd ~/Desktop/graph
rm -rf build
mkdir build && cd build
cmake ..
make
```

### Запуск программы

```bash
./graph_vis -i ../input.txt -o output.svg
open -a Safari output.svg
```

### Запуск всех тестов (24 теста)

```bash
./test_graph && ./test_config && ./test_layout && ./test_renderer
```

---

## Аргументы командной строки

| Аргумент | Описание | Значение по умолчанию |
|----------|----------|----------------------|
| `-i <file>` | Входной файл (обязательный) | — |
| `-o <file>` | Выходной SVG файл | `output.svg` |
| `-width <px>` | Ширина изображения | `800` |
| `-height <px>` | Высота изображения | `600` |
| `-radius <px>` | Радиус вершин | `20` |
| `-bgcolor <color>` | Цвет фона | `#ffffff` |
| `-vertexcolor <color>` | Цвет заливки вершин | `#88aaff` |
| `-edgecolor <color>` | Цвет рёбер | `#333333` |
| `-directed` | Ориентированный граф | `false` |
| `-h, --help` | Показать справку | — |

### Примеры

```bash
# Обычный граф
./graph_vis -i input.txt -o graph.svg
open -a Safari graph.svg

# Ориентированный граф
./graph_vis -i input.txt -o graph.svg -directed
open -a Safari graph.svg

# С настройками
./graph_vis -i input.txt -o graph.svg -width 1000 -height 700 -radius 25 -vertexcolor "#ff8888"
open -a Safari graph.svg
```

---

## Формат входного файла

Первая строка — количество вершин (N).  
Следующие N строк — список смежных вершин через пробел.

**Пример для графа из 4 вершин:**

```
4
1 2
0 2 3
0 1
1
```

**Визуализация:**

```
    0
   / \
  1---2
  |
  3
```

---

## Результаты тестирования

Всего реализовано **24 unit-теста**. Результаты запуска:

| Модуль | Тестов пройдено | Тестов не пройдено |
|--------|-----------------|-------------------|
| Graph | 7 | 0 |
| Config | 6 | 0 |
| LayoutEngine | 6 | 0 |
| SvgRenderer | 5 | 0 |
| **Итого** | **24** | **0** |

```
[==========] 7 tests from GraphTest
[  PASSED  ] 7 tests.

[==========] 6 tests from ConfigTest
[  PASSED  ] 6 tests.

[==========] 6 tests from LayoutTest
[  PASSED  ] 6 tests.

[==========] 5 tests from RendererTest
[  PASSED  ] 5 tests.
```

---

## Контейнеризация (Docker)

### Сборка образа

```bash
docker build -t graph-visualizer .
```

### Запуск тестов в контейнере

```bash
docker run --rm --entrypoint /usr/local/bin/test_graph graph-visualizer
docker run --rm --entrypoint /usr/local/bin/test_config graph-visualizer
docker run --rm --entrypoint /usr/local/bin/test_layout graph-visualizer
docker run --rm --entrypoint /usr/local/bin/test_renderer graph-visualizer
```

### Запуск программы в контейнере

```bash
mkdir -p output
cp input.txt output/

docker run --rm -v $(pwd)/output:/workspace --entrypoint /usr/local/bin/graph_vis graph-visualizer \
    -i /workspace/input.txt -o /workspace/output.svg -directed

open -a Safari output/output.svg
```

---

## Быстрый запуск со своими данными

### Создайте файл с графом

```bash
nano my_graph.txt
```

**Формат файла:**
```
<количество вершин>
<список соседей для вершины 0>
<список соседей для вершины 1>
...
```

### Примеры графов

Треугольник (3 вершины):

```bash
echo -e "3\n1 2\n0 2\n0 1" > triangle.txt
./graph_vis -i triangle.txt -o triangle.svg
open -a Safari triangle.svg
Квадрат (4 вершины):

bash
echo -e "4\n1 3\n0 2\n1 3\n0 2" > square.txt
./graph_vis -i square.txt -o square.svg
open -a Safari square.svg
Звезда (центр с 5 лучами):

bash
echo -e "6\n1 2 3 4 5\n0\n0\n0\n0\n0" > star.txt
./graph_vis -i star.txt -o star.svg
open -a Safari star.svg
Ориентированный цикл (со стрелками):

bash
echo -e "4\n1\n2\n3\n0" > cycle.txt
./graph_vis -i cycle.txt -o cycle.svg -directed
open -a Safari cycle.svg
```

---

## Параметры командной строки

| Параметр | Пример | Эффект |
|----------|--------|--------|
| `-width` | `-width 1200` | Шире изображение |
| `-height` | `-height 900` | Выше изображение |
| `-radius` | `-radius 30` | Крупнее вершины |
| `-vertexcolor` | `-vertexcolor "#ff6666"` | Красные вершины |
| `-edgecolor` | `-edgecolor "#ffffff"` | Белые рёбра |
| `-bgcolor` | `-bgcolor "#1a1a2e"` | Тёмный фон |
| `-directed` | `-directed` | Стрелки вместо линий |

### Пример с настройками

```bash
./graph_vis -i my_graph.txt -o result.svg \
    -width 1000 -height 700 \
    -radius 25 \
    -vertexcolor "#4a90e2" \
    -bgcolor "#f0f4f8" \
    -directed

open -a Safari result.svg
```

---

## Открыть результат

```bash
open -a Safari result.svg   # macOS
xdg-open result.svg      # Linux
start result.svg         # Windows
# или просто дважды кликните на файл
```