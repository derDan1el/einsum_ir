#!/bin/bash

# torch_jit_gemm_benchmark.sh - GEMM Benchmarking Script für verschiedene k-Werte

# Output files
OUTPUT_CSV="gemm_k_benchmark_results.csv"
FULL_LOG="gemm_k_benchmark.log"

# Binary path
BINARY="/home/daniel/einsum_ir/build/bench_binary"

# GEMM parameters
EINSUM="km,nk->nm"
M=256
N=256

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
        echo "\"/home/daniel/einsum_ir/build/bench_binary\" got hardcoded change if needed"
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
echo "Einsum,Dimensions,K_Value,Dtype,Torch_GFLOPS,JIT_GFLOPS" > "$OUTPUT_CSV"
echo "=== GEMM k-Value Benchmark Log Started: $(date) ===" > "$FULL_LOG"
echo "Testing einsum: $EINSUM with M=$M, N=$N, varying K from 2 to 512" >> "$FULL_LOG"
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
    local k_value="$1"
    local dtype="$2"
    
    # Create dimension string: m,n,k -> 256,256,k (lexicographic order)
    local dimensions="$k_value,$M,$N"
    
    # Build command
    local cmd="OMP_NUM_THREADS=1 $BINARY \"$EINSUM\" \"$dimensions\" $dtype"
    
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
    echo "\"$EINSUM\",\"$dimensions\",$k_value,\"$dtype\",$torch_val,$jit_val" >> "$OUTPUT_CSV"
    
    # Print progress
    echo "  $dtype: Torch=$torch_val GFLOPS, JIT=$jit_val GFLOPS"
}

# Function to run benchmark for all data types for a given k
run_benchmark_all_dtypes() {
    local k_value="$1"
    
    echo ""
    echo "=== Testing k=$k_value (M=$M, N=$N) ===" 
    echo "=== Testing k=$k_value (M=$M, N=$N) ===" >> "$FULL_LOG"
    
    # Run for each data type
    run_benchmark "$k_value" "BF16"
    run_benchmark "$k_value" "FP32"
    run_benchmark "$k_value" "FP64"
    
    echo "" >> "$FULL_LOG"
}

echo "Starting GEMM k-value benchmark suite..."
echo "Einsum expression: $EINSUM"
echo "Fixed dimensions: M=$M, N=$N"
echo "Variable k: 2, 4, 6, 8, ..., 512 (256 different values)"
echo "Results will be written to: $OUTPUT_CSV"
echo "Full logs will be written to: $FULL_LOG"
echo ""

# Generate k values from 2 to 512 in steps of 2
total_k_values=0
for k in $(seq 2 2 512); do
    run_benchmark_all_dtypes "$k"
    ((total_k_values++))
done

echo ""
echo "Benchmark completed!"
echo "Results saved to: $OUTPUT_CSV"
echo "Full logs saved to: $FULL_LOG"
echo ""

echo "Summary:"
echo "  Einsum expression: $EINSUM"
echo "  Fixed dimensions: M=$M, N=$N"
echo "  K values tested: $total_k_values (from 2 to 512 in steps of 2)"
echo "  Data types tested: BF16, FP32, FP64"
echo "  Total benchmark runs: $((total_k_values * 3))"
echo ""

echo "You can view the results with:"
echo "  cat $OUTPUT_CSV                    # CSV results"
echo "  less $FULL_LOG                     # Full detailed logs"
echo "  grep 'gflops:' $FULL_LOG           # Just GFLOPS values"
echo "  grep 'ERROR' $OUTPUT_CSV           # Check for failed runs"