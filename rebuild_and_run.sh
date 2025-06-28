#!/usr/bin/env bash
set -euo pipefail

# Usage: ./rebuild_and_run.sh [BUILD_DIR]
# Default BUILD_DIR is build-debug
BUILD_DIR=${1:-build-debug}

echo "Rebuilding project in '${BUILD_DIR}'..."
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

# Configure and build
cmake -S . -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Debug
cmake --build "${BUILD_DIR}" -- -j$(nproc)

# Run the main executable
echo "Running 'ThaiCheckers' from '${BUILD_DIR}'..."
"${BUILD_DIR}/ThaiCheckers"
