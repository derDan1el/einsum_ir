

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cmath>
#include <libxsmm_math.h>


constexpr int N = 512;

void compute_bf16_errors(libxsmm_bfloat16 (&bf16_arr)[N], 
                                float (&fp32_arr)[N],
                                float (&fp32_to_bf16_err_abs)[N], 
                                float (&fp32_to_bf16_err_rel)[N]) noexcept { // dem compiler versprechen,das die funktionen keine Fehler werfen

  for (int i = 0; i < N; ++i) {
    float f                 = static_cast<float>(i + 1);
    bf16_arr[i]             = libxsmm_convert_f32_to_bf16_rne(f);
    fp32_arr[i]             = libxsmm_convert_bf16_to_f32(bf16_arr[i]); // um den Fehler präzise zu berechnen
    fp32_to_bf16_err_abs[i] = std::fabs(fp32_arr[i] - f);
    fp32_to_bf16_err_rel[i] = fp32_to_bf16_err_abs[i] / f;
  }
}

inline std::string to_hex(uint16_t bits) {
  std::ostringstream oss;
  oss << "0x" 
      << std::hex           //Zahlen wie 255 werden also "ff" ausgegeben
      << std::uppercase     //aus "ff" wird "FF"
      << std::setw(4)       //4 Stellen weil 4 *4 bit = 16 bit 
      << std::setfill('0')  //mit 0 auffüllen von links
      << bits;              //hier wird die Zahl in hexadezimal umgewandelt
  return oss.str();
}

int main() {

  std::ios::sync_with_stdio(false);

  libxsmm_bfloat16 bf16_arr[N];

  float fp32_arr[N];
  float fp32_to_bf16_err_abs[N];
  float fp32_to_bf16_err_rel[N];

  
  compute_bf16_errors(bf16_arr, fp32_arr, fp32_to_bf16_err_abs, fp32_to_bf16_err_rel);

  std::cout << std::setw(4) << "int"
            << std::setw(12) << "bf16_val"
            << std::setw(10) << "abs_err"
            << std::setw(12) << "rel_err" 
            << std::setw(12) << "bf16_hex"<< '\n'
            << std::string(50, '-') << '\n';

  // Ausgabe der Ergebnisse
  for (int i = 0; i < N; ++i) {
    std::cout << std::setw(3) << (i + 1)
              << std::setw(12) << std::fixed << std::setprecision(2)  << fp32_arr[i]
              << std::setw(8) << std::fixed << std::setprecision(3)  << fp32_to_bf16_err_abs[i]
              << std::setw(15) << std::fixed << std::setprecision(10)  << fp32_to_bf16_err_rel[i]
              << std::setw(8) << to_hex(*reinterpret_cast<uint16_t*>(&bf16_arr[i])) // um die bits nicht zu verändern und als uint16_t zu interpretieren
              << std::endl;
  }

  return 0;
}

/*
g++ show_values_up_to_512.cpp \
    -std=c++17 -O0 \
    -I/home/daniel/libxsmm/include \
    -L/home/daniel/libxsmm/lib -lxsmm \
    -o show_values_up_to_512

export LD_LIBRARY_PATH=/home/daniel/libxsmm/lib:$LD_LIBRARY_PATH
./show_values_up_to_512





*/