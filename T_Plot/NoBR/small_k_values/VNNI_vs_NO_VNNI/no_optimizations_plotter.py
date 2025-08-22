#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

div2_jit = pd.read_csv('128_standard_gemm_div2only_jit.csv')
k_values_div2 = div2_jit['K_Value'].values
jit_gflops_div2 = div2_jit['JIT_GFLOPS'].values
div2_torch = pd.read_csv('128_standard_gemm_div2only_torch.csv')
torch_gflops_div2 = div2_torch['Torch_GFLOPS'].values

div4_jit = pd.read_csv('128_standard_gemm_div4_jit.csv')
k_values_div4 = div4_jit['K_Value'].values
jit_gflops_div4 = div4_jit['JIT_GFLOPS'].values
div4_torch = pd.read_csv('128_standard_gemm_div4_torch.csv')
torch_gflops_div4 = div4_torch['Torch_GFLOPS'].values

plt.figure(figsize=(14, 8))

plt.plot(k_values_div2, jit_gflops_div2, 
         label='einsum_ir: k is div. by 2 only', color='#4a90b8', linewidth=0,
         marker='^', markersize=7, linestyle='None')
plt.plot(k_values_div4, jit_gflops_div4, 
         label='einsum_ir: k is div. by 4', color='#6db4db', linewidth=0,
         marker='D', markersize=6, linestyle='None')
plt.plot(k_values_div2, torch_gflops_div2, 
         label='PyTorch: k is div. by 2 only', color='black', linewidth=0,
         marker='v', markersize=7, linestyle='None')
plt.plot(k_values_div4, torch_gflops_div4, 
         label='PyTorch: k is div. by 4', color='#f57a9b', linewidth=0,
         marker='*', markersize=8, linestyle='None')

plt.xlabel('k value', fontsize=17)
plt.ylabel('gflops', fontsize=17)

plt.legend(fontsize=14, frameon=True, shadow=True)
plt.grid(True, alpha=0.6, linestyle='-', linewidth=1)
plt.ylim(bottom=0)
all_k_values = np.concatenate([k_values_div4, k_values_div2])
k_min = np.min(all_k_values)
k_max = np.max(all_k_values)
x_ticks = np.arange(0, k_max + 128, 128)
plt.xticks(x_ticks, fontsize=15)
plt.yticks(fontsize=15)

plt.tight_layout()
plt.savefig('standard_gemm_bf16_comparison_128.pdf', dpi=600, bbox_inches='tight', format='pdf')