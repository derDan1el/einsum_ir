#!/bin/bash

# performance_of_23_einsum_expressions_multicore.sh - Multi-Core Benchmarking Script für einsum_ir
# Optimiert für NVIDIA Grace CPU mit 72 Arm Neoverse-V2 Kernen

# Output files
OUTPUT_CSV="benchmark_results_multicore.csv"
FULL_LOG="expressions_multicore.log"

# Binary path
BINARY="/home/daniel/einsum_ir/build/bench_binary"

echo "=========================================="
echo "NVIDIA Grace CPU Multi-Core Benchmark"
echo "72 Arm Neoverse-V2 CPU Kerne"
echo "Optimiert für führende Leistung pro Kern"
echo "=========================================="
echo ""

# Check if binary exists, if not compile it
if [ ! -f "$BINARY" ]; then
    echo "Binary $BINARY not found!"
    echo "Starting compilation..."
    echo "Running: scons libtorch=/home/daniel/venv_pytorch/lib/python3.9/site-packages/torch libxsmm=/home/daniel/einsum_ir/../libxsmm -j8"
    
    # Run compilation
    scons libtorch=/home/daniel/venv_pytorch/lib/python3.9/site-packages/torch libxsmm=/home/daniel/einsum_ir/../libxsmm -j8
    
    # Check if compilation was successful
    if [ $? -eq 0 ]; then
        echo "Compilation successful!"
        echo ""
    else
        echo "Error: Compilation failed!"
        exit 1
    fi
    
    # Double-check if binary exists now
    if [ ! -f "$BINARY" ]; then
        echo "Error: Binary $BINARY still not found after compilation!"
        exit 1
    fi
else
    echo "Binary $BINARY found, proceeding with multi-core benchmarks..."
    echo ""
fi

# Initialize files
echo "Einsum,Dimensions,Dtype,Torch_GFLOPS,JIT_GFLOPS" > "$OUTPUT_CSV"
echo "=== Multi-Core Benchmark Log Started: $(date) ===" > "$FULL_LOG"
echo "=== Using 72 CPU Threads (NVIDIA Grace CPU) ===" >> "$FULL_LOG"
echo "" >> "$FULL_LOG"

# Function to extract GFLOPS from output
extract_gflops() {
    local output="$1"
    
    # Extract Torch GFLOPS from "at::einsum:" section
    local torch_gflops=$(echo "$output" | grep -A2 "at::einsum:" | grep "gflops:" | awk '{print $2}')
    
    # Extract JIT GFLOPS from einsum_ir section (after "JIT compile err:")
    local jit_gflops=$(echo "$output" | grep -A3 "JIT compile err:" | grep "gflops:" | awk '{print $2}')
    
    echo "$torch_gflops,$jit_gflops"
}

# Function to run benchmark
run_benchmark() {
    local einsum="$1"
    local dimensions="$2"
    local dtype="$3"
    
    # Build command with 72 threads for NVIDIA Grace CPU
    local cmd="OMP_NUM_THREADS=72 $BINARY \"$einsum\" \"$dimensions\" $dtype"
    
    # Show the actual command being executed
    echo "Running (72 threads): $cmd"
    echo "Running (72 threads): $cmd" >> "$FULL_LOG"
    echo "----------------------------------------" >> "$FULL_LOG"
    
    # Run the benchmark and capture all output
    local full_output=$(eval $cmd 2>&1)
    local exit_code=$?
    
    # Write full output to log file
    echo "$full_output" >> "$FULL_LOG"
    echo "" >> "$FULL_LOG"
    echo "Exit code: $exit_code" >> "$FULL_LOG"
    echo "========================================" >> "$FULL_LOG"
    echo "" >> "$FULL_LOG"
    
    # Extract GFLOPS values
    local metrics=$(extract_gflops "$full_output")
    local torch_val=$(echo $metrics | cut -d',' -f1)
    local jit_val=$(echo $metrics | cut -d',' -f2)
    
    # Handle empty values
    if [ -z "$torch_val" ]; then
        torch_val="ERROR"
    fi
    if [ -z "$jit_val" ]; then
        jit_val="ERROR"
    fi
    
    # Write to CSV
    echo "\"$einsum\",\"$dimensions\",\"$dtype\",$torch_val,$jit_val" >> "$OUTPUT_CSV"
    
    # Print progress
    echo "  Torch (72 cores): $torch_val GFLOPS"
    echo "  JIT (72 cores):   $jit_val GFLOPS"
    echo ""
}

