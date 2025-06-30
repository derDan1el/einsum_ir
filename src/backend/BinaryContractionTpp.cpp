#include "BinaryContractionTpp.h"
#include "BinaryPrimitives.h"
#include "ContractionPackingTpp.h"

#include <algorithm>
//#include <iostream> //das und iostream entfernen
//#include <iomanip>

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
  l_bin_prim.init(m_dtype_out, // hier stand comp eig
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

  if (l_err != err_t::SUCCESS)
  {
    return l_err;
  }

  // DEBUG: Print K dimension information after blocking
  /* //std::cout << "\n=== DEBUG K Dimensions After Blocking ===" << std::endl;
  //std::cout << "m_dim_ids_k (all K dimensions): ";
  for (size_t i = 0; i < m_dim_ids_k.size(); i++)
  {
    //std::cout << "[" << i << "]=" << m_dim_ids_k[i] << " ";
  }
  //std::cout << std::endl;

  //std::cout << "l_dim_ids_kb (blocked K dimensions): ";
  for (size_t i = 0; i < l_dim_ids_kb.size(); i++)
  {
    //std::cout << "[" << i << "]=" << l_dim_ids_kb[i] << " ";
  }
  //std::cout << std::endl;

  //std::cout << "l_dim_ids_cb (blocked C dimensions): ";
  for (size_t i = 0; i < l_dim_ids_cb.size(); i++)
  {
    //std::cout << "[" << i << "]=" << l_dim_ids_cb[i] << " ";
  }
  //std::cout << std::endl;

  //std::cout << "l_dim_ids_mb (blocked M dimensions): ";
  for (size_t i = 0; i < l_dim_ids_mb.size(); i++)
  {
    //std::cout << "[" << i << "]=" << l_dim_ids_mb[i] << " ";
  }
  //std::cout << std::endl;

  //std::cout << "l_dim_ids_nb (blocked N dimensions): ";
  for (size_t i = 0; i < l_dim_ids_nb.size(); i++)
  {
    //std::cout << "[" << i << "]=" << l_dim_ids_nb[i] << " ";
  }
  //std::cout << std::endl;
  //std::cout << "=== END DEBUG K Dimensions ===\n"
            << std::endl; */

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

  // DEBUG: Print stride calculations and dimension sizes
  /* //std::cout << "\n=== DEBUG Stride Calculation ===" << std::endl;
  // Print computed strides
  //std::cout << "\nComputed strides:" << std::endl;
  //std::cout << "l_strides_left: ";
  for (const auto &pair : l_strides_left)
  {
    //std::cout << "[" << pair.first << "]=" << pair.second << " ";
  }
  //std::cout << std::endl;

  //std::cout << "l_strides_right: ";
  for (const auto &pair : l_strides_right)
  {
    //std::cout << "[" << pair.first << "]=" << pair.second << " ";
  }
  //std::cout << std::endl;

  //std::cout << "l_strides_out: ";
  for (const auto &pair : l_strides_out)
  {
    //std::cout << "[" << pair.first << "]=" << pair.second << " ";
  }
  //std::cout << std::endl;

  // Print l_dim_sizes (copied from m_dim_sizes_inner)
  //std::cout << "\nDimension sizes (l_dim_sizes from m_dim_sizes_inner):" << std::endl;
  //std::cout << "l_dim_sizes: ";
  for (const auto &pair : l_dim_sizes)
  {
    //std::cout << "[" << pair.first << "]=" << pair.second << " ";
  }
  //std::cout << std::endl;

  //std::cout << "=== END DEBUG Stride Calculations ===\n"
            << std::endl; */
  uint64_t l_batch_reduce_size = 1;
  if (m_loop_ids_ext == nullptr)
  {
    // VOR compileLoopOrder: Non-blocked Dimensionen sammeln
    std::vector<int64_t> l_dim_ids_k_non_blocked;
    std::vector<int64_t> l_dim_ids_c_non_blocked;
    std::vector<int64_t> l_dim_ids_m_non_blocked;
    std::vector<int64_t> l_dim_ids_n_non_blocked;

    // K-Dimensionen: alle minus die geblockte
    for (const auto &k_dim : m_dim_ids_k)
    {
      if (std::find(l_dim_ids_kb.begin(), l_dim_ids_kb.end(), k_dim) == l_dim_ids_kb.end())
      {
        l_dim_ids_k_non_blocked.push_back(k_dim);
      }
    }

    // C-Dimensionen: alle minus die geblockte
    for (const auto &c_dim : m_dim_ids_c)
    {
      if (std::find(l_dim_ids_cb.begin(), l_dim_ids_cb.end(), c_dim) == l_dim_ids_cb.end())
      {
        l_dim_ids_c_non_blocked.push_back(c_dim);
      }
    }

    // M-Dimensionen: alle minus die geblockte
    for (const auto &m_dim : m_dim_ids_m)
    {
      if (std::find(l_dim_ids_mb.begin(), l_dim_ids_mb.end(), m_dim) == l_dim_ids_mb.end())
      {
        l_dim_ids_m_non_blocked.push_back(m_dim);
      }
    }

    // N-Dimensionen: alle minus die geblockte
    for (const auto &n_dim : m_dim_ids_n)
    {
      if (std::find(l_dim_ids_nb.begin(), l_dim_ids_nb.end(), n_dim) == l_dim_ids_nb.end())
      {
        l_dim_ids_n_non_blocked.push_back(n_dim);
      }
    }

    // Stride-Maps für non-blocked Dimensionen
    std::map<int64_t, int64_t> l_strides_left_non_blocked;
    std::map<int64_t, int64_t> l_strides_right_non_blocked;

    // Alle non-blocked Dimensionen sammeln
    std::vector<int64_t> all_non_blocked_dims;
    all_non_blocked_dims.insert(all_non_blocked_dims.end(), l_dim_ids_k_non_blocked.begin(), l_dim_ids_k_non_blocked.end());
    all_non_blocked_dims.insert(all_non_blocked_dims.end(), l_dim_ids_c_non_blocked.begin(), l_dim_ids_c_non_blocked.end());
    all_non_blocked_dims.insert(all_non_blocked_dims.end(), l_dim_ids_m_non_blocked.begin(), l_dim_ids_m_non_blocked.end());
    all_non_blocked_dims.insert(all_non_blocked_dims.end(), l_dim_ids_n_non_blocked.begin(), l_dim_ids_n_non_blocked.end());

    // Strides extrahieren für non-blocked Dimensionen
    for (const auto &dim_id : all_non_blocked_dims)
    {
      auto left_stride_it = l_strides_left.find(dim_id);
      auto right_stride_it = l_strides_right.find(dim_id);

      if (left_stride_it != l_strides_left.end())
      {
        l_strides_left_non_blocked[dim_id] = left_stride_it->second;
      }
      if (right_stride_it != l_strides_right.end())
      {
        l_strides_right_non_blocked[dim_id] = right_stride_it->second;
      }
    }
    std::vector<int64_t> l_batch_reduce_k_dims;

    ////std::cout << "\n=== DEBUG Batch-Reduce K Detection ===" << std::endl;

    while (!l_strides_left_non_blocked.empty() && !l_strides_right_non_blocked.empty())
    {

      //Finde Dimension mit kleinstem Stride im linken Tensor
      auto min_left_it = std::min_element(l_strides_left_non_blocked.begin(),
                                          l_strides_left_non_blocked.end(),
                                          [](const auto &a, const auto &b)
                                          {
                                            return a.second < b.second;
                                          });

      //Finde Dimension mit kleinstem Stride im rechten Tensor
      auto min_right_it = std::min_element(l_strides_right_non_blocked.begin(),
                                           l_strides_right_non_blocked.end(),
                                           [](const auto &a, const auto &b)
                                           {
                                             return a.second < b.second;
                                           });

      int64_t min_left_dim = min_left_it->first;
      int64_t min_right_dim = min_right_it->first;

/*       //std::cout << "Min stride left: dim_id=" << min_left_dim << " stride=" << min_left_it->second << std::endl;
      //std::cout << "Min stride right: dim_id=" << min_right_dim << " stride=" << min_right_it->second << std::endl;
 */
      // Prüfe ob es die gleiche dim id ist
      if (min_left_dim != min_right_dim)
      {
        //std::cout << "Different dimensions with min strides - stopping batch detection" << std::endl;
        break;
      }

      // Schritt 3d: Prüfe ob es eine K-Dimension ist
      bool is_k_dimension = std::find(l_dim_ids_k_non_blocked.begin(),
                                      l_dim_ids_k_non_blocked.end(),
                                      min_left_dim) != l_dim_ids_k_non_blocked.end();

      if (!is_k_dimension)
      {
        ////std::cout << "Dimension " << min_left_dim << " is not a K-dimension - stopping batch detection" << std::endl;
        break;
      }

      // Zur Batch-Liste hinzufügen
      l_batch_reduce_k_dims.push_back(min_left_dim);
      l_batch_reduce_size *= l_dim_sizes.at(min_left_dim);

/*       //std::cout << "Found batch-reduce K dimension: " << min_left_dim
                << " (size=" << l_dim_sizes.at(min_left_dim) << ")" << std::endl;
 */
      //Dimension aus beiden Stride-Maps entfernen
      l_strides_left_non_blocked.erase(min_left_it);
      l_strides_right_non_blocked.erase(min_right_it);

      //aus der non-blocked K-Liste entfernen
      auto k_it = std::find(l_dim_ids_k_non_blocked.begin(),
                            l_dim_ids_k_non_blocked.end(),
                            min_left_dim);
      if (k_it != l_dim_ids_k_non_blocked.end())
      {
        l_dim_ids_k_non_blocked.erase(k_it);
      }
    }

/*     //std::cout << "Total batch-reduce K dimensions found: " << l_batch_reduce_k_dims.size() << std::endl;
    //std::cout << "Total batch size: " << l_batch_reduce_size << std::endl;
    //std::cout << "=== END DEBUG Batch-Reduce K Detection ===\n"
              << std::endl; */

    for (const auto &batch_dim : l_batch_reduce_k_dims)
    {
      l_dim_ids_kb.push_back(batch_dim);
      ////std::cout << "[DEBUG] Temporarily blocking batch-reduce K dim " << batch_dim << std::endl;
    }
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

    // NACH compileLoopOrder: Batch-reduce K-Dimensionen wieder aus blocked-Liste entfernen
    ////std::cout << "\n[DEBUG] Removing batch-reduce K dimensions from blocked list after compileLoopOrder" << std::endl;
    for (const auto &batch_dim : l_batch_reduce_k_dims)
    {
      auto it = std::find(l_dim_ids_kb.begin(), l_dim_ids_kb.end(), batch_dim);
      if (it != l_dim_ids_kb.end())
      {
        l_dim_ids_kb.erase(it);
        ////std::cout << "[DEBUG] Removed batch-reduce K dim " << batch_dim << " from blocked list" << std::endl;
      }
    }

    // DEBUG: Print m_loop_ids_int after compileLoopOrder
    ////std::cout << "\n=== DEBUG compileLoopOrder Results ===" << std::endl;
    ////std::cout << "m_loop_ids_int content (" << m_loop_ids_int.size() << " elements): ";
    for (size_t i = 0; i < m_loop_ids_int.size(); i++)
    {
      ////std::cout << "[" << i << "]=" << m_loop_ids_int[i] << " ";
    }
    ////std::cout << std::endl;

    // Print dimension types for each loop ID
    ////std::cout << "Dimension types for each loop ID:" << std::endl;
    for (size_t i = 0; i < m_loop_ids_int.size(); i++)
    {
      int64_t dim_id = m_loop_ids_int[i];
      auto type_it = m_dim_types.find(dim_id);
      if (type_it != m_dim_types.end())
      {
        //std::cout << "  dim_id[" << dim_id << "] -> type=" << static_cast<int>(type_it->second);
        // Print human-readable type names
        switch (type_it->second)
        {
        case dim_t::C:
          //std::cout << " (C/batch)";
          break;
        case dim_t::M:
          //std::cout << " (M)";
          break;
        case dim_t::N:
          //std::cout << " (N)";
          break;
        case dim_t::K:
          //std::cout << " (K)";
          break;
        default:
          //std::cout << " (unknown)";
          break;
        }
        //std::cout << std::endl;
      }
      else
      {
        //std::cout << "  dim_id[" << dim_id << "] -> type=NOT_FOUND" << std::endl;
      }
    }

    // Print dimension sizes for each loop ID
    //std::cout << "Dimension sizes for each loop ID:" << std::endl;
    for (size_t i = 0; i < m_loop_ids_int.size(); i++)
    {
      int64_t dim_id = m_loop_ids_int[i];
      auto size_it = l_dim_sizes.find(dim_id);
      if (size_it != l_dim_sizes.end())
      {
        //std::cout << "  dim_id[" << dim_id << "] -> size=" << size_it->second << std::endl;
      }
      else
      {
        //std::cout << "  dim_id[" << dim_id << "] -> size=NOT_FOUND" << std::endl;
      }
    }

    //std::cout << "=== END DEBUG compileLoopOrder Results ===\n"<< std::endl;
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
  libxsmm_blasint l_r = 1; // daniel r = reduce Todo besser machen

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
  l_ldb = l_dim_ids_nb.size() > 0 ? l_strides_right.at(l_dim_ids_nb.back()) : l_k * l_r;
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
      //std::cout << "[DEBUG] m_xmm_kernel_first_touch_unary is nullptr" << std::endl;
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
  // l_flags_brgemm |= LIBXSMM_GEMM_FLAG_BETA_0;

  if (m_vnni_a)
  {
    l_flags_brgemm |= LIBXSMM_GEMM_FLAG_VNNI_A;
  }

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

  if (l_batch_reduce_size > 1)
  {
    //std::cout << "[DEBUG] Configuring BRGEMM with batch_reduce_size = " << l_batch_reduce_size << std::endl;
    l_brconfig.br_type = LIBXSMM_GEMM_BATCH_REDUCE_STRIDE;
    // Set stride hints for batch reduction
    l_brconfig.br_stride_a_hint = l_lda * l_k * ce_n_bytes(m_dtype_left);
    l_brconfig.br_stride_b_hint = l_ldb * l_n * ce_n_bytes(m_dtype_right);
    l_brconfig.br_unroll_hint = 0;
  }
  else
  {
    //std::cout << "[DEBUG] Using standard GEMM (no batch-reduce)" << std::endl;
    l_brconfig.br_type = LIBXSMM_GEMM_BATCH_REDUCE_NONE;
    l_brconfig.br_stride_a_hint = 0;
    l_brconfig.br_stride_b_hint = 0;
    l_brconfig.br_unroll_hint = 0;
  }

  if (m_num_dims_out > 0 && m_dim_types_out[m_num_dims_out - 1] != dim_t::C)
  {
    m_xmm_kernel_main.gemm = libxsmm_dispatch_brgemm(l_shape_brgemm,
                                                     l_flags_brgemm,
                                                     l_prefetch_flags_brgemm,
                                                     l_brconfig);
    // daniel : check if kernel is nullptr
/*     if (m_xmm_kernel_main.gemm == NULL)
    {
      std::cerr << "[ERROR] BRGEMM dispatch failed!" << std::endl;
    }
    else
    {
      //std::cout << "[OK] BRGEMM kernel erfolgreich generiert." << std::endl;
    } */
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
  //std::cout << "[DEBUG] Main GEMM ptr: "
  //         << (void *)m_xmm_kernel_main.gemm << std::endl;
  //std::cout << "[DEBUG] GEMM shape: m=" << l_shape_brgemm.m
  //          << " n=" << l_shape_brgemm.n
  //          << " k=" << l_shape_brgemm.k
  //          << " lda=" << l_shape_brgemm.lda
  //          << " ldb=" << l_shape_brgemm.ldb
  //          << " ldc=" << l_shape_brgemm.ldc
  //          << " br_stride_a_hint=" << l_brconfig.br_stride_a_hint
  //          << " br_stride_b_hint=" << l_brconfig.br_stride_b_hint
  //          << " l_batch_reduce_size=" << l_batch_reduce_size
  //          << std::endl;
  //std::cout << "[DEBUG] dim_types_out: ";
  for (auto t : m_dim_types_out)
    //std::cout << int(t) << " ";
  //std::cout << std::endl;
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
                    m_packing,
                    l_batch_reduce_size);

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
