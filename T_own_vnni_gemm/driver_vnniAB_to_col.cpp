#include "/home/daniel/libxsmm/include/libxsmm.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <random>
#include <iomanip>

//  gemm_ref_bf16_vnni(a_ref, b_ref, c_ref, m, n, k,| m, k, m);
void gemm_ref_bf16_vnni(float const *i_a,
                        float const *i_b,
                        float *io_c,
                        libxsmm_blasint i_m,
                        libxsmm_blasint i_n,
                        libxsmm_blasint i_k,
                        libxsmm_blasint i_lda,
                        libxsmm_blasint i_ldb,
                        libxsmm_blasint i_ldc)
{

  // Interpretiere die Daten genauso wie LIBXSMM VNNI es tut
  for (libxsmm_blasint vnni_bn = 0; vnni_bn < i_n / 2; ++vnni_bn) // durch 2 weil die c blöcke abgearbeitet werden
  {
    for (libxsmm_blasint vnni_bm = 0; vnni_bm < i_m / 2; ++vnni_bm) // -||-
    {
      for (libxsmm_blasint bn = 0; bn < 2; ++bn) // 0-1, weil pro block 2 spalten
      {
        for (libxsmm_blasint bm = 0; bm < 2; ++bm) // 0-1, weil pro block 2 zeilen
        {
          for (libxsmm_blasint bk = 0; bk < 4; ++bk)
          {

            /**
             *
             * um den index der für die blockzeile zu finden geht man davon aus das es keine spalten (1) gibt
             * es gibt bspw: 4 blöcke pro spalte also ist block 4 mit index 3 belegt. wenn man nun block 3 abarbeitet
             * dann hat dieser index 2. pro block gibt es 4 elemente dh heißt wenn man den dritten block
             * erreichen will in der zeile muss man zeilen block index * 4 nehmen.
             * also                                           vnni_bm  * 4
             *
             * um den index der für die blockspalte zu bekommen muss man die anzahl der blöcke
             * pro spalte kennen.. wenn es wieder 4 blöcke pro spalte gibt und man in spalte 2 will was index 1
             * hat dann muss man : blöcke pro blockspalte * spaltenblockindex * anzahl der elemente pro block
             * also                (i_m / 2)              * vnni_bm           * 4
             *
             *
             *
             *
             */
            // richtige spalte              + richtige zeile + interne spalte *2 weil 2 elemente springen um nächste spalte zu erreichen + m sagt welche zeile
            io_c[(i_ldc / 2) * vnni_bn * 4 + vnni_bm * 4 + bn * 2 + bm] +=

                i_a[(i_lda / 2) * vnni_bn * 8 + vnni_bm * 8 + bm * 4 + bk] *

                i_b[(i_ldb / 2) * vnni_bn * 8 + vnni_bm * 8 + bn * 4 + bk];
          }
        }
      }
    }
  }
}

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

  libxsmm_bitfield flags = LIBXSMM_GEMM_FLAGS('N', 'Y') | LIBXSMM_GEMM_FLAG_VNNI_A | LIBXSMM_GEMM_FLAG_VNNI_B;
  flags |= LIBXSMM_GEMM_FLAG_BETA_0;
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

  /* for (int m = 0; m < l_m; m++)
  {
    for (int k = 0; k < l_k; k++)
    {
      b_bf16[m * l_lda + k] = libxsmm_convert_f32_to_bf16_rne((float)((m * l_lda + k))); // erstmal mit 0en danach die richtigen mit 1 überschreiben
      b_ref[m * l_lda + k] = (float)(((m * l_lda + k)));
    }
  } */

     b_bf16[0] = libxsmm_convert_f32_to_bf16_rne((float) (1  % 10));
     b_bf16[1] = libxsmm_convert_f32_to_bf16_rne((float) (2  % 10));
     b_bf16[2] = libxsmm_convert_f32_to_bf16_rne((float) (3  % 10));
     b_bf16[3] = libxsmm_convert_f32_to_bf16_rne((float) (4  % 10));
     b_bf16[4] = libxsmm_convert_f32_to_bf16_rne((float) (9  % 10));
     b_bf16[5] = libxsmm_convert_f32_to_bf16_rne((float) (10 % 10));
     b_bf16[6] = libxsmm_convert_f32_to_bf16_rne((float) (11 % 10));
     b_bf16[7] = libxsmm_convert_f32_to_bf16_rne((float) (12 % 10));
     b_bf16[8] = libxsmm_convert_f32_to_bf16_rne((float) (17 % 10));
     b_bf16[9] = libxsmm_convert_f32_to_bf16_rne((float) (18 % 10));
    b_bf16[10] = libxsmm_convert_f32_to_bf16_rne((float) (19 % 10));
    b_bf16[11] = libxsmm_convert_f32_to_bf16_rne((float) (20 % 10));
    b_bf16[12] = libxsmm_convert_f32_to_bf16_rne((float) (25 % 10));
    b_bf16[13] = libxsmm_convert_f32_to_bf16_rne((float) (26 % 10));
    b_bf16[14] = libxsmm_convert_f32_to_bf16_rne((float) (27 % 10));
    b_bf16[15] = libxsmm_convert_f32_to_bf16_rne((float) (28 % 10));
    b_bf16[16] = libxsmm_convert_f32_to_bf16_rne((float) (33 % 10));
    b_bf16[17] = libxsmm_convert_f32_to_bf16_rne((float) (34 % 10));
    b_bf16[18] = libxsmm_convert_f32_to_bf16_rne((float) (35 % 10));
    b_bf16[19] = libxsmm_convert_f32_to_bf16_rne((float) (36 % 10));
    b_bf16[20] = libxsmm_convert_f32_to_bf16_rne((float) (41 % 10));
    b_bf16[21] = libxsmm_convert_f32_to_bf16_rne((float) (42 % 10));
    b_bf16[22] = libxsmm_convert_f32_to_bf16_rne((float) (43 % 10));
    b_bf16[23] = libxsmm_convert_f32_to_bf16_rne((float) (44 % 10));
    b_bf16[24] = libxsmm_convert_f32_to_bf16_rne((float) (49 % 10));
    b_bf16[25] = libxsmm_convert_f32_to_bf16_rne((float) (50 % 10));
    b_bf16[26] = libxsmm_convert_f32_to_bf16_rne((float) (51 % 10));
    b_bf16[27] = libxsmm_convert_f32_to_bf16_rne((float) (52 % 10));
    b_bf16[28] = libxsmm_convert_f32_to_bf16_rne((float) (57 % 10));
    b_bf16[29] = libxsmm_convert_f32_to_bf16_rne((float) (58 % 10));
    b_bf16[30] = libxsmm_convert_f32_to_bf16_rne((float) (59 % 10));
    b_bf16[31] = libxsmm_convert_f32_to_bf16_rne((float) (60 % 10));
    b_bf16[32] = libxsmm_convert_f32_to_bf16_rne((float) (5  % 10));
    b_bf16[33] = libxsmm_convert_f32_to_bf16_rne((float) (6  % 10));
    b_bf16[34] = libxsmm_convert_f32_to_bf16_rne((float) (7  % 10));
    b_bf16[35] = libxsmm_convert_f32_to_bf16_rne((float) (8  % 10));
    b_bf16[36] = libxsmm_convert_f32_to_bf16_rne((float) (13 % 10));
    b_bf16[37] = libxsmm_convert_f32_to_bf16_rne((float) (14 % 10));
    b_bf16[38] = libxsmm_convert_f32_to_bf16_rne((float) (15 % 10));
    b_bf16[39] = libxsmm_convert_f32_to_bf16_rne((float) (16 % 10));
    b_bf16[40] = libxsmm_convert_f32_to_bf16_rne((float) (21 % 10));
    b_bf16[41] = libxsmm_convert_f32_to_bf16_rne((float) (22 % 10));
    b_bf16[42] = libxsmm_convert_f32_to_bf16_rne((float) (23 % 10));
    b_bf16[43] = libxsmm_convert_f32_to_bf16_rne((float) (24 % 10));
    b_bf16[44] = libxsmm_convert_f32_to_bf16_rne((float) (29 % 10));
    b_bf16[45] = libxsmm_convert_f32_to_bf16_rne((float) (30 % 10));
    b_bf16[46] = libxsmm_convert_f32_to_bf16_rne((float) (31 % 10));
    b_bf16[47] = libxsmm_convert_f32_to_bf16_rne((float) (32 % 10));
    b_bf16[48] = libxsmm_convert_f32_to_bf16_rne((float) (37 % 10));
    b_bf16[49] = libxsmm_convert_f32_to_bf16_rne((float) (38 % 10));
    b_bf16[50] = libxsmm_convert_f32_to_bf16_rne((float) (39 % 10));
    b_bf16[51] = libxsmm_convert_f32_to_bf16_rne((float) (40 % 10));
    b_bf16[52] = libxsmm_convert_f32_to_bf16_rne((float) (45 % 10));
    b_bf16[53] = libxsmm_convert_f32_to_bf16_rne((float) (46 % 10));
    b_bf16[54] = libxsmm_convert_f32_to_bf16_rne((float) (47 % 10));
    b_bf16[55] = libxsmm_convert_f32_to_bf16_rne((float) (48 % 10));
    b_bf16[56] = libxsmm_convert_f32_to_bf16_rne((float) (53 % 10));
    b_bf16[57] = libxsmm_convert_f32_to_bf16_rne((float) (54 % 10));
    b_bf16[58] = libxsmm_convert_f32_to_bf16_rne((float) (55 % 10));
    b_bf16[59] = libxsmm_convert_f32_to_bf16_rne((float) (56 % 10));
    b_bf16[60] = libxsmm_convert_f32_to_bf16_rne((float) (61 % 10));
    b_bf16[61] = libxsmm_convert_f32_to_bf16_rne((float) (62 % 10));
    b_bf16[62] = libxsmm_convert_f32_to_bf16_rne((float) (63 % 10));
    b_bf16[63] = libxsmm_convert_f32_to_bf16_rne((float) (64 % 10));

  // B befüllen
  /*   int counter = 0;
    for (int i = 0; i < l_k; i++)
    {
      for (int j = 0; j < l_n; j++)
      {
        if (i == j)
        {
          b_ref[i * l_k + j] = (float)1;
          b_bf16[i * l_k + j] = libxsmm_convert_f32_to_bf16_rne(1.0f);
        }
        else
        {
          b_ref[i * l_k + j] = 0;
          b_bf16[i * l_k + j] = libxsmm_convert_f32_to_bf16_rne(0.0f);
        }
      }
    } */
  /*   for (int i = 0; i < l_m; i++)
    {
      for (int j = 0; j < l_k; j++)
      {
        if (i == j)
        {
          b_bf16[i * l_k + j] = libxsmm_convert_f32_to_bf16_rne(1.0f);
          b_ref[i * l_k + j] = 1.0f;
        }
        b_ref[i * l_k + j] = 0.0f;
      }
    } */

  for (int j = 0; j < l_k * l_n; j++)
  {
    a_ref[j] = (float)((j+1) %10 );
    b_ref[j] = (float)((j+1) %10 );
  }


