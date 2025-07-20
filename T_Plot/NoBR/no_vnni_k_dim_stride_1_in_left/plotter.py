import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load and plot data
df = pd.read_csv('xaby_cxy_cab_k_benchmark_results.csv')
df = df.sort_values('k_Value')

plt.figure(figsize=(12, 8))
plt.plot(df['k_Value'], df['Torch_GFLOPS'], label='pytorch bf16', color='#cc3333', linewidth=2)
plt.plot(df['k_Value'], df['JIT_GFLOPS'], label='jit bf16', color='#0066cc', linewidth=2)

plt.xlabel('x dimension size', fontsize=12, fontweight='bold')
plt.ylabel('performance (gflops)', fontsize=12, fontweight='bold')
plt.title('xaby,cxy->cab with dimensions 16,16,96,x,4', fontsize=14, fontweight='bold')

# Set x-axis ticks in steps of 128
plt.xticks(np.arange(0, 513, 128), fontweight='bold')
plt.xlim(0, 512)

# Set y-axis to start at 0 and include 7
plt.ylim(bottom=0)
current_yticks = list(plt.yticks()[0])
if 7 not in current_yticks:
    current_yticks.append(7)
    current_yticks.sort()
    plt.yticks(current_yticks, fontweight='bold')

plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)

plt.savefig('xaby_performance.png', dpi=600, bbox_inches='tight')
plt.show()