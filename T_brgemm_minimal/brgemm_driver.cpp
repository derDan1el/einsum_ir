#include "/home/daniel/libxsmm/include/libxsmm.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <random>
#include <iomanip>

void ref_gemm_strided_batch(float const *i_a,
                            float const *i_b,
                            float *io_c,
                            libxsmm_blasint i_m,
                            libxsmm_blasint i_n,
                            libxsmm_blasint i_k,
                            libxsmm_blasint i_lda,
                            libxsmm_blasint i_ldb,
                            libxsmm_blasint i_ldc,
                            libxsmm_blasint i_batch_size,
                            size_t stride_a,
                            size_t stride_b)
{
  for (libxsmm_blasint batch = 0; batch < i_batch_size; batch++)
  {
    const float *a_batch = i_a + (batch * stride_a / sizeof(libxsmm_bfloat16));
    const float *b_batch = i_b + (batch * stride_b / sizeof(libxsmm_bfloat16));

    for (libxsmm_blasint n = 0; n < i_n; ++n)
    {
      for (libxsmm_blasint m = 0; m < i_m; ++m)
      {
        for (libxsmm_blasint k = 0; k < i_k; ++k)
        {
          io_c[m + n * i_ldc] += a_batch[m + k * i_lda] * b_batch[k + n * i_ldb];
        }
      }
    }
  }
}

