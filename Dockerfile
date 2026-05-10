# Use a base image with C++ build tools
FROM ubuntu:22.04

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libasound2-dev \
    libx11-dev \
    libxrandr-dev \
    libxi-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxcursor-dev \
    libxinerama-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy the project
COPY . .

# Build the project
RUN cmake -B build -D FETCH_RAYLIB=ON \
    && cmake --build build --config Release

# Command to run tests (or build artifacts)
CMD ["ctest", "--test-dir", "build"]
