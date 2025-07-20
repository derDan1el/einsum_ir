import matplotlib.pyplot as plt
import numpy as np
import csv

# HIER FÜGST DU DEINE DATEN EIN (als String)
data_string = """
"km,nk->nm","128,128,128","BF16",52.62,207.62,50.4662
"km,nk->nm","128,128,128","FP32",35.9617,82.1522,50.824
"km,nk->nm","128,128,128","FP64",40.5134,39.3437,50.8329
"km,nk->nm","256,128,128","BF16",71.6315,226.983,61.7023
"km,nk->nm","256,128,128","FP32",35.8061,81.8472,60.9892
"km,nk->nm","256,128,128","FP64",42.284,39.4815,61.1558
"km,nk->nm","512,128,128","BF16",64.1075,233.201,70.2401
"km,nk->nm","512,128,128","FP32",46.7632,80.9761,69.6667
"km,nk->nm","512,128,128","FP64",42.9303,35.3291,68.8089
"km,nk->nm","768,128,128","BF16",73.5741,217.278,72.8283
"km,nk->nm","768,128,128","FP32",49.557,69.9209,73.5243
"km,nk->nm","768,128,128","FP64",42.843,23.8754,72.5614
"km,nk->nm","1024,128,128","BF16",73.5829,211.003,71.8336
"km,nk->nm","1024,128,128","FP32",44.2728,73.8989,71.9587
"km,nk->nm","1024,128,128","FP64",42.5862,31.8609,71.597
"km,nk->nm","1536,128,128","BF16",76.8124,186.481,77.5994
"km,nk->nm","1536,128,128","FP32",51.9335,49.0737,76.9632
"km,nk->nm","1536,128,128","FP64",43.3973,29.0672,74.7927
"km,nk->nm","2048,128,128","BF16",64.9356,182.067,74.6961
"km,nk->nm","2048,128,128","FP32",45.144,66.6839,75.7174
"km,nk->nm","2048,128,128","FP64",43.2237,29.053,71.4046
"km,nk->nm","3072,128,128","BF16",75.6719,152.896,76.2425
"km,nk->nm","3072,128,128","FP32",46.4087,64.1644,76.4767
"km,nk->nm","3072,128,128","FP64",43.576,30.5837,76.9067
"km,nk->nm","4096,128,128","BF16",66.089,152.002,75.9894
"km,nk->nm","4096,128,128","FP32",45.7479,63.2399,76.8984
"km,nk->nm","4096,128,128","FP64",43.0146,29.6304,76.3967
"km,nk->nm","6144,128,128","BF16",65.4826,131.986,77.7718
"km,nk->nm","6144,128,128","FP32",44.8061,66.3248,76.2092
"km,nk->nm","6144,128,128","FP64",43.4517,31.7687,77.2251
"km,nk->nm","8192,128,128","BF16",67.7948,144.396,74.2339
"km,nk->nm","8192,128,128","FP32",43.8081,67.4952,75.0298
"km,nk->nm","8192,128,128","FP64",42.7572,32.1899,72.8692
"km,nk->nm","12288,128,128","BF16",64.7543,152.018,77.3713
"km,nk->nm","12288,128,128","FP32",43.478,67.9089,76.7977
"km,nk->nm","12288,128,128","FP64",43.0778,32.2063,76.6383
"km,nk->nm","16384,128,128","BF16",56.7504,150.477,63.528
"km,nk->nm","16384,128,128","FP32",36.7197,68.5291,66.3993
"km,nk->nm","16384,128,128","FP64",41.7614,30.465,60.3132
"""

def parse_and_plot_bf16_data(data_string):
    # Parse die Daten
    lines = data_string.strip().split('\n')
    
    k_values = []
    torch_gflops = []
    jit_gflops = []
    einsum_pattern = ""
    
    for line in lines:
        if line.strip() == "":
            continue
          
        from io import StringIO
        parts = next(csv.reader(StringIO(line)))
        
        # Nur BF16 Zeilen verarbeiten
        print(parts[2]) # Debug-Ausgabe
        if len(parts) == 6 and parts[2] == "BF16":
            if einsum_pattern == "":
                einsum_pattern = parts[0]

            
            # K-Dimension extrahieren (erste Zahl aus dimensions)
            dimensions = parts[1]
            print(dimensions) # Debug-Ausgabe
            k_dim = int(dimensions.split(',')[0]) # nimmt das erste Element von dimensions "16384,128,128"->16384 
            
            # GFLOPS Werte
            torch_val = float(parts[3])
            jit_val = float(parts[4])
            # parts[5] ist matmul
            
            k_values.append(k_dim)
            torch_gflops.append(torch_val)
            jit_gflops.append(jit_val)
    
    # Plot erstellen
    plt.figure(figsize=(12, 8))
    
    # X-Positionen für die Balken
    x_pos = np.arange(len(k_values)) # in x_pos werden die Indizes der K-Dimensionen gespeichert damit sie gleichmäßig verteilt werden können
    width = 0.35  # Breite der Balken
    
    # Balken erstellen
    bars1 = plt.bar(x_pos - width/2, torch_gflops, width, 
                    label='PyTorch: at::einsum', color='#ff6b6b', alpha=0.8)
    bars2 = plt.bar(x_pos + width/2, jit_gflops, width, 
                    label='LIBSMM JIT', color='#4ecdc4', alpha=0.8)
    
    # Achsen beschriften
    plt.xlabel('K-Dimension', fontweight='bold', fontsize=12)
    plt.ylabel('GFLOPS', fontweight='bold', fontsize=12)
    plt.title(f'{einsum_pattern} - (m=n=128) - BF16 Performance Comparison', 
              fontweight='bold', fontsize=14)
    
    # X-Achse Labels setzen
    plt.xticks(x_pos, k_values, rotation=45)
    
    # Legende
    plt.legend()
    
    # Grid hinzufügen
    plt.grid(True, alpha=0.3, axis='y')
    
    # Speedup Annotations hinzufügen
    for i, (k, torch_val, jit_val) in enumerate(zip(k_values, torch_gflops, jit_gflops)):
        speedup = jit_val / torch_val
        plt.text(i, max(torch_val, jit_val) + 5, f'{speedup:.1f}x', 
                ha='center', va='bottom', fontweight='bold', fontsize=9)
    
    # Layout optimieren
    plt.tight_layout()
    
    plt.savefig("bf16_performance_comparison.png", dpi=400)
    
    # Print Statistiken
    print(f"\n=== BF16 Performance Analysis für {einsum_pattern} ===")
    print(f"K-Dimensionen: {k_values}")
    print(f"PyTorch GFLOPS: {torch_gflops}")
    print(f"LibXSMM GFLOPS: {jit_gflops}")
    
    speedups = [jit/torch for jit, torch in zip(jit_gflops, torch_gflops)]
    print(f"Speedups: {[f'{s:.2f}x' for s in speedups]}")
    print(f"Durchschnittlicher Speedup: {np.mean(speedups):.2f}x")
    print(f"Maximaler Speedup: {np.max(speedups):.2f}x bei K={k_values[np.argmax(speedups)]}")

# Führe die Analyse aus
if __name__ == "__main__":
    parse_and_plot_bf16_data(data_string)