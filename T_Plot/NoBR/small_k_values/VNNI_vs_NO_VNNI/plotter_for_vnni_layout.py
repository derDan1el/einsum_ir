#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df_vnni = pd.read_csv('gemm_l_benchmark_results.csv')
df_standard = pd.read_csv('divisible_by_4_km_nk_nm.csv')
df_vnni_lr = pd.read_csv('sh_results_AB_vnni_only_JIT.csv')  # New CSV file


k_product_vnni = []
for dimensions_str in df_vnni['Dimensions']:
    dims = dimensions_str.split(',')
    k_product = int(dims[0]) * int(dims[1])
    k_product_vnni.append(k_product)

k_product_vnni = np.array(k_product_vnni)

vnni_jit_gflops = df_vnni['JIT_GFLOPS'].values
vnni_torch_gflops = df_vnni['Torch_GFLOPS'].values

# Process new CSV data (left and right BF16 layout)
k_product_vnni_lr = []
for dimensions_str in df_vnni_lr['Dimensions']:
    dims = dimensions_str.split(',')
    k_product = int(dims[0]) * int(dims[1])  # k * l_value = actual k
    k_product_vnni_lr.append(k_product)

k_product_vnni_lr = np.array(k_product_vnni_lr)
vnni_lr_jit_gflops = df_vnni_lr['JIT_GFLOPS'].values


standard_k_values = df_standard['K_Value'].values
standard_jit_gflops = df_standard['JIT_GFLOPS'].values
standard_torch_gflops = df_standard['Torch_GFLOPS'].values

plt.figure(figsize=(14, 8))  # Größer für bessere Legende

plt.plot(k_product_vnni, vnni_jit_gflops, 
         label='JIT with BF16 layout left', color='#1a4d72', linewidth=2)
plt.plot(standard_k_values, standard_jit_gflops, 
         label='JIT with column Major', color='#4a90b8', linewidth=2)
plt.plot(k_product_vnni_lr, vnni_lr_jit_gflops, 
         label='JIT: left and right bf16 layout', color='#2d7a2d', linewidth=2)
plt.plot(k_product_vnni, vnni_torch_gflops, 
         label='PyTorch with BF16 layout left', color='#8b1538', linewidth=2)
plt.plot(standard_k_values, standard_torch_gflops, 
         label='PyTorch with column Major', color='#d14d6b', linewidth=2)

plt.xlabel('K Value', fontsize=12)
plt.ylabel('GFlops', fontsize=12)
plt.title('BF16 Layout (Left Tensor) vs Column Major Comparison\n M=N=256', fontsize=14)

plt.legend(fontsize=12, frameon=True, shadow=True)
plt.grid(True, alpha=0.6, linestyle='-', linewidth=1)
plt.ylim(bottom=0)

# X-Achse in 128er-Schritten konfigurieren
all_k_values = np.concatenate([k_product_vnni, standard_k_values, k_product_vnni_lr])
k_min = np.min(all_k_values)
k_max = np.max(all_k_values)
# 128er-Schritte: 0, 128, 256, 384, 512, ...
x_ticks = np.arange(0, k_max + 128, 128)
plt.xticks(x_ticks, fontsize=12)
plt.yticks(fontsize=12)

plt.tight_layout()


plt.savefig('vnni_vs_standard_bf16_comparison.png', dpi=600, bbox_inches='tight')

print("=== PERFORMANCE STATISTIKEN (BF16 only) ===")
print()

vnni_jit_max = np.max(vnni_jit_gflops)
vnni_jit_max_idx = np.argmax(vnni_jit_gflops)
vnni_jit_max_k = k_product_vnni[vnni_jit_max_idx]
vnni_jit_avg = np.mean(vnni_jit_gflops)
print(f"JIT with VNNI-A Flag:")
print(f"  Maximum: {vnni_jit_max:.2f} GFLOPS bei K={vnni_jit_max_k}")
print(f"  Durchschnitt: {vnni_jit_avg:.2f} GFLOPS")


