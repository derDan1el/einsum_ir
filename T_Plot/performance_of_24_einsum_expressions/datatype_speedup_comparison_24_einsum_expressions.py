import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load data
df = pd.read_csv('Benchmark_results.csv')

# Get unique expressions for x-axis
expressions = df['Einsum'].unique()
n_expressions = len(expressions)

# Create figure
fig, ax = plt.subplots(figsize=(18, 10))

# Set up x positions for bars (4 bars per expression)
x = np.arange(n_expressions)
width = 0.18  # Width of each bar
spacing = 0.02  # Small gap between PyTorch and JIT groups

# Initialize lists for speedup values
torch_fp32_speedup = []
torch_bf16_speedup = []
jit_fp32_speedup = []
jit_bf16_speedup = []

# Calculate speedups for each expression
for expr in expressions:
    expr_data = df[df['Einsum'] == expr]
    
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
# PyTorch group: slightly left of center
torch_fp32_pos = x - width*1.5 - spacing/2
torch_bf16_pos = x - width*0.5 - spacing/2

# JIT group: slightly right of center  
jit_fp32_pos = x + width*0.5 + spacing/2
jit_bf16_pos = x + width*1.5 + spacing/2

# Create bars with different colors
bars1 = ax.bar(torch_fp32_pos, torch_fp32_speedup, width, 
               label='PyTorch FP32 Speedup', color='#7fcdbb', alpha=0.8, 
               edgecolor='black', linewidth=0.5)
bars2 = ax.bar(torch_bf16_pos, torch_bf16_speedup, width, 
               label='PyTorch BF16 Speedup', color='#41b6c4', alpha=0.8,
               edgecolor='black', linewidth=0.5)
bars3 = ax.bar(jit_fp32_pos, jit_fp32_speedup, width, 
               label='JIT FP32 Speedup', color='#2c7fb8', alpha=0.8,
               edgecolor='black', linewidth=0.5)
bars4 = ax.bar(jit_bf16_pos, jit_bf16_speedup, width, 
               label='JIT BF16 Speedup', color='#253494', alpha=0.8,
               edgecolor='black', linewidth=0.5)

# Add horizontal line at y=1 (baseline)
ax.axhline(y=1, color='red', linestyle='--', alpha=0.7, linewidth=2, 
           label='FP64 Baseline (1.0x)')

# Customize the plot
ax.set_xlabel('Einsum Expressions', fontsize=12, fontweight='bold')
ax.set_ylabel('Speedup Factor (relative to own FP64)', fontsize=12, fontweight='bold')
ax.set_title('Data Type Speedup Comparison: PyTorch vs JIT\n(Separate FP64 Baselines for Each Implementation)', 
             fontsize=14, fontweight='bold', pad=20)

# Set x-axis labels
ax.set_xticks(x)
ax.set_xticklabels([f'E{i+1}' for i in range(n_expressions)], rotation=45, ha='right')

# Add legend with better positioning
ax.legend(loc='upper left', fontsize=10, ncol=2)

# Add grid for better readability
ax.grid(True, alpha=0.3, axis='y')

# Set y-axis limits (start from 0, add padding at top)
max_speedup = max(max(torch_fp32_speedup), max(torch_bf16_speedup), 
                  max(jit_fp32_speedup), max(jit_bf16_speedup))
ax.set_ylim(0, max_speedup * 1.1)

# Add value labels on bars (only for values > 3x to avoid clutter)
def add_speedup_labels(bars, values, threshold=3.0):
    for bar, value in zip(bars, values):
        if value > threshold:
            height = bar.get_height()
            ax.text(bar.get_x() + bar.get_width()/2., height + max_speedup * 0.01,
                   f'{value:.1f}x', ha='center', va='bottom', 
                   fontsize=8, fontweight='bold')

add_speedup_labels(bars1, torch_fp32_speedup)
add_speedup_labels(bars2, torch_bf16_speedup)
add_speedup_labels(bars3, jit_fp32_speedup)
add_speedup_labels(bars4, jit_bf16_speedup)

# Add group separators (visual aid)
for i in range(n_expressions - 1):
    ax.axvline(x=i + 0.5, color='gray', linestyle=':', alpha=0.5)

# Improve layout
plt.tight_layout()

# Add expression mapping text
expression_mapping = '\n'.join([f'E{i+1}: {expr[:25]}{"..." if len(expr) > 25 else ""}' 
                                for i, expr in enumerate(expressions)])

ax.text(1.02, 0.5, f'Expression Mapping:\n{expression_mapping}', 
        transform=ax.transAxes, fontsize=8, verticalalignment='center',
        bbox=dict(boxstyle='round', facecolor='lightgray', alpha=0.8))

# Adjust figure to accommodate the legend
plt.subplots_adjust(right=0.75)

# Save the plot
plt.savefig('datatype_speedup_comparison_24_einsum_expressions.png', 
            dpi=300, bbox_inches='tight', facecolor='white')
plt.savefig('datatype_speedup_comparison_24_einsum_expressions.pdf', 
            bbox_inches='tight', facecolor='white')

print("Plot saved as:")
print("- datatype_speedup_comparison_24_einsum_expressions.png")
print("- datatype_speedup_comparison_24_einsum_expressions.pdf")

# Print some statistics
print(f"\nSpeedup Statistics:")
print(f"PyTorch FP32 avg speedup: {np.mean(torch_fp32_speedup):.2f}x")
print(f"PyTorch BF16 avg speedup: {np.mean(torch_bf16_speedup):.2f}x")
print(f"JIT FP32 avg speedup: {np.mean(jit_fp32_speedup):.2f}x")
print(f"JIT BF16 avg speedup: {np.mean(jit_bf16_speedup):.2f}x")

print(f"\nBest speedups:")
print(f"Best PyTorch BF16: {max(torch_bf16_speedup):.2f}x (E{np.argmax(torch_bf16_speedup)+1})")
print(f"Best JIT BF16: {max(jit_bf16_speedup):.2f}x (E{np.argmax(jit_bf16_speedup)+1})")

plt.show()