/*   a_bf16[0] = one;
  a_bf16[5] = one;
  a_bf16[10] = one;
  a_bf16[15] = one;
  a_bf16[48] = one;
  a_bf16[53] = one;
  a_bf16[58] = one;
  a_bf16[63] = one; */

 a_bf16[0] = libxsmm_convert_f32_to_bf16_rne((float)(1  %10));
 a_bf16[1] = libxsmm_convert_f32_to_bf16_rne((float)(9  %10));
 a_bf16[2] = libxsmm_convert_f32_to_bf16_rne((float)(17 %10));
 a_bf16[3] = libxsmm_convert_f32_to_bf16_rne((float)(25 %10));
 a_bf16[4] = libxsmm_convert_f32_to_bf16_rne((float)(2  %10));
 a_bf16[5] = libxsmm_convert_f32_to_bf16_rne((float)(10 %10));
 a_bf16[6] = libxsmm_convert_f32_to_bf16_rne((float)(18 %10));
 a_bf16[7] = libxsmm_convert_f32_to_bf16_rne((float)(26 %10));
 a_bf16[8] = libxsmm_convert_f32_to_bf16_rne((float)(3  %10));
 a_bf16[9] = libxsmm_convert_f32_to_bf16_rne((float)(11 %10));
a_bf16[10] = libxsmm_convert_f32_to_bf16_rne((float)(19 %10));
a_bf16[11] = libxsmm_convert_f32_to_bf16_rne((float)(27 %10));
a_bf16[12] = libxsmm_convert_f32_to_bf16_rne((float)(4  %10));
a_bf16[13] = libxsmm_convert_f32_to_bf16_rne((float)(12 %10));
a_bf16[14] = libxsmm_convert_f32_to_bf16_rne((float)(20 %10));
a_bf16[15] = libxsmm_convert_f32_to_bf16_rne((float)(28 %10));
a_bf16[16] = libxsmm_convert_f32_to_bf16_rne((float)(5  %10));
a_bf16[17] = libxsmm_convert_f32_to_bf16_rne((float)(13 %10));
a_bf16[18] = libxsmm_convert_f32_to_bf16_rne((float)(21 %10));
a_bf16[19] = libxsmm_convert_f32_to_bf16_rne((float)(29 %10));
a_bf16[20] = libxsmm_convert_f32_to_bf16_rne((float)(6  %10));
a_bf16[21] = libxsmm_convert_f32_to_bf16_rne((float)(14 %10));
a_bf16[22] = libxsmm_convert_f32_to_bf16_rne((float)(22 %10));
a_bf16[23] = libxsmm_convert_f32_to_bf16_rne((float)(30 %10));
a_bf16[24] = libxsmm_convert_f32_to_bf16_rne((float)(7  %10));
a_bf16[25] = libxsmm_convert_f32_to_bf16_rne((float)(15 %10));
a_bf16[26] = libxsmm_convert_f32_to_bf16_rne((float)(23 %10));
a_bf16[27] = libxsmm_convert_f32_to_bf16_rne((float)(31 %10));
a_bf16[28] = libxsmm_convert_f32_to_bf16_rne((float)(8  %10));
a_bf16[29] = libxsmm_convert_f32_to_bf16_rne((float)(16 %10));
a_bf16[30] = libxsmm_convert_f32_to_bf16_rne((float)(24 %10));
a_bf16[31] = libxsmm_convert_f32_to_bf16_rne((float)(32 %10));
a_bf16[32] = libxsmm_convert_f32_to_bf16_rne((float)(33 %10));
a_bf16[33] = libxsmm_convert_f32_to_bf16_rne((float)(41 %10));
a_bf16[34] = libxsmm_convert_f32_to_bf16_rne((float)(49 %10));
a_bf16[35] = libxsmm_convert_f32_to_bf16_rne((float)(57 %10));
a_bf16[36] = libxsmm_convert_f32_to_bf16_rne((float)(34 %10));
a_bf16[37] = libxsmm_convert_f32_to_bf16_rne((float)(42 %10));
a_bf16[38] = libxsmm_convert_f32_to_bf16_rne((float)(50 %10));
a_bf16[39] = libxsmm_convert_f32_to_bf16_rne((float)(58 %10));
a_bf16[40] = libxsmm_convert_f32_to_bf16_rne((float)(35 %10));
a_bf16[41] = libxsmm_convert_f32_to_bf16_rne((float)(43 %10));
a_bf16[42] = libxsmm_convert_f32_to_bf16_rne((float)(51 %10));
a_bf16[43] = libxsmm_convert_f32_to_bf16_rne((float)(59 %10));
a_bf16[44] = libxsmm_convert_f32_to_bf16_rne((float)(36 %10));
a_bf16[45] = libxsmm_convert_f32_to_bf16_rne((float)(44 %10));
a_bf16[46] = libxsmm_convert_f32_to_bf16_rne((float)(52 %10));
a_bf16[47] = libxsmm_convert_f32_to_bf16_rne((float)(60 %10));
a_bf16[48] = libxsmm_convert_f32_to_bf16_rne((float)(37 %10));
a_bf16[49] = libxsmm_convert_f32_to_bf16_rne((float)(45 %10));
a_bf16[50] = libxsmm_convert_f32_to_bf16_rne((float)(53 %10));
a_bf16[51] = libxsmm_convert_f32_to_bf16_rne((float)(61 %10));
a_bf16[52] = libxsmm_convert_f32_to_bf16_rne((float)(38 %10));
a_bf16[53] = libxsmm_convert_f32_to_bf16_rne((float)(46 %10));
a_bf16[54] = libxsmm_convert_f32_to_bf16_rne((float)(54 %10));
a_bf16[55] = libxsmm_convert_f32_to_bf16_rne((float)(62 %10));
a_bf16[56] = libxsmm_convert_f32_to_bf16_rne((float)(39 %10));
a_bf16[57] = libxsmm_convert_f32_to_bf16_rne((float)(47 %10));
a_bf16[58] = libxsmm_convert_f32_to_bf16_rne((float)(55 %10));
a_bf16[59] = libxsmm_convert_f32_to_bf16_rne((float)(63 %10));
a_bf16[60] = libxsmm_convert_f32_to_bf16_rne((float)(40 %10));
a_bf16[61] = libxsmm_convert_f32_to_bf16_rne((float)(48 %10));
a_bf16[62] = libxsmm_convert_f32_to_bf16_rne((float)(56 %10));
a_bf16[63] = libxsmm_convert_f32_to_bf16_rne((float)(64 %10));

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
g++ driver_vnniAB_to_col.cpp \
    -std=c++17 -O0 \
    -I/home/daniel/libxsmm/include \
    -L/home/daniel/libxsmm/lib -lxsmm \
    -o driver

export LD_LIBRARY_PATH=/home/daniel/libxsmm/lib:$LD_LIBRARY_PATH
./driver
*/