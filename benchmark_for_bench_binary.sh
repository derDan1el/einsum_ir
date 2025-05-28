#!/bin/bash

# benchmark_einsum.sh - Benchmarking Script für einsum_ir

# Output CSV file
OUTPUT_CSV="benchmark_results.csv"
ERROR_LOG="benchmark_errors.log"

# Binary path
BINARY="./build/bench_binary"

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

# Initialize CSV with header and clear error log
echo "Einsum,Dimensions,Dtype,Torch_GFLOPS,JIT_GFLOPS,Matmul_GFLOPS" > "$OUTPUT_CSV"
echo "=== Benchmark Error Log ===" > "$ERROR_LOG"
echo "Date: $(date)" >> "$ERROR_LOG"
echo "" >> "$ERROR_LOG"

# Function to extract GFLOPS from output
extract_metrics() {
    local output="$1"
    
    # Extract alle gflops-Werte in der Reihenfolge wie sie erscheinen
    local all_gflops=($(echo "$output" | grep "gflops:" | awk '{print $2}'))
    
    # Erste ist torch, zweite ist JIT, dritte ist matmul
    local torch_gflops="${all_gflops[0]}"
    local jit_gflops="${all_gflops[1]}"
    local matmul_gflops="${all_gflops[2]}"
    
    echo "$torch_gflops,$jit_gflops,$matmul_gflops"
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
    
    # Run the benchmark and capture both stdout and stderr
    local output
    local stderr_output
    
    # Create temporary files for stdout and stderr
    local stdout_file=$(mktemp)
    local stderr_file=$(mktemp)
    
    # Run command with separated output streams
    eval $cmd > "$stdout_file" 2> "$stderr_file"
    local exit_code=$?
    
    # Read outputs
    output=$(cat "$stdout_file")
    stderr_output=$(cat "$stderr_file")
    
    # Clean up temp files
    rm "$stdout_file" "$stderr_file"
    
    # Log any errors
    if [ $exit_code -ne 0 ] || [ -n "$stderr_output" ]; then
        echo "----------------------------------------" >> "$ERROR_LOG"
        echo "Command: $cmd" >> "$ERROR_LOG"
        echo "Exit code: $exit_code" >> "$ERROR_LOG"
        echo "Timestamp: $(date)" >> "$ERROR_LOG"
        if [ -n "$stderr_output" ]; then
            echo "STDERR:" >> "$ERROR_LOG"
            echo "$stderr_output" >> "$ERROR_LOG"
        fi
        if [ $exit_code -ne 0 ]; then
            echo "STDOUT:" >> "$ERROR_LOG"
            echo "$output" >> "$ERROR_LOG"
        fi
        echo "" >> "$ERROR_LOG"
    fi
    
    local metrics=$(extract_metrics "$output")
    
    # Write to CSV
    echo "\"$einsum\",\"$dimensions\",\"$dtype\",$metrics" >> "$OUTPUT_CSV"
    
    # Print progress with better error handling
    local torch_val=$(echo $metrics | cut -d',' -f1)
    local jit_val=$(echo $metrics | cut -d',' -f2)
    local matmul_val=$(echo $metrics | cut -d',' -f3)
    
    # Check for empty values and show warnings
    if [ -z "$torch_val" ]; then
        torch_val="ERROR"
        echo "  WARNING: Could not extract Torch GFLOPS!" >> "$ERROR_LOG"
    fi
    if [ -z "$jit_val" ]; then
        jit_val="ERROR"
        echo "  WARNING: Could not extract JIT GFLOPS for command: $cmd" >> "$ERROR_LOG"
        echo "  Full output:" >> "$ERROR_LOG"
        echo "$output" >> "$ERROR_LOG"
        echo "" >> "$ERROR_LOG"
    fi
    if [ -z "$matmul_val" ]; then
        matmul_val="ERROR"
        echo "  WARNING: Could not extract Matmul GFLOPS!" >> "$ERROR_LOG"
    fi
    
    echo "  Torch: $torch_val GFLOPS"
    echo "  JIT:   $jit_val GFLOPS" 
    echo "  Matmul: $matmul_val GFLOPS"
    echo ""
}

# Function to run benchmark for all data types
run_benchmark_all_dtypes() {
    local einsum="$1"
    local dimensions="$2"
    
    # Run for each data type
    run_benchmark "$einsum" "$dimensions" "BF16"
    run_benchmark "$einsum" "$dimensions" "FP32"
    run_benchmark "$einsum" "$dimensions" "FP64"
}

echo "Starting einsum_ir benchmark suite..."
echo "Results will be written to: $OUTPUT_CSV"
echo "Errors will be logged to: $ERROR_LOG"
echo ""

