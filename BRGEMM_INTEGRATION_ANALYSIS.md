# BRGEMM Integration Analysis: Detaillierte Zeile-für-Zeile Auswertung

## Beispiel-Kontraktion: `"km,nk->nm"` mit Dimensionen `[258,128,128]`

### Eingangsparameter für unser Beispiel:
```
Einsum: "km,nk->nm"
Dimensionen: K=258, M=128, N=128
Tensor A: [258, 128] (K×M)
Tensor B: [128, 258] (N×K) 
Tensor C: [128, 128] (N×M)
```

### Loop-Struktur vor contract_iter-Aufruf:
```
m_num_loops = 3
m_loop_ids = [0, 1, 2]  # entspricht [K, M, N]
m_loop_dim_type = [K, M, N]
m_loop_sizes = [258, 128, 128]
m_id_first_parallel = 1  # Erste nicht-K Dimension
```

---

## contract_iter Funktion - Zeile für Zeile Analyse

### Funktions-Aufruf (erste Iteration):
```cpp
contract_iter(
    i_id_task = 0,           // Task-ID (für Threading)
    i_id_loop = 1,           // Loop-Index (startet bei m_id_first_parallel)
    i_ptr_left = ptr_A,      // Zeiger auf Tensor A
    i_ptr_right = ptr_B,     // Zeiger auf Tensor B  
    i_ptr_out_aux = nullptr, // Auxiliary Output (nicht verwendet)
    i_ptr_out = ptr_C,       // Zeiger auf Output Tensor C
    i_first_access = true,   // Erster Zugriff Flag
    i_last_access = true     // Letzter Zugriff Flag
)
```

### Zeile-für-Zeile Auswertung:

```cpp
void einsum_ir::backend::ContractionLoops::contract_iter( ... ) {
```
**Kommentar**: Funktionsdefinition für rekursive Schleifenausführung

```cpp
  // derive first element and number of iterations
  int64_t l_first = m_iter_spaces.firsts(i_id_task)[ i_id_loop - m_id_first_parallel ];
```
**Auswertung**:
- `i_id_task = 0` (Thread-Task-ID)
- `i_id_loop = 1` (aktueller Loop-Index - M-Dimension)
- `m_id_first_parallel = 1` (erste parallelisierbare Dimension)
- `i_id_loop - m_id_first_parallel = 1 - 1 = 0`
- `m_iter_spaces.firsts(0)[0] = 0` (Startindex für Task 0, Loop 0)
- **Ergebnis**: `l_first = 0` (beginne bei Element 0)

```cpp
  int64_t l_size  = m_iter_spaces.sizes(i_id_task)[  i_id_loop - m_id_first_parallel ];
```
**Auswertung**:
- `m_iter_spaces.sizes(0)[0] = 128` (Anzahl Iterationen für M-Dimension)
- **Ergebnis**: `l_size = 128` (führe 128 M-Iterationen aus)

```cpp
  bool l_first_access = i_first_access;
  bool l_last_access  = i_last_access;
```
**Auswertung**:
- `l_first_access = true` (kopiere Input-Parameter)
- `l_last_access = true` (kopiere Input-Parameter)

```cpp
  // issue loop iterations
  for( int64_t l_it = l_first; l_it < l_first+l_size; l_it++ ) {
```
**Auswertung**:
- `l_it` läuft von `0` bis `127` (128 Iterationen für M-Dimension)
- **Bedeutung**: For-Schleife über alle M-Elemente

### Erste Loop-Iteration (l_it = 0):

```cpp
    char * l_ptr_left    = (char *) i_ptr_left;
    char * l_ptr_right   = (char *) i_ptr_right;
    char * l_ptr_out_aux = (char *) i_ptr_out_aux;
    char * l_ptr_out     = (char *) i_ptr_out;
```
**Auswertung**:
- `l_ptr_left = ptr_A` (Cast zu char* für Byte-Arithmetik)
- `l_ptr_right = ptr_B` (Cast zu char* für Byte-Arithmetik)
- `l_ptr_out_aux = nullptr` (Cast von nullptr)
- `l_ptr_out = ptr_C` (Cast zu char* für Byte-Arithmetik)

