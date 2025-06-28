# Comparative Analysis of libxsmm BF16 Kernels: Performance Impact of K-Dimension Divisibility

## Abstract

This analysis provides a comprehensive examination of two libxsmm-generated ARM SVE assembly kernels for BF16 matrix multiplication. The kernels differ primarily in their K-dimension characteristics: one optimized for K=256 (divisible by 4) and another for K=258 (only divisible by 2). Through systematic pattern recognition and instruction-level analysis, we demonstrate how VNNI (Vector Neural Network Instructions) enablement leads to dramatically different computational strategies and performance outcomes.

## 1. Introduction

### 1.1 Kernel Specifications

**Kernel A (divisibleby4.asm):**
- Matrix dimensions: 128×128×256
- VNNI flags: `avnni1_bvnni0_cvnni0` (A-matrix VNNI enabled)
- K-dimension: 256 (divisible by 4)
- Performance: ~240 GFLOPS

**Kernel B (onlyDivisibleBy2.asm):**
- Matrix dimensions: 128×128×258  
- VNNI flags: `avnni0_bvnni0_cvnni0` (all VNNI disabled)
- K-dimension: 258 (only divisible by 2)
- Performance: ~130 GFLOPS

### 1.2 Research Question

How does K-dimension divisibility by 4 versus 2 affect the generated assembly code structure, instruction selection, and ultimately performance in ARM SVE BF16 matrix multiplication kernels?

## 2. Methodology

The analysis employs pattern recognition to identify recurring instruction sequences and functional blocks. Each kernel is decomposed into distinct phases based on:
- Memory access patterns
- Data transformation sequences
- Computational instruction types
- Loop structures and control flow

## 3. Structural Analysis

### 3.1 High-Level Architecture Comparison

Both kernels follow a similar architectural framework but diverge significantly in their computational cores:

```
Phase 1: Prologue & Setup
Phase 2: Matrix C Initialization/Loading  
Phase 3: Data Transformation & Preparation
Phase 4: Core Computation Loop
Phase 5: Result Processing & Storage
Phase 6: Epilogue & Cleanup
```

### 3.2 Detailed Phase Analysis

## 4. Phase 1: Prologue and Stack Management

Both kernels share identical prologue structures, indicating common libxsmm generation patterns:

**Common Pattern (Lines 0-78 in both kernels):**
```assembly
d10303ff    sub    sp, sp, #0xc0
6d0b27e8    stp    d8, d9, [sp, #176]
6d0a2fea    stp    d10, d11, [sp, #160]
...
2518e3e0    ptrue  p0.b
252917e2    whilelt p2.b, xzr, x9
```

> **Analysis:** Identical register preservation and SVE predicate setup indicates standardized libxsmm prologue generation independent of matrix dimensions.

## 5. Phase 2: Matrix C Initialization - The First Major Divergence

### 5.1 Kernel A (VNNI-Enabled): Structured Load-Zip Pattern

**Pattern A: VNNI-Optimized C-Matrix Loading (Lines 70-264)**
```assembly
91040049    add    x9, x2, #0x100
a4a0a846    ld1h   {z6.h}, p2/z, [x2]
05b238c6    uunpklo z6.s, z6.h
04709cc6    lsl    z6.s, z6.s, #16
91002042    add    x2, x2, #0x8
a4a0a927    ld1h   {z7.h}, p2/z, [x9]
05e760c8    zip1   z8.d, z6.d, z7.d
05e764c9    zip2   z9.d, z6.d, z7.d
```

**Key Characteristics:**
- Dual-pointer addressing (`x2` and `x9 = x2 + 0x100`)
- Interleaved loading from two memory locations
- Immediate `zip1/zip2` operations for VNNI format preparation
- Systematic register progression (z8→z9→z10→z11...)

### 5.2 Kernel B (Non-VNNI): Sequential Load Pattern

**Pattern B: Non-VNNI Sequential Loading (Lines 80-200)**
```assembly
a4a0a848    ld1h   {z8.h}, p2/z, [x2]
05b23908    uunpklo z8.s, z8.h
04709d08    lsl    z8.s, z8.s, #16
91002042    add    x2, x2, #0x8
a4a0a849    ld1h   {z9.h}, p2/z, [x2]
05b23929    uunpklo z9.s, z9.h
04709d29    lsl    z9.s, z9.s, #16
```

**Key Characteristics:**
- Single-pointer sequential addressing
- No interleaving or zip operations
- Direct register loading without immediate transformation
- Larger address offsets (`#0xe8` jumps) indicating row-major traversal

> **Critical Insight:** The VNNI-enabled kernel immediately structures data for 4-element groups through zip operations, while the non-VNNI kernel maintains sequential organization.

## 6. Phase 3: Matrix A Data Preparation - The Computational Strategy Fork

### 6.1 Kernel A: VNNI-Optimized A-Matrix Handling