int main(int argc, char *argv[])
{
  bool debug_output = false;
  if (argc > 1 && std::string(argv[1]) == "1")
  {
    debug_output = true;
  }

  std::cout << "LIBXSMM Stride BRGEMM Test" << std::endl;
  std::cout << "==========================" << std::endl;

  // **Exakt dieselben Parameter wie gemm_kernel.c Eingabe**
  libxsmm_blasint l_m = 4;
  libxsmm_blasint l_n = 4;
  libxsmm_blasint l_k = 8;
  libxsmm_blasint l_lda = l_m;
  libxsmm_blasint l_ldb = l_k;
  libxsmm_blasint l_ldc = l_m;
  libxsmm_blasint l_batch_size = 3; // BR=3

  // **Stride Berechnung wie in gemm_kernel.c**
  size_t stride_a = l_lda * l_k * sizeof(libxsmm_bfloat16); // 64*64*2 = 8192 bytes
  size_t stride_b = l_ldb * l_n * sizeof(libxsmm_bfloat16); // 64*64*2 = 8192 bytes

  std::cout << "Matrix dimensions: M=" << l_m << ", N=" << l_n << ", K=" << l_k << std::endl;
  std::cout << "Batch size: " << l_batch_size << std::endl;
  std::cout << "Stride A: " << stride_a << " bytes" << std::endl;
  std::cout << "Stride B: " << stride_b << " bytes" << std::endl;

  // **Shape erstellen wie in gemm_kernel.c**
  libxsmm_gemm_shape shape = libxsmm_create_gemm_shape(
      l_m, l_n, l_k,
      l_lda, l_ldb, l_ldc,
      LIBXSMM_DATATYPE_BF16, // A
      LIBXSMM_DATATYPE_BF16, // B
      LIBXSMM_DATATYPE_BF16, // C
      LIBXSMM_DATATYPE_F32   // comp_type
  );

  libxsmm_bitfield flags = LIBXSMM_GEMM_FLAGS('N', 'N');
  libxsmm_bitfield prefetch_flags = 0; // nopf

  libxsmm_gemm_batch_reduce_config br_config;
  br_config.br_type = LIBXSMM_GEMM_BATCH_REDUCE_STRIDE;
  br_config.br_stride_a_hint = stride_a;
  br_config.br_stride_b_hint = stride_b;
  br_config.br_unroll_hint = 0; 

  std::cout << "\nLibXSMM Configuration:" << std::endl;
  std::cout << "br_type: LIBXSMM_GEMM_BATCH_REDUCE_STRIDE" << std::endl;
  std::cout << "br_stride_a_hint: " << br_config.br_stride_a_hint << std::endl;
  std::cout << "br_stride_b_hint: " << br_config.br_stride_b_hint << std::endl;
  std::cout << "flags: " << std::hex << flags << std::dec << std::endl;

  // **Kernel dispatch**
  libxsmm_xmmfunction kernel;
  kernel.gemm = libxsmm_dispatch_brgemm(shape, flags, prefetch_flags, br_config);

  if (kernel.gemm == NULL)
  {
    std::cout << "ERROR: Kernel dispatch failed!" << std::endl;
    return -1;
  }
  std::cout << "Kernel dispatch successful!" << std::endl;

  // **Memory allocation für batch**
  libxsmm_bfloat16 *a_bf16 = new libxsmm_bfloat16[l_m * l_k * l_batch_size];
  libxsmm_bfloat16 *b_bf16 = new libxsmm_bfloat16[l_n * l_k * l_batch_size];
  libxsmm_bfloat16 *c_bf16 = new libxsmm_bfloat16[l_m * l_n];

  float *a_ref = new float[l_m * l_k * l_batch_size];
  float *b_ref = new float[l_n * l_k * l_batch_size];
  float *c_ref = new float[l_m * l_n];

  // **Initialisierung mit Testdaten**
  libxsmm_bfloat16 zero = libxsmm_convert_f32_to_bf16_rne(0.0f);
  libxsmm_bfloat16 one = libxsmm_convert_f32_to_bf16_rne(1.0f);

  // C auf 0 setzen
  for (int i = 0; i < l_m * l_n; i++)
  {
    c_bf16[i] = zero;
    c_ref[i] = 0.0f;
  }

  // A mit sequenziellen Werten füllen
  for (int batch = 0; batch < l_batch_size; batch++)
  {
    for (int i = 0; i < l_m * l_k; i++)
    {
      float val = (float)i;
      a_ref[batch * l_m * l_k + i] = val;
      a_bf16[batch * l_m * l_k + i] = libxsmm_convert_f32_to_bf16_rne(val);
    }
  }

  // B mit einsen füllen
  for (int batch = 0; batch < l_batch_size; batch++)
  {
    for (int n = 0; n < l_n; n++)
    {
      for (int k = 0; k < l_k; k++)
      {
        b_bf16[batch * l_n * l_k + k * l_n + n] = one;
        b_ref[batch * l_n * l_k + k * l_n + n] = 1.0f;
      }
    }
  }

  if (debug_output)
  {
    std::cout << "\n=== DEBUG OUTPUT ===" << std::endl;
    std::cout << "batch 0 von A" << std::endl;

    // Ausgabe der ersten Matrix A (Batch 0)
    for (int m = 0; m < l_m; m++)
    {
      for (int k = 0; k < l_k; k++)
      {
        std::cout << std::setw(6) << libxsmm_convert_bf16_to_f32(a_bf16[k * l_lda + m]) << " ";
      }
      std::cout << std::endl;
    }

    // Ausgabe der ersten Matrix B (Batch 0)
    std::cout<< "Batch 0 von B:" << std::endl;
    for (int k = 0; k < l_k; k++)
    {
      for (int n = 0; n < l_n; n++)
      {
        std::cout << std::setw(6) << libxsmm_convert_bf16_to_f32(b_bf16[k * l_ldb + n]) << " ";
      }
      std::cout << std::endl;
    }
  }

  // **Gold Standard Berechnung**
  ref_gemm_strided_batch(a_ref, b_ref, c_ref, l_m, l_n, l_k, l_lda, l_ldb, l_ldc,
                         l_batch_size, stride_a, stride_b);

  // **LibXSMM BRGEMM Aufruf**
  libxsmm_gemm_param gemm_param;
  memset(&gemm_param, 0, sizeof(libxsmm_gemm_param));

  unsigned long long batch_size_ull = l_batch_size;
  gemm_param.a.primary = a_bf16;
  gemm_param.b.primary = b_bf16;
  gemm_param.c.primary = c_bf16;
  gemm_param.op.tertiary = &batch_size_ull; // sagt wie viele batches reduziert werden sollen

  std::cout << "\nExecuting BRGEMM kernel..." << std::endl;
  kernel.gemm(&gemm_param);
  std::cout << "BRGEMM kernel executed!" << std::endl;

  // **Vergleich der Ergebnisse**
  double max_error = 0.0;
  for (int i = 0; i < l_m * l_n; i++)
  {
    float libxsmm_val = libxsmm_convert_bf16_to_f32(c_bf16[i]);
    float ref_val = c_ref[i];
    double error = std::abs(libxsmm_val - ref_val);
    max_error = std::max(max_error, error);
  }

  std::cout << "\n=== RESULTS ===" << std::endl;
  std::cout << "Maximum error: " << max_error << std::endl;

  if (max_error < 0.1)
  {
    std::cout << "PASS: Results match!" << std::endl;
  }
  else
  {
    std::cout << "FAIL: Results differ!" << std::endl;
  }

  if (debug_output)
  {
    // Ausgabe der Ergebnismatrix C
    std::cout << "\nResult matrix C:" << std::endl;
    std::cout << "LibXSMM result:" << std::endl;
    for (int m = 0; m < l_m; m++)
    {
      for (int n = 0; n < l_n; n++)
      {
        std::cout << std::setw(8) << libxsmm_convert_bf16_to_f32(c_bf16[m + l_ldc * n]) << " ";
      }
      std::cout << std::endl;
    }

    std::cout << "\nReference result:" << std::endl;
    for (int m = 0; m < l_m; m++)
    {
      for (int n = 0; n < l_n; n++)
      {
        std::cout << std::setw(8) << c_ref[m + n * l_ldc] << " ";
      }
      std::cout << std::endl;
    }
  }

  // Cleanup
  delete[] a_bf16;
  delete[] b_bf16;
  delete[] c_bf16;
  delete[] a_ref;
  delete[] b_ref;
  delete[] c_ref;

  std::cout << "\nFinished!" << std::endl;
  return 0;
}

/*
Kompilieren und ausführen:

g++ brgemm_driver.cpp -std=c++17 \
    -I/home/daniel/libxsmm/include \
    -L/home/daniel/libxsmm/lib -lxsmm \
    -o brgemm_driver

export LD_LIBRARY_PATH=/home/daniel/libxsmm/lib:$LD_LIBRARY_PATH

# Ohne Debug-Ausgabe:
./brgemm_driver

# Mit Debug-Ausgabe:
./z_brgemm 1
*/