```cpp
    l_ptr_left    += l_it * m_loop_strides_left[    i_id_loop ];
```
**Auswertung**:
- `l_it = 0` (erste Iteration)
- `i_id_loop = 1` (M-Dimension-Index)
- `m_loop_strides_left[1] = stride_M_in_A` (Byte-Stride für M-Dimension in Tensor A)
- Angenommen `stride_M_in_A = 128 * 2 = 256` Bytes (128 K-Elemente × 2 Bytes pro BF16)
- `l_ptr_left += 0 * 256 = l_ptr_left` (keine Änderung in erster Iteration)
- **Ergebnis**: `l_ptr_left` zeigt auf A[0, 0]

```cpp
    l_ptr_right   += l_it * m_loop_strides_right[   i_id_loop ];
```
**Auswertung**:
- `m_loop_strides_right[1] = 0` (M-Dimension existiert nicht in Tensor B)
- `l_ptr_right += 0 * 0 = l_ptr_right` (keine Änderung)
- **Ergebnis**: `l_ptr_right` zeigt weiterhin auf B[0, 0]

```cpp
    l_ptr_out_aux += l_it * m_loop_strides_out_aux[ i_id_loop ] * (l_ptr_out_aux != nullptr);
```
**Auswertung**:
- `l_ptr_out_aux = nullptr`, also `(l_ptr_out_aux != nullptr) = false = 0`
- `l_ptr_out_aux += l_it * stride * 0 = l_ptr_out_aux` (keine Änderung)
- **Ergebnis**: `l_ptr_out_aux` bleibt `nullptr`

```cpp
    l_ptr_out     += l_it * m_loop_strides_out[     i_id_loop ];
```
**Auswertung**:
- `m_loop_strides_out[1] = stride_M_in_C` (Byte-Stride für M-Dimension in Tensor C)
- Angenommen `stride_M_in_C = 1 * 2 = 2` Bytes (1 Element × 2 Bytes pro BF16)
- `l_ptr_out += 0 * 2 = l_ptr_out` (keine Änderung in erster Iteration)
- **Ergebnis**: `l_ptr_out` zeigt auf C[0, 0]

```cpp
    if(m_loop_dim_type[i_id_loop] == einsum_ir::K){
      l_first_access = i_first_access && l_it == 0 ;
      l_last_access  = i_last_access  && l_it == m_loop_sizes[i_id_loop]-1 ;
    }
```
**Auswertung**:
- `m_loop_dim_type[1] = M` (nicht K)
- Bedingung ist `false`, also wird der Block nicht ausgeführt
- `l_first_access` und `l_last_access` bleiben unverändert
- **Ergebnis**: `l_first_access = true`, `l_last_access = true`

```cpp
    if( m_id_loop_first_last_touch == i_id_loop && l_first_access ) {
      kernel_first_touch( l_ptr_out_aux, l_ptr_out );
    }
```
**Auswertung**:
- `m_id_loop_first_last_touch` = Index der Schleife für First/Last Touch (meist 0)
- `i_id_loop = 1`, also Bedingung wahrscheinlich `false`
- **Ergebnis**: Kein First-Touch ausgeführt

```cpp
    if( m_id_loop_packing_left == i_id_loop ){
      l_ptr_left = m_packing->kernel_pack_left( l_ptr_left );
    }
    if( m_id_loop_packing_right == i_id_loop ){
      l_ptr_right = m_packing->kernel_pack_right( l_ptr_right );
    }
```
**Auswertung**:
- Packing meist nicht aktiv für Standard-GEMM
- **Ergebnis**: Keine Packing-Operation

```cpp
    if( i_id_loop + 1 < m_num_loops ) {
```
**Auswertung**:
- `i_id_loop + 1 = 1 + 1 = 2`
- `m_num_loops = 3`
- `2 < 3` ist `true`
- **Ergebnis**: Rekursiver Aufruf wird ausgeführt

```cpp
      contract_iter( i_id_task,
                     i_id_loop+1,
                     l_ptr_left,
                     l_ptr_right,
                     l_ptr_out_aux,
                     l_ptr_out,
                     l_first_access,
                     l_last_access );
```
**Auswertung - Rekursiver Aufruf**:
- `i_id_task = 0` (unverändert)
- `i_id_loop+1 = 2` (nächste Schleife - N-Dimension)
- `l_ptr_left` zeigt auf A[0, 0]
- `l_ptr_right` zeigt auf B[0, 0]
- `l_ptr_out` zeigt auf C[0, 0]
- **Ergebnis**: Rufe contract_iter für N-Dimension auf

