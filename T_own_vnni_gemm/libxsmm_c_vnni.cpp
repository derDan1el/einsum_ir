#include <iostream>
#include <cstring>
#include "/home/daniel/libxsmm/include/libxsmm.h"
#include <iomanip>

int main() {

  const int m   = 8;
  const int n   = 8;
  const int ldc = 8;


  libxsmm_bfloat16* h_c_gold = (libxsmm_bfloat16*) malloc(m * ldc * sizeof(libxsmm_bfloat16));
  for (int i = 0; i < m * n; ++i) {
    h_c_gold[i] = (libxsmm_bfloat16)i;  // Werte 0,1,2,...,255
  }


  libxsmm_bfloat16* tmp_c = (libxsmm_bfloat16*)
  libxsmm_aligned_malloc((size_t)ldc * n * sizeof(libxsmm_bfloat16), 64);
  memcpy(tmp_c, h_c_gold, (size_t)ldc * n * sizeof(libxsmm_bfloat16));


  for (int l_i = 0; l_i < n/4; ++l_i) {
    for (int l_j = 0; l_j < m; ++l_j) {
      for (int l_i2 = 0; l_i2 < 4; ++l_i2) {
        h_c_gold[(l_i * ldc * 4) + (l_j * 4) + l_i2] =
          tmp_c[(((l_i * 4) + l_i2) * ldc) + l_j];
      }
    }
  }

    std::cout << "das unformatierte C-Array:" << std::endl;
    for(uint l_n = 0; l_n < n; ++l_n) {
        for(uint l_m = 0; l_m < m; ++l_m) {
            std::cout << std::setw(4) << (int)tmp_c[l_m * ldc + l_n] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "das formatierte C-Array:" << std::endl;

    for(uint l_n = 0; l_n < n; ++l_n) {
        for(uint l_m = 0; l_m < m; ++l_m) {
            std::cout << std::setw(4) << (int)h_c_gold[l_m * ldc + l_n] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------------------------------" << std::endl;

        std::cout << "--------------------------------------------" << std::endl;
    std::cout << "das formatierte C-Array row:" << std::endl;

    for(uint l_n = 0; l_n < n; ++l_n) {
        for(uint l_m = 0; l_m < m; ++l_m) {
            std::cout << std::setw(4) << (int)h_c_gold[l_n * ldc + l_m] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------------------------------" << std::endl;





  libxsmm_free(tmp_c);
  free(h_c_gold);
  return 0;
}

/**
 * g++ libxsmm_c_vnni.cpp     -std=c++17 -O0     -I/home/daniel/libxsmm/include     -L/home/daniel/libxsmm/lib -lxsmm     -o vnni
 *  export LD_LIBRARY_PATH=/home/daniel/libxsmm/lib:$LD_LIBRARY_PATH
 * 
 * 
 */