#!/usr/bin/env bash
# Cloud Agent install script for llama.cpp.
# Idempotent: safe to run repeatedly against cached/partial state.
set -euo pipefail

# Always operate from the repository root (this file lives in <root>/.cursor).
cd "$(dirname "${BASH_SOURCE[0]}")/.."

APT_OPTS="-o Acquire::Retries=3 -o Acquire::http::Timeout=60 -o Acquire::https::Timeout=60"

echo "==> Installing system build dependencies"
export DEBIAN_FRONTEND=noninteractive
sudo apt-get $APT_OPTS update -qq
sudo apt-get $APT_OPTS install -y --no-install-recommends \
    build-essential \
    ninja-build \
    ccache \
    pkg-config \
    libssl-dev \
    libcurl4-openssl-dev \
    git-lfs \
    python3-venv

echo "==> Setting up Python virtualenv (.venv)"
if [ ! -x .venv/bin/python ]; then
    python3 -m venv .venv
fi
# shellcheck disable=SC1091
. .venv/bin/activate
python -m pip install --upgrade pip
# Conversion scripts (torch/transformers/gguf) + llama-server test suite deps.
pip install -r requirements.txt -r tools/server/tests/requirements.txt

echo "==> Configuring and building llama.cpp (CPU, portable)"
# GGML_NATIVE=OFF keeps the compiled binaries portable across build/snapshot hosts.
# gcc/g++ are used explicitly: the default `c++` alternative is clang++, whose
# linker cannot locate libstdc++ in the base image.
CC=gcc CXX=g++ cmake -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DGGML_NATIVE=OFF \
    -DGGML_CCACHE=ON
cmake --build build -j"$(nproc)"

echo "==> Install complete. Binaries are in ./build/bin"
