import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# CSV-Datei einlesen
df = pd.read_csv('bf16_error_analysis.csv')

# Figure und Achsen erstellen
fig, ax1 = plt.subplots(figsize=(14, 8))

# Erste Y-Achse (links) für absolute Fehler
color1 = '#e74c3c'  # Rot
ax1.set_xlabel('k-dimension size ', fontweight='bold', fontsize=12)
ax1.set_ylabel('max. absolute error', color=color1, fontweight='bold', fontsize=12)
line1 = ax1.plot(df['k_dimension'], df['max_absolute_error'], 
                 color=color1, linewidth=2.5, marker='o', markersize=4, 
                 label='max. absolute error', alpha=0.8)
ax1.tick_params(axis='y', labelcolor=color1)
ax1.grid(True, alpha=0.3)

# Zweite Y-Achse (rechts) für relative Fehler
ax2 = ax1.twinx()
color2 = '#3498db'  # Blau
ax2.set_ylabel('max. relative error', color=color2, fontweight='bold', fontsize=12)
line2 = ax2.plot(df['k_dimension'], df['max_relative_error'], 
                 color=color2, linewidth=2.5, marker='s', markersize=4,
                 label='relative rrror', alpha=0.8)
ax2.tick_params(axis='y', labelcolor=color2)

# Titel
plt.title('Numerical Divergence of PyTorch at::einsum and LIBXSMM Kernel Output of km,nk->nm', 
          fontweight='bold', fontsize=14, pad=20)

# Legende kombinieren
lines1, labels1 = ax1.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
ax1.legend(lines1 + lines2, labels1 + labels2, loc='upper left', fontsize=11)

# X-Achse formatieren
ax1.set_xlim(df['k_dimension'].min(), df['k_dimension'].max())

# Y-Achsen-Bereiche optimieren
ax1.set_ylim(0, df['max_absolute_error'].max() * 1.1)
ax2.set_ylim(0, df['max_relative_error'].max() * 1.1)

# Layout optimieren
plt.tight_layout()

# Speichern
plt.savefig('bf16_error_dual_axis_plot.png', dpi=400, bbox_inches='tight')

# Zeigen
plt.show()

# Zusätzliche Statistiken ausgeben
print("=== BF16 Error Analysis Statistics ===")
print(f"K-Dimension Range: {df['k_dimension'].min()} - {df['k_dimension'].max()}")
print(f"Max Absolute Error: {df['max_absolute_error'].max()}")
print(f"Max Relative Error: {df['max_relative_error'].max():.6f}")
print(f"K at Max Absolute Error: {df.loc[df['max_absolute_error'].idxmax(), 'k_dimension']}")
print(f"K at Max Relative Error: {df.loc[df['max_relative_error'].idxmax(), 'k_dimension']}")

# Trend-Analyse
correlation_abs = np.corrcoef(df['k_dimension'], df['max_absolute_error'])[0,1]
correlation_rel = np.corrcoef(df['k_dimension'], df['max_relative_error'])[0,1]
print(f"\nCorrelation K vs Absolute Error: {correlation_abs:.4f}")
print(f"Correlation K vs Relative Error: {correlation_rel:.4f}")

# Bereiche wo Fehler = 0
zero_error_k = df[df['max_absolute_error'] == 0]['k_dimension'].tolist()
if zero_error_k:
    print(f"K-Dimensions with zero error: {zero_error_k}")
else:
    print("No K-dimensions with zero error found")