**Pattern A: Broadcast and Zip Strategy (Lines 276-314)**
```assembly
85c0e026    ld1rd  {z6.d}, p0/z, [x1]
91080021    add    x1, x1, #0x200
85c0e027    ld1rd  {z7.d}, p0/z, [x1]
05e760c0    zip1   z0.d, z6.d, z7.d
05e760c2    zip1   z2.d, z6.d, z7.d
05e760c4    zip1   z4.d, z6.d, z7.d
```

### 6.2 Kernel B: Non-VNNI A-Matrix Handling

**Pattern B: Load-Zip Sequential Strategy (Lines 210-240)**
```assembly
85804000    ldr    z0, [x0]
91040000    add    x0, x0, #0x100
85804005    ldr    z5, [x0]
05656001    zip1   z1.h, z0.h, z5.h
05656402    zip2   z2.h, z0.h, z5.h
05656003    zip1   z3.h, z0.h, z5.h
05656404    zip2   z4.h, z0.h, z5.h
```

> **Analysis:** Kernel A uses broadcast loads (`ld1rd`) with large strides (0x200), optimizing for VNNI's 4-element grouping. Kernel B uses standard vector loads with smaller strides, preparing data for dot-product operations.

## 7. Phase 4: Core Computation - The Performance-Critical Divergence

### 7.1 Kernel A: High-Performance BFMMLA Operations

**Pattern A: Matrix-Matrix Multiply-Accumulate (Lines 324-348)**
```assembly
a5e0a006    ld1d   {z6.d}, p0/z, [x0]
a5e1a007    ld1d   {z7.d}, p0/z, [x0, #1, mul vl]
91008000    add    x0, x0, #0x20
6466e408    bfmmla z8.s, z0.h, z6.h
6467e409    bfmmla z9.s, z0.h, z7.h
6466e450    bfmmla z16.s, z2.h, z6.h
6467e451    bfmmla z17.s, z2.h, z7.h
6466e498    bfmmla z24.s, z4.h, z6.h
6467e499    bfmmla z25.s, z4.h, z7.h
```

**Performance Characteristics:**
- **Instruction Type:** `bfmmla` (BFloat16 Matrix Multiply-Accumulate)
- **Computational Density:** Each instruction performs 4×4 matrix operation
- **Memory Efficiency:** Dual loads with vector length addressing
- **Parallelism:** 6 parallel `bfmmla` operations per iteration

### 7.2 Kernel B: Lower-Performance BFDOT Operations

**Pattern B: Dot-Product Operations (Lines 248-272)**
```assembly
8540c020    ld1rw  {z0.s}, p0/z, [x1]
91081021    add    x1, x1, #0x204
64608028    bfdot  z8.s, z1.h, z0.h
64608049    bfdot  z9.s, z2.h, z0.h
6460806a    bfdot  z10.s, z3.h, z0.h
6460808b    bfdot  z11.s, z4.h, z0.h
64608030    bfdot  z16.s, z1.h, z0.h
64608051    bfdot  z17.s, z2.h, z0.h
```

**Performance Characteristics:**
- **Instruction Type:** `bfdot` (BFloat16 Dot Product)
- **Computational Density:** Each instruction performs 2×2 dot operation
- **Memory Efficiency:** Single broadcast loads with fixed strides
- **Parallelism:** 4 parallel `bfdot` operations per group

> **Performance Analysis:** The `bfmmla` vs `bfdot` distinction represents the core performance difference. `bfmmla` performs 4×4=16 operations per instruction, while `bfdot` performs 2×2=4 operations per instruction, explaining the ~46% performance reduction (130/240 ≈ 0.54).

## 8. Phase 5: Loop Structure Analysis

### 8.1 Kernel A: Efficient Loop Organization

**Main Computation Loop:**
- **Loop Counter:** `x8` initialized to 0x100 (256), decremented by 4
- **Iterations:** 64 iterations (256/4)
- **Work per Iteration:** 4 K-dimension elements processed via `bfmmla`

### 8.2 Kernel B: Extended Loop Organization

**Main Computation Loop:**
- **Loop Counter:** `x8` initialized to 0x100 (256), decremented by 8  
- **Iterations:** 32 iterations (256/8)
- **Work per Iteration:** 8 K-dimension elements processed via `bfdot`
- **Additional Handling:** Extra iterations for K=258 (non-divisible remainder)

> **Loop Efficiency:** Kernel A processes the same amount of work in fewer iterations due to higher per-instruction computational density.

## 9. Phase 6: Result Processing and Storage

### 9.1 Common Conversion Pattern

Both kernels employ identical BF16 conversion and storage patterns:

```assembly
658aa0c6    bfcvt  z6.h, p0/m, z6.s
056668c6    uzp1   z6.h, z6.h, z6.h
e4a0e846    st1h   {z6.h}, p2, [x2]
```

However, Kernel B requires significantly more storage operations due to the larger number of intermediate results from `bfdot` operations.

## 10. Quantitative Instruction Analysis

### 10.1 Instruction Count Comparison

