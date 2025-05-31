#!/bin/bash
# filepath: /home/daniel/einsum_ir/T_Plot/error_of_24_einsum_expressions/error_analysis_of_einsum_expressions.sh

# Output-Dateien
output_file="errors_of_24_einsum_expressions.csv"
log_file="errors_of_24_einsum_expressions.log"

# CSV-Header schreiben
echo "einsum_expression,dimensions,max_element_torch,max_element_jit,max_difference,biggest_relative_error" > "$output_file"

# Log-Datei initialisieren
echo "=== BF16 Error Analysis of 24 Einsum Expressions ===" > "$log_file"
echo "Started at: $(date)" >> "$log_file"
echo "=======================================================" >> "$log_file"
echo "" >> "$log_file"

# Array mit allen Einsum-Ausdrücken und Dimensionen
einsum_tests=(
    #"abdfe,cf->abcde" "48,36,24,36,48,36"
    #"acdfe,bf->abcde" "48,24,36,36,48,36"
    #"abed,ce->abcd" "96,84,24,96,96"
    #"abcfe,df->abcde" "48,36,36,24,48,48"
    #"aced,be->abcd" "96,24,84,96,84"
    #"jki,efghjk->efghi" "6,64,6,64,24,6,64"
    #"abed,ce->abcd" "96,84,24,84,96"
    #"bcgf,adeg->abcdef" "24,20,20,24,20,20,24"
    #"bdgf,aceg->abcdef" "24,20,20,24,20,20,24"
    #"acgf,bdeg->abcdef" "24,20,20,24,20,20,24"
    #"abgf,cdeg->abcdef" "24,20,20,24,20,20,24"
    #"efhjki,gjk->efghi" "6,64,24,4,94,6,64"
    #"bced,ae->abcd" "96,84,84,84,96"
    #"aced,be->abcd" "96,84,84,84,96"
    #"abed,ce->abcd" "96,84,84,84,96"
    #"aedc,ebd->abc" "96,84,84,84,96"
    #"gkiljh,ekilfj->efgh" "6,64,4,94,6,64,6,64"
    #"gikljh,eiklfj->efgh" "6,64,4,94,4,94,6,64"
    #"efiklj,ghkl->efghij" "6,64,4,94,4,94,6,64"
    #"efiklj,ghkl->efghij" "6,64,6,64,4,94,4,94"
    #"fihg,dieh->defg" "151,48,181,40,151,48"
    #"cefd,aebf->abcd" "96,84,84,84,84,96"
    #"aefd,becf->abcd" "96,84,84,84,96,96"
    #"cfed,afbe->abcd" "96,84,84,96,84,84"
    #neues Set
    "efbad,cf->abcde" "48,36,24,36,48,36"
    "efcad,bf->abcde" "48,24,36,36,48,36"
    "dbea,ec->abcd" "96,84,24,96,96"
    "ecbfa,fd->abcde" "48,36,36,24,48,48"
    "deca,be->abcd" "96,24,84,96,84"
    "ghjkef,jki->efghi" "6,64,6,64,24,6,64"
    "ebad,ce->abcd" "96,84,24,84,96"
    "dega,gfbc->abcdef" "24,20,20,24,20,20,24"
    "dfgb,geac->abcdef" "24,20,20,24,20,20,24"
    "degb,gfac->abcdef" "24,20,20,24,20,20,24"
    "degc,gfab->abcdef" "24,20,20,24,20,20,24"
    "jkhief,gjk->efghi" "6,64,24,4,94,6,64"
    "ea,ebcd->abcd" "96,84,84,84,96"
    "eb,aecd->abcd" "96,84,84,84,96"
    "ec,abed->abcd" "96,84,84,84,96"
    "adec,ebd->abc" "96,84,84,84,96"
    "ijefkl,klijgh->efgh" "6,64,4,94,6,64,6,64"
    "efijkl,klghij->efgh" "6,64,4,94,4,94,6,64"
    "efijkl,klgh->efghij" "6,64,4,94,4,94,6,64"
    "efklij,ghkl->efghij" "6,64,6,64,4,94,4,94"
    "dehi,hifg->defg" "151,48,181,40,151,48"
    "aebf,fdec->abcd" "96,84,84,84,84,96"
    "eafd,fbec->abcd" "96,84,84,84,96,96"
    "aebf,dfce->abcd" "96,84,84,96,84,84"
)

