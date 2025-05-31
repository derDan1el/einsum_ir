#!/bin/bash
# filepath: /home/daniel/einsum_ir/bf16_rounding_error.sh


# Diese .sh datei führt tensorkontraktionen aus von der form  km,nk ->nm wobei alle dimension=128 groß sind
# bis auf die k dimension. Ziel ist es absoluten und relativen Fehler zu analysieren bei steigenden k werten.
output_file="bf16_error_analysis.csv"

echo "k_dimension,max_absolute_error,max_relative_error" > "$output_file"

for k in $(seq 256 256 32768); do
    echo "k-dimension size: $k"
    
    max_abs_error=0
    max_rel_error=0.0
    
    for run in $(seq 1 20); do
      echo "$run"
      output=$(OMP_NUM_THREADS=1 ./build/bench_binary "km,nk->nm" "$k,128,128" BF16 2>&1)
        
        abs_error=$(echo "$output" | grep "max difference" | awk '{print $4}')
        
        if [[ "$abs_error" != "0" ]]; then
            if (( $(echo "$abs_error > $max_abs_error" | bc -l) )); then
                max_abs_error=$abs_error
            fi
            
            rel_error=$(echo "$output" | grep "Biggest relative error:" | awk '{print $4}')
            
            if (( $(echo "$rel_error > $max_rel_error" | bc -l) )); then
                max_rel_error=$rel_error
            fi
        fi
    done  
    
    echo "$k,$max_abs_error,$max_rel_error" >> "$output_file"
done  

echo "Analysis complete! Results saved to $output_file"
echo "------------------finished-----------------"