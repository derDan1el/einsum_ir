#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#ifdef _OPENMP
#include <omp.h>
#endif
#include <cstdlib>             //daniel kann entfernt werden wenn nicht gebraucht
#include <c10/util/TypeList.h> // für c10::toString //wegmachen wenn nicht gebraucht
#include <ATen/ATen.h>
#include "backend/BinaryContractionTpp.h"
#include "backend/EinsumNode.h"
#include "frontend/EinsumExpressionAscii.h"

void bench_binary(std::map<int64_t, int64_t> &i_dim_sizes_map,
                  std::vector<int64_t> &i_dim_ids_in_left,
                  std::vector<int64_t> &i_dim_ids_in_right,
                  std::vector<int64_t> &i_dim_ids_out,
                  std::vector<int64_t> *i_loop_order,
                  at::ScalarType i_dtype_at,
                  einsum_ir::data_t i_dtype_einsum_ir,
                  std::string i_einsum_string)
{
  std::chrono::steady_clock::time_point l_tp0, l_tp1;
  std::chrono::duration<double> l_dur;
  int64_t l_n_flops = 0;
  int64_t l_repetitions = 1000000;
  int64_t l_repetitions_warm_up = 10;
  std::vector<int64_t> l_dim_ids_permute_left;
  std::vector<int64_t> l_dim_ids_permute_right;
  double l_time_compile = 0;
  double l_time = 0;
  double l_gflops = 0;

  // create vectors of sizes and einsum string
  std::vector<int64_t> l_sizes_left;  // daniel: [0] =32,[1] =8,[2] =4
  std::vector<int64_t> l_sizes_right; // daniel: [0] =32,[1] =4,[2] =2, [3] =8
  std::vector<int64_t> l_sizes_out;   // daniel: [0] =32,[1] =8,[2] =2
  for (std::size_t l_di = 0; l_di < i_dim_ids_in_left.size(); l_di++)
  {
    l_sizes_left.push_back(i_dim_sizes_map[i_dim_ids_in_left[l_di]]);
  }
  for (std::size_t l_di = 0; l_di < i_dim_ids_in_right.size(); l_di++)
  {
    l_sizes_right.push_back(i_dim_sizes_map[i_dim_ids_in_right[l_di]]);
  }
  for (std::size_t l_di = 0; l_di < i_dim_ids_out.size(); l_di++)
  {
    l_sizes_out.push_back(i_dim_sizes_map[i_dim_ids_out[l_di]]);
  }

  // number of flops
  l_n_flops = 2;
  for (std::map<int64_t, int64_t>::iterator l_di = i_dim_sizes_map.begin(); l_di != i_dim_sizes_map.end(); l_di++)
  {
    int64_t l_dim_size = l_di->second;
    l_n_flops *= l_dim_size;
  }

  /*
      auto f32_opts = at::TensorOptions().dtype(at::kFloat).device(at::kCPU);

      // --- 2) Werte als 1D‐Liste (row-major) für a, b, c
      std::vector<float> a_vals = {
          1.0f, 0.5f, 0.2f, 0.1f,
          0.0f, 0.3f, 0.7f, 0.9f,
          1.0f, 1.0f, 0.5f, 0.5f,
          0.2f, 0.2f, 0.8f, 0.4f};

      std::vector<float> b_vals = {
          0.1f, 0.2f, 0.3f, 0.4f,
          0.5f, 0.6f, 0.7f, 0.8f,
          0.9f, 1.0f, 0.1f, 0.2f,
          0.3f, 0.4f, 0.5f, 0.6f};

      // c (Output-Puffer mit festen Werten, wird vom Kernel überschrieben)
      std::vector<float> c_vals = {
          0.1f, 0.1f, 0.1f, 0.1f,
          0.2f, 0.2f, 0.2f, 0.2f,
          0.3f, 0.3f, 0.3f, 0.3f,
          0.4f, 0.4f, 0.4f, 0.4f};

      // --- 3) Erzeuge 1D-Tensoren und reshaped auf 4×4
      at::Tensor tmp_a = at::tensor(a_vals, f32_opts).reshape({4, 4});
      at::Tensor tmp_b = at::tensor(b_vals, f32_opts).reshape({4, 4});
      at::Tensor tmp_c = at::tensor(c_vals, f32_opts).reshape({4, 4});

      // --- 4) In BF16 casten (Inputs und Output-Puffer)
      at::Tensor l_ten_left = tmp_a.to(at::kBFloat16);
      at::Tensor l_ten_right = tmp_b.to(at::kBFloat16);
      at::Tensor l_ten_out = tmp_c.to(l_dtype_out_at);
  */

  at::ScalarType l_dtype_out_at = i_dtype_at;
  if (i_dtype_at == at::ScalarType::BFloat16)
  {
    l_dtype_out_at = at::ScalarType::Float; // daniel: Outputtensor ist FP32
  }
  at::Tensor l_ten_left = at::rand(at::IntArrayRef(l_sizes_left.data(), l_sizes_left.size()), i_dtype_at); // daniel: zufällige Werte [0,1]
  at::Tensor l_ten_right = at::rand(at::IntArrayRef(l_sizes_right.data(), l_sizes_right.size()), i_dtype_at);
  at::Tensor l_ten_out = at::rand(at::IntArrayRef(l_sizes_out.data(), l_sizes_out.size()), l_dtype_out_at);
  std::cout << "at::einsum:" << std::endl;

  at::Tensor l_ten_out_torch;
  l_tp0 = std::chrono::steady_clock::now();
  for (int64_t l_rep = 0; l_rep < l_repetitions_warm_up; l_rep++)
  {
    l_ten_out_torch = at::einsum(i_einsum_string,
                                 {l_ten_left, l_ten_right},
                                 {{0, 1}});
  }
  l_tp1 = std::chrono::steady_clock::now();
  l_dur = std::chrono::duration_cast<std::chrono::duration<double>>(l_tp1 - l_tp0);
  l_repetitions = l_repetitions_warm_up / l_dur.count() + 1;

  // run with repititions
  l_tp0 = std::chrono::steady_clock::now();
  for (int64_t l_rep = 0; l_rep < l_repetitions; l_rep++)
  {
    l_ten_out_torch = at::einsum(i_einsum_string,
                                 {l_ten_left, l_ten_right},
                                 {{0, 1}});
  }
  l_tp1 = std::chrono::steady_clock::now();
  l_dur = std::chrono::duration_cast<std::chrono::duration<double>>(l_tp1 - l_tp0);
  l_time = l_dur.count() / l_repetitions;
  l_gflops = 1.0E-9 * l_n_flops / l_time;

  /*────────────────────── Hiermit kann man Input / Output von torch printen ────────────────────────────────────
    std::cout << "Input Left  (dtype=" << c10::toString(l_ten_left.dtype()) << "):\n"
              << l_ten_left << "\n\n";
    std::cout << "Input Right (dtype=" << c10::toString(l_ten_right.dtype()) << "):\n"
              << l_ten_right << "\n\n";

    std::cout << "Torch-Output VOR Cast (dtype="
              << c10::toString(l_ten_out_torch.dtype()) << "):\n"
              << l_ten_out_torch << std::endl;

    if (l_ten_out_torch.dtype() == at::kBFloat16)
    {
      l_ten_out_torch = l_ten_out_torch.to(at::kFloat);
    }
    std::cout << "Torch-Output NACH Cast (dtype="
              << c10::toString(l_ten_out_torch.dtype()) << "):\n"
              << l_ten_out_torch << std::endl;
  ──────────────────────────────────────────────────────────*/
  std::cout << "  time (contract): " << l_time << std::endl;
  std::cout << "  gflops: " << l_gflops << std::endl;

  /*
   * einsum_ir
   */
  std::cout << "einsum_ir:" << std::endl;
  // daniel : checke ob dtype_einsum_ir == BF16 wenn ja solls in FP32 gerechnet werden
  einsum_ir::data_t l_dtype_comp_and_out = (i_dtype_einsum_ir == einsum_ir::BF16) ? einsum_ir::FP32 : i_dtype_einsum_ir; // daniel siehe kommentar darüber
  einsum_ir::backend::MemoryManager l_memory;
  einsum_ir::backend::BinaryContractionTpp l_bin_cont;
  l_bin_cont.init(i_dim_ids_in_left.size(),
                  i_dim_ids_in_right.size(),
                  i_dim_ids_out.size(),
                  &i_dim_sizes_map,
                  &i_dim_sizes_map,
                  &i_dim_sizes_map,
                  nullptr,
                  &i_dim_sizes_map,
                  i_loop_order,
                  i_dim_ids_in_left.data(),
                  i_dim_ids_in_right.data(),
                  i_dim_ids_out.data(),
                  l_dim_ids_permute_left.data(),
                  l_dim_ids_permute_right.data(),
                  &l_memory,
                  i_dtype_einsum_ir,
                  i_dtype_einsum_ir,
                  l_dtype_comp_and_out,        // daniel : hier stand vorher auch i_dtype_einsum_ir nur muss bei BF16 in FP32 gerechnet werden siehe oben
                  l_dtype_comp_and_out,        // daniel : der outputtensor ist auch fp32
                  einsum_ir::ZERO,             // daniel: first touch
                  einsum_ir::MADD,             // daniel: main kernel
                  einsum_ir::UNDEFINED_KTYPE); // daniel: last touch

  l_tp0 = std::chrono::steady_clock::now();
  auto comp_err = l_bin_cont.compile(); // daniel :  l_bin_cont.compile(); stand schon so vorher da aber ich fügte auto comp_err hinzu um zu debuggen
  // daniel :checke ob kernel kompiliert wurde
  std::cout << "JIT compile err: " << static_cast<int>(comp_err) << std::endl;
  // daniel :ende

  l_memory.alloc_all_memory();
  l_tp1 = std::chrono::steady_clock::now();
  l_dur = std::chrono::duration_cast<std::chrono::duration<double>>(l_tp1 - l_tp0);
  l_time_compile = l_dur.count();

  // enable threading
#ifdef _OPENMP
  // four times overload
  int64_t l_num_tasks = omp_get_max_threads() * 4;

  l_bin_cont.threading(l_num_tasks);
#endif

  // warm up
  l_tp0 = std::chrono::steady_clock::now();
  for (int64_t l_rep = 0; l_rep < l_repetitions_warm_up; l_rep++)
  {
    l_bin_cont.contract(l_ten_left.data_ptr(),
                        l_ten_right.data_ptr(),
                        l_ten_out.data_ptr());
  }
  l_tp1 = std::chrono::steady_clock::now();
  l_dur = std::chrono::duration_cast<std::chrono::duration<double>>(l_tp1 - l_tp0);
  l_repetitions = l_repetitions_warm_up / l_dur.count() + 1;
  l_tp0 = std::chrono::steady_clock::now();
  for (int64_t l_rep = 0; l_rep < l_repetitions; l_rep++)
  {
    l_bin_cont.contract(l_ten_left.data_ptr(),
                        l_ten_right.data_ptr(),
                        l_ten_out.data_ptr());
  }
  l_tp1 = std::chrono::steady_clock::now();
  l_dur = std::chrono::duration_cast<std::chrono::duration<double>>(l_tp1 - l_tp0);
  l_time = l_dur.count() / l_repetitions;
  l_gflops = 1.0E-9 * l_n_flops / l_time;

  std::cout << "  time (compile): " << l_time_compile << std::endl;
  std::cout << "  time (contract): " << l_time << std::endl;
  std::cout << "  gflops: " << l_gflops << std::endl;

  /*────────────────────────Ausgabe: ten out von kernel und dtype────────────────────────────────────────────────
    // daniel : beginn den datentyp der tensoren verifizieren
   std::cout << "Torch-Output DType:  "
             << c10::toString(l_ten_out_torch.dtype())
             << std::endl;
   std::cout << "Kernel-Output DType: "
             << c10::toString(l_ten_out.dtype())
             << std::endl;

   daniel : ende

   std::cout << "JIT-Output (dtype="
   << c10::toString(l_ten_out.dtype()) << "):\n"
   << l_ten_out << std::endl;
   ────────────────────────────────────────────────────────────────────────*/

   // daniel : cast ist wichtig um allclose aufzurufen da beide den gleichen typ haben müssen und torch bei .einsum keine dtype angibt und der output dtype vom input abhängt
  if (l_ten_out_torch.dtype() == at::kBFloat16)
  {
    l_ten_out_torch = l_ten_out_torch.to(at::kFloat);
  }

  // 1) max Wert
  std::cout << "max torch = " << l_ten_out_torch.max() << std::endl;
  std::cout << "max jit   = " << l_ten_out.max()<< std::endl;
  // 2) max abs diff
  auto diff = (l_ten_out_torch - l_ten_out).abs();
  auto max_e = diff.max();
  std::cout << "max abs difference = " << max_e << std::endl;

  if (!at::allclose(l_ten_out_torch, l_ten_out, 1e-03))
  {
    std::cerr << "error: einsum_ir solution is not close to aten!" << std::endl;
    std::cout << "acceptable difference:\t" << 1e-03 << std::endl;
    std::cout << "maximal difference is:\t" << at::max(l_ten_out_torch - l_ten_out) << std::endl;
    std::cout << "max value of l_ten_out_torch is:\t" << at::max(l_ten_out_torch) << std::endl;
    std::cout << "max value of l_ten_out is:\t" << at::max(l_ten_out) << std::endl;
  }
  else
  {
    std::cout << "  results are close" << std::endl;
    std::cout << "maximal difference is:\t" << at::max(l_ten_out_torch - l_ten_out) << std::endl;
    std::cout << "max value of l_ten_out_torch is:\t" << at::max(l_ten_out_torch) << std::endl;
    std::cout << "max value of l_ten_out is:\t" << at::max(l_ten_out) << std::endl;
  }

  /**
   * Matmul
   **/
  int64_t l_size_c = 1;
  int64_t l_size_m = 1;
  int64_t l_size_n = 1;
  int64_t l_size_k = 1;

  for (int64_t l_di = 0; l_di < l_bin_cont.m_num_dims_c; l_di++)
  {
    l_size_c *= l_bin_cont.m_sizes_c[l_di];
  }
  for (int64_t l_di = 0; l_di < l_bin_cont.m_num_dims_m; l_di++)
  {
    l_size_m *= l_bin_cont.m_sizes_m[l_di];
  }
  for (int64_t l_di = 0; l_di < l_bin_cont.m_num_dims_n; l_di++)
  {
    l_size_n *= l_bin_cont.m_sizes_n[l_di];
  }
  for (int64_t l_di = 0; l_di < l_bin_cont.m_num_dims_k; l_di++)
  {
    l_size_k *= l_bin_cont.m_sizes_k[l_di];
  }

  at::Tensor l_mat_a = at::rand({l_size_c, l_size_k, l_size_m});
  at::Tensor l_mat_b = at::rand({l_size_c, l_size_n, l_size_k});
  at::Tensor l_out_matmul;
  std::cout << "at::matmul:" << std::endl;

  // warm up
  l_tp0 = std::chrono::steady_clock::now();
  for (int64_t l_rep = 0; l_rep < l_repetitions_warm_up; l_rep++)
  {
    l_out_matmul = at::matmul(l_mat_b, l_mat_a);
  }
  l_tp1 = std::chrono::steady_clock::now();
  l_dur = std::chrono::duration_cast<std::chrono::duration<double>>(l_tp1 - l_tp0);
  l_repetitions = l_repetitions_warm_up / l_dur.count() + 1;

  // run with repititions
  l_tp0 = std::chrono::steady_clock::now();
  for (int64_t l_rep = 0; l_rep < l_repetitions; l_rep++)
  {
    l_out_matmul = at::matmul(l_mat_b, l_mat_a);
  }
  l_tp1 = std::chrono::steady_clock::now();
  l_dur = std::chrono::duration_cast<std::chrono::duration<double>>(l_tp1 - l_tp0);
  l_time = l_dur.count() / l_repetitions;
  l_gflops = 1.0E-9 * l_n_flops / l_time;

  std::cout << "  time:   " << l_time << std::endl;
  std::cout << "  gflops: " << l_gflops << std::endl;
}

