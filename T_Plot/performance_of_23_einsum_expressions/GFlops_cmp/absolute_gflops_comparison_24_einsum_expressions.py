import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load data
df = pd.read_csv('../speedup_cmp_times/Benchmark_results.csv')
df['Unique_ID'] = df['Einsum'] + ' | ' + df['Dimensions']
expressions = df['Unique_ID'].unique()
n_expressions = len(expressions)

print(f"Found {n_expressions} unique expressions:")
for i, expr in enumerate(expressions):
    print(f"E{i+1}: {expr}")

# Create figure
fig, ax = plt.subplots(figsize=(22, 12))

# Set up x positions for bars (6 bars per expression: 3 datatypes x 2 implementations)
x = np.arange(n_expressions)
width = 0.13
spacing = 0.02

# Initialize lists for GFLOPS values
torch_fp64_gflops = []
torch_fp32_gflops = []
torch_bf16_gflops = []
jit_fp64_gflops = []
jit_fp32_gflops = []
jit_bf16_gflops = []

# Extract GFLOPS for each unique expression
for expr in expressions:
    expr_data = df[df['Unique_ID'] == expr]
    
    # Get performance values for each data type
    torch_fp64_gflops.append(expr_data[expr_data['Dtype'] == 'FP64']['Torch_GFLOPS'].iloc[0])
    torch_fp32_gflops.append(expr_data[expr_data['Dtype'] == 'FP32']['Torch_GFLOPS'].iloc[0])
    torch_bf16_gflops.append(expr_data[expr_data['Dtype'] == 'BF16']['Torch_GFLOPS'].iloc[0])
    
    jit_fp64_gflops.append(expr_data[expr_data['Dtype'] == 'FP64']['JIT_GFLOPS'].iloc[0])
    jit_fp32_gflops.append(expr_data[expr_data['Dtype'] == 'FP32']['JIT_GFLOPS'].iloc[0])
    jit_bf16_gflops.append(expr_data[expr_data['Dtype'] == 'BF16']['JIT_GFLOPS'].iloc[0])

# Create bar positions
torch_fp64_pos = x - width*2.5 - spacing
torch_fp32_pos = x - width*1.5 - spacing/2
torch_bf16_pos = x - width*0.5 - spacing/2
jit_fp64_pos = x + width*0.5 + spacing/2
jit_fp32_pos = x + width*1.5 + spacing/2
jit_bf16_pos = x + width*2.5 + spacing

# Create bars with color scheme: Torch = Rot/Orange, JIT = Blau
bars1 = ax.bar(torch_fp64_pos, torch_fp64_gflops, width, 
               label='PyTorch FP64', color='#ffcccc', alpha=0.8,         # Sehr helles Rot
               edgecolor='black', linewidth=0.5)
bars2 = ax.bar(torch_fp32_pos, torch_fp32_gflops, width, 
               label='PyTorch FP32', color='#ff9999', alpha=0.8,         # Helles Rot
               edgecolor='black', linewidth=0.5)
bars3 = ax.bar(torch_bf16_pos, torch_bf16_gflops, width, 
               label='PyTorch BF16', color='#cc3333', alpha=0.8,         # Dunkles Rot
               edgecolor='black', linewidth=0.5)
bars4 = ax.bar(jit_fp64_pos, jit_fp64_gflops, width, 
               label='JIT FP64', color='#ccddff', alpha=0.8,             # Sehr helles Blau
               edgecolor='black', linewidth=0.5)
bars5 = ax.bar(jit_fp32_pos, jit_fp32_gflops, width, 
               label='JIT FP32', color='#66b3ff', alpha=0.8,             # Helles Blau
               edgecolor='black', linewidth=0.5)
bars6 = ax.bar(jit_bf16_pos, jit_bf16_gflops, width, 
               label='JIT BF16', color='#0066cc', alpha=0.8,             # Dunkles Blau
               edgecolor='black', linewidth=0.5)

# Customize the plot
ax.set_xlabel('Einsum Expressions', fontsize=14, fontweight='bold')
ax.set_ylabel('Performance (GFLOPS)', fontsize=14, fontweight='bold')
ax.set_title(f'Absolute GFLOPS Performance Comparison: PyTorch vs JIT\n({n_expressions} Expressions, All Data Types)', 
             fontsize=16, fontweight='bold', pad=20)

# Set x-axis labels mit E1-E23
ax.set_xticks(x)
ax.set_xticklabels([f'E{i+1}' for i in range(n_expressions)], fontsize=11)