---

## Rekursiver Aufruf für N-Dimension (i_id_loop = 2):

```cpp
void contract_iter(i_id_task=0, i_id_loop=2, ...) {
  int64_t l_first = m_iter_spaces.firsts(0)[ 2 - 1 ] = m_iter_spaces.firsts(0)[1] = 0;
  int64_t l_size  = m_iter_spaces.sizes(0)[ 2 - 1 ]  = m_iter_spaces.sizes(0)[1] = 128;
```
**Auswertung**:
- **Ergebnis**: `l_first = 0`, `l_size = 128` (N-Dimension: 128 Iterationen)

```cpp
  for( int64_t l_it = 0; l_it < 128; l_it++ ) {
    // Pointer Updates für N-Dimension (l_it = 0):
    l_ptr_left    += 0 * m_loop_strides_left[2];     // Stride für N in A = 0
    l_ptr_right   += 0 * m_loop_strides_right[2];    // Stride für N in B = 258*2 = 516 Bytes
    l_ptr_out     += 0 * m_loop_strides_out[2];      // Stride für N in C = 128*2 = 256 Bytes
```
**Auswertung**:
- `l_ptr_left` unverändert (N nicht in A)
- `l_ptr_right` unverändert in erster Iteration
- `l_ptr_out` unverändert in erster Iteration

```cpp
    if( 2 + 1 < 3 ) {  // false
      // Kein weiterer rekursiver Aufruf
    }
    else {
      // HIER: Der innerste Kernel wird ausgeführt!
      kernel_main( l_ptr_left, l_ptr_right, l_ptr_out );
    }
```
**Auswertung**:
- `3 < 3` ist `false`
- **Ergebnis**: `kernel_main()` wird ausgeführt mit:
  - `l_ptr_left` → A[0, 0] (Start der K×M Submatrix)
  - `l_ptr_right` → B[0, 0] (Start der N×K Submatrix)  
  - `l_ptr_out` → C[0, 0] (Output-Element)

---

## Kernel-Main Ausführung:

```cpp
kernel_main( A[0,0], B[0,0], C[0,0] )
```
**Was passiert**:
- LIBXSMM GEMM berechnet: `C[0,0] += A[0,:] * B[0,:]` 
- Das ist ein 1×1×K GEMM (Dot-Product von K=258 Elementen)
- In BF16-Präzision mit FP32-Akkumulation

---

## Vollständige Schleifenausführung (konzeptionell):

```
Schleifenstruktur (vereinfacht):
for m = 0 to 127:                    // M-Dimension
    for n = 0 to 127:                // N-Dimension  
        kernel_main(A[m,:], B[n,:], C[n,m])  // K-Reduktion

Tatsächliche GEMM-Aufrufe:
- kernel_main(A[0,:], B[0,:], C[0,0])   # 258 FLOPS
- kernel_main(A[0,:], B[1,:], C[1,0])   # 258 FLOPS
- ...
- kernel_main(A[127,:], B[127,:], C[127,127])  # 258 FLOPS

Gesamt: 128 × 128 = 16,384 GEMM-Aufrufe!
```

---

## BRGEMM-Integration Analyse:

### Aktueller Status:
- **Problem**: 16,384 separate GEMM-Aufrufe für K-Reduktion
- **Ineffizient**: Jeder Aufruf macht nur 258 FLOPS (sehr klein)

### BRGEMM-Lösung:
```cpp
// Statt 16,384 kleine GEMMs:
for m = 0 to 127:
    for n = 0 to 127:
        for k = 0 to 257:  // K-Schleife
            C[n,m] += A[m,k] * B[n,k]

// Könnte werden zu:
for m = 0 to 127:
    for n = 0 to 127:
        // Ein BRGEMM-Aufruf für alle K:
        brgemm_kernel(A[m,:], B[n,:], C[n,m], batch_count=1, k_size=258)
```

### Konkrete Änderungen benötigt:

1. **Loop-Erkennung**: K-Schleifen identifizieren und zusammenfassen
2. **Stride-Berechnung**: BRGEMM-Strides für A und B berechnen  
3. **Batch-Count**: Bestimmen, wie viele K-Blöcke reduziert werden
4. **Kernel-Parameter**: `tertiary` Parameter richtig setzen

