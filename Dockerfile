# Use Ubuntu LTS as base image
FROM ubuntu:20.04

# Set noninteractive mode for apt
ENV DEBIAN_FRONTEND=noninteractive

# Install system tools, dev dependencies, and analysis tools
RUN apt-get update && apt-get install -y \
    autoconf \
    automake \
    curl \
    build-essential \
    git \
    libtool \
    make \
    g++ \
    pkg-config \
    ca-certificates \
    software-properties-common \
    clang-tidy \
    clang-format \
    pmccabe \
    python3 \
    python3-setuptools \
    python3-pip \
    cmake \
    ninja-build \
    wget \
    libssl-dev \
    libxml2-dev \
    libxslt1-dev \
    python3-dev \
    bzip2 \
    unzip \
    && mkdir -p /home/dev/tools \
    && rm -rf /var/lib/apt/lists/*

# Install Python tools
RUN pip3 install gcovr

# Set working directory
WORKDIR /home/dev

# Use Docker's automatic build arg for architecture (via BuildKit)
ARG TARGETARCH

# Download correct version of gcc-arm-none-eabi based on host arch
RUN if [ "$TARGETARCH" = "amd64" ]; then \
      TOOLCHAIN_URL="https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2"; \
    elif [ "$TARGETARCH" = "arm64" ]; then \
      TOOLCHAIN_URL="https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-aarch64-linux.tar.bz2"; \
    else \
      echo "Unsupported architecture: $TARGETARCH" && exit 1; \
    fi && \
    FILENAME=$(basename "$TOOLCHAIN_URL") && \
    curl -L "$TOOLCHAIN_URL" -o "$FILENAME" && \
    tar -xjf "$FILENAME" && \
    rm "$FILENAME"

# Export toolchain path
ENV PATH="/home/dev/gcc-arm-none-eabi-10-2020-q4-major/bin:${PATH}"

# Clone, build, and install CppUTest
WORKDIR /home
RUN git clone https://github.com/cpputest/cpputest.git && \
    cd cpputest && \
    ./autogen.sh && \
    ./configure && \
    make -j$(nproc) install

# Set CppUTest environment variable
ENV CPPUTEST_HOME=/home/cpputest

# Final working directory
WORKDIR /home/app

# Default command
CMD ["/bin/bash"]
