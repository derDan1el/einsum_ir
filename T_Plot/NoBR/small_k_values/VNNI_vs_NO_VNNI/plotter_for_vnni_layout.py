#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# 1. 128_gemm_VNNI_A_benchmark_results.csv - nur JIT, x-Achse = L_Value * 4
df_gemm_l = pd.read_csv('128_gemm_VNNI_A_benchmark_results.csv')
k_values_div4 = df_gemm_l['L_Value'].values * 4  # L_Value * 4 für k-Wert
jit_gflops_VNNI_A = df_gemm_l['JIT_GFLOPS'].values

# 2. 128_gemm_VNNI_A_B_benchmark_JIT.csv - nur JIT, x-Achse = L_Value * 4
df_sh_vnni = pd.read_csv('128_gemm_VNNI_A_B_benchmark_JIT.csv')
jit_gflops_VNNI_AB = df_sh_vnni['JIT_GFLOPS'].values

# 3. 128_standard_gemm_div2only_jit.csv - nur JIT, x-Achse = K_Value direkt
df_div2_jit = pd.read_csv('128_standard_gemm_div2only_jit.csv')
k_values_div2 = df_div2_jit['K_Value'].values
jit_gflops_div2 = df_div2_jit['JIT_GFLOPS'].values

# 4. 128_standard_gemm_div2only_torch.csv - nur Torch, x-Achse = K_Value direkt
df_div2_torch = pd.read_csv('128_standard_gemm_div2only_torch.csv')
torch_gflops_div2 = df_div2_torch['Torch_GFLOPS'].values

# 5. 128_standard_gemm_div4_jit.csv - nur JIT, x-Achse = K_Value direkt
df_div4_jit = pd.read_csv('128_standard_gemm_div4_jit.csv')
jit_gflops_div4 = df_div4_jit['JIT_GFLOPS'].values

# 6. 128_standard_gemm_div4_torch.csv - nur Torch, x-Achse = K_Value direkt
df_div4_torch = pd.read_csv('128_standard_gemm_div4_torch.csv')
torch_gflops_div4 = df_div4_torch['Torch_GFLOPS'].values

plt.figure(figsize=(14, 8))

# Plot alle 6 Linien mit verschiedenen Markern für Farbenblinde - nur Punkte ohne Verbindungslinien
plt.plot(k_values_div4, jit_gflops_VNNI_A, 
         label='JIT: VNNI A-Layout (left only)', color='#1a4d72', linewidth=0,
         marker='o', markersize=6, linestyle='None')
plt.plot(k_values_div4, jit_gflops_VNNI_AB, 
         label='JIT: VNNI A+B Layout (left+right)', color='#2d7a2d', linewidth=0,
         marker='s', markersize=6, linestyle='None')
plt.plot(k_values_div2, jit_gflops_div2, 
         label='JIT: Standard (k div by 2 only)', color='#4a90b8', linewidth=0,
         marker='^', markersize=7, linestyle='None')
plt.plot(k_values_div4, jit_gflops_div4, 
         label='JIT: Standard (k div by 4)', color='#6db4db', linewidth=0,
         marker='D', markersize=6, linestyle='None')
plt.plot(k_values_div2, torch_gflops_div2, 
         label='Torch: Standard (k div by 2 only)', color='black', linewidth=0,
         marker='v', markersize=7, linestyle='None')
plt.plot(k_values_div4, torch_gflops_div4, 
         label='Torch: Standard (k div by 4)', color='#f57a9b', linewidth=0,
         marker='*', markersize=8, linestyle='None')

plt.xlabel('K Value', fontsize=12)
plt.ylabel('GFlops', fontsize=12)
plt.title('BF16 VNNI Layout vs Standard GEMM Comparison\nM=N=128', fontsize=14)

plt.legend(fontsize=10, frameon=True, shadow=True)
plt.grid(True, alpha=0.6, linestyle='-', linewidth=1)
plt.ylim(bottom=0)

# X-Achse in 128er-Schritten konfigurieren
all_k_values = np.concatenate([k_values_div4, k_values_div2])
k_min = np.min(all_k_values)
k_max = np.max(all_k_values)
# 128er-Schritte: 0, 128, 256, 384, 512, ...
x_ticks = np.arange(0, k_max + 128, 128)
plt.xticks(x_ticks, fontsize=12)
plt.yticks(fontsize=12)

plt.tight_layout()
plt.savefig('vnni_vs_standard_bf16_comparison_128.png', dpi=600, bbox_inches='tight')

print("=== PERFORMANCE STATISTIKEN (BF16, M=N=128) ===")
print()

# Statistiken für alle 6 Linien
datasets = [
    ("JIT: VNNI A-Layout (left only)", k_values_div4, jit_gflops_VNNI_A),
    ("JIT: VNNI A+B Layout (left+right)", k_values_div4, jit_gflops_VNNI_AB),
    ("JIT: Standard (k div by 2 only)", k_values_div2, jit_gflops_div2),
    ("JIT: Standard (k div by 4)", k_values_div4, jit_gflops_div4),
    ("Torch: Standard (k div by 2 only)", k_values_div2, torch_gflops_div2),
    ("Torch: Standard (k div by 4)", k_values_div4, torch_gflops_div4)
]

max_gflops_overall = 0
best_method = ""

for name, k_vals, gflops in datasets:
    max_gflops = np.max(gflops)
    max_idx = np.argmax(gflops)
    max_k = k_vals[max_idx]
    avg_gflops = np.mean(gflops)
    
    print(f"{name}:")
    print(f"  Maximum: {max_gflops:.2f} GFLOPS bei K={max_k}")
    print(f"  Durchschnitt: {avg_gflops:.2f} GFLOPS")
    print(f"  Anzahl Datenpunkte: {len(gflops)}")
    print()
    
    if max_gflops > max_gflops_overall:
        max_gflops_overall = max_gflops
        best_method = name

print("=== VERGLEICH ===")
print(f"Beste Performance insgesamt: {max_gflops_overall:.2f} GFLOPS ({best_method})")
print()

# Vergleiche zwischen VNNI und Standard
jit_vnni_a_max = np.max(jit_gflops_VNNI_A)
jit_vnni_ab_max = np.max(jit_gflops_VNNI_AB)
jit_standard_div4_max = np.max(jit_gflops_div4)
torch_standard_div4_max = np.max(torch_gflops_div4)

jit_vnni_a_improvement = ((jit_vnni_a_max - jit_standard_div4_max) / jit_standard_div4_max) * 100
jit_vnni_ab_improvement = ((jit_vnni_ab_max - jit_standard_div4_max) / jit_standard_div4_max) * 100

print("=== PERFORMANCE VERBESSERUNG vs Standard (k div by 4) ===")
print(f"JIT VNNI A-Layout vs Standard: {jit_vnni_a_improvement:+.1f}%")
print(f"JIT VNNI A+B Layout vs Standard: {jit_vnni_ab_improvement:+.1f}%")