---

## Nächste Schritte für BRGEMM-Integration:

1. **K-Schleifen finden**: In `compile()` alle aufeinanderfolgenden K-Dimensionen identifizieren
2. **BRGEMM-Entscheidung**: Wenn mehrere K-Schleifen → BRGEMM aktivieren
3. **Stride-Berechnung**: Korrekte A/B-Strides für BRGEMM berechnen
4. **Kernel-Dispatch**: `LIBXSMM_GEMM_BATCH_REDUCE_STRIDE` verwenden
5. **Parameter-Übergabe**: `tertiary` mit Batch-Count setzen

Die detaillierte Analyse zeigt: Ihr System ist perfekt für BRGEMM geeignet!

---

## Konkrete Pointer-Arithmetik Beispiel

### Memory Layout für unser Beispiel:
```
Tensor A [258×128]:  K=258, M=128, Column-Major Layout
- A[k,m] = base_ptr_A + (m * 258 + k) * sizeof(BF16)
- A[k,m] = base_ptr_A + (m * 258 + k) * 2

Tensor B [128×258]:  N=128, K=258, Column-Major Layout  
- B[n,k] = base_ptr_B + (k * 128 + n) * sizeof(BF16)
- B[n,k] = base_ptr_B + (k * 128 + n) * 2

Tensor C [128×128]:  N=128, M=128, Column-Major Layout
- C[n,m] = base_ptr_C + (m * 128 + n) * sizeof(BF16)
- C[n,m] = base_ptr_C + (m * 128 + n) * 2
```

### Stride-Berechnung:
```cpp
// M-Dimension Strides (in Bytes):
m_loop_strides_left[1]  = 258 * 2 = 516   // Für A: springe 258 K-Elemente
m_loop_strides_right[1] = 0                // Für B: M existiert nicht
m_loop_strides_out[1]   = 128 * 2 = 256   // Für C: springe 128 N-Elemente

// N-Dimension Strides (in Bytes):
m_loop_strides_left[2]  = 0                // Für A: N existiert nicht  
m_loop_strides_right[2] = 1 * 2 = 2        // Für B: springe 1 N-Element
m_loop_strides_out[2]   = 1 * 2 = 2        // Für C: springe 1 N-Element
```

### Konkrete Iteration (M=1, N=2):

**M-Schleife (i_id_loop=1, l_it=1)**:
```cpp
l_ptr_left  = base_ptr_A + 1 * 516 = base_ptr_A + 516  // Zeigt auf A[0,1] 
l_ptr_right = base_ptr_B + 1 * 0   = base_ptr_B        // Zeigt auf B[0,0]
l_ptr_out   = base_ptr_C + 1 * 256 = base_ptr_C + 256  // Zeigt auf C[0,1]
```

**N-Schleife (i_id_loop=2, l_it=2)**:
```cpp
l_ptr_left  = (base_ptr_A + 516) + 2 * 0 = base_ptr_A + 516  // Zeigt auf A[0,1]
l_ptr_right = base_ptr_B + 2 * 2         = base_ptr_B + 4    // Zeigt auf B[2,0]  
l_ptr_out   = (base_ptr_C + 256) + 2 * 2 = base_ptr_C + 260  // Zeigt auf C[2,1]
```

**Kernel-Main Aufruf**:
```cpp
kernel_main(
    A_ptr = base_ptr_A + 516,    // A[0,1] - Spalte 1 von A (alle K-Werte)
    B_ptr = base_ptr_B + 4,      // B[2,0] - Zeile 2 von B (alle K-Werte)  
    C_ptr = base_ptr_C + 260     // C[2,1] - Output-Element
)
```

**Was der GEMM-Kernel macht**:
```cpp
// Dot-Product über K-Dimension:
C[2,1] += A[0,1] * B[2,0] + A[1,1] * B[2,1] + ... + A[257,1] * B[2,257]
```

### BRGEMM-Optimierung Potential:

**Aktuell**: 16,384 separate kernel_main() Aufrufe
```cpp
for m in range(128):
    for n in range(128):
        kernel_main(A[:,m], B[n,:], C[n,m])  // 258 FLOPS pro Aufruf
```