# Schleife durch alle Tests (Array wird in 2er-Paaren verarbeitet)
test_counter=1
total_tests=$((${#einsum_tests[@]}/2))

for ((i=0; i<${#einsum_tests[@]}; i+=2)); do
    einsum_expr="${einsum_tests[i]}"
    dimensions="${einsum_tests[i+1]}"
    
    echo "Testing ($test_counter/$total_tests): $einsum_expr with dimensions $dimensions"
    
    # Log-Header für diesen Test
    echo "--- Test $test_counter/$total_tests ---" >> "$log_file"
    echo "Einsum: $einsum_expr" >> "$log_file"
    echo "Dimensions: $dimensions" >> "$log_file"
    echo "Command: OMP_NUM_THREADS=144 /home/daniel/einsum_ir/build/bench_binary \"$einsum_expr\" \"$dimensions\" BF16" >> "$log_file"
    echo "" >> "$log_file"
    
    # Befehl ausführen und sowohl in Variable als auch in Log-Datei speichern
    output=$(OMP_NUM_THREADS=144 /home/daniel/einsum_ir/build/bench_binary "$einsum_expr" "$dimensions" BF16 2>&1)
    
    # Komplette Ausgabe in Log-Datei schreiben
    echo "$output" >> "$log_file"
    
    # Werte extrahieren
    max_torch=$(echo "$output" | grep "max element torch" | awk '{print $5}')
    max_jit=$(echo "$output" | grep "max element jit" | awk '{print $5}')
    max_diff=$(echo "$output" | grep "max difference" | awk '{print $4}')
    rel_error=$(echo "$output" | grep "Biggest relative error:" | awk '{print $4}')
    
    # Falls relative error nicht vorhanden (bei max_diff = 0), setze auf 0.0
    if [[ -z "$rel_error" ]]; then
        rel_error="0.0"
    fi
    
    # Debug-Ausgabe im Terminal
    echo "  Max torch: $max_torch, Max jit: $max_jit"
    echo "  Max diff: $max_diff, Rel error: $rel_error"
    
    # Zusammenfassung in Log-Datei
    echo "" >> "$log_file"
    echo "EXTRACTED VALUES:" >> "$log_file"
    echo "  Max element torch: $max_torch" >> "$log_file"
    echo "  Max element jit: $max_jit" >> "$log_file"
    echo "  Max difference: $max_diff" >> "$log_file"
    echo "  Biggest relative error: $rel_error" >> "$log_file"
    
    # In CSV schreiben
    echo "\"$einsum_expr\",\"$dimensions\",$max_torch,$max_jit,$max_diff,$rel_error" >> "$output_file"
    
    # Trennlinie in Log-Datei
    echo "" >> "$log_file"
    echo "=================================================================" >> "$log_file"
    echo "" >> "$log_file"
    
    echo "----------------------------------------"
    test_counter=$((test_counter + 1))
done

# Abschluss-Log
echo "=== ANALYSIS COMPLETED ===" >> "$log_file"
echo "Finished at: $(date)" >> "$log_file"
echo "Total tests: $total_tests" >> "$log_file"
echo "Results saved to: $output_file" >> "$log_file"

echo "Analysis complete! Results saved to $output_file"
echo "All terminal outputs logged to $log_file"

# Zusammenfassung anzeigen
echo ""
echo "=== SUMMARY ==="
echo "Total expressions tested: $total_tests"
echo ""
echo "First 5 results:"
head -6 "$output_file"
echo ""
echo "Expressions with highest relative errors:"
tail -n +2 "$output_file" | sort -t',' -k6 -nr | head -5