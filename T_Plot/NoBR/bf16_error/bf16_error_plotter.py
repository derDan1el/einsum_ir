import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# CSV-Datei einlesen
df = pd.read_csv('bf16_error_analysis_random_values.csv')

# Figure und Achsen erstellen
fig, ax1 = plt.subplots(figsize=(14, 8))

# Erste Y-Achse (links) für relative Fehler
color1 = '#3498db'  # Blau
ax1.set_xlabel('k-dimension size', fontsize=17, color='black')
ax1.set_ylabel('max. relative error', color=color1, fontsize=15)
line1 = ax1.plot(df['k_dimension'], df['max_relative_error'], 
                 color=color1, linewidth=2.5, marker='s', markersize=4,
                 label='relative error', alpha=0.8)
ax1.tick_params(axis='y', labelcolor='black', labelsize=17)
ax1.tick_params(axis='x', labelcolor='black', labelsize=17)
ax1.grid(True, alpha=0.3)

# Zweite Y-Achse (rechts) für absolute Fehler
ax2 = ax1.twinx()
color2 = '#e74c3c'  # Rot
ax2.set_ylabel('max. absolute error', color=color2, fontweight='bold', fontsize=15)
line2 = ax2.plot(df['k_dimension'], df['max_absolute_error'], 
                 color=color2, linewidth=2.5, marker='o', markersize=4, 
                 label='max. absolute error', alpha=0.8)
ax2.tick_params(axis='y', labelcolor='black', labelsize=15)

# Legende kombinieren
lines1, labels1 = ax1.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
ax1.legend(lines1 + lines2, labels1 + labels2, loc='upper left', fontsize=11)

# X-Achse formatieren
ax1.set_xlim(df['k_dimension'].min(), df['k_dimension'].max())

# Alle 2er-Potenzen im Datenbereich finden (für graue Rasterlinien)
unique_k_values = sorted(df['k_dimension'].unique())
min_k = min(unique_k_values)
max_k = max(unique_k_values)

all_powers_of_2 = []
power = 1
while power <= max_k:
    if power >= min_k:
        all_powers_of_2.append(power)
    power *= 2

# Graue vertikale Rasterlinien für ALLE 2er-Potenzen
for power in all_powers_of_2:
    ax1.axvline(x=power, color='black', linestyle='--', alpha=0.5, linewidth=1)

# Y-Achsen-Bereiche optimieren
ax1.set_ylim(0, df['max_relative_error'].max() * 1.1)
ax2.set_ylim(0, df['max_absolute_error'].max() * 1.1)

# Layout optimieren
plt.tight_layout()

# Speichern
plt.savefig('bf16_error_dual_axis_plot.pdf', dpi=1000, bbox_inches='tight')
