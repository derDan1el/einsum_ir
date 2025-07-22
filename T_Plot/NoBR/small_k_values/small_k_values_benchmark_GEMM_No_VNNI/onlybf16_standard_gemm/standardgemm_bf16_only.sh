#!/bin/bash

# standardgemm_bf16_only.sh - BF16 Standard GEMM Benchmarking Script für verschiedene k-Werte

# Output files for divisible by 4
OUTPUT_CSV_DIV4_JIT="standard_gemm_div4_jit.csv"
OUTPUT_CSV_DIV4_TORCH="standard_gemm_div4_torch.csv"

# Output files for divisible by 2 only (not by 4)
OUTPUT_CSV_DIV2ONLY_JIT="standard_gemm_div2only_jit.csv"
OUTPUT_CSV_DIV2ONLY_TORCH="standard_gemm_div2only_torch.csv"

# Full log file
FULL_LOG="standard_gemm_bf16_benchmark.log"

# Binary path
BINARY="/home/daniel/einsum_ir/build/bench_binary"

# GEMM parameters
EINSUM="km,nk->nm"
M=128
N=128

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

# Initialize files (append mode - don't overwrite existing data)
if [ ! -f "$OUTPUT_CSV_DIV4_TORCH" ]; then
    echo "K_Value,Torch_GFLOPS" > "$OUTPUT_CSV_DIV4_TORCH"
fi
if [ ! -f "$OUTPUT_CSV_DIV4_JIT" ]; then
    echo "K_Value,JIT_GFLOPS" > "$OUTPUT_CSV_DIV4_JIT"
fi
if [ ! -f "$OUTPUT_CSV_DIV2ONLY_TORCH" ]; then
    echo "K_Value,Torch_GFLOPS" > "$OUTPUT_CSV_DIV2ONLY_TORCH"
fi
if [ ! -f "$OUTPUT_CSV_DIV2ONLY_JIT" ]; then
    echo "K_Value,JIT_GFLOPS" > "$OUTPUT_CSV_DIV2ONLY_JIT"
fi

echo "=== BF16 Standard GEMM Benchmark Log Continued: $(date) ===" >> "$FULL_LOG"
echo "Testing einsum: $EINSUM with M=$M, N=$N, varying K from 254 to 512" >> "$FULL_LOG"
echo "Only testing BF16 dtype" >> "$FULL_LOG"
echo "" >> "$FULL_LOG"

# Function to extract GFLOPS from output
extract_gflops() {
    local output="$1"
    
    # Extract Torch GFLOPS from "at::einsum:" section
    local torch_gflops=$(echo "$output" | grep -A3 "at::einsum:" | grep "gflops:" | awk '{print $2}')
    
    # Extract JIT GFLOPS from einsum_ir section (after repetitions line)
    local jit_gflops=$(echo "$output" | grep -A20 "einsum_ir:" | grep "gflops:" | awk '{print $2}')
    
    echo "$torch_gflops,$jit_gflops"
}

# Function to run benchmark
run_benchmark() {
    local k_value="$1"
    
    # Create dimension string: k,m,n -> k,128,128 (lexicographic order)
    local dimensions="$k_value,$M,$N"
    
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
    
    # Determine which CSV files to write to based on k divisibility
    if [ $((k_value % 4)) -eq 0 ]; then
        # k is divisible by 4
        echo "$k_value,$torch_val" >> "$OUTPUT_CSV_DIV4_TORCH"
        echo "$k_value,$jit_val" >> "$OUTPUT_CSV_DIV4_JIT"
        echo "  k=$k_value (div by 4): Torch=$torch_val GFLOPS, JIT=$jit_val GFLOPS"
    else
        # k is divisible by 2 but not by 4
        echo "$k_value,$torch_val" >> "$OUTPUT_CSV_DIV2ONLY_TORCH"
        echo "$k_value,$jit_val" >> "$OUTPUT_CSV_DIV2ONLY_JIT"
        echo "  k=$k_value (div by 2 only): Torch=$torch_val GFLOPS, JIT=$jit_val GFLOPS"
    fi
}

# Function to run benchmark for a given k
run_benchmark_for_k() {
    local k_value="$1"
    
    echo ""
    echo "=== Testing k=$k_value (M=$M, N=$N) BF16 ===" 
    echo "=== Testing k=$k_value (M=$M, N=$N) BF16 ===" >> "$FULL_LOG"
    
    # Run benchmark for BF16 only
    run_benchmark "$k_value"
    
    echo "" >> "$FULL_LOG"
}

echo "Starting BF16 Standard GEMM k-value benchmark suite..."
echo "Einsum expression: $EINSUM"
echo "Fixed dimensions: M=$M, N=$N"
echo "Variable k: 2, 4, 6, 8, ..., 512 (256 different values)"
echo "Data type: BF16 only"
echo "Results will be written to:"
echo "  - $OUTPUT_CSV_DIV4_TORCH (k divisible by 4, Torch performance)"
echo "  - $OUTPUT_CSV_DIV4_JIT (k divisible by 4, JIT performance)"
echo "  - $OUTPUT_CSV_DIV2ONLY_TORCH (k divisible by 2 only, Torch performance)"
echo "  - $OUTPUT_CSV_DIV2ONLY_JIT (k divisible by 2 only, JIT performance)"
echo "Full logs will be written to: $FULL_LOG"
echo ""

# Generate k values from 2 to 512 in steps of 2
total_k_values=0
div4_count=0
div2only_count=0

for k in $(seq 438 2 512); do
    run_benchmark_for_k "$k"
    ((total_k_values++))
    
    if [ $((k % 4)) -eq 0 ]; then
        ((div4_count++))
    else
        ((div2only_count++))
    fi
done

echo ""
echo "Benchmark completed!"
echo "Results saved to:"
echo "  - $OUTPUT_CSV_DIV4_TORCH (k divisible by 4, Torch)"
echo "  - $OUTPUT_CSV_DIV4_JIT (k divisible by 4, JIT)"
echo "  - $OUTPUT_CSV_DIV2ONLY_TORCH (k divisible by 2 only, Torch)"
echo "  - $OUTPUT_CSV_DIV2ONLY_JIT (k divisible by 2 only, JIT)"
echo "Full logs saved to: $FULL_LOG"
echo ""

echo "Summary:"
echo "  Einsum expression: $EINSUM"
echo "  Fixed dimensions: M=$M, N=$N"
echo "  K values tested: $total_k_values (from 2 to 512 in steps of 2)"
echo "  K values divisible by 4: $div4_count"
echo "  K values divisible by 2 only: $div2only_count"
echo "  Data type tested: BF16 only"
echo "  Total benchmark runs: $total_k_values"
echo ""

echo "You can view the results with:"
echo "  cat $OUTPUT_CSV_DIV4_TORCH           # Torch results (k div by 4)"
echo "  cat $OUTPUT_CSV_DIV4_JIT             # JIT results (k div by 4)"
echo "  cat $OUTPUT_CSV_DIV2ONLY_TORCH       # Torch results (k div by 2 only)"
echo "  cat $OUTPUT_CSV_DIV2ONLY_JIT         # JIT results (k div by 2 only)"
echo "  less $FULL_LOG                       # Full detailed logs"
echo "  grep 'gflops:' $FULL_LOG             # Just GFLOPS values"
echo "  grep 'ERROR' *.csv                   # Check for failed runs"