FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Установка зависимостей
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    make \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

# Сборка GoogleTest
RUN cd /usr/src/gtest && \
    cmake CMakeLists.txt && \
    make && \
    cp lib/*.a /usr/lib

WORKDIR /app

# Копируем все файлы проекта
COPY . .

# Создаём отдельную папку для сборки
RUN mkdir -p build && cd build && \
    cmake .. && \
    make

# Копируем бинарные файлы в PATH
RUN cp build/graph_vis /usr/local/bin/ && \
    cp build/test_graph /usr/local/bin/ && \
    cp build/test_config /usr/local/bin/ && \
    cp build/test_layout /usr/local/bin/ && \
    cp build/test_renderer /usr/local/bin/

WORKDIR /workspace

ENTRYPOINT ["/usr/local/bin/graph_vis"]
CMD ["-h"]