standard_jit_max = np.max(standard_jit_gflops)
standard_jit_max_idx = np.argmax(standard_jit_gflops)
standard_jit_max_k = standard_k_values[standard_jit_max_idx]
standard_jit_avg = np.mean(standard_jit_gflops)
print(f"JIT without VNNI layout:")
print(f"  Maximum: {standard_jit_max:.2f} GFLOPS bei K={standard_jit_max_k}")
print(f"  Durchschnitt: {standard_jit_avg:.2f} GFLOPS")

vnni_lr_jit_max = np.max(vnni_lr_jit_gflops)
vnni_lr_jit_max_idx = np.argmax(vnni_lr_jit_gflops)
vnni_lr_jit_max_k = k_product_vnni_lr[vnni_lr_jit_max_idx]
vnni_lr_jit_avg = np.mean(vnni_lr_jit_gflops)
print(f"JIT with left and right BF16 layout:")
print(f"  Maximum: {vnni_lr_jit_max:.2f} GFLOPS bei K={vnni_lr_jit_max_k}")
print(f"  Durchschnitt: {vnni_lr_jit_avg:.2f} GFLOPS")


vnni_torch_max = np.max(vnni_torch_gflops)
vnni_torch_max_idx = np.argmax(vnni_torch_gflops)
vnni_torch_max_k = k_product_vnni[vnni_torch_max_idx]
vnni_torch_avg = np.mean(vnni_torch_gflops)
print(f"PyTorch at::einsum with VNNI-A layout:")
print(f"  Maximum: {vnni_torch_max:.2f} GFLOPS bei K={vnni_torch_max_k}")
print(f"  Durchschnitt: {vnni_torch_avg:.2f} GFLOPS")

standard_torch_max = np.max(standard_torch_gflops)
standard_torch_max_idx = np.argmax(standard_torch_gflops)
standard_torch_max_k = standard_k_values[standard_torch_max_idx]
standard_torch_avg = np.mean(standard_torch_gflops)
print(f"PyTorch at::einsum without VNNI-A layout:")
print(f"  Maximum: {standard_torch_max:.2f} GFLOPS bei K={standard_torch_max_k}")
print(f"  Durchschnitt: {standard_torch_avg:.2f} GFLOPS")

print()
print("=== VERGLEICH ===")
print(f"Bester JIT Wert: {max(vnni_jit_max, standard_jit_max, vnni_lr_jit_max):.2f} GFLOPS")
print(f"Bester PyTorch Wert: {max(vnni_torch_max, standard_torch_max):.2f} GFLOPS")

jit_improvement = ((vnni_jit_max - standard_jit_max) / standard_jit_max) * 100 if standard_jit_max > 0 else 0
torch_improvement = ((vnni_torch_max - standard_torch_max) / standard_torch_max) * 100 if standard_torch_max > 0 else 0
jit_lr_improvement = ((vnni_lr_jit_max - standard_jit_max) / standard_jit_max) * 100 if standard_jit_max > 0 else 0

print()
print("=== PERFORMANCE VERBESSERUNG ===")
print(f"JIT VNNI vs Standard: {jit_improvement:+.1f}%")
print(f"JIT Left+Right BF16 vs Standard: {jit_lr_improvement:+.1f}%")
print(f"PyTorch VNNI vs Standard: {torch_improvement:+.1f}%")


print()
print("=== ZUSÄTZLICHE STATISTIKEN ===")
print(f"Anzahl VNNI-Datenpunkte: {len(vnni_jit_gflops)}")
print(f"Anzahl Standard-Datenpunkte: {len(standard_jit_gflops)}")
print(f"Anzahl Left+Right BF16-Datenpunkte: {len(vnni_lr_jit_gflops)}")
print(f"K-Wert Bereich (VNNI): {np.min(k_product_vnni)} - {np.max(k_product_vnni)}")
print(f"K-Wert Bereich (Standard): {np.min(standard_k_values)} - {np.max(standard_k_values)}")
print(f"K-Wert Bereich (Left+Right BF16): {np.min(k_product_vnni_lr)} - {np.max(k_product_vnni_lr)}")