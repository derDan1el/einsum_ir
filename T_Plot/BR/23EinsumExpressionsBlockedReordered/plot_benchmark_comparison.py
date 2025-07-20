#!/usr/bin/env python3
"""
Plot benchmark comparison between with and without BR (Blocked Reordered) results.
Compares Torch GFLOPS and JIT GFLOPS for each einsum expression.
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns

def load_csv_files():
    """Load both CSV files and return as dataframes."""
    try:
        df_with_br = pd.read_csv('benchmark_results_with_BR.csv')
        df_without_br = pd.read_csv('benchmark_results_without_BR.csv')
        return df_with_br, df_without_br
    except FileNotFoundError as e:
        print(f"Error: Could not find CSV file: {e}")
        return None, None

def create_comparison_plot(df_with_br, df_without_br):
    """Create comparison plots for Torch and JIT GFLOPS."""
    
    # Set up the plotting style
    plt.style.use('seaborn-v0_8')
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(16, 12))
    
    # Get the number of expressions
    n_expressions = len(df_with_br)
    x_pos = np.arange(n_expressions)
    
    # Create shortened labels for x-axis (first 8 chars of einsum expression)
    labels = [expr[:12] + '...' if len(expr) > 12 else expr for expr in df_with_br['Einsum']]
    
    # Plot 1: Torch GFLOPS comparison
    width = 0.35
    ax1.bar(x_pos - width/2, df_with_br['Torch_GFLOPS'], width, 
            label='With BR', alpha=0.8, color='skyblue')
    ax1.bar(x_pos + width/2, df_without_br['Torch_GFLOPS'], width, 
            label='Without BR', alpha=0.8, color='lightcoral')
    
    ax1.set_xlabel('Einsum Expressions')
    ax1.set_ylabel('GFLOPS')
    ax1.set_title('PyTorch GFLOPS Comparison')
    ax1.set_xticks(x_pos)
    ax1.set_xticklabels(labels, rotation=45, ha='right', fontsize=8)
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # Plot 2: JIT GFLOPS comparison
    ax2.bar(x_pos - width/2, df_with_br['JIT_GFLOPS'], width, 
            label='With BR', alpha=0.8, color='lightgreen')
    ax2.bar(x_pos + width/2, df_without_br['JIT_GFLOPS'], width, 
            label='Without BR', alpha=0.8, color='orange')
    
    ax2.set_xlabel('Einsum Expressions')
    ax2.set_ylabel('GFLOPS')
    ax2.set_title('JIT GFLOPS Comparison')
    ax2.set_xticks(x_pos)
    ax2.set_xticklabels(labels, rotation=45, ha='right', fontsize=8)
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    # Plot 3: Speedup ratios for Torch
    torch_speedup = df_with_br['Torch_GFLOPS'] / df_without_br['Torch_GFLOPS']
    colors_torch = ['green' if x >= 1.0 else 'red' for x in torch_speedup]
    bars3 = ax3.bar(x_pos, torch_speedup, color=colors_torch, alpha=0.7)
    ax3.axhline(y=1.0, color='black', linestyle='--', alpha=0.5)
    ax3.set_xlabel('Einsum Expressions')
    ax3.set_ylabel('Speedup Ratio (With BR / Without BR)')
    ax3.set_title('PyTorch Speedup with BR')
    ax3.set_xticks(x_pos)
    ax3.set_xticklabels(labels, rotation=45, ha='right', fontsize=8)
    ax3.grid(True, alpha=0.3)
    
    # Add value labels on bars
    for i, (bar, val) in enumerate(zip(bars3, torch_speedup)):
        ax3.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.01,
                f'{val:.2f}', ha='center', va='bottom', fontsize=7)
    
    # Plot 4: Speedup ratios for JIT
    jit_speedup = df_with_br['JIT_GFLOPS'] / df_without_br['JIT_GFLOPS']
    colors_jit = ['green' if x >= 1.0 else 'red' for x in jit_speedup]
    bars4 = ax4.bar(x_pos, jit_speedup, color=colors_jit, alpha=0.7)
    ax4.axhline(y=1.0, color='black', linestyle='--', alpha=0.5)
    ax4.set_xlabel('Einsum Expressions')
    ax4.set_ylabel('Speedup Ratio (With BR / Without BR)')
    ax4.set_title('JIT Speedup with BR')
    ax4.set_xticks(x_pos)
    ax4.set_xticklabels(labels, rotation=45, ha='right', fontsize=8)
    ax4.grid(True, alpha=0.3)
    
    # Add value labels on bars
    for i, (bar, val) in enumerate(zip(bars4, jit_speedup)):
        ax4.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.01,
                f'{val:.2f}', ha='center', va='bottom', fontsize=7)
    
    plt.tight_layout()
    return fig

def create_scatter_plot(df_with_br, df_without_br):
    """Create scatter plots comparing performance."""
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
    
    # Torch GFLOPS scatter plot
    ax1.scatter(df_without_br['Torch_GFLOPS'], df_with_br['Torch_GFLOPS'], 
                alpha=0.7, s=50, color='blue')
    
    # Add diagonal line (y=x)
    max_torch = max(df_with_br['Torch_GFLOPS'].max(), df_without_br['Torch_GFLOPS'].max())
    ax1.plot([0, max_torch], [0, max_torch], 'r--', alpha=0.5, label='y=x (no change)')
    
    ax1.set_xlabel('PyTorch GFLOPS (Without BR)')
    ax1.set_ylabel('PyTorch GFLOPS (With BR)')
    ax1.set_title('PyTorch Performance Comparison')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # JIT GFLOPS scatter plot
    ax2.scatter(df_without_br['JIT_GFLOPS'], df_with_br['JIT_GFLOPS'], 
                alpha=0.7, s=50, color='green')
    
    # Add diagonal line (y=x)
    max_jit = max(df_with_br['JIT_GFLOPS'].max(), df_without_br['JIT_GFLOPS'].max())
    ax2.plot([0, max_jit], [0, max_jit], 'r--', alpha=0.5, label='y=x (no change)')
    
    ax2.set_xlabel('JIT GFLOPS (Without BR)')
    ax2.set_ylabel('JIT GFLOPS (With BR)')
    ax2.set_title('JIT Performance Comparison')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    return fig

def print_statistics(df_with_br, df_without_br):
    """Print summary statistics."""
    
    print("=== BENCHMARK COMPARISON STATISTICS ===\n")
    
    # Torch statistics
    torch_speedup = df_with_br['Torch_GFLOPS'] / df_without_br['Torch_GFLOPS']
    print("PyTorch GFLOPS:")
    print(f"  Average speedup: {torch_speedup.mean():.3f}x")
    print(f"  Median speedup:  {torch_speedup.median():.3f}x")
    print(f"  Max speedup:     {torch_speedup.max():.3f}x")
    print(f"  Min speedup:     {torch_speedup.min():.3f}x")
    print(f"  Improved cases:  {(torch_speedup > 1.0).sum()}/{len(torch_speedup)}")
    print()
    
    # JIT statistics
    jit_speedup = df_with_br['JIT_GFLOPS'] / df_without_br['JIT_GFLOPS']
    print("JIT GFLOPS:")
    print(f"  Average speedup: {jit_speedup.mean():.3f}x")
    print(f"  Median speedup:  {jit_speedup.median():.3f}x")
    print(f"  Max speedup:     {jit_speedup.max():.3f}x")
    print(f"  Min speedup:     {jit_speedup.min():.3f}x")
    print(f"  Improved cases:  {(jit_speedup > 1.0).sum()}/{len(jit_speedup)}")
    print()
    
    # Best and worst performers
    print("Top 3 PyTorch improvements:")
    torch_sorted = torch_speedup.sort_values(ascending=False)
    for i in range(min(3, len(torch_sorted))):
        idx = torch_sorted.index[i]
        print(f"  {df_with_br.iloc[idx]['Einsum'][:20]}: {torch_sorted.iloc[i]:.3f}x")
    
    print("\nTop 3 JIT improvements:")
    jit_sorted = jit_speedup.sort_values(ascending=False)
    for i in range(min(3, len(jit_sorted))):
        idx = jit_sorted.index[i]
        print(f"  {df_with_br.iloc[idx]['Einsum'][:20]}: {jit_sorted.iloc[i]:.3f}x")

def main():
    """Main function to run the benchmark comparison."""
    
    print("Loading benchmark data...")
    df_with_br, df_without_br = load_csv_files()
    
    if df_with_br is None or df_without_br is None:
        return
    
    print(f"Loaded {len(df_with_br)} benchmark results.")
    
    # Print statistics
    print_statistics(df_with_br, df_without_br)
    
    # Create plots
    print("\nCreating comparison plots...")
    fig1 = create_comparison_plot(df_with_br, df_without_br)
    fig1.suptitle('Einsum Benchmark Comparison: With vs Without Blocked Reordering', 
                  fontsize=16, y=0.98)
    plt.savefig('benchmark_comparison.png', dpi=300, bbox_inches='tight')
    print("Saved: benchmark_comparison.png")
    
    fig2 = create_scatter_plot(df_with_br, df_without_br)
    fig2.suptitle('Performance Correlation: With vs Without Blocked Reordering', 
                  fontsize=14, y=0.98)
    plt.savefig('performance_correlation.png', dpi=300, bbox_inches='tight')
    print("Saved: performance_correlation.png")
    
    # Show plots
    plt.show()

if __name__ == "__main__":
    main()
