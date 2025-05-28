import matplotlib.pyplot as plt
import numpy as np
import csv

# HIER FÜGST DU DEINE DATEN EIN (als String)
data_string = """
"bkm,bnk->nm","16,128,128,128","BF16",84.9835,211.884,74.8947
"bkm,bnk->nm","16,128,128,128","FP32",53.2979,81.3725,73.342
"bkm,bnk->nm","16,128,128,128","FP64",38.3848,36.9541,72.3162
"bkm,bnk->nm","16,256,128,128","BF16",85.3504,220.139,76.6799
"bkm,bnk->nm","16,256,128,128","FP32",50.3874,77.1717,74.1761
"bkm,bnk->nm","16,256,128,128","FP64",38.1869,37.5284,73.601
"bkm,bnk->nm","16,512,128,128","BF16",76.1337,212.194,75.376
"bkm,bnk->nm","16,512,128,128","FP32",48.2816,76.6597,72.0256
"bkm,bnk->nm","16,512,128,128","FP64",34.3783,36.4074,72.294
"bkm,bnk->nm","16,768,128,128","BF16",83.2337,204.955,77.1504
"bkm,bnk->nm","16,768,128,128","FP32",49.0877,76.8617,74.4065
"bkm,bnk->nm","16,768,128,128","FP64",37.455,24.7927,73.9674
"bkm,bnk->nm","16,1024,128,128","BF16",62.9797,203.426,60.9642
"bkm,bnk->nm","16,1024,128,128","FP32",36.4946,72.88,60.9142
"bkm,bnk->nm","16,1024,128,128","FP64",33.7381,35.2349,63.7482
"bkm,bnk->nm","16,1536,128,128","BF16",70.8151,194.879,68.4296
"bkm,bnk->nm","16,1536,128,128","FP32",42.5706,49.2307,67.194
"bkm,bnk->nm","16,1536,128,128","FP64",33.2089,30.5175,65.9243
"bkm,bnk->nm","16,2048,128,128","BF16",55.1501,176.808,51.7922
"bkm,bnk->nm","16,2048,128,128","FP32",31.914,68.9698,53.9907
"bkm,bnk->nm","16,2048,128,128","FP64",30.2115,28.463,50.8833
"bkm,bnk->nm","16,3072,128,128","BF16",65.6118,147.225,66.6771
"bkm,bnk->nm","16,3072,128,128","FP32",34.8729,64.2327,64.3664
"bkm,bnk->nm","16,3072,128,128","FP64",29.8914,29.909,64.8966
"bkm,bnk->nm","16,4096,128,128","BF16",55.8219,153.536,58.4879
"bkm,bnk->nm","16,4096,128,128","FP32",31.0699,65.356,56.8488
"bkm,bnk->nm","16,4096,128,128","FP64",29.2639,27.2652,57.0304
"bkm,bnk->nm","16,6144,128,128","BF16",54.6682,127.125,59.0906
"bkm,bnk->nm","16,6144,128,128","FP32",31.4779,63.0083,58.1522
"bkm,bnk->nm","16,6144,128,128","FP64",28.513,24.0625,58.6983
"bkm,bnk->nm","16,8192,128,128","BF16",51.7524,143.261,59.7128
"bkm,bnk->nm","16,8192,128,128","FP32",31.9051,59.8464,60.1616
"bkm,bnk->nm","16,8192,128,128","FP64",28.4043,25.7895,60.5932
"bkm,bnk->nm","16,12288,128,128","BF16",50.1555,147.835,58.862
"bkm,bnk->nm","16,12288,128,128","FP32",32.3215,56.0139,60.2111
"bkm,bnk->nm","16,12288,128,128","FP64",28.1787,27.2185,60.4828
"bkm,bnk->nm","16,16384,128,128","BF16",50.1875,143.503,60.2305
"bkm,bnk->nm","16,16384,128,128","FP32",31.917,57.4746,60.2415
"bkm,bnk->nm","16,16384,128,128","FP64",27.8451,25.5356,59.9953
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
            
            # K-Dimension extrahieren (Index 1 für batched Pattern)
            dimensions = parts[1]
            k_dim = int(dimensions.split(',')[1])  # Index 1: "16,256,128,128"->256
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
    plt.title(f'Precision-Induced Performance Acceleration: FP32 -> BF16 Transition\n{einsum_pattern} - (B=16, M=N=128)', 
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
    plt.savefig("batched_fp32_to_bf16_speedup_km_nk_nm.png", dpi=400, bbox_inches='tight')
    
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