# Function to run benchmark for all data types
run_benchmark_all_dtypes() {
    local einsum="$1"
    local dimensions="$2"
    
    echo "=== Multi-Core Testing: $einsum with dimensions: $dimensions ===" >> "$FULL_LOG"
    
    # Run for each data type
    run_benchmark "$einsum" "$dimensions" "BF16"
    run_benchmark "$einsum" "$dimensions" "FP32"
    run_benchmark "$einsum" "$dimensions" "FP64"
    
    echo "" >> "$FULL_LOG"
}

echo "Starting einsum_ir multi-core benchmark suite (72 threads)..."
echo "Results will be written to: $OUTPUT_CSV"
echo "Full logs will be written to: $FULL_LOG"
echo ""

# Run all benchmarks with 72 threads
run_benchmark_all_dtypes "abdfe,cf->abcde" "48,36,24,36,48,36"
run_benchmark_all_dtypes "acdfe,bf->abcde" "48,24,36,36,48,36"
run_benchmark_all_dtypes "abed,ce->abcd" "96,84,24,96,96"
run_benchmark_all_dtypes "abcfe,df->abcde" "48,36,36,24,48,48"
run_benchmark_all_dtypes "aced,be->abcd" "96,24,84,96,84"
run_benchmark_all_dtypes "jki,efghjk->efghi" "6,64,6,64,24,6,64"
run_benchmark_all_dtypes "abed,ce->abcd" "96,84,24,84,96"
run_benchmark_all_dtypes "bcgf,adeg->abcdef" "24,20,20,24,20,20,24"
run_benchmark_all_dtypes "bdgf,aceg->abcdef" "24,20,20,24,20,20,24"
run_benchmark_all_dtypes "acgf,bdeg->abcdef" "24,20,20,24,20,20,24"
run_benchmark_all_dtypes "abgf,cdeg->abcdef" "24,20,20,24,20,20,24"
run_benchmark_all_dtypes "efhjki,gjk->efghi" "6,64,24,4,94,6,64"
run_benchmark_all_dtypes "bced,ae->abcd" "96,84,84,84,96"
run_benchmark_all_dtypes "aced,be->abcd" "96,84,84,84,96"
run_benchmark_all_dtypes "abed,ce->abcd" "96,84,84,84,96"
run_benchmark_all_dtypes "aedc,ebd->abc" "96,84,84,84,96"
run_benchmark_all_dtypes "gkiljh,ekilfj->efgh" "6,64,4,94,6,64,6,64"
run_benchmark_all_dtypes "gikljh,eiklfj->efgh" "6,64,4,94,4,94,6,64"
run_benchmark_all_dtypes "efiklj,ghkl->efghij" "6,64,4,94,4,94,6,64"
run_benchmark_all_dtypes "efiklj,ghkl->efghij" "6,64,6,64,4,94,4,94"
run_benchmark_all_dtypes "cefd,aebf->abcd" "96,84,84,84,84,96"
run_benchmark_all_dtypes "aefd,becf->abcd" "96,84,84,84,96,96"
run_benchmark_all_dtypes "cfed,afbe->abcd" "96,84,84,96,84,84"

echo "Multi-Core Benchmark completed!"
echo "Results saved to: $OUTPUT_CSV"
echo "Full logs saved to: $FULL_LOG"
echo ""

echo "Multi-Core Summary (72 threads):"
echo "  Total expressions tested: 23"
echo "  Data types tested: BF16, FP32, FP64"
echo "  Total benchmark runs: 69"
echo "  CPU: NVIDIA Grace with 72 Arm Neoverse-V2 cores"
echo ""

echo "You can view the results with:"
echo "  cat $OUTPUT_CSV                    # CSV results"
echo "  less $FULL_LOG                     # Full detailed logs"
echo "  grep 'gflops:' $FULL_LOG           # Just GFLOPS values"
echo "  grep 'ERROR' $OUTPUT_CSV           # Check for failed runs"
