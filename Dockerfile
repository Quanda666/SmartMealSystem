FROM ubuntu:22.04

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    make \
    locales \
    && rm -rf /var/lib/apt/lists/*

# Set locale to UTF-8 to support Chinese characters
RUN locale-gen zh_CN.UTF-8
ENV LANG=zh_CN.UTF-8
ENV LANGUAGE=zh_CN:zh
ENV LC_ALL=zh_CN.UTF-8

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Build the project
RUN mkdir -p build && cd build && \
    cmake .. && \
    make -j$(nproc)

# Create data directory if it doesn't exist (though CMake might have done it)
RUN mkdir -p build/bin/data

# The application expects to run from the directory containing the executable
# and looks for 'data/' relative to it.
WORKDIR /app/build/bin

# Command to run the application
CMD ["./MealRecommendationSystem"]