**Mit BRGEMM**: Deutlich weniger Aufrufe
```cpp
for m in range(128):
    for n in range(128):
        brgemm_kernel(
            A_ptr = &A[0,m],           // Start der m-ten Spalte
            B_ptr = &B[n,0],           // Start der n-ten Zeile
            C_ptr = &C[n,m],           // Output-Element
            batch_count = 1,           // Nur eine K-Reduktion
            stride_a = 2,              // A[k+1,m] = A[k,m] + 2 Bytes
            stride_b = 128*2           // B[n,k+1] = B[n,k] + 256 Bytes
        )
```

---

## Debug-Information für BRGEMM-Integration:

### 1. Loop-Information ausgeben:
```cpp
// In ContractionLoops::compile() hinzufügen:
std::cout << "=== LOOP ANALYSIS ===" << std::endl;
for(int64_t i = 0; i < m_num_loops; i++) {
    std::cout << "Loop " << i << ": "
              << "Type=" << (int)m_loop_dim_type[i] 
              << " Size=" << m_loop_sizes[i]
              << " Stride_A=" << m_loop_strides_left[i]
              << " Stride_B=" << m_loop_strides_right[i] << std::endl;
}
```

### 2. BRGEMM-Kandidaten identifizieren:
```cpp
// K-Schleifen finden:
std::vector<int64_t> k_loops;
for(int64_t i = 0; i < m_num_loops; i++) {
    if(m_loop_dim_type[i] == einsum_ir::K) {
        k_loops.push_back(i);
    }
}

if(k_loops.size() >= 1) {
    std::cout << "BRGEMM-Kandidat gefunden: " << k_loops.size() 
              << " K-Dimensionen" << std::endl;
}
```

### 3. Stride-Validation:
```cpp
// In kernel_main() vor LIBXSMM-Aufruf:
std::cout << "GEMM-Call: A=" << (void*)i_left 
          << " B=" << (void*)i_right 
          << " C=" << (void*)io_out << std::endl;
```

Diese detaillierte Analyse zeigt genau, wo und wie BRGEMM in Ihr System integriert werden kann!

## 6. Detaillierte Klammer-Analyse: `m_iter_spaces.sizes(i_id_task)[i_id_loop - m_id_first_parallel]`

### 6.1 Aufbau der Datenstrukturen

Basierend auf der Code-Analyse von `IterationSpaces.h/.cpp`:

```cpp
class IterationSpaces {
private:
    typedef struct {
        std::vector< int64_t > firsts;  // Startindizes für jeden Loop
        std::vector< int64_t > sizes;   // Größen für jeden Loop
    } IterSpace;
    
    IterSpace m_global_space;                      // Globaler Iterationsraum
    std::vector< IterSpace > m_thread_local_spaces; // Task-spezifische Räume
    
public:
    // Gibt einen Pointer auf das sizes-Array für eine bestimmte Task zurück
    int64_t const * sizes( int64_t i_task_id ) {
        return m_thread_local_spaces[i_task_id].sizes.data();
    }
};
```

### 6.2 Bedeutung der Klammern - Schritt für Schritt

Der Ausdruck `m_iter_spaces.sizes(i_id_task)[i_id_loop - m_id_first_parallel]` zerfällt in mehrere Ebenen:

#### **Ebene 1: `m_iter_spaces` (Objekt)**
- Typ: `IterationSpaces`
- Enthält die gesamte Schleifenorganisation für alle Tasks

#### **Ebene 2: `.sizes(i_id_task)` (Methodenaufruf mit runden Klammern)**
- **Runde Klammern `()`**: Methodenaufruf-Syntax in C++
- Parameter: `i_id_task` (aktuelle Task-ID, z.B. 0, 1, 2, ...)
- Rückgabe: `int64_t const *` - Pointer auf Array der Schleifengrößen für diese Task
- Beispiel: Wenn `i_id_task = 1`, dann wird `m_thread_local_spaces[1].sizes.data()` zurückgegeben

#### **Ebene 3: `[i_id_loop - m_id_first_parallel]` (Array-Zugriff mit eckigen Klammern)**
- **Eckige Klammern `[]`**: Array-Zugriff-Syntax in C++
- Index: `i_id_loop - m_id_first_parallel`
- Greift auf ein spezifisches Element im sizes-Array zu

### 6.3 Warum die Index-Berechnung `i_id_loop - m_id_first_parallel`?

