#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
RPL4_DIR="$SCRIPT_DIR"

BUILD_DIR=build
BUILD_TYPE=Release
JOBS="$(getconf _NPROCESSORS_ONLN 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1)"
BUILD_EXAMPLES=ON
RPL4_LOG_LEVEL=OFF

usage() {
  cat <<EOF
Usage: $(basename "$0") [options]

Options:
  -b DIR     Build directory (default: build)
  -t TYPE    CMake build type (Release/Debug) (default: ${BUILD_TYPE})
  -j N       Parallel jobs (default: autodetect)
  -e 0|1     Enable building examples (default: 1)
  -l LEVEL   RPL4 log level (DEBUG, INFO, WARNING, ERROR, FATAL, OFF) (default: ${RPL4_LOG_LEVEL})
  -c         Clean build directory (rm -rf DIR)
  -h         Show this help

Example:
  ./build_all.sh -b build -t Debug -j 8 -e 1 -l INFO

This script configures and builds the project from the repository root.
It enables building the `example/` directory when -e 1 is passed (default).
EOF
}

while getopts ":b:t:j:e:l:ch" opt; do
  case ${opt} in
    b) BUILD_DIR=${OPTARG} ;;
    t) BUILD_TYPE=${OPTARG} ;;
    j) JOBS=${OPTARG} ;;
    e) if [[ "${OPTARG}" == "0" ]]; then BUILD_EXAMPLES=OFF; else BUILD_EXAMPLES=ON; fi ;;
    l) RPL4_LOG_LEVEL=${OPTARG} ;;
    c) CLEAN=1 ;;
    h) usage; exit 0 ;;
    :) echo "Option -${OPTARG} requires an argument."; usage; exit 1 ;;
    \?) echo "Invalid option: -${OPTARG}"; usage; exit 1 ;;
  esac
done

if [[ "${CLEAN:-0}" == "1" ]]; then
  echo "Cleaning build directory: ${BUILD_DIR}"
  rm -rf "${RPL4_DIR}/${BUILD_DIR}"
  exit 0
fi

echo "Repository root: ${RPL4_DIR}"
echo "Build dir: ${BUILD_DIR}"
echo "Build type: ${BUILD_TYPE}"
echo "Parallel jobs: ${JOBS}"
echo "Build examples: ${BUILD_EXAMPLES}"
echo "RPL4_LOG_LEVEL: ${RPL4_LOG_LEVEL}"

mkdir -p "${RPL4_DIR}/${BUILD_DIR}"
cd "${RPL4_DIR}/${BUILD_DIR}"

cmake -DRPL4_BUILD_EXAMPLE=${BUILD_EXAMPLES} \
      -DRPL4_LOG_LEVEL=${RPL4_LOG_LEVEL} \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      ..

echo "Starting build..."
# Use cmake --build for portability; pass -j for Makefile generators
cmake --build . -- -j ${JOBS}

echo "Build finished. Artifacts are in: ${RPL4_DIR}/${BUILD_DIR}"
