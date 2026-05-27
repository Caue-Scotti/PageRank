#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

PARALLEL_BIN="$ROOT_DIR/parallelized/pageRank"
SEQUENTIAL_BIN="$ROOT_DIR/sequential/pageRank"

INPUTS=(
    "$ROOT_DIR/test_data/1000.txt"
    "$ROOT_DIR/test_data/5000.txt"
    "$ROOT_DIR/test_data/10000.txt"
    "$ROOT_DIR/test_data/20000.txt"
)

THREADS=(1 2 4 8)

RESULTS_DIR="$ROOT_DIR/results"
VTUNE_DIR="$RESULTS_DIR/vtune_results"

mkdir -p "$RESULTS_DIR"
mkdir -p "$VTUNE_DIR"

CSV="$RESULTS_DIR/results.csv"
echo "version,input,threads,time" > "$CSV"

run_benchmark() {
    local version="$1"
    local bin="$2"
    local input="$3"
    local threads="$4"

    local filename
    filename=$(basename "$input" .txt)

    echo "=============================="
    echo "Running $version - $filename with $threads threads"
    echo "=============================="

    export OMP_NUM_THREADS="$threads"

    result=$("$bin" "$input" | grep EXEC_TIME)
    time=$(echo "$result" | cut -d',' -f2)

    echo "$version,$filename,$threads,$time" >> "$CSV"
}

for input in "${INPUTS[@]}"; do
    filename=$(basename "$input" .txt)

    run_benchmark "sequential" "$SEQUENTIAL_BIN" "$input" 1

    for t in "${THREADS[@]}"; do
        run_benchmark "parallelized" "$PARALLEL_BIN" "$input" "$t"

        vtune_result_dir="$VTUNE_DIR/${filename}_${t}threads"

        vtune -collect hotspots \
            -result-dir "$vtune_result_dir" \
            -- "$PARALLEL_BIN" "$input"

        echo "VTune salvo em: $vtune_result_dir"
    done
done

echo "Benchmark finalizado!"