# Test cases - verschiedene Einsum-Operationen und Größen

# 1. Simple matrix multiply variations
run_benchmark_all_dtypes "km,nk->nm" "256,256,256"
run_benchmark_all_dtypes "km,nk->nm" "512,512,512"

run_benchmark_all_dtypes "km,nk->nm" "128,128,128"
run_benchmark_all_dtypes "km,nk->nm" "256,128,128"
run_benchmark_all_dtypes "km,nk->nm" "512,128,128"
run_benchmark_all_dtypes "km,nk->nm" "768,128,128"
run_benchmark_all_dtypes "km,nk->nm" "1024,128,128"
run_benchmark_all_dtypes "km,nk->nm" "1536,128,128"
run_benchmark_all_dtypes "km,nk->nm" "2048,128,128"
run_benchmark_all_dtypes "km,nk->nm" "3072,128,128"
run_benchmark_all_dtypes "km,nk->nm" "4096,128,128"
run_benchmark_all_dtypes "km,nk->nm" "6144,128,128"
run_benchmark_all_dtypes "km,nk->nm" "8192,128,128"
run_benchmark_all_dtypes "km,nk->nm" "12288,128,128"
run_benchmark_all_dtypes "km,nk->nm" "16384,128,128"

# 2. Batch matrix multiply
run_benchmark_all_dtypes "bkm,bnk->bnm" "4,128,128,128"
run_benchmark_all_dtypes "bkm,bnk->bnm" "8,128,128,128"
run_benchmark_all_dtypes "bkm,bnk->bnm" "16,128,128,128"
run_benchmark_all_dtypes "bkm,bnk->bnm" "32,128,128,128"

# brgemm
run_benchmark_all_dtypes "bkm,bnk->nm" "16,128,128,128"
run_benchmark_all_dtypes "bkm,bnk->nm" "16,256,128,128"
run_benchmark_all_dtypes "bkm,bnk->nm" "16,512,128,128"
run_benchmark_all_dtypes "bkm,bnk->nm" "16,768,128,128"
run_benchmark_all_dtypes "bkm,bnk->nm" "16,1024,128,128"
run_benchmark_all_dtypes "bkm,bnk->nm" "16,1536,128,128"
run_benchmark_all_dtypes "bkm,bnk->nm" "16,2048,128,128"
run_benchmark_all_dtypes "bkm,bnk->nm" "16,3072,128,128"
run_benchmark_all_dtypes "bkm,bnk->nm" "16,4096,128,128"
run_benchmark_all_dtypes "bkm,bnk->nm" "16,6144,128,128"
run_benchmark_all_dtypes "bkm,bnk->nm" "16,8192,128,128"
run_benchmark_all_dtypes "bkm,bnk->nm" "16,12288,128,128"
run_benchmark_all_dtypes "bkm,bnk->nm" "16,16384,128,128"

# 3. Multi-dimensional contractions , k dimensions get reduced
run_benchmark_all_dtypes "ikm,nik->nm" "8,8,128,128"
run_benchmark_all_dtypes "ikm,nik->nm" "16,16,128,128"
run_benchmark_all_dtypes "ikm,nik->nm" "31,31,64,64"
run_benchmark_all_dtypes "ikm,nik->nm" "32,32,64,64"
run_benchmark_all_dtypes "ikm,nik->nm" "64,64,64,64"

# 5. Complex batch operations
run_benchmark_all_dtypes "abkm,abnk->abnm" "2,4,128,128,128"
run_benchmark_all_dtypes "abkm,abnk->abnm" "2,8,128,128,128"
run_benchmark_all_dtypes "abkm,abnk->abnm" "4,4,128,128,128"


# 9. Additional complex test cases
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
run_benchmark_all_dtypes "fihg,dieh->defg" "151,48,181,40,151,48"
run_benchmark_all_dtypes "cefd,aebf->abcd" "96,84,84,84,84,96"
#run_benchmark_all_dtypes "aefd,becf->abcd" "96,84,84,84,96,96"
#run_benchmark_all_dtypes "cfed,afbe->abcd" "96,84,84,96,84,84"

echo "Benchmark completed!"
echo "Results saved to: $OUTPUT_CSV"
echo "Errors logged to: $ERROR_LOG"
echo ""

# Check if there were errors
if [ -s "$ERROR_LOG" ] && [ $(wc -l < "$ERROR_LOG") -gt 3 ]; then
    echo "WARNING: Errors were detected during benchmarking!"
    echo "Check $ERROR_LOG for details."
    echo ""
fi


echo ""
echo "You can view the detailed results with:"
echo "  cat $OUTPUT_CSV"
echo "  cat $ERROR_LOG  # For error details"
echo "  or open $OUTPUT_CSV in a spreadsheet application"