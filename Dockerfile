# Stage 1: Build
FROM ubuntu:24.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential meson ninja-build pkg-config git cmake \
    libsoci-dev libsqlite3-dev libjsoncpp-dev libfmt-dev \
    libargparse-dev libbrotli-dev libssl-dev uuid-dev \
    libc-ares-dev zlib1g-dev libboost-dev libcriterion-dev \
    && rm -rf /var/lib/apt/lists/*

# Build and install Drogon from source
RUN git clone --depth 1 --recursive --branch v1.9.11 \
    https://github.com/drogonframework/drogon.git /tmp/drogon && \
    cmake -S /tmp/drogon -B /tmp/drogon/build \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_SHARED_LIBS=ON \
      -DBUILD_CTL=ON && \
    cmake --build /tmp/drogon/build -j$(nproc) && \
    cmake --install /tmp/drogon/build && \
    ldconfig && \
    rm -rf /tmp/drogon

# Copy pkg-config files for Drogon, Trantor, and SOCI
COPY docker/drogon.pc /usr/local/lib/pkgconfig/drogon.pc
COPY docker/trantor.pc /usr/local/lib/pkgconfig/trantor.pc
COPY docker/soci_core.pc /usr/local/lib/pkgconfig/soci_core.pc
COPY docker/soci_sqlite3.pc /usr/local/lib/pkgconfig/soci_sqlite3.pc

WORKDIR /app

# Copy only source, vendor, test files, and build files needed
COPY src/ src/
COPY test/ test/
COPY vendor/ vendor/
COPY videos/ videos/
COPY meson.build config.json server_config.json index.html styles.css ./
COPY demo.db ./
COPY contacts.csv ./

# Build and test the application with Meson
RUN meson setup builddir \
    --buildtype=release \
    --cmake-prefix-path=/usr/local && \
    meson compile -C builddir && \
    cp server_config.json demo.db builddir/ && \
    meson test -C builddir --print-errorlogs

# Stage 2: Runtime
FROM ubuntu:24.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    libsqlite3-0 libssl3t64 libbrotli1 libc-ares2 \
    libjsoncpp25 libsoci-core4.0 libsoci-sqlite3-4.0 \
    uuid-runtime ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Copy Drogon and Trantor shared libraries from builder
COPY --from=builder /usr/local/lib/libdrogon* /usr/local/lib/
COPY --from=builder /usr/local/lib/libtrantor* /usr/local/lib/
RUN ldconfig

# Copy application binary, config, and data files
COPY --from=builder /app/builddir/demo_web_server /app/demo_web_server
COPY --from=builder /app/builddir/config.json /app/config.json
COPY --from=builder /app/builddir/server_config.json /app/server_config.json
COPY --from=builder /app/builddir/index.html /app/index.html
COPY --from=builder /app/builddir/styles.css /app/styles.css
COPY --from=builder /app/builddir/vendor/ /app/vendor/
COPY --from=builder /app/builddir/videos/ /app/videos/
COPY contacts.csv /app/contacts.csv
COPY demo.db /app/demo.db

WORKDIR /app
EXPOSE 3000

CMD ["./demo_web_server", "-i", "0.0.0.0", "-p", "3000"]
