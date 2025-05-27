#include "BinaryContractionTpp.h"
#include "BinaryPrimitives.h"
#include "ContractionPackingTpp.h"

#include <algorithm>
#include <iostream> //das und iostream entfernen
#include <iomanip>

libxsmm_datatype einsum_ir::backend::BinaryContractionTpp::dtype_to_libxsmm(data_t i_dtype)
{
  if (i_dtype == BF16)
  { // daniel :
    return libxsmm_datatype::LIBXSMM_DATATYPE_BF16;
  }
  else if (i_dtype == FP32)
  {
    return libxsmm_datatype::LIBXSMM_DATATYPE_F32;
  }
  else if (i_dtype == FP64)
  {
    return libxsmm_datatype::LIBXSMM_DATATYPE_F64;
  }

  return libxsmm_datatype::LIBXSMM_DATATYPE_UNSUPPORTED;
}

einsum_ir::backend::BinaryContractionTpp::~BinaryContractionTpp()
{
  if (m_packing != nullptr)
  {
    delete m_packing;
  }
}

einsum_ir::err_t einsum_ir::backend::BinaryContractionTpp::compile()
{
  BinaryContraction::compile_base(); // daniel: kompilier wird nichts ,man findet dim typen heraus und setzt member variablen
  err_t l_err = err_t::UNDEFINED_ERROR;

  // determine blocking type
  BinaryPrimitives l_bin_prim;
  l_bin_prim.init(m_dtype_comp, // achtung compute type wird mitgegeben nicht input type vielleicht left/right type angeben?
                  backend_t::TPP);

  int64_t const *l_dim_ids_left_active = m_dim_ids_permute_left != nullptr ? m_dim_ids_permute_left : m_dim_ids_left;
  int64_t const *l_dim_ids_right_active = m_dim_ids_permute_right != nullptr ? m_dim_ids_permute_right : m_dim_ids_right;

  // perform blocking
  std::vector<int64_t> l_dim_ids_cb;
  std::vector<int64_t> l_dim_ids_mb;
  std::vector<int64_t> l_dim_ids_nb;
  std::vector<int64_t> l_dim_ids_kb;

  l_err = l_bin_prim.blocking(primblo_t::LEFT_KB_X_MB_CB_RIGHT_NB_X_KB_CB_OUT_NB_X_MB_CB, // // daniel : setzt blocking größen o_dim_ids_mb[0] = 3 ,o_dim_ids_nb[0] = 4,o_dim_ids_kb[0] = 2
                              m_num_dims_left,
                              m_num_dims_right,
                              m_num_dims_out,
                              l_dim_ids_left_active,
                              l_dim_ids_right_active,
                              m_dim_ids_out,
                              m_dim_sizes_inner,
                              nullptr,
                              nullptr,
                              nullptr,
                              &l_dim_ids_cb,
                              &l_dim_ids_mb,
                              &l_dim_ids_nb,
                              &l_dim_ids_kb);

  // daniel: start

  std::cout << std::left
            << std::setw(25) << "Parameter"
            << std::setw(15) << "Wert" << std::endl;
  std::cout << std::string(40, '-') << std::endl;

  // primblo
  std::cout << std::setw(25) << "i_primitive_blocking"
            << static_cast<int>(primblo_t::LEFT_KB_X_MB_CB_RIGHT_NB_X_KB_CB_OUT_NB_X_MB_CB)
            << std::endl;

  // Dimensionen
  std::cout << std::setw(25) << "m_num_dims_left" << m_num_dims_left << std::endl;
  std::cout << std::setw(25) << "m_num_dims_right" << m_num_dims_right << std::endl;
  std::cout << std::setw(25) << "m_num_dims_out" << m_num_dims_out << std::endl;

  // i_dim_ids_left_active
  for (int64_t i = 0; i < m_num_dims_left; ++i)
  {
    std::cout << std::setw(25) << (i == 0 ? "l_dim_ids_left_active" : "")
              << l_dim_ids_left_active[i] << std::endl;
  }
  // l_dim_ids_right_active
  for (int64_t i = 0; i < m_num_dims_right; ++i)
  {
    std::cout << std::setw(25) << (i == 0 ? "l_dim_ids_right_active" : "")
              << l_dim_ids_right_active[i] << std::endl;
  }
  // m_dim_ids_out
  for (int64_t i = 0; i < m_num_dims_out; ++i)
  {
    std::cout << std::setw(25) << (i == 0 ? "m_dim_ids_out" : "")
              << m_dim_ids_out[i] << std::endl;
  }

  // i_dim_sizes (Map)
  std::cout << std::setw(25) << "m_dim_sizes_inner";
  if (m_dim_sizes_inner)
  {
    bool first = true;
    for (auto &kv : *m_dim_sizes_inner)
    {
      if (!first)
        std::cout << ";";
      std::cout << "[" << kv.first << "->" << kv.second << "]";
      first = false;
    }
  }
  else
  {
    std::cout << "nullptr";
  }
  std::cout << std::endl;

  // i_strides_left/right/out = nullptr
  std::cout << std::setw(25) << "i_strides_left" << "nullptr" << std::endl;
  std::cout << std::setw(25) << "i_strides_right" << "nullptr" << std::endl;
  std::cout << std::setw(25) << "i_strides_out" << "nullptr" << std::endl;

  // Output-Vektoren
  auto _printVec = [&](const char *name, const std::vector<int64_t> *v)
  {
    std::cout << std::setw(25) << name;
    if (v)
    {
      for (size_t i = 0; i < v->size(); ++i)
      {
        if (i)
          std::cout << ",";
        std::cout << (*v)[i];
      }
    }
    else
    {
      std::cout << "nullptr";
    }
    std::cout << std::endl;
  };

  _printVec("l_dim_ids_cb", &l_dim_ids_cb);
  _printVec("l_dim_ids_mb", &l_dim_ids_mb);
  _printVec("l_dim_ids_nb", &l_dim_ids_nb);
  _printVec("l_dim_ids_kb", &l_dim_ids_kb);

  std::cout << std::string(40, '-') << std::endl
            << std::endl;
  // daniel :end
  if (l_err != err_t::SUCCESS)
  {
    return l_err;
  }

  // derive strides
  std::map<int64_t, int64_t> l_strides_left;
  std::map<int64_t, int64_t> l_strides_right;
  std::map<int64_t, int64_t> l_strides_out;
  std::map<int64_t, int64_t> l_strides_out_aux;

  strides(m_num_dims_left,
          m_dim_ids_left,
          m_dim_sizes_outer_left,
          &l_strides_left);

  strides(m_num_dims_right,
          m_dim_ids_right,
          m_dim_sizes_outer_right,
          &l_strides_right);

  strides(m_num_dims_out,
          m_dim_ids_out,
          m_dim_sizes_outer_out,
          &l_strides_out);

  // daniel : strides ausgeben:
  std::cout << std::left
            << std::setw(25) << "Parameter"
            << std::setw(15) << "Wert" << std::endl;
  std::cout << std::string(40, '-') << std::endl;
  // strides
  std::cout << std::setw(25) << "l_strides_left" << std::endl;
  for (auto &kv : l_strides_left)
  {
    std::cout << std::setw(25) << kv.first << "->" << kv.second << std::endl;
  }
  std::cout << std::setw(25) << "l_strides_right" << std::endl;
  for (auto &kv : l_strides_right)
  {
    std::cout << std::setw(25) << kv.first << "->" << kv.second << std::endl;
  }
  std::cout << std::setw(25) << "l_strides_out" << std::endl;
  for (auto &kv : l_strides_out)
  {
    std::cout << std::setw(25) << kv.first << "->" << kv.second << std::endl;
  }
  std::cout << std::string(40, '-') << std::endl
            << std::endl;
  // daniel : end

  if (m_dim_sizes_outer_out_aux != nullptr)
  {
    strides(m_num_dims_out,
            m_dim_ids_out,
            m_dim_sizes_outer_out_aux,
            &l_strides_out_aux);
  }
  else
  {
    l_strides_out_aux = l_strides_out;
  }

  std::map<int64_t, int64_t> l_dim_sizes;
  l_dim_sizes.insert(m_dim_sizes_inner->begin(), m_dim_sizes_inner->end());

  // daniel begin l_dim_sizes ausgeben:
  std::cout << std::left
            << std::setw(25) << "Parameter"
            << std::setw(15) << "Wert" << std::endl;
  std::cout << std::string(40, '-') << std::endl;
  // strides
  std::cout << std::setw(25) << "l_dim_sizes" << std::endl;
  for (auto &kv : l_dim_sizes)
  {
    std::cout << std::setw(25) << kv.first << "->" << kv.second << std::endl;
  }
  std::cout << std::string(40, '-') << std::endl
            << std::endl;
  // daniel : end

  // determine loop execution order
  if (m_loop_ids_ext == nullptr)
  {
    l_bin_prim.compileLoopOrder(m_dim_types,
                                l_dim_sizes,
                                l_strides_left,
                                l_strides_right,
                                l_strides_out,
                                m_dim_ids_c,
                                m_dim_ids_m,
                                m_dim_ids_n,
                                m_dim_ids_k,
                                l_dim_ids_cb,
                                l_dim_ids_mb,
                                l_dim_ids_nb,
                                l_dim_ids_kb,
                                m_loop_ids_int);
  }
  else
  {
    std::vector<int64_t> l_dim_ids_kernel;
    l_dim_ids_kernel.reserve(l_dim_ids_cb.size() + l_dim_ids_mb.size() + l_dim_ids_nb.size() + l_dim_ids_kb.size());
    l_dim_ids_kernel.insert(l_dim_ids_kernel.end(), l_dim_ids_cb.begin(), l_dim_ids_cb.end());
    l_dim_ids_kernel.insert(l_dim_ids_kernel.end(), l_dim_ids_nb.begin(), l_dim_ids_nb.end());
    l_dim_ids_kernel.insert(l_dim_ids_kernel.end(), l_dim_ids_mb.begin(), l_dim_ids_mb.end());
    l_dim_ids_kernel.insert(l_dim_ids_kernel.end(), l_dim_ids_kb.begin(), l_dim_ids_kb.end());
    for (std::size_t l_di = 0; l_di < l_dim_ids_kernel.size(); l_di++)
    {
      auto l_found = std::find(m_loop_ids_int.begin(), m_loop_ids_int.end(), l_dim_ids_kernel[l_di]);
      if (l_found != m_loop_ids_int.end())
      {
        m_loop_ids_int.erase(l_found);
      }
    }
  }

  std::vector<int64_t> l_dim_ids_packed_left;
  std::vector<int64_t> l_dim_ids_packed_right;

  if (m_dim_ids_permute_left != nullptr)
  {
    l_dim_ids_packed_left.reserve(l_dim_ids_cb.size() + l_dim_ids_mb.size() + l_dim_ids_kb.size());
    l_dim_ids_packed_left.insert(l_dim_ids_packed_left.end(), l_dim_ids_kb.begin(), l_dim_ids_kb.end());
    l_dim_ids_packed_left.insert(l_dim_ids_packed_left.end(), l_dim_ids_mb.begin(), l_dim_ids_mb.end());
    l_dim_ids_packed_left.insert(l_dim_ids_packed_left.end(), l_dim_ids_cb.begin(), l_dim_ids_cb.end());
  }
  if (m_dim_ids_permute_right != nullptr)
  {
    l_dim_ids_packed_right.reserve(l_dim_ids_cb.size() + l_dim_ids_kb.size() + l_dim_ids_nb.size());
    l_dim_ids_packed_right.insert(l_dim_ids_packed_right.end(), l_dim_ids_nb.begin(), l_dim_ids_nb.end());
    l_dim_ids_packed_right.insert(l_dim_ids_packed_right.end(), l_dim_ids_kb.begin(), l_dim_ids_kb.end());
    l_dim_ids_packed_right.insert(l_dim_ids_packed_right.end(), l_dim_ids_cb.begin(), l_dim_ids_cb.end());
  }

  if (m_dim_ids_permute_left != nullptr || m_dim_ids_permute_right != nullptr)
  {
    // compile packing
    m_packing = new ContractionPackingTpp;
    m_packing->init(m_num_dims_left,
                    m_num_dims_right,
                    &l_dim_sizes,
                    &l_strides_left,
                    &l_strides_right,
                    &m_dim_types,
                    m_dim_ids_left,
                    m_dim_ids_right,
                    &l_dim_ids_packed_left,
                    &l_dim_ids_packed_right,
                    &m_loop_ids_int,
                    m_dtype_left,
                    m_dtype_out,
                    m_memory);
    m_packing->compile();

    // update strides
    for (auto const &elem : m_packing->m_strides_packed_left)
    {
      l_strides_left.insert_or_assign(elem.first, elem.second);
    }
    for (auto const &elem : m_packing->m_strides_packed_right)
    {
      l_strides_right.insert_or_assign(elem.first, elem.second);
    }
  }

  // libxsmm data types
  libxsmm_datatype l_xmm_dtype_left = dtype_to_libxsmm(m_dtype_left);
  libxsmm_datatype l_xmm_dtype_right = dtype_to_libxsmm(m_dtype_right);
  libxsmm_datatype l_xmm_dtype_comp = dtype_to_libxsmm(m_dtype_comp);
  libxsmm_datatype l_xmm_dtype_out = dtype_to_libxsmm(m_dtype_out);

  if (l_xmm_dtype_left == libxsmm_datatype::LIBXSMM_DATATYPE_UNSUPPORTED || l_xmm_dtype_right == libxsmm_datatype::LIBXSMM_DATATYPE_UNSUPPORTED || l_xmm_dtype_comp == libxsmm_datatype::LIBXSMM_DATATYPE_UNSUPPORTED || l_xmm_dtype_out == libxsmm_datatype::LIBXSMM_DATATYPE_UNSUPPORTED)
  {
    return einsum_ir::COMPILATION_FAILED;
  }

  // libxsmm parameters
  libxsmm_blasint l_m = 1;
  libxsmm_blasint l_n = 1;
  libxsmm_blasint l_k = 1;
  libxsmm_blasint l_r = 1; // tiefe daniel

  libxsmm_blasint l_lda = 1; // daniel: lda = leading dimension für tensor a (left tensor)
  libxsmm_blasint l_ldb = 1;
  libxsmm_blasint l_ldc = 1;

  for (std::size_t l_cb = 0; l_cb < l_dim_ids_cb.size(); l_cb++)
  {
    int64_t l_dim_id = l_dim_ids_cb[l_cb];
    l_r *= l_dim_sizes.at(l_dim_id);
  }
  for (std::size_t l_mb = 0; l_mb < l_dim_ids_mb.size(); l_mb++)
  {
    int64_t l_dim_id = l_dim_ids_mb[l_mb];
    l_m *= l_dim_sizes.at(l_dim_id);
  }
  for (std::size_t l_nb = 0; l_nb < l_dim_ids_nb.size(); l_nb++)
  {
    int64_t l_dim_id = l_dim_ids_nb[l_nb];
    l_n *= l_dim_sizes.at(l_dim_id);
  }
  for (std::size_t l_kb = 0; l_kb < l_dim_ids_kb.size(); l_kb++)
  {
    int64_t l_dim_id = l_dim_ids_kb[l_kb];
    l_k *= l_dim_sizes.at(l_dim_id);
  }

  // set leading dimensions
  l_lda = l_dim_ids_kb.size() > 0 ? l_strides_left.at(l_dim_ids_kb.back()) : l_m * l_r;
  l_ldb = l_dim_ids_nb.size() > 0 ? l_strides_right.at(l_dim_ids_nb.back()) : l_k * l_r; // daniel : nur ldb wird = 8 die anderen sind 1 : vermutung weil tensor b 4 dimensionen hat braucht man noch eine leading dim?
  l_ldc = l_dim_ids_nb.size() > 0 ? l_strides_out.at(l_dim_ids_nb.back()) : l_m * l_r;

  // first-touch and last-touch shape

  libxsmm_meltw_unary_shape l_shape_single_touch = libxsmm_create_meltw_unary_shape(l_m * l_r,
                                                                                    l_n,
                                                                                    l_ldc,
                                                                                    l_ldc,
                                                                                    l_xmm_dtype_out,
                                                                                    l_xmm_dtype_out,
                                                                                    l_xmm_dtype_out);

  // derive the leading dimension of the auxiliary output tensor
  int64_t l_ld_out_aux = l_dim_ids_nb.size() > 0 ? l_strides_out_aux.at(l_dim_ids_nb.back()) : l_m * l_r;
  libxsmm_bitfield l_flag_out_aux_unary = LIBXSMM_MELTW_FLAG_UNARY_NONE;
  libxsmm_bitfield l_flag_out_aux_binary = LIBXSMM_MELTW_FLAG_BINARY_NONE;

  // column to matrix bcast
  if (l_dim_ids_mb.size() > 0 && l_dim_ids_nb.size() > 0 && l_strides_out_aux[l_dim_ids_mb.back()] > 0 && l_strides_out_aux[l_dim_ids_nb.back()] == 0)
  {
    l_ld_out_aux = l_m * l_r;
    l_flag_out_aux_unary = LIBXSMM_MELTW_FLAG_UNARY_BCAST_COL;
    l_flag_out_aux_binary = LIBXSMM_MELTW_FLAG_BINARY_BCAST_COL_IN_1;
  }
  // row to matrix bcast
  else if (l_dim_ids_mb.size() > 0 && l_dim_ids_nb.size() > 0 && l_strides_out_aux[l_dim_ids_mb.back()] == 0 && l_strides_out_aux[l_dim_ids_nb.back()] > 0)
  {
    l_flag_out_aux_unary = LIBXSMM_MELTW_FLAG_UNARY_BCAST_ROW;
    l_flag_out_aux_binary = LIBXSMM_MELTW_FLAG_BINARY_BCAST_ROW_IN_1;
  }
  // scalar to matrix bcast
  else if (l_dim_ids_mb.size() > 0 && l_dim_ids_nb.size() > 0 && l_strides_out_aux[l_dim_ids_mb.back()] == 0 && l_strides_out_aux[l_dim_ids_nb.back()] == 0)
  {
    l_ld_out_aux = l_m * l_r;
    l_flag_out_aux_unary = LIBXSMM_MELTW_FLAG_UNARY_BCAST_SCALAR;
    l_flag_out_aux_binary = LIBXSMM_MELTW_FLAG_BINARY_BCAST_SCALAR_IN_1;
  }
  else if (l_dim_ids_mb.size() == 0 && l_dim_ids_nb.size() > 0 && l_strides_out_aux[l_dim_ids_nb.back()] == 0)
  {
    l_flag_out_aux_unary = LIBXSMM_MELTW_FLAG_UNARY_BCAST_SCALAR;
    l_flag_out_aux_binary = LIBXSMM_MELTW_FLAG_BINARY_BCAST_SCALAR_IN_1;
  }
  // daniel : da hier der aux tensor ein unary op bekommt und die eingabetypen auch FP32 sowie comp und out auch FP32 sind bleibt das
  libxsmm_meltw_unary_shape l_shape_single_touch_aux_unary = libxsmm_create_meltw_unary_shape(l_m * l_r,        // rows (M·R)
                                                                                              l_n,              // cols (N)
                                                                                              l_ld_out_aux,     // leading dimension input/output A
                                                                                              l_ldc,            // leading dimension output C
                                                                                              l_xmm_dtype_out,  // input data type
                                                                                              l_xmm_dtype_out,  // output data type
                                                                                              l_xmm_dtype_out); // compute data type

  libxsmm_meltw_binary_shape l_shape_single_touch_aux_binary = libxsmm_create_meltw_binary_shape(l_m * l_r, // rows (M·R)
                                                                                                 l_n,       // cols (N)
                                                                                                 l_ldc,     // leading dimension C
                                                                                                 l_ld_out_aux,
                                                                                                 l_ldc, // leading dimension  C
                                                                                                 l_xmm_dtype_out,
                                                                                                 l_xmm_dtype_out,
                                                                                                 l_xmm_dtype_out,
                                                                                                 l_xmm_dtype_out);

  // first touch kernel
  if (m_ktype_first_touch == ZERO)
  {
    m_xmm_kernel_first_touch_unary = libxsmm_dispatch_meltw_unary(LIBXSMM_MELTW_TYPE_UNARY_XOR, // daniel : XOred wird jedes element mit sich selbst -> matrix wird genullt
                                                                  l_shape_single_touch,         // daniel : hier wird etwas übergeben was es niht gibt bei BF16
                                                                  LIBXSMM_MELTW_FLAG_UNARY_NONE);
    // daniel: beginn
    if (m_xmm_kernel_first_touch_unary == nullptr) // TODO: hier kommt er bei BF17 rein aber nicht bei FP32
    {
      std::cout << "[DEBUG] m_xmm_kernel_first_touch_unary is nullptr" << std::endl;
    }
    // daniel ende
  }
  else if (m_ktype_first_touch == COPY)
  {
    m_xmm_kernel_first_touch_unary = libxsmm_dispatch_meltw_unary(LIBXSMM_MELTW_TYPE_UNARY_IDENTITY,
                                                                  l_shape_single_touch_aux_unary,
                                                                  l_flag_out_aux_unary);
  }
  else if (m_ktype_first_touch == ADD)
  {
    m_xmm_kernel_first_touch_binary = libxsmm_dispatch_meltw_binary(LIBXSMM_MELTW_TYPE_BINARY_ADD,
                                                                    l_shape_single_touch_aux_binary,
                                                                    l_flag_out_aux_binary);
  }
  else if (m_ktype_first_touch != UNDEFINED_KTYPE)
  {
    return err_t::COMPILATION_FAILED;
  }

  // last touch kernel
  if (m_ktype_last_touch == RELU)
  {
    m_xmm_kernel_last_touch_unary = libxsmm_dispatch_meltw_unary(LIBXSMM_MELTW_TYPE_UNARY_RELU,
                                                                 l_shape_single_touch,
                                                                 LIBXSMM_MELTW_FLAG_UNARY_NONE);
  }
  else if (m_ktype_last_touch == ADD)
  {
    m_xmm_kernel_last_touch_binary = libxsmm_dispatch_meltw_binary(LIBXSMM_MELTW_TYPE_BINARY_ADD,
                                                                   l_shape_single_touch_aux_binary,
                                                                   l_flag_out_aux_binary);
  }
  else if (m_ktype_last_touch != UNDEFINED_KTYPE)
  {
    return err_t::COMPILATION_FAILED;
  }

  // remove packed size form leading dimension (implicit in LIBXSMM)
  if (l_lda % l_r != 0)
  {
    return einsum_ir::COMPILATION_FAILED;
  }
  if (l_ldb % l_r != 0)
  {
    return einsum_ir::COMPILATION_FAILED;
  }
  if (l_ldc % l_r != 0)
  {
    return einsum_ir::COMPILATION_FAILED;
  }
  l_lda /= l_r;
  l_ldb /= l_r;
  l_ldc /= l_r;

  // create main kernel
  libxsmm_gemm_shape l_shape_brgemm;
  libxsmm_bitfield l_flags_brgemm = LIBXSMM_GEMM_FLAGS('N', 'N'); // daniel: 'N' = no transpose
  libxsmm_bitfield l_prefetch_flags_brgemm = 0;

  l_shape_brgemm = libxsmm_create_gemm_shape(l_m,
                                             l_n,
                                             l_k,
                                             l_lda,
                                             l_ldb,
                                             l_ldc,
                                             l_xmm_dtype_left,
                                             l_xmm_dtype_right,
                                             l_xmm_dtype_out,
                                             l_xmm_dtype_comp);

  libxsmm_gemm_batch_reduce_config l_brconfig;
  l_brconfig.br_type = LIBXSMM_GEMM_BATCH_REDUCE_NONE;
  l_brconfig.br_stride_a_hint = 0;
  l_brconfig.br_stride_b_hint = 0;
  l_brconfig.br_unroll_hint = 0;

  if (m_num_dims_out > 0 && m_dim_types_out[m_num_dims_out - 1] != dim_t::C)
  {
    m_xmm_kernel_main.gemm = libxsmm_dispatch_brgemm(l_shape_brgemm,
                                                     l_flags_brgemm,
                                                     l_prefetch_flags_brgemm,
                                                     l_brconfig);
    // daniel : check if kernel is nullptr
    if (m_xmm_kernel_main.gemm == NULL)
    {
      std::cerr << "[ERROR] BRGEMM dispatch failed!" << std::endl;
    }
    else
    {
      std::cout << "[OK] BRGEMM kernel erfolgreich generiert." << std::endl;
    }
    // daniel : ende
  }
  else
  {
    m_xmm_kernel_main.gemm = libxsmm_create_packed_gemm(l_shape_brgemm,
                                                        l_flags_brgemm,
                                                        l_prefetch_flags_brgemm,
                                                        l_r);
  }

  // daniel start ─────────────────────────────────────────────────────────
  std::cout << "[DEBUG] Main GEMM ptr: "
            << (void *)m_xmm_kernel_main.gemm << std::endl;
  std::cout << "[DEBUG] GEMM shape: m=" << l_shape_brgemm.m
            << " n=" << l_shape_brgemm.n
            << " k=" << l_shape_brgemm.k
            << " lda=" << l_shape_brgemm.lda
            << " ldb=" << l_shape_brgemm.ldb
            << " ldc=" << l_shape_brgemm.ldc
            << std::endl;
  std::cout << "[DEBUG] dim_types_out: ";
  for (auto t : m_dim_types_out)
    std::cout << int(t) << " ";
  std::cout << std::endl;
  // daniel ende debug check ────────────────────────────────────────

  // check for existing kernels
  if (m_xmm_kernel_first_touch_unary == nullptr)
  {
    if (m_ktype_first_touch != kernel_t::UNDEFINED_KTYPE && m_ktype_first_touch != kernel_t::ADD)
    {
      return einsum_ir::COMPILATION_FAILED;
    }
  }
  if (m_xmm_kernel_first_touch_binary == nullptr)
  {
    if (m_ktype_first_touch != kernel_t::UNDEFINED_KTYPE && m_ktype_first_touch != kernel_t::ZERO && m_ktype_first_touch != kernel_t::COPY && m_ktype_first_touch != kernel_t::RELU)
    {
      return einsum_ir::COMPILATION_FAILED;
    }
  }
  if (m_ktype_main != UNDEFINED_KTYPE && m_xmm_kernel_main.gemm == nullptr)
  {
    return einsum_ir::COMPILATION_FAILED;
  }
  if (m_xmm_kernel_last_touch_unary == nullptr)
  {
    if (m_ktype_last_touch != kernel_t::UNDEFINED_KTYPE && m_ktype_last_touch != kernel_t::ADD)
    {
      return einsum_ir::COMPILATION_FAILED;
    }
  }
  if (m_xmm_kernel_last_touch_binary == nullptr)
  {
    if (m_ktype_last_touch != kernel_t::UNDEFINED_KTYPE && m_ktype_last_touch != kernel_t::RELU)
    {
      return einsum_ir::COMPILATION_FAILED;
    }
  }

  // contraction loop interface
  m_cont_loops.init(&l_dim_sizes,
                    &l_strides_left,
                    &l_strides_right,
                    &l_strides_out_aux,
                    &l_strides_out,
                    &m_dim_types,
                    &m_loop_ids_int,
                    ce_n_bytes(m_dtype_left),
                    ce_n_bytes(m_dtype_right),
                    ce_n_bytes(m_dtype_out),
                    m_ktype_first_touch,
                    m_ktype_main,
                    m_ktype_last_touch,
                    m_xmm_kernel_first_touch_unary,
                    m_xmm_kernel_first_touch_binary,
                    m_xmm_kernel_main.gemm,
                    m_xmm_kernel_last_touch_unary,
                    m_xmm_kernel_last_touch_binary,
                    m_packing);

  l_err = m_cont_loops.compile();
  if (l_err != einsum_ir::SUCCESS)
  {
    return einsum_ir::COMPILATION_FAILED;
  }
  m_compiled = true;

  return einsum_ir::SUCCESS;
}

void einsum_ir::backend::BinaryContractionTpp::threading(int64_t i_num_tasks_target)
{
  m_cont_loops.threading(i_num_tasks_target);
}

void einsum_ir::backend::BinaryContractionTpp::contract(void const *i_tensor_left,
                                                        void const *i_tensor_right,
                                                        void const *i_tensor_out_aux,
                                                        void *io_tensor_out)
{
  m_cont_loops.contract(i_tensor_left,
                        i_tensor_right,
                        i_tensor_out_aux,
                        io_tensor_out);
}

void einsum_ir::backend::BinaryContractionTpp::contract(void const *i_tensor_left,
                                                        void const *i_tensor_right,
                                                        void *io_tensor_out)
{
  contract(i_tensor_left,
           i_tensor_right,
           nullptr,
           io_tensor_out);
}