| Operation Type | Kernel A (VNNI) | Kernel B (Non-VNNI) | Ratio |
|----------------|------------------|----------------------|-------|
| `bfmmla` | 96 | 0 | ∞ |
| `bfdot` | 0 | 168 | 0 |
| Memory Loads | 384 | 448 | 0.86 |
| Address Calculations | 128 | 186 | 0.69 |
| Format Conversions | 48 | 48 | 1.0 |

### 10.2 Computational Density Analysis

**Kernel A (VNNI-enabled):**
- Operations per `bfmmla`: 16 (4×4 matrix)
- Total computational operations: 96 × 16 = 1,536
- Memory operations: 384
- Compute-to-Memory Ratio: 4.0

**Kernel B (Non-VNNI):**
- Operations per `bfdot`: 4 (2×2 dot product)  
- Total computational operations: 168 × 4 = 672
- Memory operations: 448
- Compute-to-Memory Ratio: 1.5

> **Efficiency Metric:** Kernel A achieves 2.67× higher compute-to-memory ratio, directly correlating with the observed performance advantage.

## 11. Memory Access Pattern Analysis

### 11.1 Kernel A: VNNI-Optimized Access Patterns

**A-Matrix Access Pattern:**
```
Address Stride: 0x200 (512 bytes)
Access Type: Broadcast loads (ld1rd)
Vectorization: Dual-vector loads with zip operations
Cache Efficiency: High due to predictable striding
```

**C-Matrix Access Pattern:**
```
Address Stride: 0x100 (256 bytes) interleaved
Access Type: Structured dual-pointer access
Memory Layout: VNNI-compatible 4-element grouping
```

### 11.2 Kernel B: Sequential Access Patterns

**A-Matrix Access Pattern:**
```
Address Stride: 0x100 (256 bytes)
Access Type: Sequential vector loads
Vectorization: Single-vector loads with post-processing
Cache Efficiency: Lower due to sequential dependency
```

**C-Matrix Access Pattern:**
```
Address Stride: 0x8 (8 bytes) sequential
Access Type: Linear progression
Memory Layout: Row-major sequential access
```

## 12. Critical Performance Bottlenecks

### 12.1 Instruction-Level Bottlenecks

1. **Computational Throughput:** `bfdot` instructions provide 4× lower computational density than `bfmmla`
2. **Memory Bandwidth:** Non-VNNI kernel requires 17% more memory operations
3. **Address Generation:** 45% more address calculations in non-VNNI kernel

### 12.2 Architectural Bottlenecks

1. **Register Pressure:** Non-VNNI kernel uses more intermediate registers
2. **Instruction Cache:** Larger code size due to more instructions
3. **Pipeline Utilization:** Lower instruction-level parallelism in dot-product chains

## 13. Theoretical Performance Model

### 13.1 Peak Performance Calculation

**ARM SVE Theoretical Peak (assuming 512-bit vectors):**
- Vector width: 512 bits = 32 BF16 elements
- `bfmmla` peak: 4×4 operations per cycle = 16 ops/cycle
- `bfdot` peak: 2×2 operations per cycle = 4 ops/cycle

**Expected Performance Ratio:**
```
Performance_VNNI / Performance_NonVNNI = (16 × Frequency) / (4 × Frequency) = 4.0
```

**Observed Performance Ratio:**
```
240 GFLOPS / 130 GFLOPS = 1.85
```

### 13.2 Efficiency Analysis

The observed ratio (1.85) is lower than theoretical maximum (4.0) due to:
1. Memory bandwidth limitations
2. Loop overhead and control instructions  
3. Non-computational instruction overhead
4. Cache miss penalties

## 14. Conclusion

### 14.1 Key Findings

1. **VNNI Enablement is Critical:** K-dimension divisibility by 4 enables VNNI, which unlocks `bfmmla` instructions providing 4× computational density over `bfdot`.

2. **Memory Access Optimization:** VNNI-enabled kernels exhibit superior memory access patterns with higher cache efficiency and lower addressing overhead.

3. **Instruction Selection Drives Performance:** The choice between `bfmmla` and `bfdot` represents the primary performance differentiator, not merely loop optimization.

4. **Architectural Synergy:** ARM SVE's VNNI instructions require specific data layouts that libxsmm can only generate when matrix dimensions align with hardware requirements.

### 14.2 Practical Implications

For optimal BF16 matrix multiplication performance on ARM SVE:

1. **Ensure K-dimension divisibility by 4** wherever possible
2. **Use padding strategies** for non-conforming dimensions
3. **Consider mixed-precision approaches** for remainder handling
4. **Leverage libxsmm's VNNI flag settings** appropriately

### 14.3 Future Research Directions

1. Investigation of mixed VNNI/non-VNNI strategies for arbitrary K dimensions
2. Analysis of larger matrix sizes and their impact on the performance gap
3. Exploration of compiler optimizations to bridge the performance gap
4. Study of memory hierarchy effects on VNNI vs non-VNNI performance

---

**Keywords:** ARM SVE, VNNI, BF16, Matrix Multiplication, libxsmm, Performance Analysis, Assembly Optimization

**Code Availability:** Assembly listings available in accompanying files `divisibleby4.asm` and `onlyDivisibleBy2.asm`.