# Grid-Einstellungen
all_gflops = (torch_fp64_gflops + torch_fp32_gflops + torch_bf16_gflops + 
              jit_fp64_gflops + jit_fp32_gflops + jit_bf16_gflops)
max_gflops = max(all_gflops)
y_max = int(np.ceil(max_gflops / 50) * 50)  # Round up to nearest 50

# Major Grid alle 50 GFLOPS
ax.set_yticks(range(0, y_max + 50, 50))
ax.set_yticks(range(0, y_max + 50, 25), minor=True)
ax.grid(True, which='major', axis='y', color='gray', linestyle='-', linewidth=0.8, alpha=0.6)
ax.grid(True, which='minor', axis='y', color='gray', linestyle=':', linewidth=0.4, alpha=0.4)

# Set y-axis limits
ax.set_ylim(0, y_max)

# Verbesserte Legende - 2 Zeilen für bessere Übersicht
import matplotlib.patches as mpatches
patches = [
    mpatches.Patch(color='#ffcccc', label='PyTorch FP64'),      # Sehr helles Rot
    mpatches.Patch(color='#ff9999', label='PyTorch FP32'),      # Helles Rot
    mpatches.Patch(color='#cc3333', label='PyTorch BF16'),      # Dunkles Rot
    mpatches.Patch(color='#ccddff', label='JIT FP64'),          # Sehr helles Blau
    mpatches.Patch(color='#66b3ff', label='JIT FP32'),          # Helles Blau
    mpatches.Patch(color='#0066cc', label='JIT BF16'),          # Dunkles Blau
]
ax.legend(handles=patches, loc='upper left', fontsize=11, ncol=3)

plt.tight_layout()

# Save the plot
plt.savefig('absolute_gflops_comparison_E1_E23.png', 
            dpi=300, bbox_inches='tight', facecolor='white')
plt.savefig('absolute_gflops_comparison_E1_E23.pdf', 
            dpi=300, bbox_inches='tight', facecolor='white', format='pdf')

# Print statistics
print(f"\n=== ABSOLUTE GFLOPS STATISTIKEN ===")
print()

print("PyTorch Performance:")
print(f"  FP64 - Max: {max(torch_fp64_gflops):.1f} GFLOPS, Durchschnitt: {np.mean(torch_fp64_gflops):.1f} GFLOPS")
print(f"  FP32 - Max: {max(torch_fp32_gflops):.1f} GFLOPS, Durchschnitt: {np.mean(torch_fp32_gflops):.1f} GFLOPS")
print(f"  BF16 - Max: {max(torch_bf16_gflops):.1f} GFLOPS, Durchschnitt: {np.mean(torch_bf16_gflops):.1f} GFLOPS")

print("\nJIT Performance:")
print(f"  FP64 - Max: {max(jit_fp64_gflops):.1f} GFLOPS, Durchschnitt: {np.mean(jit_fp64_gflops):.1f} GFLOPS")
print(f"  FP32 - Max: {max(jit_fp32_gflops):.1f} GFLOPS, Durchschnitt: {np.mean(jit_fp32_gflops):.1f} GFLOPS")
print(f"  BF16 - Max: {max(jit_bf16_gflops):.1f} GFLOPS, Durchschnitt: {np.mean(jit_bf16_gflops):.1f} GFLOPS")

print(f"\n=== OVERALL BEST PERFORMANCE ===")
print(f"Höchste absolute Performance: {max(all_gflops):.1f} GFLOPS")

# Find which expression and implementation achieved the highest performance
max_idx = all_gflops.index(max(all_gflops))
if max_idx < n_expressions:
    best_type = "PyTorch FP64"
    best_expr_idx = max_idx
elif max_idx < 2*n_expressions:
    best_type = "PyTorch FP32"
    best_expr_idx = max_idx - n_expressions
elif max_idx < 3*n_expressions:
    best_type = "PyTorch BF16"
    best_expr_idx = max_idx - 2*n_expressions
elif max_idx < 4*n_expressions:
    best_type = "JIT FP64"
    best_expr_idx = max_idx - 3*n_expressions
elif max_idx < 5*n_expressions:
    best_type = "JIT FP32"
    best_expr_idx = max_idx - 4*n_expressions
else:
    best_type = "JIT BF16"
    best_expr_idx = max_idx - 5*n_expressions

print(f"Erreicht von: {best_type} bei Expression E{best_expr_idx+1}")

print(f"\nPlots gespeichert:")
print("- absolute_gflops_comparison_E1_E23.png")
print("- absolute_gflops_comparison_E1_E23.pdf")

plt.show()
