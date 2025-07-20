#!/bin/bash

# torch_jit_gemm_benchmark.sh - Benchmark für xaby,cxy->cab mit variablen K-Werten

# Output files
OUTPUT_CSV="xaby_cxy_cab_k_benchmark_results.csv"
FULL_LOG="xaby_cxy_cab_k_benchmark.log"

# Binary path
BINARY="/home/daniel/einsum_ir/build/bench_binary"

# Einsum parameters
EINSUM="xaby,cxy->cab"
# Dimensions: 16,16,96,k,4 where k varies from 2 to 512

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
echo "Einsum,Dimensions,k_Value,Dtype,Torch_GFLOPS,JIT_GFLOPS" > "$OUTPUT_CSV"
echo "=== xaby,cxy->cab k-value benchmark log started: $(date) ===" > "$FULL_LOG"
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
    
    # Create dimension string: 16,16,96,k,4
    local dimensions="16,16,96,$k_value,4"
    
    # Build command (only bf16)
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
    echo "\"$EINSUM\",\"$dimensions\",$k_value,\"bf16\",$torch_val,$jit_val" >> "$OUTPUT_CSV"
    
    # Print progress
    echo "  bf16: torch=$torch_val gflops, jit=$jit_val gflops"
}

# Function to run benchmark for given k
run_benchmark_for_k() {
    local k_value="$1"
    
    echo ""
    echo "=== testing k=$k_value (dimensions: 16,16,96,$k_value,4) ===" 
    echo "=== testing k=$k_value (dimensions: 16,16,96,$k_value,4) ===" >> "$FULL_LOG"
    
    # Run only bf16
    run_benchmark "$k_value"
    
    echo "" >> "$FULL_LOG"
}

echo "starting xaby,cxy->cab k-value benchmark suite..."
echo "einsum expression: $EINSUM"
echo "dimensions: 16,16,96,k,4 where k varies from 2 to 512"
echo "data type: bf16 only"
echo "results will be written to: $OUTPUT_CSV"
echo "full logs will be written to: $FULL_LOG"
echo ""

# Generate k values from 2 to 512 in steps of 2
total_k_values=0
for k in $(seq 2 2 512); do
    run_benchmark_for_k "$k"
    ((total_k_values++))
done

echo ""
echo "benchmark completed!"
echo "results saved to: $OUTPUT_CSV"
echo "full logs saved to: $FULL_LOG"
echo ""

echo "summary:"
echo "  einsum expression: $EINSUM"
echo "  dimensions: 16,16,96,k,4"
echo "  k values tested: $total_k_values (from 2 to 512 in steps of 2)"
echo "  data type: bf16 only" 
echo "  total benchmark runs: $total_k_values"
echo ""

echo "you can view the results with:"
echo "  cat $OUTPUT_CSV                    # csv results"
echo "  less $FULL_LOG                     # full detailed logs"
echo "  grep 'gflops:' $FULL_LOG           # just gflops values"
echo "  grep 'ERROR' $OUTPUT_CSV           # check for failed runs"