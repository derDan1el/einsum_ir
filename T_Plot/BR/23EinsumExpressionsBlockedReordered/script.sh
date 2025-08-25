#!/bin/bash

# einsum_ir_benchmark.sh - Comprehensive Benchmarking Script für einsum_ir

# Output files
OUTPUT_CSV="benchmark_results.csv"
FULL_LOG="benchmark_full.log"

# Binary path
BINARY="/home/daniel/einsum_ir/build/bench_binary"

# Check if binary exists, if not compile it
if [ ! -f "$BINARY" ]; then
    echo "Binary $BINARY not found!"
    echo "Starting compilation..."
    echo "Running: scons parallel=none libtorch=/home/daniel/venv_pytorch/lib/python3.9/site-packages/torch libxsmm=/home/daniel/einsum_ir/../libxsmm -j8"
    
    # Run compilation
    scons parallel=none libtorch=/home/daniel/venv_pytorch/lib/python3.9/site-packages/torch libxsmm=/home/daniel/einsum_ir/../libxsmm -j8
    
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
    echo "Binary $BINARY found, proceeding with benchmarks..."
    echo ""
fi

# Initialize files
echo "Einsum,Dimensions,Dtype,Torch_GFLOPS,JIT_GFLOPS,Max_Absolute_Diff,Max_Relative_Error" > "$OUTPUT_CSV"
echo "=== Benchmark Log Started: $(date) ===" > "$FULL_LOG"
echo "" >> "$FULL_LOG"

# Function to extract all metrics from output
extract_metrics() {
    local output="$1"
    
    # Extract PyTorch GFLOPS from "at::einsum:" section
    local torch_gflops=$(echo "$output" | grep -A10 "at::einsum:" | grep "gflops:" | head -1 | awk '{print $2}')
    
    # Extract JIT GFLOPS from the second gflops line (after einsum_ir section)
    local jit_gflops=$(echo "$output" | grep "gflops:" | tail -1 | awk '{print $2}')
    
    # Extract maximum absolute difference
    local max_diff=$(echo "$output" | grep "max difference" | awk '{print $4}')
    
    # Extract maximum relative error
    local max_rel_error=$(echo "$output" | grep "Biggest relative error:" | awk '{print $4}')
    
    # Handle empty values
    if [ -z "$torch_gflops" ]; then
        torch_gflops="ERROR"
    fi
    if [ -z "$jit_gflops" ]; then
        jit_gflops="ERROR"
    fi
    if [ -z "$max_diff" ]; then
        max_diff="ERROR"
    fi
    if [ -z "$max_rel_error" ]; then
        max_rel_error="ERROR"
    fi
    
    echo "$torch_gflops,$jit_gflops,$max_diff,$max_rel_error"
}

# Function to run benchmark
run_benchmark() {
    local einsum="$1"
    local dimensions="$2"
    local dtype="$3"
    
    # Build command
    local cmd="OMP_NUM_THREADS=1 $BINARY \"$einsum\" \"$dimensions\" $dtype"
    
    # Show the actual command being executed
    echo "Running: $cmd"
    echo "Running: $cmd" >> "$FULL_LOG"
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
    
    # Extract all metrics
    local metrics=$(extract_metrics "$full_output")
    local torch_val=$(echo $metrics | cut -d',' -f1)
    local jit_val=$(echo $metrics | cut -d',' -f2)
    local max_diff=$(echo $metrics | cut -d',' -f3)
    local max_rel_error=$(echo $metrics | cut -d',' -f4)
    
    # Write to CSV
    echo "\"$einsum\",\"$dimensions\",\"$dtype\",$torch_val,$jit_val,$max_diff,$max_rel_error" >> "$OUTPUT_CSV"
    
    # Print progress
    echo "  Torch GFLOPS: $torch_val"
    echo "  JIT GFLOPS:   $jit_val"
    echo "  Max Abs Diff: $max_diff"
    echo "  Max Rel Err:  $max_rel_error"
    echo ""
}

echo "Starting einsum_ir benchmark suite..."
echo "Results will be written to: $OUTPUT_CSV"
echo "Full logs will be written to: $FULL_LOG"
echo ""


run_benchmark "abdfe,cf->abcde" "48,36,24,36,48,36" "BF16"
run_benchmark "acdfe,bf->abcde" "48,24,36,36,48,36" "BF16"
run_benchmark "abed,ce->abcd" "96,84,24,96,96" "BF16"
run_benchmark "abcfe,df->abcde" "48,36,36,24,48,48" "BF16"
run_benchmark "aced,be->abcd" "96,24,84,96,84" "BF16"
run_benchmark "jki,efghjk->efghi" "6,64,6,64,24,6,64" "BF16"
run_benchmark "abed,ce->abcd" "96,84,24,84,96" "BF16"
run_benchmark "bcgf,adeg->abcdef" "24,20,20,24,20,20,24" "BF16"
run_benchmark "bdgf,aceg->abcdef" "24,20,20,24,20,20,24" "BF16"
run_benchmark "acgf,bdeg->abcdef" "24,20,20,24,20,20,24" "BF16"
run_benchmark "abgf,cdeg->abcdef" "24,20,20,24,20,20,24" "BF16"
run_benchmark "efhjki,gjk->efghi" "6,64,24,4,94,6,64" "BF16"
run_benchmark "bced,ae->abcd" "96,84,84,84,96" "BF16"
run_benchmark "aced,be->abcd" "96,84,84,84,96" "BF16"
run_benchmark "abed,ce->abcd" "96,84,84,84,96" "BF16"
run_benchmark "aedc,ebd->abc" "96,84,84,84,96" "BF16"
run_benchmark "gkiljh,ekilfj->efgh" "6,64,4,94,6,64,6,64" "BF16"
run_benchmark "gikljh,eiklfj->efgh" "6,64,4,94,4,94,6,64" "BF16"
run_benchmark "efiklj,ghkl->efghij" "6,64,4,94,4,94,6,64" "BF16"
run_benchmark "efiklj,ghkl->efghij" "6,64,6,64,4,94,4,94" "BF16"
run_benchmark "cefd,aebf->abcd" "96,84,84,84,84,96" "BF16"
run_benchmark "aefd,becf->abcd" "96,84,84,84,96,96" "BF16"
run_benchmark "cfed,afbe->abcd" "96,84,84,96,84,84" "BF16"

echo "Benchmark completed!"
echo "Results saved to: $OUTPUT_CSV"
echo "Full logs saved to: $FULL_LOG"
echo ""

echo "Summary:"
echo "  Total expressions tested: 24"
echo "  Data type tested: BF16"
echo "  Total benchmark runs: 24"
echo ""

echo "You can view the results with:"
echo "  cat $OUTPUT_CSV                    # CSV results with all metrics"
echo "  less $FULL_LOG                     # Full detailed logs"
echo "  grep 'gflops:' $FULL_LOG           # Just GFLOPS values"
echo "  grep 'ERROR' $OUTPUT_CSV           # Check for failed runs"
echo "  grep 'relative error:' $FULL_LOG   # Check relative errors"