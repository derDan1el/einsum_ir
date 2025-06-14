import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load data for table creation
df = pd.read_csv('Benchmark_results.csv')
df['Unique_ID'] = df['Einsum'] + ' | ' + df['Dimensions']
expressions = df['Unique_ID'].unique()

# Create mapping table with separated columns
table_data = []
for i, expr in enumerate(expressions):
    einsum_part, dim_part = expr.split(' | ')
    table_data.append([f'E{i+1}', einsum_part, dim_part])

# Create figure for table - Größe passend zu den Spaltenbreiten
fig, ax = plt.subplots(figsize=(8, 6))  # Angepasst an kleinere Spalten
ax.axis('tight')
ax.axis('off')

# Berechne Spaltenbreiten in cm relativ zur Figure-Breite
# Figure ist 8 inches = ~20.3 cm
# Gewünschte Breiten: ID=1cm, Einsum=8cm, Dimensions=6cm = 15cm total
total_width_cm = 15
fig_width_cm = 8 * 2.54  # 8 inches in cm

# Spaltenbreiten relativ zur Figure
id_width = 1.0 / fig_width_cm      # ~1cm
einsum_width = 5.0 / fig_width_cm  # ~8cm  
dim_width = 5.0 / fig_width_cm     # ~6cm

# Create table mit absoluten Breiten
table = ax.table(cellText=table_data,
                colLabels=['ID', 'Einsum Expression', 'Dimensions'],
                cellLoc='left',
                loc='center',
                colWidths=[id_width, einsum_width, dim_width])

# Style the table
table.auto_set_font_size(False)
table.set_fontsize(8)
table.scale(1, 1.4)  # Kompakte Zeilen

# Style header
for i in range(3):
    table[(0, i)].set_facecolor('#4CAF50')
    table[(0, i)].set_text_props(weight='bold', color='white', fontsize=9)

# Alternate row colors
for i in range(1, len(table_data) + 1):
    for j in range(3):
        if i % 2 == 0:
            table[(i, j)].set_facecolor('#f0f0f0')
        else:
            table[(i, j)].set_facecolor('white')

# Style ID column (centered and bold)
for i in range(len(table_data) + 1):
    table[(i, 0)].set_text_props(ha='center')
    if i > 0:  # Skip header
        table[(i, 0)].set_text_props(fontsize=8, weight='bold', ha='center')

# Style expression and dimensions columns
for i in range(1, len(table_data) + 1):
    table[(i, 1)].set_text_props(fontsize=8, ha='left')
    table[(i, 2)].set_text_props(fontsize=7, ha='left')

# Save table
plt.savefig('einsum_expression_mapping_table.png', 
            dpi=300, bbox_inches='tight', facecolor='white')


print("Table saved as:")
print("- einsum_expression_mapping_table.png")
print("- einsum_expression_mapping_table.pdf")
print(f"Approximate column widths: ID=1cm, Einsum=8cm, Dimensions=6cm")

plt.show()