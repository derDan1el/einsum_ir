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

def parse_and_plot_speedup_data(data_string):
    # Parse die Daten und gruppiere nach K-Dimension
    lines = data_string.strip().split('\n')
    
    # Dictionary um FP32 und BF16 Werte zu gruppieren
    data_by_k = {}
    einsum_pattern = ""
    
    for line in lines:
        if line.strip() == "":
            continue
          
        from io import StringIO
        parts = next(csv.reader(StringIO(line)))
        
        # Nur FP32 und BF16 Zeilen verarbeiten
        if len(parts) == 6 and parts[2] in ["FP32", "BF16"]:
            if einsum_pattern == "":
                einsum_pattern = parts[0]
            
            # K-Dimension extrahieren
            dimensions = parts[1]
            k_dim = int(dimensions.split(',')[0])
            dtype = parts[2]
            
            # GFLOPS Werte
            torch_val = float(parts[3])
            jit_val = float(parts[4])
            
            # Initialisiere Dictionary für diese K-Dimension
            if k_dim not in data_by_k:
                data_by_k[k_dim] = {}
            
            data_by_k[k_dim][dtype] = {
                'torch': torch_val,
                'jit': jit_val
            }
    
    # Berechne Speedups für jede K-Dimension
    k_values = []
    torch_speedups = []
    jit_speedups = []
    
    for k_dim in sorted(data_by_k.keys()):
        if 'FP32' in data_by_k[k_dim] and 'BF16' in data_by_k[k_dim]:
            fp32_data = data_by_k[k_dim]['FP32']
            bf16_data = data_by_k[k_dim]['BF16']
            
            # Speedup = BF16_GFLOPS / FP32_GFLOPS
            torch_speedup = bf16_data['torch'] / fp32_data['torch']
            jit_speedup = bf16_data['jit'] / fp32_data['jit']
            
            k_values.append(k_dim)
            torch_speedups.append(torch_speedup)
            jit_speedups.append(jit_speedup)
    
    # Plot erstellen
    plt.figure(figsize=(14, 9))
    
    # X-Positionen für die Balken
    x_pos = np.arange(len(k_values))
    width = 0.35
    
    # Balken erstellen
    bars1 = plt.bar(x_pos - width/2, torch_speedups, width, 
                    label='PyTorch: at::einsum', color='#7fcdbb', alpha=0.8)
    bars2 = plt.bar(x_pos + width/2, jit_speedups, width, 
                    label='LIBXSMM JIT', color='#2c7fb8', alpha=0.8)
    
    # Achsen beschriften
    plt.xlabel('K-Dimension', fontweight='bold', fontsize=13)
    plt.ylabel('Performance Speedup (BF16/FP32)', fontweight='bold', fontsize=13)
    plt.title(f'Precision-Induced Performance Acceleration: FP32 -> BF16 Transition\n{einsum_pattern} - (m=n=128)', 
              fontweight='bold', fontsize=15)
    
    # X-Achse Labels setzen
    plt.xticks(x_pos, k_values, rotation=45)
    
    # Y-Achse formatting für Speedup
    plt.gca().yaxis.set_major_formatter(plt.FuncFormatter(lambda y, _: f'{y:.1f}x'))
    
    # Baseline-Linie bei 1.0x (kein Speedup)
    plt.axhline(y=1.0, color='red', linestyle='--', alpha=0.7, linewidth=2, label='No Speedup (1.0x)')
    
    # Legende
    plt.legend(loc='upper right')
    
    # Grid hinzufügen
    plt.grid(True, alpha=0.3, axis='y')
    
    # Speedup-Werte über den Balken anzeigen
    for i, (k, torch_speedup, jit_speedup) in enumerate(zip(k_values, torch_speedups, jit_speedups)):
        # PyTorch Speedup
        plt.text(i - width/2-0.05, torch_speedup + 0.05, f'{torch_speedup:.2f}x', 
                ha='center', va='bottom', fontweight='bold', fontsize=9, color='darkred')
        # JIT Speedup  
        plt.text(i + width/2, jit_speedup + 0.05, f'{jit_speedup:.2f}x', 
                ha='center', va='bottom', fontweight='bold', fontsize=9, color='darkgreen')
    
    # Y-Achse Range anpassen
    max_speedup = max(max(torch_speedups), max(jit_speedups))
    plt.ylim(0, max_speedup * 1.15)
    
    # Layout optimieren
    plt.tight_layout()
    
    # Speichern
    plt.savefig("fp32_to_bf16_speedup_plot.png", dpi=400, bbox_inches='tight')
    
    # Print Statistiken
    print(f"\n=== FP32 → BF16 Speedup Analysis für {einsum_pattern} ===")
    print(f"K-Dimensionen: {k_values}")
    print(f"PyTorch Speedups: {[f'{s:.2f}x' for s in torch_speedups]}")
    print(f"LibXSMM Speedups: {[f'{s:.2f}x' for s in jit_speedups]}")
    
    print(f"\n=== PyTorch Statistiken ===")
    print(f"Durchschnittlicher Speedup: {np.mean(torch_speedups):.2f}x")
    print(f"Maximaler Speedup: {np.max(torch_speedups):.2f}x bei K={k_values[np.argmax(torch_speedups)]}")
    print(f"Minimaler Speedup: {np.min(torch_speedups):.2f}x bei K={k_values[np.argmin(torch_speedups)]}")
    
    print(f"\n=== LibXSMM Statistiken ===")
    print(f"Durchschnittlicher Speedup: {np.mean(jit_speedups):.2f}x")
    print(f"Maximaler Speedup: {np.max(jit_speedups):.2f}x bei K={k_values[np.argmax(jit_speedups)]}")
    print(f"Minimaler Speedup: {np.min(jit_speedups):.2f}x bei K={k_values[np.argmin(jit_speedups)]}")

# Führe die Analyse aus
if __name__ == "__main__":
    parse_and_plot_speedup_data(data_string)