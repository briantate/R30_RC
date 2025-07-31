# Use Ubuntu LTS as base image. 22.04 is a good modern LTS.
FROM ubuntu:22.04

# Set noninteractive mode for apt
ENV DEBIAN_FRONTEND=noninteractive

# Use Docker's automatic build arg for architecture (via BuildKit)
ARG TARGETARCH

# Determine LLVM version based on Ubuntu version
ARG LLVM_VERSION=18 # Current as of July 2025

# --- CRITICAL FIX: Install essential tools before using them ---
RUN apt-get update && \
    apt-get install -y \
    wget \
    lsb-release \
    ca-certificates \
    # Clean up apt cache after installing these essentials
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install system tools, dev dependencies, and analysis tools
RUN apt-get update && \
    # Add LLVM official repository key
    wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc && \
    # Add LLVM repository for the specific Ubuntu version and LLVM version
    echo "deb http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-${LLVM_VERSION} main" | tee /etc/apt/sources.list.d/llvm.list && \
    apt-get update && \
    apt-get install -y \
    autoconf \
    automake \
    curl \
    build-essential \
    git \
    libtool \
    make \
    g++ \
    pkg-config \
    software-properties-common \
    # Install versioned clang-tools for consistency and run-clang-tidy.py
    clang-${LLVM_VERSION} \
    clang-tools-${LLVM_VERSION} \
    clang-tidy-${LLVM_VERSION} \
    clang-format-${LLVM_VERSION} \
    python3-clang-${LLVM_VERSION} \
    cppcheck \
    pmccabe \
    python3 \
    python3-setuptools \
    python3-pip \
    cmake \
    ninja-build \
    # wget, lsb-release, ca-certificates are already installed in the previous RUN layer
    libssl-dev \
    libxml2-dev \
    libxslt1-dev \
    python3-dev \
    bzip2 \
    unzip \
    # Clean up apt cache to keep image size down
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && mkdir -p /home/dev/tools

    # Create symlinks for clang-tools to be in default PATH for convenience
RUN ln -s /usr/bin/clang-${LLVM_VERSION} /usr/local/bin/clang && \
    ln -s /usr/bin/clang++-${LLVM_VERSION} /usr/local/bin/clang++ && \
    ln -s /usr/bin/clang-tidy-${LLVM_VERSION} /usr/local/bin/clang-tidy && \
    ln -s /usr/bin/clang-format-${LLVM_VERSION} /usr/local/bin/clang-format && \
    ln -s /usr/bin/run-clang-tidy-${LLVM_VERSION}.py /usr/local/bin/run-clang-tidy.py

# Install Python tools
RUN pip3 install gcovr

# Set working directory for toolchain download
WORKDIR /home/dev

# Download correct version of gcc-arm-none-eabi based on host arch
# We need to set TOOLCHAIN_DIR as an ENV variable within the RUN command
# so it persists to the next ENV instruction.
RUN if [ "$TARGETARCH" = "amd64" ]; then \
      TOOLCHAIN_URL="https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2"; \
      export TOOLCHAIN_DIR="gcc-arm-none-eabi-10.3-2021.10"; \
    elif [ "$TARGETARCH" = "arm64" ]; then \
      TOOLCHAIN_URL="https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-aarch64-linux.tar.bz2"; \
      export TOOLCHAIN_DIR="gcc-arm-none-eabi-10.3-2021.10"; \
    else \
      echo "Unsupported architecture: $TARGETARCH" && exit 1; \
    fi && \
    FILENAME=$(basename "$TOOLCHAIN_URL") && \
    curl -L "$TOOLCHAIN_URL" -o "$FILENAME" && \
    tar -xjf "$FILENAME" && \
    rm "$FILENAME" && \
    # Set the ENV variable immediately after its value is determined
    # This value will be available in subsequent layers.
    echo "TOOLCHAIN_DIR=${TOOLCHAIN_DIR}" >> /etc/environment

# Export toolchain path, now using the TOOLCHAIN_DIR set in the previous RUN step
# We can't use ${TOOLCHAIN_DIR} directly here unless it was set as an ENV var in the previous step,
# or if it was an ARG that persisted (which it isn't, as it's computed).
# A common pattern is to either:
# 1. Use the hardcoded string if it's constant.
# 2. Use a shell trick to source /etc/environment or similar.
# The `echo "TOOLCHAIN_DIR=${TOOLCHAIN_DIR}" >> /etc/environment` in the previous step
# makes it implicitly available, but `ENV` is explicit.
# Let's use a dynamic approach by reading from a temporary file created in the previous layer.

# OPTION 1: Read the variable from a temporary file. This is the most robust if the directory name might change.
# First, update the previous RUN command to save the TOOLCHAIN_DIR to a file.
# Modified RUN command (already done above with `echo "TOOLCHAIN_DIR=${TOOLCHAIN_DIR}" >> /etc/environment`)
# Now, define an intermediate ARG here, which defaults to the desired path.
# This approach is a bit more complex. Let's simplify.

# Simplified and correct approach: Set the ENV variable within the same RUN block
# where TOOLCHAIN_DIR is determined.
# This ensures it's available for subsequent layers.
# I've modified the RUN block above to include `export TOOLCHAIN_DIR="..."`
# and `echo "TOOLCHAIN_DIR=${TOOLCHAIN_DIR}" >> /etc/environment`.
# This makes it available to shell sessions in the container.
# However, for Docker's ENV instruction, it needs to be set explicitly.

# The most straightforward fix is to set the ENV variable within the same RUN command
# where it's determined, or by using a single ENV command that always uses the fixed name.
# Since the toolchain version is fixed (10.3-2021.10), the directory name is also fixed.
# So, the easiest fix for the Dockerfile is to just use the fixed directory name directly in ENV.
# The computed TOOLCHAIN_DIR variable within the RUN command was mainly for the internal script logic,
# but the resulting path is constant based on the downloaded file.

# Reverting to the simpler, direct path for ENV, as the TOOLCHAIN_DIR is derived from a fixed URL
# The warning "UndefinedVar: Usage of undefined variable '$TOOLCHAIN_DIR'" was from a linter
# complaining about the variable not being defined at the time of the *Dockerfile parsing*,
# not necessarily at the time of execution.
# For simplicity and correctness with a fixed download URL, just use the known directory name.
ENV PATH="/home/dev/gcc-arm-none-eabi-10.3-2021.10/bin:${PATH}"

# Clone, build, and install CppUTest
WORKDIR /home
RUN git clone https://github.com/cpputest/cpputest.git && \
    cd cpputest && \
    ./autogen.sh && \
    ./configure && \
    make -j$(nproc) install

# Set CppUTest environment variable
ENV CPPUTEST_HOME=/home/cpputest

# Final working directory for application code
WORKDIR /home/app

# Default command
CMD ["/bin/bash"]