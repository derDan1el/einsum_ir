#!/bin/bash

# torch_jit_gemm_benchmark.sh - GEMM Benchmarking Script für verschiedene l-Werte

# Output files
OUTPUT_CSV="gemm_l_benchmark_results.csv"
FULL_LOG="gemm_l_benchmark.log"

# Binary path
BINARY="/home/daniel/einsum_ir/build/bench_binary"

# GEMM parameters
EINSUM="lmk,nlk->nm"
M=128
N=128
K=4

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
echo "Einsum,Dimensions,L_Value,Dtype,Torch_GFLOPS,JIT_GFLOPS" > "$OUTPUT_CSV"
echo "=== GEMM l-Value Benchmark Log Started: $(date) ===" > "$FULL_LOG"
echo "Testing einsum: $EINSUM with M=$M, N=$N, K=$K, varying l from 1 to 128" >> "$FULL_LOG"
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
    local l_value="$1"
    
    # Create dimension string: k,l,m,n -> 4,l,128,128 (lexicographic order)
    local dimensions="$K,$l_value,$M,$N"
    
    # Build command (only BF16)
    local cmd="OMP_NUM_THREADS=1 $BINARY \"$EINSUM\" \"$dimensions\" BF16"
    
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
    echo "\"$EINSUM\",\"$dimensions\",$l_value,\"BF16\",$torch_val,$jit_val" >> "$OUTPUT_CSV"
    
    # Print progress
    echo "  BF16: Torch=$torch_val GFLOPS, JIT=$jit_val GFLOPS"
}

# Function to run benchmark for given l
run_benchmark_for_l() {
    local l_value="$1"
    
    echo ""
    echo "=== Testing l=$l_value (M=$M, N=$N, K=$K) ===" 
    echo "=== Testing l=$l_value (M=$M, N=$N, K=$K) ===" >> "$FULL_LOG"
    
    # Run only BF16
    run_benchmark "$l_value"
    
    echo "" >> "$FULL_LOG"
}

echo "Starting GEMM l-value benchmark suite..."
echo "Einsum expression: $EINSUM"
echo "Fixed dimensions: M=$M, N=$N, K=$K"
echo "Variable l: 1, 2, 3, 4, ..., 128 (128 different values)"
echo "Data type: BF16 only"
echo "Results will be written to: $OUTPUT_CSV"
echo "Full logs will be written to: $FULL_LOG"
echo ""

# Generate l values from 1 to 128 in steps of 1
total_l_values=0
for l in $(seq 1 1 128); do
    run_benchmark_for_l "$l"
    ((total_l_values++))
done