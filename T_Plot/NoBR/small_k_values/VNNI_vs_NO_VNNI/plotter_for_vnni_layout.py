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
         label='JIT A-BF16-Layout', color='#1a4d72', linewidth=0,
         marker='o', markersize=6, linestyle='None')
#plt.plot(k_values_div4, jit_gflops_VNNI_AB, 
#         label='JIT A-and-B-BF16-Layout', color='#2d7a2d', linewidth=0,
#         marker='s', markersize=6, linestyle='None')
plt.plot(k_values_div2, jit_gflops_div2, 
         label='JIT (k div by 2 only)', color='#4a90b8', linewidth=0,
         marker='^', markersize=7, linestyle='None')
plt.plot(k_values_div4, jit_gflops_div4, 
         label='JIT (k div by 4)', color='#6db4db', linewidth=0,
         marker='D', markersize=6, linestyle='None')
plt.plot(k_values_div2, torch_gflops_div2, 
         label='PyTorch (k div by 2 only)', color='black', linewidth=0,
         marker='v', markersize=7, linestyle='None')
plt.plot(k_values_div4, torch_gflops_div4, 
         label='PyTorch (k div. by 4)', color='#f57a9b', linewidth=0,
         marker='*', markersize=8, linestyle='None')

plt.xlabel('k value', fontsize=15)
plt.ylabel('gflops', fontsize=15)
plt.legend(fontsize=12, frameon=True, shadow=True)
plt.grid(True, alpha=0.6, linestyle='-', linewidth=1)
plt.ylim(bottom=0)

# X-Achse in 128er-Schritten konfigurieren
all_k_values = np.concatenate([k_values_div4, k_values_div2])
k_min = np.min(all_k_values)
k_max = np.max(all_k_values)
# 128er-Schritte: 0, 128, 256, 384, 512, ...
x_ticks = np.arange(0, k_max + 128, 128)
plt.xticks(x_ticks, fontsize=15)
plt.yticks(fontsize=15)

plt.tight_layout()
plt.savefig('vnni_vs_standard_bf16_comparison_128.pdf', dpi=1000, bbox_inches='tight')
