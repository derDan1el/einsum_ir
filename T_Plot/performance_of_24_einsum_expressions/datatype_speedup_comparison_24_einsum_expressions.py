import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load data
df = pd.read_csv('Benchmark_results.csv')
df['Unique_ID'] = df['Einsum'] + ' | ' + df['Dimensions']
expressions = df['Unique_ID'].unique()
n_expressions = len(expressions)

print(f"Found {n_expressions} unique expressions:")
for i, expr in enumerate(expressions):
    print(f"E{i+1}: {expr}")

# Create figure
fig, ax = plt.subplots(figsize=(20, 10))

# Set up x positions for bars (4 bars per expression)
x = np.arange(n_expressions)
width = 0.18
spacing = 0.02

# Initialize lists for speedup values
torch_fp32_speedup = []
torch_bf16_speedup = []
jit_fp32_speedup = []
jit_bf16_speedup = []

# Calculate speedups for each unique expression
for expr in expressions:
    expr_data = df[df['Unique_ID'] == expr]
    
    # Get performance values for each data type
    torch_fp64 = expr_data[expr_data['Dtype'] == 'FP64']['Torch_GFLOPS'].iloc[0]
    torch_fp32 = expr_data[expr_data['Dtype'] == 'FP32']['Torch_GFLOPS'].iloc[0]
    torch_bf16 = expr_data[expr_data['Dtype'] == 'BF16']['Torch_GFLOPS'].iloc[0]
    
    jit_fp64 = expr_data[expr_data['Dtype'] == 'FP64']['JIT_GFLOPS'].iloc[0]
    jit_fp32 = expr_data[expr_data['Dtype'] == 'FP32']['JIT_GFLOPS'].iloc[0]
    jit_bf16 = expr_data[expr_data['Dtype'] == 'BF16']['JIT_GFLOPS'].iloc[0]
    
    # Calculate speedups (relative to own FP64 baseline)
    torch_fp32_speedup.append(torch_fp32 / torch_fp64)
    torch_bf16_speedup.append(torch_bf16 / torch_fp64)
    jit_fp32_speedup.append(jit_fp32 / jit_fp64)
    jit_bf16_speedup.append(jit_bf16 / jit_fp64)

# Create bar positions
torch_fp32_pos = x - width*1.5 - spacing/2
torch_bf16_pos = x - width*0.5 - spacing/2
jit_fp32_pos = x + width*0.5 + spacing/2
jit_bf16_pos = x + width*1.5 + spacing/2

# Create bars with new color scheme: Torch = Rot, JIT = Blau
bars1 = ax.bar(torch_fp32_pos, torch_fp32_speedup, width, 
               label='PyTorch FP32 Speedup', color='#ff9999', alpha=0.8,  # Helles Rot
               edgecolor='black', linewidth=0.5)
bars2 = ax.bar(torch_bf16_pos, torch_bf16_speedup, width, 
               label='PyTorch BF16 Speedup', color='#cc3333', alpha=0.8,  # Dunkles Rot
               edgecolor='black', linewidth=0.5)
bars3 = ax.bar(jit_fp32_pos, jit_fp32_speedup, width, 
               label='JIT FP32 Speedup', color='#66b3ff', alpha=0.8,      # Helles Blau
               edgecolor='black', linewidth=0.5)
bars4 = ax.bar(jit_bf16_pos, jit_bf16_speedup, width, 
               label='JIT BF16 Speedup', color='#0066cc', alpha=0.8,      # Dunkles Blau
               edgecolor='black', linewidth=0.5)

# Add horizontal line at y=1 (baseline)
ax.axhline(y=1, color='red', linestyle='--', alpha=0.7, linewidth=2)

# Customize the plot
ax.set_xlabel('Einsum Expressions', fontsize=12, fontweight='bold')
ax.set_ylabel('Speedup Factor (relative to own FP64)', fontsize=12, fontweight='bold')
ax.set_title(f'Data Type Speedup Comparison: PyTorch vs JIT\n({n_expressions} Expressions)', 
             fontsize=14, fontweight='bold', pad=20)

# Set x-axis labels mit E1-E24
ax.set_xticks(x)
ax.set_xticklabels([f'E{i+1}' for i in range(n_expressions)], fontsize=10)

# Verbesserte Grid-Einstellungen
max_speedup = max(max(torch_fp32_speedup), max(torch_bf16_speedup), 
                  max(jit_fp32_speedup), max(jit_bf16_speedup))
y_max = int(np.ceil(max_speedup / 2) * 2)

# Major Grid alle 2x Speedup
ax.set_yticks(range(0, y_max + 2, 2))
ax.set_yticks(range(0, y_max + 2, 1), minor=True)
ax.grid(True, which='major', axis='y', color='gray', linestyle='-', linewidth=0.8, alpha=0.6)
ax.grid(True, which='minor', axis='y', color='gray', linestyle=':', linewidth=0.4, alpha=0.4)

# Set y-axis limits
ax.set_ylim(0, y_max)

# Kompakte Legende oben links mit neuen Farben
import matplotlib.patches as mpatches
patches = [
    mpatches.Patch(color='#ff9999', label='PyTorch FP32'),      # Helles Rot
    mpatches.Patch(color='#cc3333', label='PyTorch BF16'),      # Dunkles Rot
    mpatches.Patch(color='#66b3ff', label='JIT FP32'),          # Helles Blau
    mpatches.Patch(color='#0066cc', label='JIT BF16'),          # Dunkles Blau
    mpatches.Patch(color='red', label='FP64 Baseline (1.0x)')
]
ax.legend(handles=patches, loc='upper left', fontsize=10, ncol=5)

plt.tight_layout()

# Save the plot
plt.savefig('datatype_speedup_comparison_E1_E24.png', 
            dpi=300, bbox_inches='tight', facecolor='white')


print(f"\nSpeedup plot saved with {n_expressions} expressions")
print("- datatype_speedup_comparison_E1_E24.png")
print("- datatype_speedup_comparison_E1_E24.pdf")

plt.show()