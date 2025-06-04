#!/bin/bash
# filepath: /home/daniel/einsum_ir/bf16_rounding_error.sh


# Diese .sh datei führt tensorkontraktionen aus von der form  km,nk ->nm wobei alle dimension=128 groß sind
# bis auf die k dimension. Ziel ist es absoluten und relativen Fehler zu analysieren bei steigenden k werten.
output_file="bf16_error_analysis_input_are_ones_144.csv"
log_file="bf16_error_analysis_input_are_ones_144.log"

echo "k_dimension,max_absolute_error,max_relative_error" > "$output_file"

for k in $(seq 4 4 20000); do
    echo "k-dimension size: $k"
    
      abs_error=0
      rel_error=0.0
      output=$(OMP_NUM_THREADS=144 /home/daniel/einsum_ir/build/bench_binary "km,nk->nm" "$k,128,128" BF16 2>&1)
      
      
      abs_error=$(echo "$output" | grep "max difference" | awk '{print $4}')
        
        if [[ "$abs_error" != "0" ]]; then
          rel_error=$(echo "$output" | grep "Biggest relative error:" | awk '{print $4}')
        fi
    
    echo "$k,$abs_error,$rel_error" >> "$output_file"
    echo "k=$k, abs_error=$abs_error, rel_error=$rel_error"
    echo "$output" >> "$log_file"
done  

echo "Analysis complete! Results saved to $output_file"
echo "------------------finished-----------------"