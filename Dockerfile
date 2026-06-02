FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    make \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

RUN cd /usr/src/gtest && \
    cmake CMakeLists.txt && \
    make && \
    cp lib/*.a /usr/lib

WORKDIR /app

COPY src/ ./src/
COPY tests/ ./tests/
COPY CMakeLists.txt ./CMakeLists.txt

RUN mkdir -p build && cd build && \
    cmake .. && \
    make

FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /app/build/graph_vis /usr/local/bin/graph_vis
COPY --from=builder /app/build/test_graph /usr/local/bin/test_graph
COPY --from=builder /app/build/test_config /usr/local/bin/test_config
COPY --from=builder /app/build/test_layout /usr/local/bin/test_layout
COPY --from=builder /app/build/test_renderer /usr/local/bin/test_renderer

RUN ls -la /usr/local/bin/

WORKDIR /workspace
ENTRYPOINT ["/usr/local/bin/graph_vis"]
CMD ["-h"]
