import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Global arrays for all data
k_vals_bf16, k_vals_fp32, k_vals_fp64 = [], [], []  # Separate K values for each dtype
torch_bf16, torch_fp32, torch_fp64 = [], [], []
jit_bf16_div4, jit_bf16_div2 = [], []  # div4 = K%4==0, div2 = K%4!=0
jit_fp32, jit_fp64 = [], []
k_div4, k_div2 = [], []  # K values for BF16 split

# Load data
df = pd.read_csv('gemm_k_benchmark_results.csv')

# Go through each row
for i in range(len(df)):
    # Check if row is empty, if yes break
    if df.iloc[i].isnull().all():
        break
        
    k = df.iloc[i]['K_Value']
    dtype = df.iloc[i]['Dtype'] 
    torch_gflops = df.iloc[i]['Torch_GFLOPS']
    jit_gflops = df.iloc[i]['JIT_GFLOPS']
    
    # Add data based on dtype
    if dtype == 'BF16':
        k_vals_bf16.append(k)
        torch_bf16.append(torch_gflops)
        # Split BF16 JIT by K divisibility
        if k % 4 == 0:
            jit_bf16_div4.append(jit_gflops)
            k_div4.append(k)
        else:
            jit_bf16_div2.append(jit_gflops)
            k_div2.append(k)
    elif dtype == 'FP32':
        k_vals_fp32.append(k)
        torch_fp32.append(torch_gflops)
        jit_fp32.append(jit_gflops)
    elif dtype == 'FP64':
        k_vals_fp64.append(k)
        torch_fp64.append(torch_gflops)
        jit_fp64.append(jit_gflops)

def make_pytorch_plot():
    """Create PyTorch plot."""
    fig, ax = plt.subplots(1, 1, figsize=(10, 6))
    
    ax.plot(k_vals_bf16, torch_bf16, 'darkblue', linewidth=2, label='BF16')
    ax.plot(k_vals_fp32, torch_fp32, 'g-', linewidth=2, label='FP32')
    ax.plot(k_vals_fp64, torch_fp64, 'brown', linewidth=2, label='FP64')
    
    ax.set_title('PyTorch: "km,nk->nm" m=n=256 GFlops Comparison', fontsize=14)
    ax.set_xlabel('K Value', fontsize=12)
    ax.set_ylabel('GFlops', fontsize=12)
    ax.legend(fontsize=11, framealpha=0.7)
    
    # Better grid with finer spacing
    ax.grid(True, alpha=0.6, linestyle='-', linewidth=1)
    
    # Set specific x-axis ticks (64 steps, starting from 0)
    ax.set_xticks([0, 64, 128, 192, 256, 320, 384, 448, 512])
    
    plt.tight_layout()
    return fig

def make_jit_plot():
    """Create JIT plot."""
    fig, ax = plt.subplots(1, 1, figsize=(10, 6))
    
    ax.plot(k_div4, jit_bf16_div4, 'darkblue', linewidth=2, label='BF16 (K%4=0)')
    ax.plot(k_div2, jit_bf16_div2, 'black', linewidth=2, label='BF16 (K%2≠0 only)')
    ax.plot(k_vals_fp32, jit_fp32, 'g-', linewidth=2, label='FP32')
    ax.plot(k_vals_fp64, jit_fp64, 'brown', linewidth=2, label='FP64')
    
    ax.set_title('JIT: "km,nk->nm" m=n=256 GFlops Comparison', fontsize=14)
    ax.set_xlabel('K Value', fontsize=12)
    ax.set_ylabel('GFlops', fontsize=12)
    ax.legend(fontsize=11, framealpha=0.0)
    
    # Set y-axis limit to show up to 250 GFlops
    ax.set_ylim(0, 250)
    
    # Better grid with finer spacing
    ax.grid(True, alpha=0.6, linestyle='-', linewidth=1)
    
    # Set specific x-axis ticks (64 steps, starting from 0)
    ax.set_xticks([0, 64, 128, 192, 256, 320, 384, 448, 512])
    
    plt.tight_layout()
    return fig

def print_stats():
    """Print simple stats."""
    print(f"PyTorch BF16 avg: {np.mean(torch_bf16):.1f} GFLOPS")
    if jit_bf16_div4:
        print(f"JIT BF16 (K%4=0): {np.mean(jit_bf16_div4):.1f} GFLOPS")
    if jit_bf16_div2:
        print(f"JIT BF16 (K%4≠0): {np.mean(jit_bf16_div2):.1f} GFLOPS")
    if jit_bf16_div4 and jit_bf16_div2:
        ratio = np.mean(jit_bf16_div4) / np.mean(jit_bf16_div2)
        print(f"Performance ratio: {ratio:.1f}x better when K%4=0")

# Main execution
if k_vals_bf16:  # Check if we have any data
    total_points = len(k_vals_bf16) + len(k_vals_fp32) + len(k_vals_fp64)
    print(f"Loaded {total_points} data points")
    print_stats()
    
    # Make PyTorch plot
    print("Creating PyTorch plot...")
    fig1 = make_pytorch_plot()
    plt.savefig('pytorch_bf16vsfp32vsfp64.png', dpi=300, bbox_inches='tight')
    print("PyTorch plot saved!")
    
    # Make JIT plot  
    print("Creating JIT plot...")
    fig2 = make_jit_plot()
    plt.savefig('jit_bf16vsfp32vsfp64.png', dpi=300, bbox_inches='tight')
    print("JIT plot saved!")

    
else:
    print("No data found!")

