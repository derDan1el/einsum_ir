#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load multi-core data
df = pd.read_csv('benchmark_results_multicore.csv')
df['Unique_ID'] = df['Einsum'] + ' | ' + df['Dimensions']
expressions = df['Unique_ID'].unique()
n_expressions = len(expressions)

print(f"Found {n_expressions} unique expressions for multi-core analysis:")
for i, expr in enumerate(expressions):
    print(f"E{i+1}: {expr}")

# Create figure for absolute GFLOPS comparison
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
ax.set_xlabel('ID', fontsize=18)
ax.set_ylabel('GFLOPS', fontsize=18)

# Set x-axis labels mit E1-E23
ax.set_xticks(x)
ax.set_xticklabels([f'E{i+1}' for i in range(n_expressions)], fontsize=17)

# Grid-Einstellungen - verbesserte Y-Achsen-Skalierung
all_gflops = (torch_fp64_gflops + torch_fp32_gflops + torch_bf16_gflops + 
              jit_fp64_gflops + jit_fp32_gflops + jit_bf16_gflops)
max_gflops = max(all_gflops)

# Intelligente Y-Achsen-Skalierung basierend auf Maximalwert
if max_gflops <= 1000:
    # Für Werte bis 1000: 200er-Schritte
    step_size = 200
    y_max = int(np.ceil(max_gflops / step_size) * step_size)
elif max_gflops <= 5000:
    # Für Werte bis 5000: 500er-Schritte
    step_size = 500
    y_max = int(np.ceil(max_gflops / step_size) * step_size)
elif max_gflops <= 10000:
    # Für Werte bis 10000: 1000er-Schritte
    step_size = 1000
    y_max = int(np.ceil(max_gflops / step_size) * step_size)
else:
    # Für Werte über 10000: 2000er-Schritte
    step_size = 2000
    y_max = int(np.ceil(max_gflops / step_size) * step_size)

# Major Grid mit angepassten Schritten
ax.set_yticks(range(0, y_max + step_size, step_size))
# Minor Grid mit halben Schritten
ax.set_yticks(range(0, y_max + step_size, step_size//2), minor=True)

# Grid-Linien
ax.grid(True, which='major', axis='y', color='gray', linestyle='-', linewidth=0.8, alpha=0.6)
ax.grid(True, which='minor', axis='y', color='gray', linestyle=':', linewidth=0.4, alpha=0.4)

# Y-Achsen-Limits setzen
ax.set_ylim(0, y_max)

# Y-Achsen-Tick-Labels anpassen für bessere Lesbarkeit
ax.tick_params(axis='y', labelsize=17)
if max_gflops > 1000:
    # Für große Werte: Formatierung mit Tausender-Trennzeichen
    ax.yaxis.set_major_formatter(plt.FuncFormatter(lambda x, p: f'{int(x):,}'.replace(',', '.')))

print(f"Y-Achsen-Konfiguration: Max={max_gflops:.0f} GFLOPS, Schritte={step_size}, Y-Max={y_max}")

# Verbesserte Legende - 2 Zeilen für bessere Übersicht
import matplotlib.patches as mpatches
patches = [
    mpatches.Patch(color='#cc3333', label='PyTorch BF16'),      # Dunkles Rot
    mpatches.Patch(color='#0066cc', label='JIT BF16'),          # Dunkles Blau
    mpatches.Patch(color='#ff9999', label='PyTorch FP32'),      # Helles Rot
    mpatches.Patch(color='#66b3ff', label='JIT FP32'),          # Helles Blau
    mpatches.Patch(color='#ffcccc', label='PyTorch FP64'),      # Sehr helles Rot
    mpatches.Patch(color='#ccddff', label='JIT FP64'),          # Sehr helles Blau
]
ax.legend(handles=patches, loc='upper left', fontsize=15, ncol=3)

plt.tight_layout()

# Save the plot
plt.savefig('multicore_gflops_comparison_E1_E23.png', 
            dpi=300, bbox_inches='tight', facecolor='white')
plt.savefig('multicore_gflops_comparison_E1_E23.pdf', 
            dpi=300, bbox_inches='tight', facecolor='white', format='pdf')

# Print statistics
print(f"\n=== MULTI-CORE GFLOPS STATISTIKEN (72 Threads) ===")
print()

print("PyTorch Multi-Core Performance:")
print(f"  FP64 - Max: {max(torch_fp64_gflops):.1f} GFLOPS, Durchschnitt: {np.mean(torch_fp64_gflops):.1f} GFLOPS")
print(f"  FP32 - Max: {max(torch_fp32_gflops):.1f} GFLOPS, Durchschnitt: {np.mean(torch_fp32_gflops):.1f} GFLOPS")
print(f"  BF16 - Max: {max(torch_bf16_gflops):.1f} GFLOPS, Durchschnitt: {np.mean(torch_bf16_gflops):.1f} GFLOPS")

print("\nJIT Multi-Core Performance:")
print(f"  FP64 - Max: {max(jit_fp64_gflops):.1f} GFLOPS, Durchschnitt: {np.mean(jit_fp64_gflops):.1f} GFLOPS")
print(f"  FP32 - Max: {max(jit_fp32_gflops):.1f} GFLOPS, Durchschnitt: {np.mean(jit_fp32_gflops):.1f} GFLOPS")
print(f"  BF16 - Max: {max(jit_bf16_gflops):.1f} GFLOPS, Durchschnitt: {np.mean(jit_bf16_gflops):.1f} GFLOPS")

print(f"\n=== OVERALL BEST MULTI-CORE PERFORMANCE ===")
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
print(f"CPU: NVIDIA Grace mit 72 Arm Neoverse-V2 Kernen")

print(f"\nMulti-Core Plots gespeichert:")
print("- multicore_gflops_comparison_E1_E23.png")
print("- multicore_gflops_comparison_E1_E23.pdf")

# Additional comparison plot: Multi-Core vs Single-Core speedup
print(f"\n=== MULTI-CORE SCALING ANALYSIS ===")
print("Hinweis: Für vollständige Scaling-Analyse werden Single-Core Referenzdaten benötigt")

plt.show()
