import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load CSV data
df = pd.read_csv('gemm_k_benchmark_results.csv')

# Filter for BF16 only
bf16_data = df[df['Dtype'] == 'BF16'].copy()

# Sort by k value for proper line plotting
bf16_data = bf16_data.sort_values('K_Value').reset_index(drop=True)

# Extract data
k_values = bf16_data['K_Value'].values
torch_gflops = bf16_data['Torch_GFLOPS'].values
jit_gflops = bf16_data['JIT_GFLOPS'].values

# Create the plot
fig, ax = plt.subplots(figsize=(16, 10))

# Plot lines with markers
torch_line = ax.plot(k_values, torch_gflops, 
                     marker='o', markersize=4, linewidth=2, 
                     color='#cc3333', label='PyTorch BF16', 
                     markerfacecolor='#cc3333', markeredgecolor='white', 
                     markeredgewidth=0.5)

jit_line = ax.plot(k_values, jit_gflops, 
                   marker='s', markersize=4, linewidth=2, 
                   color='#0066cc', label='JIT BF16',
                   markerfacecolor='#0066cc', markeredgecolor='white', 
                   markeredgewidth=0.5)

# Customize the plot
ax.set_xlabel('K Dimension Size', fontsize=12, fontweight='bold')
ax.set_ylabel('Performance (GFLOPS)', fontsize=12, fontweight='bold')
ax.set_title('GEMM Performance vs K Dimension: PyTorch vs JIT (BF16)\nEinsum: km,nk->nm with M=256, N=256', 
             fontsize=14, fontweight='bold', pad=20)

# Set x-axis ticks (every 50 for readability)
ax.set_xticks(range(0, 550, 50))
ax.set_xlim(0, 520)

# Grid settings
max_gflops = max(max(torch_gflops), max(jit_gflops))
y_max = int(np.ceil(max_gflops / 50) * 50)  # Round to next 50

# Major grid every 50 GFLOPS
ax.set_yticks(range(0, y_max + 50, 50))
ax.set_yticks(range(0, y_max + 50, 25), minor=True)
ax.grid(True, which='major', axis='y', color='gray', linestyle='-', linewidth=0.8, alpha=0.6)
ax.grid(True, which='minor', axis='y', color='gray', linestyle=':', linewidth=0.4, alpha=0.4)
ax.grid(True, which='major', axis='x', color='lightgray', linestyle=':', linewidth=0.3, alpha=0.5)

ax.set_ylim(0, y_max)

# Legend
ax.legend(loc='upper left', fontsize=11)

# Add some statistics as text
torch_avg = np.mean(torch_gflops)
jit_avg = np.mean(jit_gflops)
torch_max = np.max(torch_gflops)
jit_max = np.max(jit_gflops)
jit_max_k = k_values[np.argmax(jit_gflops)]
torch_max_k = k_values[np.argmax(torch_gflops)]

stats_text = f"Statistics:\nPyTorch Avg: {torch_avg:.1f} GFLOPS\nJIT Avg: {jit_avg:.1f} GFLOPS\n\nbest Performance:\nPyTorch: {torch_max:.1f} GFLOPS (k={torch_max_k})\nJIT: {jit_max:.1f} GFLOPS (k={jit_max_k})"

ax.text(0.98, 0.02, stats_text, transform=ax.transAxes, 
        verticalalignment='bottom', horizontalalignment='right',
        bbox=dict(boxstyle='round,pad=0.5', facecolor='white', alpha=0.8),
        fontsize=9)

plt.tight_layout()

# Save the plot
plt.savefig('gemm_k_dimension_performance_bf16.png', 
            dpi=300, bbox_inches='tight', facecolor='white')

print("Plot saved as:")
print("- gemm_k_dimension_performance_bf16.png")
print("- gemm_k_dimension_performance_bf16.pdf")

print(f"\nData Summary:")
print(f"K values tested: {len(k_values)} (from {min(k_values)} to {max(k_values)})")
print(f"PyTorch BF16 - Avg: {torch_avg:.1f} GFLOPS, Max: {torch_max:.1f} GFLOPS")
print(f"JIT BF16 - Avg: {jit_avg:.1f} GFLOPS, Max: {jit_max:.1f} GFLOPS")
print(f"JIT Performance Advantage: {jit_avg/torch_avg:.2f}x on average")
# Print theoretical peak performance in yellow in the terminal
print("\033[93mDie Theoretische peak performance liegt bei\n3.1GHz * 4 instrukctions/cycle * 32 FLops/Instruction = 396.8 GFLOPS\033[0m")

plt.show()