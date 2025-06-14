import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# 1. CSV-Dateien einlesen
df_vnni = pd.read_csv('gemm_l_benchmark_results.csv')
df_standard = pd.read_csv('gemm_k_benchmark_results.csv')

# 2. Array mit Produkt der ersten beiden Dimensionen für VNNI erstellen
k_product_vnni = []
for dimensions_str in df_vnni['Dimensions']:
    dims = dimensions_str.split(',')
    k_product = int(dims[0]) * int(dims[1])
    k_product_vnni.append(k_product)

k_product_vnni = np.array(k_product_vnni)

# 3. Aus df_standard alle BF16 Zeilen filtern
df_standard_bf16 = df_standard[df_standard['Dtype'] == 'BF16']

# 4. GFLOPS extrahieren und K-Werten zuordnen
# VNNI GFLOPS
vnni_jit_gflops = df_vnni['JIT_GFLOPS'].values
vnni_torch_gflops = df_vnni['Torch_GFLOPS'].values

# Standard GFLOPS  
standard_k_values = df_standard_bf16['K_Value'].values
standard_jit_gflops = df_standard_bf16['JIT_GFLOPS'].values
standard_torch_gflops = df_standard_bf16['Torch_GFLOPS'].values

# 5. Ergebnisse plotten
plt.figure(figsize=(12, 8))

plt.plot(k_product_vnni, vnni_jit_gflops, 
         label='JIT with VNNI-A Flag', color='#1a4d72', linewidth=2)  # Dunkles Blau
plt.plot(standard_k_values, standard_jit_gflops, 
         label='JIT without VNNI layout', color='#4a90b8', linewidth=2)  # Helleres Blau
plt.plot(k_product_vnni, vnni_torch_gflops, 
         label='PyTorch at::einsum with VNNI-A layout', color='#8b1538', linewidth=2)  # Dunkles Rot
plt.plot(standard_k_values, standard_torch_gflops, 
         label='PyTorch at::einsum without VNNI-A layout', color='#d14d6b', linewidth=2)  # Helleres Rot

plt.xlabel('K Dimension Size', fontsize=12, fontweight='bold')
plt.ylabel('Performance (GFLOPS)', fontsize=12, fontweight='bold')
plt.title('GEMM Performance: VNNI vs Standard (BF16)', fontsize=14, fontweight='bold')

plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)
plt.ylim(bottom=0)

plt.savefig('vnni_vs_standard_comparison.png', dpi=600, bbox_inches='tight')

# Statistiken berechnen und ausgeben
print("=== PERFORMANCE STATISTIKEN ===")
print()

# JIT with VNNI-A Flag
vnni_jit_max = np.max(vnni_jit_gflops)
vnni_jit_max_idx = np.argmax(vnni_jit_gflops)
vnni_jit_max_k = k_product_vnni[vnni_jit_max_idx]
print(f"JIT with VNNI-A Flag:")
print(f"  Maximum: {vnni_jit_max:.2f} GFLOPS bei K={vnni_jit_max_k}")

# JIT without VNNI layout
standard_jit_max = np.max(standard_jit_gflops)
standard_jit_max_idx = np.argmax(standard_jit_gflops)
standard_jit_max_k = standard_k_values[standard_jit_max_idx]
print(f"JIT without VNNI layout:")
print(f"  Maximum: {standard_jit_max:.2f} GFLOPS bei K={standard_jit_max_k}")

# PyTorch with VNNI-A layout
vnni_torch_max = np.max(vnni_torch_gflops)
vnni_torch_max_idx = np.argmax(vnni_torch_gflops)
vnni_torch_max_k = k_product_vnni[vnni_torch_max_idx]
print(f"PyTorch at::einsum with VNNI-A layout:")
print(f"  Maximum: {vnni_torch_max:.2f} GFLOPS bei K={vnni_torch_max_k}")

# PyTorch without VNNI-A layout
standard_torch_max = np.max(standard_torch_gflops)
standard_torch_max_idx = np.argmax(standard_torch_gflops)
standard_torch_max_k = standard_k_values[standard_torch_max_idx]
print(f"PyTorch at::einsum without VNNI-A layout:")
print(f"  Maximum: {standard_torch_max:.2f} GFLOPS bei K={standard_torch_max_k}")

print()
print("=== VERGLEICH ===")
print(f"Bester JIT Wert: {max(vnni_jit_max, standard_jit_max):.2f} GFLOPS")
print(f"Bester PyTorch Wert: {max(vnni_torch_max, standard_torch_max):.2f} GFLOPS")

plt.show()