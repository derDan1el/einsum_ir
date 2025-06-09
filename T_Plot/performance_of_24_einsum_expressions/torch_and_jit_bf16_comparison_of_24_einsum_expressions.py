import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load and filter data
df = pd.read_csv('Benchmark_results.csv')

# Filter for BF16 only
bf16_data = df[df['Dtype'] == 'BF16'].copy()

# Sort by einsum expression to maintain consistent order
bf16_data = bf16_data.sort_values('Einsum').reset_index(drop=True)

# Get unique expressions for x-axis
expressions = bf16_data['Einsum'].unique()
n_expressions = len(expressions)

# Create figure
fig, ax = plt.subplots(figsize=(16, 8))

# Set up x positions for bars
x = np.arange(n_expressions)
width = 0.35

# Extract data for plotting
torch_gflops = []
jit_gflops = []

for expr in expressions:
    expr_data = bf16_data[bf16_data['Einsum'] == expr]
    torch_gflops.append(expr_data['Torch_GFLOPS'].iloc[0])
    jit_gflops.append(expr_data['JIT_GFLOPS'].iloc[0])

# Create bars
bars1 = ax.bar(x - width/2, torch_gflops, width, label='PyTorch BF16', 
               color='#7fcdbb', alpha=0.8, edgecolor='black', linewidth=0.5)
bars2 = ax.bar(x + width/2, jit_gflops, width, label='JIT BF16', 
               color='#2c7fb8', alpha=0.8, edgecolor='black', linewidth=0.5)

# Customize the plot
ax.set_xlabel('Einsum Expressions', fontsize=12, fontweight='bold')
ax.set_ylabel('Performance (GFLOPS)', fontsize=12, fontweight='bold')
ax.set_title('PyTorch vs JIT Performance Comparison for BF16\n24 Einsum Expressions', 
             fontsize=14, fontweight='bold', pad=20)

# Set x-axis labels (use expression numbers for readability)
ax.set_xticks(x)
ax.set_xticklabels([f'E{i+1}' for i in range(n_expressions)], rotation=45, ha='center')

# Add legend
ax.legend(loc='upper left', fontsize=11)

# Verbesserte Grid-Einstellungen für 10 GFLOPS Raster
max_val = max(max(torch_gflops), max(jit_gflops))
y_max = int(np.ceil(max_val / 10) * 10)  # Runde auf nächste 10er-Stelle auf

# Setze Major Ticks alle 10 GFLOPS
ax.set_yticks(range(0, y_max + 10, 10))
# Setze Minor Ticks alle 5 GFLOPS (optional für feineres Raster)
ax.set_yticks(range(0, y_max + 10, 5), minor=True)

# Major Grid (alle 10 GFLOPS) - deutlich sichtbar
ax.grid(True, which='major', axis='y', color='gray', linestyle='-', linewidth=0.8, alpha=0.6)
# Minor Grid (alle 5 GFLOPS) - schwächer sichtbar
ax.grid(True, which='minor', axis='y', color='gray', linestyle=':', linewidth=0.4, alpha=0.4)

# Vertikale Grid-Linien zwischen Expressions (optional)
ax.grid(True, which='major', axis='x', color='lightgray', linestyle=':', linewidth=0.3, alpha=0.5)

# Set y-axis to start from 0 and use calculated max
ax.set_ylim(0, y_max)

# Improve layout
plt.tight_layout()

# Add a small table showing expression mapping (optional)
# Create a text box with expression mapping
expression_mapping = '\n'.join([f'E{i+1}: {expr[:20]}{"..." if len(expr) > 20 else ""}' 
                                for i, expr in enumerate(expressions)])

# Add text box to the plot
ax.text(1.02, 0.5, f'Expression Mapping:\n{expression_mapping}', 
        transform=ax.transAxes, fontsize=8, verticalalignment='center',
        bbox=dict(boxstyle='round', facecolor='lightgray', alpha=0.8))

# Adjust figure size to accommodate the legend
plt.subplots_adjust(right=0.75)

# Save the plot
plt.savefig('torch_and_jit_bf16_comparison_of_24_einsum_expressions.png', 
            dpi=300, bbox_inches='tight', facecolor='white')

# Also save as PDF for better quality
plt.savefig('torch_and_jit_bf16_comparison_of_24_einsum_expressions.pdf', 
            bbox_inches='tight', facecolor='white')

print("Plot saved as:")
print("- torch_and_jit_bf16_comparison_of_24_einsum_expressions.png")
print("- torch_and_jit_bf16_comparison_of_24_einsum_expressions.pdf")

# Show some statistics
print(f"\nStatistics:")
print(f"Average PyTorch BF16 performance: {np.mean(torch_gflops):.1f} GFLOPS")
print(f"Average JIT BF16 performance: {np.mean(jit_gflops):.1f} GFLOPS")
print(f"Average speedup (JIT/PyTorch): {np.mean(np.array(jit_gflops)/np.array(torch_gflops)):.2f}x")
print(f"Best JIT performance: {max(jit_gflops):.1f} GFLOPS (Expression {np.argmax(jit_gflops)+1})")
print(f"Best PyTorch performance: {max(torch_gflops):.1f} GFLOPS (Expression {np.argmax(torch_gflops)+1})")

plt.show()