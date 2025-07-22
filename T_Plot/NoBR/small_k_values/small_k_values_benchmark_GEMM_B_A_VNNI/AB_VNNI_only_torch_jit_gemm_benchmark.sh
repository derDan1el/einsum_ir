#!/bin/bash


# Output files
OUTPUT_CSV="sh_results_AB_vnni_only_JIT.csv"
FULL_LOG="sh_results_AB_vnni_only_JIT.log"

# Binary path
BINARY="/home/daniel/einsum_ir/build/bench_binary"

# GEMM parameters
EINSUM="lmk,lnk->nm"
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
echo "Einsum,Dimensions,L_Value,Dtype,JIT_GFLOPS" > "$OUTPUT_CSV"
echo "=== GEMM l-Value Benchmark Log Started: $(date) ===" > "$FULL_LOG"
echo "Testing einsum: $EINSUM with M=$M, N=$N, K=$K, varying l from 1 to 128" >> "$FULL_LOG"
echo "" >> "$FULL_LOG"

# Function to extract GFLOPS from output
extract_gflops() {
    local output="$1"
    # Extract the second GFLOPS value (JIT) - first is torch, second is jit
    local jit_gflops=$(echo "$output" | grep "gflops:" | sed -n '2p' | awk '{print $2}')
    
    echo "$jit_gflops"
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
    
    # Extract GFLOPS value
    local jit_gflops=$(extract_gflops "$full_output")
    
    # Handle empty values
    if [ -z "$jit_gflops" ]; then
        jit_gflops="ERROR"
    fi
    
    # Write to CSV
    echo "\"$EINSUM\",\"$dimensions\",$l_value,\"BF16\",$jit_gflops" >> "$OUTPUT_CSV"
    
    # Print progress
    echo "  BF16: JIT=$jit_gflops GFLOPS"
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

echo ""
echo "Benchmark completed!"
echo "Results saved to: $OUTPUT_CSV"
echo "Full logs saved to: $FULL_LOG"
echo ""

echo "Summary:"
echo "  Einsum expression: $EINSUM"
echo "  Fixed dimensions: M=$M, N=$N, K=$K"
echo "  L values tested: $total_l_values (from 1 to 128 in steps of 1)"
echo "  Data type: BF16 only"
echo "  Total benchmark runs: $total_l_values"
echo ""

echo "You can view the results with:"
echo "  cat $OUTPUT_CSV                    # CSV results"
echo "  less $FULL_LOG                     # Full detailed logs"
echo "  grep 'gflops:' $FULL_LOG           # Just GFLOPS values"
echo "  grep 'ERROR' $OUTPUT_CSV           # Check for failed runs"