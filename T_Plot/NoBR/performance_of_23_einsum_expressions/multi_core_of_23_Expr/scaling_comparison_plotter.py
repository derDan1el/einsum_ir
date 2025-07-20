#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load both single-core and multi-core data
df_single = pd.read_csv('benchmark_results_singlecore.csv')
df_multi = pd.read_csv('benchmark_results_multicore.csv')

# Create unique IDs
df_single['Unique_ID'] = df_single['Einsum'] + ' | ' + df_single['Dimensions']
df_multi['Unique_ID'] = df_multi['Einsum'] + ' | ' + df_multi['Dimensions']

expressions = df_single['Unique_ID'].unique()
n_expressions = len(expressions)

print(f"Comparing Single-Core vs Multi-Core performance for {n_expressions} expressions")
print("Single-Core: OMP_NUM_THREADS=1")
print("Multi-Core: OMP_NUM_THREADS=72 (NVIDIA Grace CPU)")
print()

# Focus on BF16 performance for cleaner comparison
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(20, 14))

# === Plot 1: Absolute Performance Comparison ===
x = np.arange(n_expressions)
width = 0.2

# Extract BF16 data
single_torch_bf16 = []
single_jit_bf16 = []
multi_torch_bf16 = []
multi_jit_bf16 = []

for expr in expressions:
    # Single-core BF16
    single_data = df_single[(df_single['Unique_ID'] == expr) & (df_single['Dtype'] == 'BF16')]
    single_torch_bf16.append(single_data['Torch_GFLOPS'].iloc[0])
    single_jit_bf16.append(single_data['JIT_GFLOPS'].iloc[0])
    
    # Multi-core BF16
    multi_data = df_multi[(df_multi['Unique_ID'] == expr) & (df_multi['Dtype'] == 'BF16')]
    multi_torch_bf16.append(multi_data['Torch_GFLOPS'].iloc[0])
    multi_jit_bf16.append(multi_data['JIT_GFLOPS'].iloc[0])

# Create bars for absolute performance
bars1 = ax1.bar(x - width*1.5, single_torch_bf16, width, 
                label='PyTorch BF16 (1 Thread)', color='#ff6b6b', alpha=0.8)
bars2 = ax1.bar(x - width*0.5, multi_torch_bf16, width, 
                label='PyTorch BF16 (72 Threads)', color='#cc1f1f', alpha=0.8)
bars3 = ax1.bar(x + width*0.5, single_jit_bf16, width, 
                label='JIT BF16 (1 Thread)', color='#4ecdc4', alpha=0.8)
bars4 = ax1.bar(x + width*1.5, multi_jit_bf16, width, 
                label='JIT BF16 (72 Threads)', color='#1a8080', alpha=0.8)

ax1.set_xlabel('Einsum Expressions', fontsize=12, fontweight='bold')
ax1.set_ylabel('Performance (GFLOPS)', fontsize=12, fontweight='bold')
ax1.set_title('Single-Core vs Multi-Core Performance Comparison (BF16)\nNVIDIA Grace CPU: 1 Thread vs 72 Threads', 
              fontsize=14, fontweight='bold', pad=20)
ax1.set_xticks(x)
ax1.set_xticklabels([f'E{i+1}' for i in range(n_expressions)], fontsize=10)
ax1.legend(fontsize=10, loc='upper left')
ax1.grid(True, alpha=0.3)

# === Plot 2: Speedup Factors ===
torch_speedup = np.array(multi_torch_bf16) / np.array(single_torch_bf16)
jit_speedup = np.array(multi_jit_bf16) / np.array(single_jit_bf16)

bars5 = ax2.bar(x - width/2, torch_speedup, width, 
                label='PyTorch BF16 Speedup (72x/1x)', color='#ff6b6b', alpha=0.8)
bars6 = ax2.bar(x + width/2, jit_speedup, width, 
                label='JIT BF16 Speedup (72x/1x)', color='#4ecdc4', alpha=0.8)

# Add theoretical maximum line (72x)
ax2.axhline(y=72, color='red', linestyle='--', alpha=0.7, linewidth=2, 
            label='Theoretical Maximum (72x)')

ax2.set_xlabel('Einsum Expressions', fontsize=12, fontweight='bold')
ax2.set_ylabel('Speedup Factor (Multi-Core / Single-Core)', fontsize=12, fontweight='bold')
ax2.set_title('Multi-Core Scaling Efficiency (72 Threads vs 1 Thread)', 
              fontsize=14, fontweight='bold')
ax2.set_xticks(x)
ax2.set_xticklabels([f'E{i+1}' for i in range(n_expressions)], fontsize=10)
ax2.legend(fontsize=10)
ax2.grid(True, alpha=0.3)

plt.tight_layout()

# Save the plot
plt.savefig('single_vs_multicore_scaling_comparison.png', 
            dpi=300, bbox_inches='tight', facecolor='white')
plt.savefig('single_vs_multicore_scaling_comparison.pdf', 
            dpi=300, bbox_inches='tight', facecolor='white', format='pdf')

# Print detailed statistics
print("=== SCALING ANALYSIS RESULTS ===")
print()
print("PyTorch BF16 Scaling:")
print(f"  Single-Core Max: {max(single_torch_bf16):.1f} GFLOPS")
print(f"  Multi-Core Max: {max(multi_torch_bf16):.1f} GFLOPS")
print(f"  Best Speedup: {max(torch_speedup):.1f}x")
print(f"  Average Speedup: {np.mean(torch_speedup):.1f}x")
print(f"  Scaling Efficiency: {np.mean(torch_speedup)/72*100:.1f}%")

print("\nJIT BF16 Scaling:")
print(f"  Single-Core Max: {max(single_jit_bf16):.1f} GFLOPS")
print(f"  Multi-Core Max: {max(multi_jit_bf16):.1f} GFLOPS") 
print(f"  Best Speedup: {max(jit_speedup):.1f}x")
print(f"  Average Speedup: {np.mean(jit_speedup):.1f}x")
print(f"  Scaling Efficiency: {np.mean(jit_speedup)/72*100:.1f}%")

print(f"\n=== OVERALL PERFORMANCE IMPROVEMENT ===")
print(f"Maximum single-core performance: {max(max(single_torch_bf16), max(single_jit_bf16)):.1f} GFLOPS")
print(f"Maximum multi-core performance: {max(max(multi_torch_bf16), max(multi_jit_bf16)):.1f} GFLOPS")
total_improvement = max(max(multi_torch_bf16), max(multi_jit_bf16)) / max(max(single_torch_bf16), max(single_jit_bf16))
print(f"Overall performance improvement: {total_improvement:.1f}x")

print(f"\nScaling comparison plots saved:")
print("- single_vs_multicore_scaling_comparison.png")
print("- single_vs_multicore_scaling_comparison.pdf")

plt.show()
