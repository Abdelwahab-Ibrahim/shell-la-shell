#!/usr/bin/env bash
set -euo pipefail

project_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source_dir="$project_dir/src"

g++ -std=c++17 -I"$project_dir/include" \
    "$source_dir/main.cpp" \
    "$source_dir/lexer.cpp" \
    "$source_dir/executor.cpp" \
    "$source_dir/utils.cpp" \
    "$source_dir/builtins.cpp" \
    "$source_dir/declare.cpp" \
    "$source_dir/redirection.cpp" \
    "$source_dir/jobs.cpp" \
    "$source_dir/runPipeline.cpp" \
    -o "sls"