int main(int i_argc,
         char *i_argv[])
{
  std::cout << "running bench_binary!" << std::endl;

  if (i_argc < 3)
  {
    std::cerr << "Usage:" << std::endl;
    std::cerr << "  bench_binary einsum_string dimension_sizes dtype loop_order" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Arguments:" << std::endl;
    std::cerr << "  * einsum_string:    Einsum string with a binary contraction" << std::endl;
    std::cerr << "  * dimension_sizes:  Dimension sizes have to be in ascending order of the dimension ids." << std::endl;
    std::cerr << "  * dtype:            FP32, FP64, CPX_FP32 or CPX_FP64, default: FP32." << std::endl;
    std::cerr << "  * loop_order:       Loop execution strategy" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Example:" << std::endl;
    std::cerr << "  ./bench_binary \"abc,acdb->abd\" \"32,8,4,2\" FP32 \"a,b,c,d\"" << std::endl;
    return EXIT_FAILURE;
  }

  /**
   * parse expression string
   **/
  std::string l_expression_string_arg(i_argv[1]);
  std::cout << "einsum string: " << l_expression_string_arg << " Daniel" << std::endl;
  std::string l_expression_string_std;
  std::string l_expression_string_schar;

  if (l_expression_string_arg[0] == '[')
  {
    l_expression_string_std = l_expression_string_arg;
    einsum_ir::frontend::EinsumExpressionAscii::standard_to_schar(l_expression_string_std,
                                                                  l_expression_string_schar);
  }
  else
  {
    l_expression_string_schar = l_expression_string_arg;
    // daniel: macht aus "abc,acdb->abd" das: "[a,b,c],[a,c,d,b]->[a,b,d]"
    einsum_ir::frontend::EinsumExpressionAscii::schar_to_standard(l_expression_string_schar,
                                                                  l_expression_string_std);
  }
  /**
   * parse input tensors and output tensors
   **/
  std::vector<std::string> l_tensors;
  // daniel: fügt l_tensor vektor folgende elemente hinzu: [0]="a,b,c" , [1] = "a,c,d,b" , [2]= "a,b,d"
  einsum_ir::frontend::EinsumExpressionAscii::parse_tensors(l_expression_string_std,
                                                            l_tensors);
  int64_t l_num_tensors = l_tensors.size();

  if (l_num_tensors != 3)
  {
    std::cerr << "Einsum string is not a binary contraction" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "parsed tensors:" << std::endl;
  for (int64_t l_te = 0; l_te < l_num_tensors; l_te++)
  {
    std::cout << "  " << l_tensors[l_te] << std::endl;
  }

  /*
   * create mapping from dimension name to id
   */
  std::map<std::string, int64_t> m_map_dim_name_to_id;
  // daniel: fügt m_map_dim_name_to_id folgendes hinzu ["a"] = 0 , ["b"] = 1, ["c"] = 2 , ["d"] = 3
  einsum_ir::frontend::EinsumExpressionAscii::parse_dim_ids(l_expression_string_std, // danielx
                                                            m_map_dim_name_to_id);
  // die dimensionsnamen werden lexikographisch sortiert
  std::cout << "parsed dimension ids:" << std::endl;
  for (std::map<std::string, int64_t>::iterator l_di = m_map_dim_name_to_id.begin(); l_di != m_map_dim_name_to_id.end(); l_di++)
  {
    std::string l_dim_name = l_di->first;
    int64_t l_dim_id = l_di->second;

    std::cout << "  " << l_dim_name << ": " << l_dim_id << std::endl;
  }

  /*
   * parse dimension sizes
   */
  std::string l_dim_sizes_string(i_argv[2]);
  std::vector<int64_t> l_dim_sizes_vec;
  // daniel: l_dim_sizes_vec: [0]= 32, [1]= 8, [2]= 4 , [3]= 2
  einsum_ir::frontend::EinsumExpressionAscii::parse_dim_sizes(l_dim_sizes_string,
                                                              l_dim_sizes_vec);

  std::cout << "parsed dimension sizes:" << std::endl;
  // iterate over keys of map dim name to id
  for (std::map<std::string, int64_t>::iterator l_di = m_map_dim_name_to_id.begin(); l_di != m_map_dim_name_to_id.end(); l_di++)
  {
    std::string l_dim_name = l_di->first;
    int64_t l_dim_id = l_di->second;
    int64_t l_dim_size = l_dim_sizes_vec[l_dim_id];

    std::cout << "  " << l_dim_name << ": " << l_dim_size << std::endl;
  }

  /*
   * parse dtype
   */
  // daniel: hier wird erstmal der standard dyte gesetzt auf FP32 / FLOAT
  at::ScalarType l_dtype_at = at::ScalarType::Float;
  einsum_ir::data_t l_dtype_einsum_ir = einsum_ir::FP32;
  if (i_argc > 3)
  {
    std::string l_arg_dtype = std::string(i_argv[3]);

    if (l_arg_dtype == "BF16")
    {
      l_dtype_at = at::ScalarType::BFloat16;
      l_dtype_einsum_ir = einsum_ir::BF16;
    }
    else if (l_arg_dtype == "FP32")
    {
      l_dtype_at = at::ScalarType::Float;
      l_dtype_einsum_ir = einsum_ir::FP32;
    }
    else if (l_arg_dtype == "FP64")
    {
      l_dtype_at = at::ScalarType::Double;
      l_dtype_einsum_ir = einsum_ir::FP64;
    }
    else
    {
      l_dtype_einsum_ir = einsum_ir::UNDEFINED_DTYPE;
    }
  }

  if (l_dtype_einsum_ir == einsum_ir::BF16)
  {
    std::cout << "dtype: BF16" << std::endl;
  }
  else if (l_dtype_einsum_ir == einsum_ir::FP32)
  {
    std::cout << "dtype: FP32" << std::endl;
  }
  else if (l_dtype_einsum_ir == einsum_ir::FP64)
  {
    std::cout << "dtype: FP64" << std::endl;
  }
  else
  {
    std::cerr << "failed to determine dtype" << std::endl;
    return EXIT_FAILURE;
  }

  /*
   * parse loop order
   */
  std::vector<int64_t> l_loop_order; // daniel: o_loop_order: [0] =0,[1] =1,[2] =2,[3] =3,
  std::vector<int64_t> *l_loop_order_ptr = nullptr;
  if (i_argc > 4)
  {
    std::string l_loop_string(i_argv[4]);
    einsum_ir::frontend::EinsumExpressionAscii::parse_loop_order(l_loop_string,
                                                                 m_map_dim_name_to_id,
                                                                 l_loop_order);
    std::cout << "loop order: " << l_loop_string << std::endl;
    l_loop_order_ptr = &l_loop_order;
  }

  /*
   * convert dim_sizes vector to map
   */
  std::map<int64_t, int64_t> l_dim_sizes_map; // daniel:l_dim_sizes_map [0] =32,[1] =8,[2] =4,[3] =2,
  for (std::map<std::string, int64_t>::iterator l_di = m_map_dim_name_to_id.begin(); l_di != m_map_dim_name_to_id.end(); l_di++)
  {
    int64_t l_dim_id = l_di->second;
    int64_t l_dim_size = l_dim_sizes_vec[l_dim_id];
    l_dim_sizes_map.insert(std::pair<int64_t, int64_t>(l_dim_id, l_dim_size));
  }

  /*
   * Convert tensors to vector of ids
   */
  std::vector<int64_t> l_dim_ids_in_left;  // daniel: [0] =0,[1] =1,[2] =2
  std::vector<int64_t> l_dim_ids_in_right; // daniel: [0] =0,[1] =2, [2] =3, [3] =1
  std::vector<int64_t> l_dim_ids_out;      // daniel: [0] =0,[1] =1,[2] =3

  std::vector<std::string> l_tensor_dim_names_left;  // daniel: [0] ="a",[1] ="b",[2] ="c"
  std::vector<std::string> l_tensor_dim_names_right; // daniel: [0] ="a",[1] ="c",[2] ="d" ,[3] ="b"
  std::vector<std::string> l_tensor_dim_names_out;   // daniel: [0] ="a",[1] ="b",[2] ="d"

  einsum_ir::frontend::EinsumExpressionAscii::split_string(l_tensors[0],
                                                           std::string(","),
                                                           l_tensor_dim_names_left);

  for (std::size_t l_na = 0; l_na < l_tensor_dim_names_left.size(); l_na++)
  {
    std::string l_dim_name = l_tensor_dim_names_left[l_na];
    int64_t l_dim_id = m_map_dim_name_to_id[l_dim_name];
    l_dim_ids_in_left.push_back(l_dim_id);
  }
  einsum_ir::frontend::EinsumExpressionAscii::split_string(l_tensors[1],
                                                           std::string(","),
                                                           l_tensor_dim_names_right);

  for (std::size_t l_na = 0; l_na < l_tensor_dim_names_right.size(); l_na++)
  {
    std::string l_dim_name = l_tensor_dim_names_right[l_na];
    int64_t l_dim_id = m_map_dim_name_to_id[l_dim_name];
    l_dim_ids_in_right.push_back(l_dim_id);
  }
  einsum_ir::frontend::EinsumExpressionAscii::split_string(l_tensors[2],
                                                           std::string(","),
                                                           l_tensor_dim_names_out);

  for (std::size_t l_na = 0; l_na < l_tensor_dim_names_out.size(); l_na++)
  {
    std::string l_dim_name = l_tensor_dim_names_out[l_na];
    int64_t l_dim_id = m_map_dim_name_to_id[l_dim_name];
    l_dim_ids_out.push_back(l_dim_id);
  }

  bench_binary(l_dim_sizes_map,
               l_dim_ids_in_left,
               l_dim_ids_in_right,
               l_dim_ids_out,
               l_loop_order_ptr,
               l_dtype_at,
               l_dtype_einsum_ir,
               l_expression_string_schar);

  std::cout << "finished running bench_binary!" << std::endl;
  return EXIT_SUCCESS;
}