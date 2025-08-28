#include "/home/daniel/libxsmm/include/libxsmm.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <random>
#include <iomanip>

void ref_gemm(float const *i_a,
              float const *i_b,
              float *io_c,
              libxsmm_blasint i_m,
              libxsmm_blasint i_n,
              libxsmm_blasint i_k,
              libxsmm_blasint i_lda,
              libxsmm_blasint i_ldb,
              libxsmm_blasint i_ldc)
{
  for (libxsmm_blasint n = 0; n < i_n; ++n)
  {
    for (libxsmm_blasint m = 0; m < i_m; ++m)
    {
      io_c[m + n * i_ldc] = 0.0f;
      for (libxsmm_blasint k = 0; k < i_k; ++k)
      {
        io_c[m + n * i_ldc] += i_a[m + k * i_lda] * i_b[k + n * i_ldb];
      }
    }
  }
}

int main()
{
  std::cout << "=====================================" << std::endl;
  // alle drei flags gemeinsam funktionieren nicht.

  libxsmm_bitfield flags = LIBXSMM_GEMM_FLAGS('N', 'N') | LIBXSMM_GEMM_FLAG_VNNI_C;
  flags |= LIBXSMM_GEMM_FLAG_BETA_0; //muss gesetzt sein wenn c gesetzt ist
  libxsmm_bitfield l_prefetch_flags_brgemm = 0;

  libxsmm_gemm_batch_reduce_config br_config;
  br_config.br_type = LIBXSMM_GEMM_BATCH_REDUCE_NONE;
  br_config.br_stride_a_hint = 0;
  br_config.br_stride_b_hint = 0;
  br_config.br_unroll_hint = 0;

  libxsmm_blasint l_m = 8;
  libxsmm_blasint l_n = 8;
  libxsmm_blasint l_k = 8;
  libxsmm_blasint l_lda = l_m;
  libxsmm_blasint l_ldb = l_k;
  libxsmm_blasint l_ldc = l_m;

  libxsmm_gemm_batch_reduce_config l_config;
  l_config.br_type = LIBXSMM_GEMM_BATCH_REDUCE_NONE;

  libxsmm_gemm_shape shape = libxsmm_create_gemm_shape(l_m,
                                                       l_n,
                                                       l_k,
                                                       l_lda,                 // lda
                                                       l_ldb,                 // ldb
                                                       l_ldc,                 // ldc
                                                       LIBXSMM_DATATYPE_BF16, // A
                                                       LIBXSMM_DATATYPE_BF16, // B
                                                       LIBXSMM_DATATYPE_BF16, // C
                                                       LIBXSMM_DATATYPE_F32); // comp_type

  libxsmm_xmmfunction l_kernel_forward;
  l_kernel_forward.gemm = libxsmm_dispatch_brgemm(shape,
                                                  flags,
                                                  l_prefetch_flags_brgemm,
                                                  br_config);

  libxsmm_gemm_param l_param;
  memset(&l_param, 0, sizeof(libxsmm_gemm_param));

  libxsmm_bfloat16 *a_bf16 = new libxsmm_bfloat16[l_m * l_k];
  libxsmm_bfloat16 *b_bf16 = new libxsmm_bfloat16[l_n * l_k];
  libxsmm_bfloat16 *c_bf16 = new libxsmm_bfloat16[l_m * l_n];

  float *a_ref = new float[l_m * l_k];
  float *b_ref = new float[l_n * l_k];
  float *c_ref = new float[l_m * l_n];

  libxsmm_bfloat16 zero = libxsmm_convert_f32_to_bf16_rne(0.0f);
  libxsmm_bfloat16 one = libxsmm_convert_f32_to_bf16_rne(1.0f);

  // C
  for (int i = 0; i < l_m * l_n; i++)
  {
    c_bf16[i] = zero;
    c_ref[i] = 0.0f;
  }

  // set A
  for (int i = 0; i < l_m * l_k; i++)
  {

    a_bf16[i] = libxsmm_convert_f32_to_bf16_rne((float)((i+1) % 10));
    a_ref[i] = (float)((i+1) % 10);
  }

// set B
  for (int i = 0; i < l_k * l_n; i++)
  {

    b_bf16[i] = libxsmm_convert_f32_to_bf16_rne((float)(i % 10));
    b_ref[i] = (float)(i % 10);
  }



std::cout << "Ausgabe A:" << std::endl;

// A:
for (int j = 0; j < l_m; j++)
{
  for (int i = 0; i < l_k; i++)
  {
    std::cout << std::setw(4) << libxsmm_convert_bf16_to_f32(a_bf16[i * l_lda + j]) << " ";
  }
  std::cout << std::endl;
}

std::cout << "--------------------------------------------" << std::endl;
std::cout << "Ausgabe B:" << std::endl;

// B:
for (int j = 0; j < l_k; j++)
{
  for (int i = 0; i < l_n; i++)
  {
    std::cout << std::setw(4) << libxsmm_convert_bf16_to_f32(b_bf16[i * l_ldb + j]) << " ";
  }
  std::cout << std::endl;
}

std::cout << "ref_gemm Aufruf" << std::endl;

ref_gemm(a_ref, b_ref, c_ref, l_m, l_n, l_k, l_lda, l_ldb, l_ldc);

std::cout << "--------------------------------------------" << std::endl;

l_param.a.primary = a_bf16;
l_param.b.primary = b_bf16;
l_param.c.primary = c_bf16;

std::cout << "--------------------------------------------" << std::endl;
l_kernel_forward.gemm(&l_param);

std::cout << "Ausgabe von C Referenz" << std::endl;
for (int j = 0; j < l_m; j++)
{
  for (int i = 0; i < l_n; i++)
  {
    std::cout << std::setw(4) << c_ref[i * l_ldc + j] << " ";
  }
  std::cout << std::endl;
}
std::cout << "--------------------------------------------" << std::endl;

std::cout << "Ausgabe von C Kernel" << std::endl;

for (int j = 0; j < l_m; j++)
{
  for (int i = 0; i < l_n; i++)
  {
    std::cout << std::setw(4) << libxsmm_convert_bf16_to_f32(c_bf16[i * l_ldc + j]) << " ";
  }
  std::cout << std::endl;
}

std::cout << "--------------------------------------------" << std::endl;

std::cout << "Ausgabe von C linear:" << std::endl;

for (int i = 0; i < l_m * l_n; i++)
{
  std::cout << libxsmm_convert_bf16_to_f32(c_bf16[i]) << ", ";
}

std::cout << "finished!" << std::endl;

delete[] a_ref;
delete[] b_ref;
delete[] c_ref;
delete[] a_bf16;
delete[] b_bf16;
delete[] c_bf16;

return 0;
}

/*
Kompilieren für Grace CPU:
g++ driver_col_col_to_A_vnni.cpp \
    -std=c++17 -O0 \
    -I/home/daniel/libxsmm/include \
    -L/home/daniel/libxsmm/lib -lxsmm \
    -o driver

export LD_LIBRARY_PATH=/home/daniel/libxsmm/lib:$LD_LIBRARY_PATH
./driver
*/