```cpp
// Aus ContractionLoops.h:
int64_t m_id_first_parallel = -1;  // ID der ersten parallelisierbaren Schleife
```

**Grundidee**: Das `sizes`-Array enthält nur die parallelisierbaren Schleifen, nicht alle Schleifen!

**Beispiel-Szenario**:
- Gesamte Schleifen: `[M, N, K, Batch]` (IDs: 0, 1, 2, 3)
- Parallele Schleifen: `[M, N]` (nur die ersten beiden)
- `m_id_first_parallel = 0` (erste parallele Schleife hat ID 0)
- `sizes`-Array enthält nur: `[size_M, size_N]` (2 Elemente)

**Index-Mapping**:
- `i_id_loop = 0` (M-Schleife) → Array-Index: `0 - 0 = 0` → `sizes[0]` = M-Größe
- `i_id_loop = 1` (N-Schleife) → Array-Index: `1 - 0 = 1` → `sizes[1]` = N-Größe
- `i_id_loop = 2` (K-Schleife) → Nicht parallel, wird nicht aufgerufen
- `i_id_loop = 3` (Batch) → Nicht parallel, wird nicht aufgerufen

### 6.4 Konkretes Beispiel mit Zahlen

**Angenommen**:
- Einsum: `"km,nk->nm"` mit Dimensionen `[258, 128, 128]`
- 4 Tasks, Task-ID = 1
- Parallele Schleifen: M (ID=0) und N (ID=1)
- K-Schleife (ID=2) ist seriell
- `m_id_first_parallel = 0`

**Task-1 könnte haben**:
```cpp
m_thread_local_spaces[1].sizes = [65, 128]  // M=65, N=128 für diese Task
```

**Aufruf**: `m_iter_spaces.sizes(1)[0 - 0]`
1. `m_iter_spaces.sizes(1)` → Pointer auf `[65, 128]`
2. `[0 - 0]` → Index 0
3. **Ergebnis**: `65` (M-Größe für Task 1)

### 6.5 Warum nicht einfach `sizes[i_id_loop]`?

Das `sizes`-Array ist **komprimiert** und enthält nur parallele Schleifen:
- **Vorteil**: Speicher-effizient, nur relevante Daten
- **Nachteil**: Index-Mapping notwendig

Alternative Designs wären:
```cpp
// Design A (aktuell): Komprimiert
sizes = [M_size, N_size]           // Nur parallele Schleifen
index = i_id_loop - m_id_first_parallel

// Design B (vollständig): Weniger effizient
sizes = [M_size, N_size, K_size, Batch_size]  // Alle Schleifen
index = i_id_loop  // Direkter Zugriff
```

### 6.6 Mögliche Fehlerquellen

**Häufige Bugs bei BRGEMM-Integration**:
1. **Falscher Index**: Vergessen der Offset-Berechnung
2. **Bounds-Check fehlt**: `i_id_loop >= m_id_first_parallel` prüfen
3. **Task-ID außerhalb**: `i_id_task >= num_tasks()` abfangen

**Debug-Tip**:
```cpp
// Sichere Variante mit Debugging
assert(i_id_loop >= m_id_first_parallel);
assert(i_id_task < m_iter_spaces.num_tasks());
int64_t array_index = i_id_loop - m_id_first_parallel;
int64_t* sizes_array = m_iter_spaces.sizes(i_id_task);
int64_t loop_size = sizes_array[array_index];

printf("Task %ld, Loop %ld -> Array[%ld] = %ld\n", 
       i_id_task, i_id_loop, array_index, loop_size);
```

### 6.7 Zusammenfassung der Klammer-Bedeutungen

| Klammer-Typ | Syntax | Bedeutung | Beispiel |
|-------------|--------|-----------|----------|
| **Runde Klammern** `()` | Methodenaufruf | C++ Funktions-/Methodenaufruf | `sizes(i_id_task)` |
| **Eckige Klammern** `[]` | Array-Zugriff | Pointer-Dereferenzierung mit Index | `[i_id_loop - m_id_first_parallel]` |

**Vollständige Bedeutung**:
> "Hole das sizes-Array für Task `i_id_task` und greife auf das Element mit Index `(i_id_loop - m_id_first_parallel)` zu, um die Schleifengröße der aktuellen parallelen Schleife zu erhalten."
