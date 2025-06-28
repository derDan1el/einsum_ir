#include "BinaryPrimitives.h"
#include "BinaryContraction.h"

#include <algorithm>
#include <math.h>
#include <iostream>

void einsum_ir::backend::BinaryPrimitives::init(int64_t i_size_cb_min,
                                                int64_t i_size_cb_max,
                                                int64_t i_size_mb_min,
                                                int64_t i_size_mb_max,
                                                int64_t i_size_nb_min,
                                                int64_t i_size_nb_max,
                                                int64_t i_size_kb_min,
                                                int64_t i_size_kb_max)
{
  m_size_cb_min = i_size_cb_min;
  m_size_cb_max = i_size_cb_max;
  m_size_mb_min = i_size_mb_min;
  m_size_mb_max = i_size_mb_max;
  m_size_nb_min = i_size_nb_min;
  m_size_nb_max = i_size_nb_max;
  m_size_kb_min = i_size_kb_min;
  m_size_kb_max = i_size_kb_max;

  // Debug output: Display all member variables set in BinaryPrimitives init function
  std::cout << "\n=== BinaryPrimitives.init function ===" << std::endl;
  std::cout << "m_size_cb_min:             " << m_size_cb_min << std::endl;
  std::cout << "m_size_cb_max:             " << m_size_cb_max << std::endl;
  std::cout << "m_size_mb_min:             " << m_size_mb_min << std::endl;
  std::cout << "m_size_mb_max:             " << m_size_mb_max << std::endl;
  std::cout << "m_size_nb_min:             " << m_size_nb_min << std::endl;
  std::cout << "m_size_nb_max:             " << m_size_nb_max << std::endl;
  std::cout << "m_size_kb_min:             " << m_size_kb_min << std::endl;
  std::cout << "m_size_kb_max:             " << m_size_kb_max << std::endl;
  std::cout << "==========================================\n"
            << std::endl;
}

einsum_ir::err_t einsum_ir::backend::BinaryPrimitives::init(data_t i_data_type,
                                                            backend_t i_backend_type)
{

  if (i_backend_type == backend_t::TPP)
  {
    if (i_data_type == data_t::FP32)
    {
      init(4, 16,    // daniel: cb min, cb max
           32, 128,  // daniel: mb min, mb max
           12, 64,   // daniel: nb min, nb max
           32, 512); // daniel: kb min, kb max
    }
    else if (i_data_type == data_t::FP64)
    {
      init(2, 8,
           16, 64,
           6, 32,
           16, 256);
    }
    else if (i_data_type == data_t::BF16)
    {
      init(4, 16,     // daniel: cb min, cb max
           32, 128,   // daniel: mb min, mb max
           12, 64,    // daniel: nb min, nb max
           32, 1024); // daniel: kb min, kb max
    }
    else
    {
      return err_t::INVALID_DTYPE;
    }
  }
  // BLAS
  else if (i_backend_type == backend_t::BLAS)
  {
    if (i_data_type == data_t::FP32)
    {
      init(4, 16,
           32, 512,
           32, 512,
           32, 512);
    }
    else if (i_data_type == data_t::FP64)
    {
      init(2, 8,
           16, 256,
           16, 256,
           16, 256);
    }
    else if (i_data_type == data_t::BF16)
    {
      init(4, 16,
           32, 512,
           32, 512,
           32, 1024);
    }
    else
    {
      return err_t::INVALID_DTYPE;
    }
  }
  // TBLIS
  else if (i_backend_type == backend_t::TBLIS)
  {
    if (i_data_type == data_t::FP32)
    {
      init(4, 16,
           32, 512,
           32, 512,
           32, 512);
    }
    else if (i_data_type == data_t::FP64)
    {
      init(2, 8,
           16, 256,
           16, 256,
           16, 256);
    }
    else if (i_data_type == data_t::BF16)
    {
      init(8, 32,
           64, 1024,
           64, 1024,
           64, 1024);
    }
    else
    {
      return err_t::INVALID_DTYPE;
    }
  }
  else
  {
    return err_t::INVALID_BACKEND;
  }

  return err_t::SUCCESS;
}

bool einsum_ir::backend::BinaryPrimitives::swap_inputs(int64_t i_num_dims_left,
                                                       int64_t i_num_dims_right,
                                                       int64_t i_num_dims_out,
                                                       int64_t const *i_dim_ids_left,
                                                       int64_t const *i_dim_ids_right,
                                                       int64_t const *i_dim_ids_out)
{
  // determine dimension types of the tensors
  std::vector<einsum_ir::dim_t> l_dim_types_left;
  std::vector<einsum_ir::dim_t> l_dim_types_right;
  std::vector<einsum_ir::dim_t> l_dim_types_out;
  BinaryContraction::dim_types(i_num_dims_left,
                               i_num_dims_right,
                               i_num_dims_out,
                               i_dim_ids_left,
                               i_dim_ids_right,
                               i_dim_ids_out,
                               &l_dim_types_left,
                               &l_dim_types_right,
                               &l_dim_types_out);

  bool l_swap_inputs = false;

  // fastest output dimension is N: swap required
  if (l_dim_types_out.size() > 0 && l_dim_types_out.back() == N)
  {
    l_swap_inputs = true;
  }
  // fastest output dimension is C: swap depends on first non-C dimension
  else if (l_dim_types_out.size() > 0 && l_dim_types_out.back() == C)
  {
    int64_t l_id_out = l_dim_types_out.size() - 1;

    while (l_id_out >= 0)
    {
      // no swap if first non-C dim is M
      if (l_dim_types_out[l_id_out] == M)
      {
        break;
      }
      // swap if first non-C is N
      else if (l_dim_types_out[l_id_out] == N)
      {
        l_swap_inputs = true;
        break;
      }
      l_id_out--;
    }
  }

  return l_swap_inputs;
}

einsum_ir::err_t einsum_ir::backend::BinaryPrimitives::blocking_left_kb_x_mb_cb_right_nb_x_kb_cb_out_nb_x_mb_cb(int64_t i_size_mb_min,
                                                                                                                int64_t i_size_mb_max,
                                                                                                                int64_t i_size_nb_min,
                                                                                                                int64_t i_size_nb_max,
                                                                                                                int64_t i_size_kb_min,
                                                                                                                int64_t i_size_kb_max,
                                                                                                                int64_t i_num_dims_left,
                                                                                                                int64_t i_num_dims_right,
                                                                                                                int64_t i_num_dims_out,
                                                                                                                int64_t const *i_dim_ids_left,
                                                                                                                int64_t const *i_dim_ids_right,
                                                                                                                int64_t const *i_dim_ids_out,
                                                                                                                std::map<int64_t, int64_t> const *i_dim_sizes,
                                                                                                                std::map<int64_t, int64_t> const *i_strides_left,
                                                                                                                std::map<int64_t, int64_t> const *i_strides_right,
                                                                                                                std::map<int64_t, int64_t> const *i_strides_out,
                                                                                                                std::vector<int64_t> *o_dim_ids_cb,
                                                                                                                std::vector<int64_t> *o_dim_ids_mb,
                                                                                                                std::vector<int64_t> *o_dim_ids_nb,
                                                                                                                std::vector<int64_t> *o_dim_ids_kb)
{
  int64_t l_size_cb = 1;
  int64_t l_size_mb = 1;
  int64_t l_size_nb = 1;
  int64_t l_size_kb = 1;

  // init output
  o_dim_ids_cb->resize(0);
  o_dim_ids_mb->resize(0);
  o_dim_ids_nb->resize(0);
  o_dim_ids_kb->resize(0);

  // determine dimension types of the tensors
  std::vector<einsum_ir::dim_t> l_dim_types_left;
  std::vector<einsum_ir::dim_t> l_dim_types_right;
  std::vector<einsum_ir::dim_t> l_dim_types_out;
  BinaryContraction::dim_types(i_num_dims_left,
                               i_num_dims_right,
                               i_num_dims_out,
                               i_dim_ids_left,
                               i_dim_ids_right,
                               i_dim_ids_out,
                               &l_dim_types_left,
                               &l_dim_types_right,
                               &l_dim_types_out);

  // Vector l_dim_types_left hat 3 Einträge:003 |Vector l_dim_types_right hat 4 Einträge:0320Vector l_dim_types_out hat 3 Einträge:002
  // C = 0, // left, right, out M = 1, // left, out N = 2, // right, out K = 3, // left, right I = 4, // left J = 5, // right

  int64_t l_di_left = i_num_dims_left - 1;
  int64_t l_di_right = i_num_dims_right - 1;
  int64_t l_di_out = i_num_dims_out - 1;

  // stride for contiguous storage
  int64_t l_stride_cont_left = 1;
  int64_t l_stride_cont_right = 1;
  int64_t l_stride_cont_out = 1;

  // determine C blocking
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::C)
    {
      int64_t l_dim_id_left = i_dim_ids_left[l_di_left];
      int64_t l_dim_id_right = i_dim_ids_right[l_di_right];
      int64_t l_dim_id_out = i_dim_ids_out[l_di_out];

      int64_t l_dim_size = i_dim_sizes->at(l_dim_id_out);

      // determine if strides indicate contiguous storage
      int64_t l_stride_left = i_strides_left->at(l_dim_id_left);
      int64_t l_stride_right = i_strides_right->at(l_dim_id_right);
      int64_t l_stride_out = i_strides_out->at(l_dim_id_out);

      bool l_cont = l_stride_left == l_stride_cont_left && l_stride_right == l_stride_cont_right && l_stride_out == l_stride_cont_out;

      if (!l_cont)
      {
        return err_t::SUCCESS;
      }

      // block dimensions
      if (l_dim_id_out == l_dim_id_left && l_dim_id_out == l_dim_id_right)
      {
        o_dim_ids_cb->push_back(l_dim_id_out);

        l_size_cb *= l_dim_size;
        l_stride_cont_left *= l_dim_size;
        l_stride_cont_right *= l_dim_size;
        l_stride_cont_out *= l_dim_size;

        l_di_out--;
        l_di_left--;
        l_di_right--;
      }
      else
      {
        return err_t::SUCCESS;
      }
    }
    else
    {
      break;
    }
  }

  // determine M blocking
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::M)
    {
      int64_t l_dim_id_left = i_dim_ids_left[l_di_left];
      int64_t l_dim_id_out = i_dim_ids_out[l_di_out];

      int64_t l_dim_size = i_dim_sizes->at(l_dim_id_out);

      // determine if strides indicate contiguous storage
      int64_t l_stride_left = i_strides_left->at(l_dim_id_left);
      int64_t l_stride_out = i_strides_out->at(l_dim_id_out);

      bool l_cont = l_stride_left == l_stride_cont_left && l_stride_out == l_stride_cont_out;

      if (!l_cont)
      {
        break;
      }

      // block dimensions
      if (l_dim_id_out == l_dim_id_left && (l_size_mb < i_size_mb_min || l_size_mb * l_dim_size <= i_size_mb_max))
      {
        o_dim_ids_mb->push_back(l_dim_id_out);
        l_size_mb *= l_dim_size;
        l_stride_cont_left *= l_dim_size;
        l_stride_cont_out *= l_dim_size;

        l_di_out--;
        l_di_left--;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }

  // find id of potential K dimension
  int64_t l_potential_K = 0;
  if (l_di_right >= 0)
  {
    l_potential_K = i_dim_ids_right[l_di_right];
  }

  // seek the first K dimension that matches the potential K dimension in left tensor
  while (l_di_left >= 0)
  {
    int64_t l_idss = i_dim_ids_left[l_di_left];
    int64_t l_dim_size = i_dim_sizes->at(l_idss);

    if (l_dim_types_left[l_di_left] == einsum_ir::dim_t::K &&
        i_dim_ids_left[l_di_left] == l_potential_K)
    {
      break;
    }
    else
    {
      l_stride_cont_left *= l_dim_size;
      l_di_left--;
    }
  }

  // reset stride for left tensor
  if (l_di_left >= 0)
  {
    l_stride_cont_left = i_strides_left->at(i_dim_ids_left[l_di_left]);
  }

  // determine K blocking
  while (l_di_left >= 0 && l_di_right >= 0)
  {
    if (l_dim_types_left[l_di_left] == einsum_ir::dim_t::K)
    {
      int64_t l_dim_id_left = i_dim_ids_left[l_di_left];
      int64_t l_dim_id_right = i_dim_ids_right[l_di_right];

      int64_t l_dim_size = i_dim_sizes->at(l_dim_id_left);

      // determine if strides indicate contiguous storage
      int64_t l_stride_left = i_strides_left->at(l_dim_id_left);
      int64_t l_stride_right = i_strides_right->at(l_dim_id_right);

      bool l_cont = l_stride_left == l_stride_cont_left && l_stride_right == l_stride_cont_right;

      if (!l_cont)
      {
        break;
      }

      // block dimensions
      if (l_dim_id_left == l_dim_id_right && (l_size_kb < i_size_kb_min || l_size_kb * l_dim_size <= i_size_kb_max))
      {
        o_dim_ids_kb->push_back(l_dim_id_left);
        l_size_kb *= l_dim_size;
        l_stride_cont_left *= l_dim_size;
        l_stride_cont_right *= l_dim_size;

        l_di_left--;
        l_di_right--;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }

  // seek first N dimension after M in output tensor
  while (l_di_out >= 0)
  {
    int64_t l_dim_size = i_dim_sizes->at(i_dim_ids_out[l_di_out]);

    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::N)
    {
      break;
    }
    else
    {
      l_stride_cont_out *= l_dim_size;
      l_di_out--;
    }
  }

  // find id of potential N dimension
  int64_t l_potential_N = 0;
  if (l_di_out >= 0)
  {
    l_potential_N = i_dim_ids_out[l_di_out];
  }

  // seek first N dimension that matches the potential N dimnesion in right tensor
  while (l_di_right >= 0)
  {
    int64_t l_dim_size = i_dim_sizes->at(i_dim_ids_right[l_di_right]);

    if (l_dim_types_right[l_di_right] == einsum_ir::dim_t::N &&
        i_dim_ids_right[l_di_right] == l_potential_N)
    {
      break;
    }
    else
    {
      l_stride_cont_right *= l_dim_size;
      l_di_right--;
    }
  }

  // reset stride for right and output tensors
  if (l_di_right >= 0)
  {
    l_stride_cont_right = i_strides_right->at(i_dim_ids_right[l_di_right]); // daniel : 4 weil um auf nächstes n zu kommen 4 k werte gesprungne werdne müssen
  }
  if (l_di_out >= 0)
  {
    l_stride_cont_out = i_strides_out->at(i_dim_ids_out[l_di_out]);
  }

  // determine N blocking
  while (l_di_out >= 0 && l_di_right >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::N)
    {
      int64_t l_dim_id_out = i_dim_ids_out[l_di_out];
      int64_t l_dim_id_right = i_dim_ids_right[l_di_right];

      int64_t l_dim_size = i_dim_sizes->at(l_dim_id_out);

      // determine if strides indicate contiguous storage
      int64_t l_stride_right = i_strides_right->at(l_dim_id_right);
      int64_t l_stride_out = i_strides_out->at(l_dim_id_out);

      bool l_cont = l_stride_right == l_stride_cont_right && l_stride_out == l_stride_cont_out;

      if (!l_cont)
      {
        break;
      }

      // block dimensions
      if (l_dim_id_out == l_dim_id_right && (l_size_nb < i_size_nb_min || l_size_nb * l_dim_size <= i_size_nb_max))
      {
        o_dim_ids_nb->push_back(l_dim_id_out);
        l_size_nb *= l_dim_size;
        l_stride_cont_right *= l_dim_size;
        l_stride_cont_out *= l_dim_size;

        l_di_out--;
        l_di_right--;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }

  // transpose dimensions to match row-major
  std::reverse(o_dim_ids_cb->begin(), o_dim_ids_cb->end());
  std::reverse(o_dim_ids_mb->begin(), o_dim_ids_mb->end());
  std::reverse(o_dim_ids_nb->begin(), o_dim_ids_nb->end());
  std::reverse(o_dim_ids_kb->begin(), o_dim_ids_kb->end());

  // DEBUG: Print all parameters and computed values for blocking_left_kb_x_mb_cb_right_nb_x_kb_cb_out_nb_x_mb_cb
  std::cout << "\n=== DEBUG blocking_left_kb_x_mb_cb_right_nb_x_kb_cb_out_nb_x_mb_cb ===\n";

  // Print computed sizes
  std::cout << "Computed sizes:\n";
  std::cout << "  l_size_cb=" << l_size_cb << ", l_size_mb=" << l_size_mb << "\n";
  std::cout << "  l_size_nb=" << l_size_nb << ", l_size_kb=" << l_size_kb << "\n";

  // Print output vectors
  std::cout << "Output vectors:\n";
  std::cout << "  o_dim_ids_cb=[";
  for (size_t i = 0; i < o_dim_ids_cb->size(); ++i)
  {
    std::cout << (*o_dim_ids_cb)[i] << (i < o_dim_ids_cb->size() - 1 ? "," : "");
  }
  std::cout << "] (size=" << o_dim_ids_cb->size() << ")\n";
  std::cout << "  o_dim_ids_mb=[";
  for (size_t i = 0; i < o_dim_ids_mb->size(); ++i)
  {
    std::cout << (*o_dim_ids_mb)[i] << (i < o_dim_ids_mb->size() - 1 ? "," : "");
  }
  std::cout << "] (size=" << o_dim_ids_mb->size() << ")\n";
  std::cout << "  o_dim_ids_nb=[";
  for (size_t i = 0; i < o_dim_ids_nb->size(); ++i)
  {
    std::cout << (*o_dim_ids_nb)[i] << (i < o_dim_ids_nb->size() - 1 ? "," : "");
  }
  std::cout << "] (size=" << o_dim_ids_nb->size() << ")\n";
  std::cout << "  o_dim_ids_kb=[";
  for (size_t i = 0; i < o_dim_ids_kb->size(); ++i)
  {
    std::cout << (*o_dim_ids_kb)[i] << (i < o_dim_ids_kb->size() - 1 ? "," : "");
  }
  std::cout << "] (size=" << o_dim_ids_kb->size() << ")\n";
  std::cout << "=== END DEBUG blocking_left_kb_x_mb_cb_right_nb_x_kb_cb_out_nb_x_mb_cb ===\n\n";

  return err_t::SUCCESS;
}

einsum_ir::err_t einsum_ir::backend::BinaryPrimitives::blocking_left_x_cb_kb_mb_right_x_cb_nb_kb_out_nb_x_mb_cb(int64_t i_size_mb_min,
                                                                                                                int64_t i_size_mb_max,
                                                                                                                int64_t i_size_nb_min,
                                                                                                                int64_t i_size_nb_max,
                                                                                                                int64_t i_size_kb_min,
                                                                                                                int64_t i_size_kb_max,
                                                                                                                int64_t i_num_dims_left,
                                                                                                                int64_t i_num_dims_right,
                                                                                                                int64_t i_num_dims_out,
                                                                                                                int64_t const *i_dim_ids_left,
                                                                                                                int64_t const *i_dim_ids_right,
                                                                                                                int64_t const *i_dim_ids_out,
                                                                                                                std::map<int64_t, int64_t> const *i_dim_sizes,
                                                                                                                std::map<int64_t, int64_t> const *i_strides_left,
                                                                                                                std::map<int64_t, int64_t> const *i_strides_right,
                                                                                                                std::map<int64_t, int64_t> const *i_strides_out,
                                                                                                                std::vector<int64_t> *o_dim_ids_cb,
                                                                                                                std::vector<int64_t> *o_dim_ids_mb,
                                                                                                                std::vector<int64_t> *o_dim_ids_nb,
                                                                                                                std::vector<int64_t> *o_dim_ids_kb)
{
  int64_t l_size_cb = 1;
  int64_t l_size_mb = 1;
  int64_t l_size_nb = 1;
  int64_t l_size_kb = 1;

  // init output
  o_dim_ids_cb->resize(0);
  o_dim_ids_mb->resize(0);
  o_dim_ids_nb->resize(0);
  o_dim_ids_kb->resize(0);

  // determine dimension types of the tensors
  std::vector<einsum_ir::dim_t> l_dim_types_left;
  std::vector<einsum_ir::dim_t> l_dim_types_right;
  std::vector<einsum_ir::dim_t> l_dim_types_out;
  BinaryContraction::dim_types(i_num_dims_left,
                               i_num_dims_right,
                               i_num_dims_out,
                               i_dim_ids_left,
                               i_dim_ids_right,
                               i_dim_ids_out,
                               &l_dim_types_left,
                               &l_dim_types_right,
                               &l_dim_types_out);

  int64_t l_di_left = i_num_dims_left - 1;
  int64_t l_di_right = i_num_dims_right - 1;
  int64_t l_di_out = i_num_dims_out - 1;

  // stride for contiguous storage
  int64_t l_stride_cont_left = 1;
  int64_t l_stride_cont_right = 1;
  int64_t l_stride_cont_out = 1;

  // jump over M dimensions in left tensor
  while (l_di_left >= 0)
  {
    if (l_dim_types_left[l_di_left] != einsum_ir::dim_t::M)
    {
      break;
    }
    else
    {
      l_stride_cont_left *= i_dim_sizes->at(i_dim_ids_left[l_di_left]);
      l_di_left--;
    }
  }

  // jump over K dimensions in left tensor
  while (l_di_left >= 0)
  {
    if (l_dim_types_left[l_di_left] != einsum_ir::dim_t::K)
    {
      break;
    }
    else
    {
      l_stride_cont_left *= i_dim_sizes->at(i_dim_ids_left[l_di_left]);
      l_di_left--;
    }
  }

  // jump over K dimensions in right tensor
  while (l_di_right >= 0)
  {
    if (l_dim_types_right[l_di_right] != einsum_ir::dim_t::K)
    {
      break;
    }
    else
    {
      l_stride_cont_right *= i_dim_sizes->at(i_dim_ids_right[l_di_right]);
      l_di_right--;
    }
  }

  // jump over N dimensions in right tensor
  while (l_di_right >= 0)
  {
    if (l_dim_types_right[l_di_right] != einsum_ir::dim_t::N)
    {
      break;
    }
    else
    {
      l_stride_cont_right *= i_dim_sizes->at(i_dim_ids_right[l_di_right]);
      l_di_right--;
    }
  }

  // determine C blocking
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::C)
    {
      int64_t l_dim_id_left = i_dim_ids_left[l_di_left];
      int64_t l_dim_id_right = i_dim_ids_right[l_di_right];
      int64_t l_dim_id_out = i_dim_ids_out[l_di_out];

      int64_t l_dim_size = i_dim_sizes->at(l_dim_id_out);

      // determine if strides indicate contiguous storage
      int64_t l_stride_left = i_strides_left->at(l_dim_id_left);
      int64_t l_stride_right = i_strides_right->at(l_dim_id_right);
      int64_t l_stride_out = i_strides_out->at(l_dim_id_out);

      bool l_cont = l_stride_left == l_stride_cont_left && l_stride_right == l_stride_cont_right && l_stride_out == l_stride_cont_out;

      if (!l_cont)
      {
        return err_t::SUCCESS;
      }

      // block dimensions
      if (l_dim_id_out == l_dim_id_left && l_dim_id_out == l_dim_id_right)
      {
        o_dim_ids_cb->push_back(l_dim_id_out);

        l_size_cb *= l_dim_size;
        l_stride_cont_left *= l_dim_size;
        l_stride_cont_right *= l_dim_size;
        l_stride_cont_out *= l_dim_size;

        l_di_out--;
        l_di_left--;
        l_di_right--;
      }
      else
      {
        return err_t::SUCCESS;
      }
    }
    else
    {
      break;
    }
  }

  // reset position and stride for left tensor
  l_di_left = i_num_dims_left - 1;
  l_stride_cont_left = 1;

  // reset posotion and stride for right tensor
  l_di_right = i_num_dims_right - 1;
  l_stride_cont_right = 1;

  // determine M blocking
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::M)
    {
      int64_t l_dim_id_left = i_dim_ids_left[l_di_left];
      int64_t l_dim_id_out = i_dim_ids_out[l_di_out];

      int64_t l_dim_size = i_dim_sizes->at(l_dim_id_out);

      // determine if strides indicate contiguous storage
      int64_t l_stride_left = i_strides_left->at(l_dim_id_left);
      int64_t l_stride_out = i_strides_out->at(l_dim_id_out);

      bool l_cont = l_stride_left == l_stride_cont_left && l_stride_out == l_stride_cont_out;

      if (!l_cont)
      {
        break;
      }

      // block dimensions
      if (l_dim_id_out == l_dim_id_left && (l_size_mb < i_size_mb_min || l_size_mb * l_dim_size <= i_size_mb_max))
      {
        o_dim_ids_mb->push_back(l_dim_id_out);

        l_size_mb *= l_dim_size;
        l_stride_cont_left *= l_dim_size;
        l_stride_cont_out *= l_dim_size;

        l_di_out--;
        l_di_left--;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }

  // determine K blocking
  while (l_di_left >= 0)
  {
    if (l_dim_types_left[l_di_left] == einsum_ir::dim_t::K)
    {
      int64_t l_dim_id_left = i_dim_ids_left[l_di_left];
      int64_t l_dim_id_right = i_dim_ids_right[l_di_right];

      int64_t l_dim_size = i_dim_sizes->at(l_dim_id_left);

      // determine if strides indicate contiguous storage
      int64_t l_stride_left = i_strides_left->at(l_dim_id_left);
      int64_t l_stride_right = i_strides_right->at(l_dim_id_right);

      bool l_cont = l_stride_left == l_stride_cont_left && l_stride_right == l_stride_cont_right;

      if (!l_cont)
      {
        break;
      }

      // block dimensions
      if (l_dim_id_left == l_dim_id_right && (l_size_kb < i_size_kb_min || l_size_kb * l_dim_size <= i_size_kb_max))
      {
        o_dim_ids_kb->push_back(l_dim_id_left);

        l_size_kb *= l_dim_size;
        l_stride_cont_left *= l_dim_size;
        l_stride_cont_right *= l_dim_size;

        l_di_left--;
        l_di_right--;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }

  // seek first N dimension after M in output tensor
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::N)
    {
      break;
    }
    else
    {
      l_di_out--;
    }
  }

  // reset stride for output tensor
  if (l_di_out >= 0)
  {
    l_stride_cont_out = i_strides_out->at(i_dim_ids_out[l_di_out]);
  }

  // determine N blocking
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::N)
    {
      int64_t l_dim_id_out = i_dim_ids_out[l_di_out];
      int64_t l_dim_id_right = i_dim_ids_right[l_di_right];

      int64_t l_dim_size = i_dim_sizes->at(l_dim_id_out);

      // determine if strides indicate contiguous storage
      int64_t l_stride_right = i_strides_right->at(l_dim_id_right);
      int64_t l_stride_out = i_strides_out->at(l_dim_id_out);

      bool l_cont = l_stride_right == l_stride_cont_right && l_stride_out == l_stride_cont_out;

      if (!l_cont)
      {
        break;
      }

      // block dimensions
      if (l_dim_id_out == l_dim_id_right && (l_size_nb < i_size_nb_min || l_size_nb * l_dim_size <= i_size_nb_max))
      {
        o_dim_ids_nb->push_back(l_dim_id_out);

        l_size_nb *= l_dim_size;
        l_stride_cont_right *= l_dim_size;
        l_stride_cont_out *= l_dim_size;

        l_di_out--;
        l_di_right--;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }

  // transpose dimensions to match row-major
  std::reverse(o_dim_ids_cb->begin(), o_dim_ids_cb->end());
  std::reverse(o_dim_ids_mb->begin(), o_dim_ids_mb->end());
  std::reverse(o_dim_ids_nb->begin(), o_dim_ids_nb->end());
  std::reverse(o_dim_ids_kb->begin(), o_dim_ids_kb->end());

  return err_t::SUCCESS;
}

einsum_ir::err_t einsum_ir::backend::BinaryPrimitives::blocking(primblo_t i_primitive_blocking,
                                                                int64_t i_num_dims_left,
                                                                int64_t i_num_dims_right,
                                                                int64_t i_num_dims_out,
                                                                int64_t const *i_dim_ids_left,
                                                                int64_t const *i_dim_ids_right,
                                                                int64_t const *i_dim_ids_out,
                                                                std::map<int64_t, int64_t> const *i_dim_sizes,
                                                                std::map<int64_t, int64_t> const *i_strides_left,
                                                                std::map<int64_t, int64_t> const *i_strides_right,
                                                                std::map<int64_t, int64_t> const *i_strides_out,
                                                                std::vector<int64_t> *o_dim_ids_cb,
                                                                std::vector<int64_t> *o_dim_ids_mb,
                                                                std::vector<int64_t> *o_dim_ids_nb,
                                                                std::vector<int64_t> *o_dim_ids_kb)
{
  err_t l_err = err_t::TENSOR_BLOCKING_FAILED;

  std::map<int64_t, int64_t> l_strides_left;
  std::map<int64_t, int64_t> l_strides_right;
  std::map<int64_t, int64_t> l_strides_out;

  // assume contiguous storage and determine strides if not provided
  if (i_strides_left == nullptr)
  {
    int64_t l_stride = 1;
    for (int64_t l_di = i_num_dims_left - 1; l_di >= 0; l_di--)
    {
      int64_t l_id_left = i_dim_ids_left[l_di];
      l_strides_left[l_id_left] = l_stride;
      l_stride *= i_dim_sizes->at(l_id_left);
    }
  }

  if (i_strides_right == nullptr)
  {
    int64_t l_stride = 1;

    for (int64_t l_di = i_num_dims_right - 1; l_di >= 0; l_di--)
    { // daniel: [0] =64, [1] =1, [2] =16, [3] =8
      int64_t l_id_right = i_dim_ids_right[l_di];
      l_strides_right[l_id_right] = l_stride;
      l_stride *= i_dim_sizes->at(l_id_right);
    }
  }

  if (i_strides_out == nullptr)
  {
    int64_t l_stride = 1;

    for (int64_t l_di = i_num_dims_out - 1; l_di >= 0; l_di--)
    {
      int64_t l_id_out = i_dim_ids_out[l_di];
      l_strides_out[l_id_out] = l_stride;
      l_stride *= i_dim_sizes->at(l_id_out);
    }
  } // daniel :: out : [4] = 2 dh die dimension mit der id 4 hat einen stride von 2

  std::map<int64_t, int64_t> const *l_strides_ptr_left = (i_strides_left != nullptr) ? i_strides_left : &l_strides_left;
  std::map<int64_t, int64_t> const *l_strides_ptr_right = (i_strides_right != nullptr) ? i_strides_right : &l_strides_right;
  std::map<int64_t, int64_t> const *l_strides_ptr_out = (i_strides_out != nullptr) ? i_strides_out : &l_strides_out;

  // Debug output: Display all variables set in blocking function
  std::cout << "\n=== BinaryPrimitives.blocking function ===" << std::endl;

  std::cout << "\nStride maps:" << std::endl;
  std::cout << "l_strides_left:            ";
  for (const auto &pair : l_strides_left)
  {
    std::cout << "[dim_id=" << pair.first << "]=" << pair.second << " ";
  }
  std::cout << std::endl;

  std::cout << "l_strides_right:           ";
  for (const auto &pair : l_strides_right)
  {
    std::cout << "[dim_id=" << pair.first << "]=" << pair.second << " ";
  }
  std::cout << std::endl;

  std::cout << "l_strides_out:             ";
  for (const auto &pair : l_strides_out)
  {
    std::cout << "[dim_id=" << pair.first << "]=" << pair.second << " ";
  }
  std::cout << std::endl;
  std::cout << "============================================\n"
            << std::endl;

  // derive blocking
  if (i_primitive_blocking == primblo_t::LEFT_KB_X_MB_CB_RIGHT_NB_X_KB_CB_OUT_NB_X_MB_CB)
  { // daniel : setzt blocking größen o_dim_ids_mb[0] = 3 ,o_dim_ids_nb[0] = 4,o_dim_ids_kb[0] = 2
    l_err = blocking_left_kb_x_mb_cb_right_nb_x_kb_cb_out_nb_x_mb_cb(m_size_mb_min,
                                                                     m_size_mb_max,
                                                                     m_size_nb_min,
                                                                     m_size_nb_max,
                                                                     m_size_kb_min,
                                                                     m_size_kb_max,
                                                                     i_num_dims_left,
                                                                     i_num_dims_right,
                                                                     i_num_dims_out,
                                                                     i_dim_ids_left,
                                                                     i_dim_ids_right,
                                                                     i_dim_ids_out,
                                                                     i_dim_sizes,
                                                                     l_strides_ptr_left,
                                                                     l_strides_ptr_right,
                                                                     l_strides_ptr_out,
                                                                     o_dim_ids_cb,
                                                                     o_dim_ids_mb,
                                                                     o_dim_ids_nb,
                                                                     o_dim_ids_kb);
  }
  else if (i_primitive_blocking == primblo_t::LEFT_X_CB_KB_MB_RIGHT_X_CB_NB_KB_OUT_NB_X_MB_CB)
  {
    l_err = blocking_left_x_cb_kb_mb_right_x_cb_nb_kb_out_nb_x_mb_cb(m_size_mb_min,
                                                                     m_size_mb_max,
                                                                     m_size_nb_min,
                                                                     m_size_nb_max,
                                                                     m_size_kb_min,
                                                                     m_size_kb_max,
                                                                     i_num_dims_left,
                                                                     i_num_dims_right,
                                                                     i_num_dims_out,
                                                                     i_dim_ids_left,
                                                                     i_dim_ids_right,
                                                                     i_dim_ids_out,
                                                                     i_dim_sizes,
                                                                     l_strides_ptr_left,
                                                                     l_strides_ptr_right,
                                                                     l_strides_ptr_out,
                                                                     o_dim_ids_cb,
                                                                     o_dim_ids_mb,
                                                                     o_dim_ids_nb,
                                                                     o_dim_ids_kb);
  }

  return l_err;
}

einsum_ir::err_t einsum_ir::backend::BinaryPrimitives::reorder_left_bc_bm_bk_bi_kb_mb_cb_right_bc_bn_bk_bj_nb_kb_cb_out_native(int64_t i_size_cb_min,
                                                                                                                               int64_t i_size_cb_max,
                                                                                                                               int64_t i_size_mb_min,
                                                                                                                               int64_t i_size_mb_max,
                                                                                                                               int64_t i_size_nb_min,
                                                                                                                               int64_t i_size_nb_max,
                                                                                                                               int64_t i_size_kb_min,
                                                                                                                               int64_t i_size_kb_max,
                                                                                                                               int64_t i_num_dims_left,
                                                                                                                               int64_t i_num_dims_right,
                                                                                                                               int64_t i_num_dims_out,
                                                                                                                               std::map<int64_t, int64_t> const *i_dim_sizes,
                                                                                                                               int64_t *io_dim_ids_left,
                                                                                                                               int64_t *io_dim_ids_right,
                                                                                                                               int64_t *io_dim_ids_out)
{
  // determine dimension types of the tensors
  std::vector<einsum_ir::dim_t> l_dim_types_left;
  std::vector<einsum_ir::dim_t> l_dim_types_right;
  std::vector<einsum_ir::dim_t> l_dim_types_out;
  BinaryContraction::dim_types(i_num_dims_left,
                               i_num_dims_right,
                               i_num_dims_out,
                               io_dim_ids_left,
                               io_dim_ids_right,
                               io_dim_ids_out,
                               &l_dim_types_left,
                               &l_dim_types_right,
                               &l_dim_types_out);

  // blocked dimension ids
  std::vector<int64_t> l_dim_ids_cb;
  std::vector<int64_t> l_dim_ids_mb;
  std::vector<int64_t> l_dim_ids_nb;
  std::vector<int64_t> l_dim_ids_kb;

  // non-blocked dimension ids
  std::vector<int64_t> l_dim_ids_bc;
  std::vector<int64_t> l_dim_ids_bm;
  std::vector<int64_t> l_dim_ids_bn;
  std::vector<int64_t> l_dim_ids_bk;
  std::vector<int64_t> l_dim_ids_bi;
  std::vector<int64_t> l_dim_ids_bj;

  // derive blocked C dimensions
  int64_t l_di_out = i_num_dims_out - 1;
  int64_t l_block_dim_size = 1;
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::C)
    {
      int64_t l_dim_id = io_dim_ids_out[l_di_out];
      int64_t l_dim_size = i_dim_sizes->at(l_dim_id);

      if (l_block_dim_size < i_size_cb_min || l_block_dim_size * l_dim_size <= i_size_cb_max)
      {
        l_block_dim_size *= l_dim_size;
        l_dim_ids_cb.push_back(l_dim_id);
        l_di_out--;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }

  // derive blocked M dimensions
  l_block_dim_size = 1;
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::M)
    {
      int64_t l_dim_id = io_dim_ids_out[l_di_out];
      int64_t l_dim_size = i_dim_sizes->at(l_dim_id);

      if (l_block_dim_size < i_size_mb_min || l_block_dim_size * l_dim_size <= i_size_mb_max)
      {
        l_block_dim_size *= l_dim_size;
        l_dim_ids_mb.push_back(l_dim_id);
        l_di_out--;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }

  // seek the first N dimension after M
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::C)
    {
      l_dim_ids_bc.push_back(io_dim_ids_out[l_di_out]);
      l_di_out--;
    }
    else if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::M)
    {
      l_dim_ids_bm.push_back(io_dim_ids_out[l_di_out]);
      l_di_out--;
    }
    else if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::N)
    {
      break;
    }
    else
    {
      return einsum_ir::err_t::DIMENSION_ORDERING_FAILED;
    }
  }

  // derive blocked N dimensions
  l_block_dim_size = 1;
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::N)
    {
      int64_t l_dim_id = io_dim_ids_out[l_di_out];
      int64_t l_dim_size = i_dim_sizes->at(l_dim_id);

      if (l_block_dim_size < i_size_nb_min || l_block_dim_size * l_dim_size <= i_size_nb_max)
      {
        l_block_dim_size *= l_dim_size;
        l_dim_ids_nb.push_back(l_dim_id);
        l_di_out--;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }

  // assign remaining non-blocked dimensions of the output tensor
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::C)
    {
      l_dim_ids_bc.push_back(io_dim_ids_out[l_di_out]);
      l_di_out--;
    }
    else if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::M)
    {
      l_dim_ids_bm.push_back(io_dim_ids_out[l_di_out]);
      l_di_out--;
    }
    else if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::N)
    {
      l_dim_ids_bn.push_back(io_dim_ids_out[l_di_out]);
      l_di_out--;
    }
    else
    {
      return einsum_ir::err_t::DIMENSION_ORDERING_FAILED;
    }
  }

  // extract K dimension ids
  std::vector<int64_t> l_dim_ids_k;
  for (int64_t l_id = 0; l_id < i_num_dims_left; l_id++)
  {
    if (l_dim_types_left[l_id] == einsum_ir::dim_t::K)
    {
      l_dim_ids_k.push_back(io_dim_ids_left[l_id]);
    }
  }

  // sort K dimension ids by their size
  std::sort(l_dim_ids_k.begin(),
            l_dim_ids_k.end(),
            [i_dim_sizes](int64_t i_lhs, int64_t i_rhs)
            {
              return i_dim_sizes->at(i_lhs) >= i_dim_sizes->at(i_rhs);
            });

  // derive blocked K dimensions
  l_block_dim_size = 1;
  for (std::size_t l_di = 0; l_di < l_dim_ids_k.size(); l_di++)
  {
    int64_t l_id = l_dim_ids_k[l_di];
    int64_t l_dim_size = i_dim_sizes->at(l_id);

    if (l_block_dim_size < i_size_kb_min || l_block_dim_size * l_dim_size <= i_size_kb_max)
    {
      l_block_dim_size *= l_dim_size;
      l_dim_ids_kb.push_back(l_id);
    }
    else
    {
      break;
    }
  }

  // derive non-blocked K dimensions
  l_dim_ids_bk = std::vector<int64_t>(l_dim_ids_k.begin() + l_dim_ids_kb.size(),
                                      l_dim_ids_k.end());

  // extract I dimension ids
  for (int64_t l_id = 0; l_id < i_num_dims_left; l_id++)
  {
    if (l_dim_types_left[l_id] == einsum_ir::dim_t::I)
    {
      l_dim_ids_bi.push_back(io_dim_ids_left[l_id]);
    }
  }

  // sort I dimension ids by their size
  std::sort(l_dim_ids_bi.begin(),
            l_dim_ids_bi.end(),
            [i_dim_sizes](int64_t i_lhs, int64_t i_rhs)
            {
              return i_dim_sizes->at(i_lhs) <= i_dim_sizes->at(i_rhs);
            });

  // extract J dimension ids
  for (int64_t l_id = 0; l_id < i_num_dims_right; l_id++)
  {
    if (l_dim_types_right[l_id] == einsum_ir::dim_t::J)
    {
      l_dim_ids_bj.push_back(io_dim_ids_right[l_id]);
    }
  }

  // sort J dimension ids by their size
  std::sort(l_dim_ids_bj.begin(),
            l_dim_ids_bj.end(),
            [i_dim_sizes](int64_t i_lhs, int64_t i_rhs)
            {
              return i_dim_sizes->at(i_lhs) <= i_dim_sizes->at(i_rhs);
            });

  // perform the reordering of the input tensors' dimensions
  int64_t l_di_left = i_num_dims_left - 1;
  int64_t l_di_right = i_num_dims_right - 1;

  // assign blocked C dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_cb.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_cb[l_di];
    io_dim_ids_right[l_di_right] = l_dim_ids_cb[l_di];
    l_di_left--;
    l_di_right--;
  }

  // assign blocked M dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_mb.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_mb[l_di];
    l_di_left--;
  }

  // assign blocked K dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_kb.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_kb[l_di];
    io_dim_ids_right[l_di_right] = l_dim_ids_kb[l_di];
    l_di_left--;
    l_di_right--;
  }

  // assign blocked N dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_nb.size(); l_di++)
  {
    io_dim_ids_right[l_di_right] = l_dim_ids_nb[l_di];
    l_di_right--;
  }

  // assign non-blocked I dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bi.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_bi[l_di];
    l_di_left--;
  }

  // assign non-blocked J dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bj.size(); l_di++)
  {
    io_dim_ids_right[l_di_right] = l_dim_ids_bj[l_di];
    l_di_right--;
  }

  // asssign non-blocked K dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bk.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_bk[l_di];
    io_dim_ids_right[l_di_right] = l_dim_ids_bk[l_di];
    l_di_left--;
    l_di_right--;
  }

  // assign non-blocked M dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bm.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_bm[l_di];
    l_di_left--;
  }

  // assign non-blocked N dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bn.size(); l_di++)
  {
    io_dim_ids_right[l_di_right] = l_dim_ids_bn[l_di];
    l_di_right--;
  }

  // assign non-blocked C dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bc.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_bc[l_di];
    io_dim_ids_right[l_di_right] = l_dim_ids_bc[l_di];
    l_di_left--;
    l_di_right--;
  }

  return err_t::SUCCESS;
}

einsum_ir::err_t einsum_ir::backend::BinaryPrimitives::reorder_left_bc_bm_bk_bi_kb_mb_right_bc_bn_bk_bj_nb_kb_out_native(int64_t i_size_mb_min,
                                                                                                                         int64_t i_size_mb_max,
                                                                                                                         int64_t i_size_nb_min,
                                                                                                                         int64_t i_size_nb_max,
                                                                                                                         int64_t i_size_kb_min,
                                                                                                                         int64_t i_size_kb_max,
                                                                                                                         int64_t i_num_dims_left,
                                                                                                                         int64_t i_num_dims_right,
                                                                                                                         int64_t i_num_dims_out,
                                                                                                                         std::map<int64_t, int64_t> const *i_dim_sizes,
                                                                                                                         int64_t *io_dim_ids_left,
                                                                                                                         int64_t *io_dim_ids_right,
                                                                                                                         int64_t *io_dim_ids_out)
{
  // determine dimension types of the tensors
  std::vector<einsum_ir::dim_t> l_dim_types_left;
  std::vector<einsum_ir::dim_t> l_dim_types_right;
  std::vector<einsum_ir::dim_t> l_dim_types_out;
  BinaryContraction::dim_types(i_num_dims_left,
                               i_num_dims_right,
                               i_num_dims_out,
                               io_dim_ids_left,
                               io_dim_ids_right,
                               io_dim_ids_out,
                               &l_dim_types_left,
                               &l_dim_types_right,
                               &l_dim_types_out);

  // blocked dimension ids
  std::vector<int64_t> l_dim_ids_mb;
  std::vector<int64_t> l_dim_ids_nb;
  std::vector<int64_t> l_dim_ids_kb;

  // non-blocked dimension ids
  std::vector<int64_t> l_dim_ids_bc;
  std::vector<int64_t> l_dim_ids_bm;
  std::vector<int64_t> l_dim_ids_bn;
  std::vector<int64_t> l_dim_ids_bk;
  std::vector<int64_t> l_dim_ids_bi;
  std::vector<int64_t> l_dim_ids_bj;

  // derive blocked M dimensions
  int64_t l_di_out = i_num_dims_out - 1;
  int64_t l_block_dim_size = 1;
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::M)
    {
      int64_t l_dim_id = io_dim_ids_out[l_di_out];
      int64_t l_dim_size = i_dim_sizes->at(l_dim_id);

      if (l_block_dim_size < i_size_mb_min || l_block_dim_size * l_dim_size <= i_size_mb_max)
      {
        l_block_dim_size *= l_dim_size;
        l_dim_ids_mb.push_back(l_dim_id);
        l_di_out--;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }

  // seek the first N dimension after M
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::C)
    {
      l_dim_ids_bc.push_back(io_dim_ids_out[l_di_out]);
      l_di_out--;
    }
    else if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::M)
    {
      l_dim_ids_bm.push_back(io_dim_ids_out[l_di_out]);
      l_di_out--;
    }
    else if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::N)
    {
      break;
    }
    else
    {
      return einsum_ir::err_t::DIMENSION_ORDERING_FAILED;
    }
  }

  // derive blocked N dimensions
  l_block_dim_size = 1;
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::N)
    {
      int64_t l_dim_id = io_dim_ids_out[l_di_out];
      int64_t l_dim_size = i_dim_sizes->at(l_dim_id);

      if (l_block_dim_size < i_size_nb_min || l_block_dim_size * l_dim_size <= i_size_nb_max)
      {
        l_block_dim_size *= l_dim_size;
        l_dim_ids_nb.push_back(l_dim_id);
        l_di_out--;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }

  // assign remaining non-blocked dimensions of the output tensor
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::C)
    {
      l_dim_ids_bc.push_back(io_dim_ids_out[l_di_out]);
      l_di_out--;
    }
    else if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::M)
    {
      l_dim_ids_bm.push_back(io_dim_ids_out[l_di_out]);
      l_di_out--;
    }
    else if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::N)
    {
      l_dim_ids_bn.push_back(io_dim_ids_out[l_di_out]);
      l_di_out--;
    }
    else
    {
      return einsum_ir::err_t::DIMENSION_ORDERING_FAILED;
    }
  }

  // extract K dimension ids
  std::vector<int64_t> l_dim_ids_k;
  for (int64_t l_id = 0; l_id < i_num_dims_left; l_id++)
  {
    if (l_dim_types_left[l_id] == einsum_ir::dim_t::K)
    {
      l_dim_ids_k.push_back(io_dim_ids_left[l_id]);
    }
  }

  // sort K dimension ids by their size
  std::sort(l_dim_ids_k.begin(),
            l_dim_ids_k.end(),
            [i_dim_sizes](int64_t i_lhs, int64_t i_rhs)
            {
              return i_dim_sizes->at(i_lhs) >= i_dim_sizes->at(i_rhs);
            });

  // derive blocked K dimensions
  l_block_dim_size = 1;
  for (std::size_t l_di = 0; l_di < l_dim_ids_k.size(); l_di++)
  {
    int64_t l_id = l_dim_ids_k[l_di];
    int64_t l_dim_size = i_dim_sizes->at(l_id);

    if (l_block_dim_size < i_size_kb_min || l_block_dim_size * l_dim_size <= i_size_kb_max)
    {
      l_block_dim_size *= l_dim_size;
      l_dim_ids_kb.push_back(l_id);
    }
    else
    {
      break;
    }
  }

  // derive non-blocked K dimensions
  l_dim_ids_bk = std::vector<int64_t>(l_dim_ids_k.begin() + l_dim_ids_kb.size(),
                                      l_dim_ids_k.end());

  // extract I dimension ids
  for (int64_t l_id = 0; l_id < i_num_dims_left; l_id++)
  {
    if (l_dim_types_left[l_id] == einsum_ir::dim_t::I)
    {
      l_dim_ids_bi.push_back(io_dim_ids_left[l_id]);
    }
  }

  // sort I dimension ids by their size
  std::sort(l_dim_ids_bi.begin(),
            l_dim_ids_bi.end(),
            [i_dim_sizes](int64_t i_lhs, int64_t i_rhs)
            {
              return i_dim_sizes->at(i_lhs) <= i_dim_sizes->at(i_rhs);
            });

  // extract J dimension ids
  for (int64_t l_id = 0; l_id < i_num_dims_right; l_id++)
  {
    if (l_dim_types_right[l_id] == einsum_ir::dim_t::J)
    {
      l_dim_ids_bj.push_back(io_dim_ids_right[l_id]);
    }
  }

  // sort J dimension ids by their size
  std::sort(l_dim_ids_bj.begin(),
            l_dim_ids_bj.end(),
            [i_dim_sizes](int64_t i_lhs, int64_t i_rhs)
            {
              return i_dim_sizes->at(i_lhs) <= i_dim_sizes->at(i_rhs);
            });

  // perform the reordering of the input tensors' dimensions
  int64_t l_di_left = i_num_dims_left - 1;
  int64_t l_di_right = i_num_dims_right - 1;

  // assign blocked M dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_mb.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_mb[l_di];
    l_di_left--;
  }

  // assign blocked K dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_kb.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_kb[l_di];
    io_dim_ids_right[l_di_right] = l_dim_ids_kb[l_di];
    l_di_left--;
    l_di_right--;
  }

  // assign blocked N dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_nb.size(); l_di++)
  {
    io_dim_ids_right[l_di_right] = l_dim_ids_nb[l_di];
    l_di_right--;
  }

  // assign non-blocked I dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bi.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_bi[l_di];
    l_di_left--;
  }

  // assign non-blocked J dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bj.size(); l_di++)
  {
    io_dim_ids_right[l_di_right] = l_dim_ids_bj[l_di];
    l_di_right--;
  }

  // asssign non-blocked K dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bk.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_bk[l_di];
    io_dim_ids_right[l_di_right] = l_dim_ids_bk[l_di];
    l_di_left--;
    l_di_right--;
  }

  // assign non-blocked M dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bm.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_bm[l_di];
    l_di_left--;
  }

  // assign non-blocked N dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bn.size(); l_di++)
  {
    io_dim_ids_right[l_di_right] = l_dim_ids_bn[l_di];
    l_di_right--;
  }

  // assign non-blocked C dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bc.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_bc[l_di];
    io_dim_ids_right[l_di_right] = l_dim_ids_bc[l_di];
    l_di_left--;
    l_di_right--;
  }

  return err_t::SUCCESS;
}

einsum_ir::err_t einsum_ir::backend::BinaryPrimitives::reorder_left_bc_bm_bk_bi_cb_kb_mb_right_bc_bn_bk_bj_cb_nb_kb_out_native(int64_t i_size_mb_min,
                                                                                                                               int64_t i_size_mb_max,
                                                                                                                               int64_t i_size_nb_min,
                                                                                                                               int64_t i_size_nb_max,
                                                                                                                               int64_t i_size_kb_min,
                                                                                                                               int64_t i_size_kb_max,
                                                                                                                               int64_t i_num_dims_left,
                                                                                                                               int64_t i_num_dims_right,
                                                                                                                               int64_t i_num_dims_out,
                                                                                                                               std::map<int64_t, int64_t> const *i_dim_sizes,
                                                                                                                               int64_t *io_dim_ids_left,
                                                                                                                               int64_t *io_dim_ids_right,
                                                                                                                               int64_t *io_dim_ids_out)
{
  // determine dimension types of the tensors
  std::vector<einsum_ir::dim_t> l_dim_types_left;
  std::vector<einsum_ir::dim_t> l_dim_types_right;
  std::vector<einsum_ir::dim_t> l_dim_types_out;
  BinaryContraction::dim_types(i_num_dims_left,
                               i_num_dims_right,
                               i_num_dims_out,
                               io_dim_ids_left,
                               io_dim_ids_right,
                               io_dim_ids_out,
                               &l_dim_types_left,
                               &l_dim_types_right,
                               &l_dim_types_out);

  // blocked dimension ids
  std::vector<int64_t> l_dim_ids_cb;
  std::vector<int64_t> l_dim_ids_mb;
  std::vector<int64_t> l_dim_ids_nb;
  std::vector<int64_t> l_dim_ids_kb;

  // non-blocked dimension ids
  std::vector<int64_t> l_dim_ids_bc;
  std::vector<int64_t> l_dim_ids_bm;
  std::vector<int64_t> l_dim_ids_bn;
  std::vector<int64_t> l_dim_ids_bk;
  std::vector<int64_t> l_dim_ids_bi;
  std::vector<int64_t> l_dim_ids_bj;

  // derive blocked C dimensions
  int64_t l_di_out = i_num_dims_out - 1;
  int64_t l_block_dim_size = 1;
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::C)
    {
      int64_t l_dim_id = io_dim_ids_out[l_di_out];
      int64_t l_dim_size = i_dim_sizes->at(l_dim_id);

      l_block_dim_size *= l_dim_size;
      l_dim_ids_cb.push_back(l_dim_id);
      l_di_out--;
    }
    else
    {
      break;
    }
  }

  // derive blocked M dimensions
  l_block_dim_size = 1;
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::M)
    {
      int64_t l_dim_id = io_dim_ids_out[l_di_out];
      int64_t l_dim_size = i_dim_sizes->at(l_dim_id);

      if (l_block_dim_size < i_size_mb_min || l_block_dim_size * l_dim_size <= i_size_mb_max)
      {
        l_block_dim_size *= l_dim_size;
        l_dim_ids_mb.push_back(l_dim_id);
        l_di_out--;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }

  // seek the first N dimension after M
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::C)
    {
      l_dim_ids_bc.push_back(io_dim_ids_out[l_di_out]);
      l_di_out--;
    }
    else if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::M)
    {
      l_dim_ids_bm.push_back(io_dim_ids_out[l_di_out]);
      l_di_out--;
    }
    else if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::N)
    {
      break;
    }
    else
    {
      return einsum_ir::err_t::DIMENSION_ORDERING_FAILED;
    }
  }

  // derive blocked N dimensions
  l_block_dim_size = 1;
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::N)
    {
      int64_t l_dim_id = io_dim_ids_out[l_di_out];
      int64_t l_dim_size = i_dim_sizes->at(l_dim_id);

      if (l_block_dim_size < i_size_nb_min || l_block_dim_size * l_dim_size <= i_size_nb_max)
      {
        l_block_dim_size *= l_dim_size;
        l_dim_ids_nb.push_back(l_dim_id);
        l_di_out--;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }

  // assign remaining non-blocked dimensions of the output tensor
  while (l_di_out >= 0)
  {
    if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::C)
    {
      l_dim_ids_bc.push_back(io_dim_ids_out[l_di_out]);
      l_di_out--;
    }
    else if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::M)
    {
      l_dim_ids_bm.push_back(io_dim_ids_out[l_di_out]);
      l_di_out--;
    }
    else if (l_dim_types_out[l_di_out] == einsum_ir::dim_t::N)
    {
      l_dim_ids_bn.push_back(io_dim_ids_out[l_di_out]);
      l_di_out--;
    }
    else
    {
      return einsum_ir::err_t::DIMENSION_ORDERING_FAILED;
    }
  }

  // extract K dimension ids
  std::vector<int64_t> l_dim_ids_k;
  for (int64_t l_id = 0; l_id < i_num_dims_left; l_id++)
  {
    if (l_dim_types_left[l_id] == einsum_ir::dim_t::K)
    {
      l_dim_ids_k.push_back(io_dim_ids_left[l_id]);
    }
  }

  // sort K dimension ids by their size
  std::sort(l_dim_ids_k.begin(),
            l_dim_ids_k.end(),
            [i_dim_sizes](int64_t i_lhs, int64_t i_rhs)
            {
              return i_dim_sizes->at(i_lhs) >= i_dim_sizes->at(i_rhs);
            });

  // derive blocked K dimensions
  l_block_dim_size = 1;
  for (std::size_t l_di = 0; l_di < l_dim_ids_k.size(); l_di++)
  {
    int64_t l_id = l_dim_ids_k[l_di];
    int64_t l_dim_size = i_dim_sizes->at(l_id);

    if (l_block_dim_size < i_size_kb_min || l_block_dim_size * l_dim_size <= i_size_kb_max)
    {
      l_block_dim_size *= l_dim_size;
      l_dim_ids_kb.push_back(l_id);
    }
    else
    {
      break;
    }
  }

  // derive non-blocked K dimensions
  l_dim_ids_bk = std::vector<int64_t>(l_dim_ids_k.begin() + l_dim_ids_kb.size(),
                                      l_dim_ids_k.end());

  // extract I dimension ids
  for (int64_t l_id = 0; l_id < i_num_dims_left; l_id++)
  {
    if (l_dim_types_left[l_id] == einsum_ir::dim_t::I)
    {
      l_dim_ids_bi.push_back(io_dim_ids_left[l_id]);
    }
  }

  // sort I dimension ids by their size
  std::sort(l_dim_ids_bi.begin(),
            l_dim_ids_bi.end(),
            [i_dim_sizes](int64_t i_lhs, int64_t i_rhs)
            {
              return i_dim_sizes->at(i_lhs) <= i_dim_sizes->at(i_rhs);
            });

  // extract J dimension ids
  for (int64_t l_id = 0; l_id < i_num_dims_right; l_id++)
  {
    if (l_dim_types_right[l_id] == einsum_ir::dim_t::J)
    {
      l_dim_ids_bj.push_back(io_dim_ids_right[l_id]);
    }
  }

  // sort J dimension ids by their size
  std::sort(l_dim_ids_bj.begin(),
            l_dim_ids_bj.end(),
            [i_dim_sizes](int64_t i_lhs, int64_t i_rhs)
            {
              return i_dim_sizes->at(i_lhs) <= i_dim_sizes->at(i_rhs);
            });

  // perform the reordering of the input tensors' dimensions
  int64_t l_di_left = i_num_dims_left - 1;
  int64_t l_di_right = i_num_dims_right - 1;

  // assign blocked M dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_mb.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_mb[l_di];
    l_di_left--;
  }

  // assign blocked K dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_kb.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_kb[l_di];
    io_dim_ids_right[l_di_right] = l_dim_ids_kb[l_di];
    l_di_left--;
    l_di_right--;
  }

  // assign blocked N dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_nb.size(); l_di++)
  {
    io_dim_ids_right[l_di_right] = l_dim_ids_nb[l_di];
    l_di_right--;
  }

  // assign blocked C dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_cb.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_cb[l_di];
    io_dim_ids_right[l_di_right] = l_dim_ids_cb[l_di];
    l_di_left--;
    l_di_right--;
  }

  // assign non-blocked I dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bi.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_bi[l_di];
    l_di_left--;
  }

  // assign non-blocked J dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bj.size(); l_di++)
  {
    io_dim_ids_right[l_di_right] = l_dim_ids_bj[l_di];
    l_di_right--;
  }

  // assign non-blocked K dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bk.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_bk[l_di];
    io_dim_ids_right[l_di_right] = l_dim_ids_bk[l_di];
    l_di_left--;
    l_di_right--;
  }

  // assign non-blocked M dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bm.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_bm[l_di];
    l_di_left--;
  }

  // assign non-blocked N dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bn.size(); l_di++)
  {
    io_dim_ids_right[l_di_right] = l_dim_ids_bn[l_di];
    l_di_right--;
  }

  // assign non-blocked C dimensions
  for (std::size_t l_di = 0; l_di < l_dim_ids_bc.size(); l_di++)
  {
    io_dim_ids_left[l_di_left] = l_dim_ids_bc[l_di];
    io_dim_ids_right[l_di_right] = l_dim_ids_bc[l_di];
    l_di_left--;
    l_di_right--;
  }

  return err_t::SUCCESS;
}

einsum_ir::err_t einsum_ir::backend::BinaryPrimitives::reorder(tenord_t i_primitive_ordering,
                                                               int64_t i_num_dims_left,
                                                               int64_t i_num_dims_right,
                                                               int64_t i_num_dims_out,
                                                               std::map<int64_t, int64_t> const *i_dim_sizes,
                                                               int64_t *io_dim_ids_left,
                                                               int64_t *io_dim_ids_right,
                                                               int64_t *io_dim_ids_out) const
{
  err_t l_err = err_t::DIMENSION_ORDERING_FAILED;

  if (i_primitive_ordering == tenord_t::LEFT_NATIVE_RIGHT_NATIVE_OUT_NATIVE)
  {
    l_err = err_t::SUCCESS;
  }
  // TODO: adjust enum to fit routine
  else if (i_primitive_ordering == tenord_t::LEFT_BC_BM_BK_BI_KB_MB_RIGHT_BC_BN_BK_BJ_NB_KB_OUT_NATIVE)
  {
    l_err = reorder_left_bc_bm_bk_bi_kb_mb_right_bc_bn_bk_bj_nb_kb_out_native(m_size_mb_min,
                                                                              m_size_mb_max,
                                                                              m_size_nb_min,
                                                                              m_size_nb_max,
                                                                              m_size_kb_min,
                                                                              m_size_kb_max,
                                                                              i_num_dims_left,
                                                                              i_num_dims_right,
                                                                              i_num_dims_out,
                                                                              i_dim_sizes,
                                                                              io_dim_ids_left,
                                                                              io_dim_ids_right,
                                                                              io_dim_ids_out);
  }
  // TODO: adjust enum to fit routine
  else if (i_primitive_ordering == tenord_t::LEFT_BC_BM_BK_BI_KB_MB_CB_RIGHT_BC_BN_BK_BJ_NB_KB_CB_OUT_NATIVE)
  {
    l_err = reorder_left_bc_bm_bk_bi_kb_mb_cb_right_bc_bn_bk_bj_nb_kb_cb_out_native(m_size_cb_min,
                                                                                    m_size_cb_max,
                                                                                    m_size_mb_min,
                                                                                    m_size_mb_max,
                                                                                    m_size_nb_min,
                                                                                    m_size_nb_max,
                                                                                    m_size_kb_min,
                                                                                    m_size_kb_max,
                                                                                    i_num_dims_left,
                                                                                    i_num_dims_right,
                                                                                    i_num_dims_out,
                                                                                    i_dim_sizes,
                                                                                    io_dim_ids_left,
                                                                                    io_dim_ids_right,
                                                                                    io_dim_ids_out);
  }
  else if (i_primitive_ordering == tenord_t::LEFT_BC_BM_BK_BI_CB_KB_MB_RIGHT_BC_BN_BK_BJ_CB_NB_KB_OUT_NATIVE)
  {
    l_err = reorder_left_bc_bm_bk_bi_cb_kb_mb_right_bc_bn_bk_bj_cb_nb_kb_out_native(m_size_mb_min,
                                                                                    m_size_mb_max,
                                                                                    m_size_nb_min,
                                                                                    m_size_nb_max,
                                                                                    m_size_kb_min,
                                                                                    m_size_kb_max,
                                                                                    i_num_dims_left,
                                                                                    i_num_dims_right,
                                                                                    i_num_dims_out,
                                                                                    i_dim_sizes,
                                                                                    io_dim_ids_left,
                                                                                    io_dim_ids_right,
                                                                                    io_dim_ids_out);
  }

  return l_err;
}

einsum_ir::err_t einsum_ir::backend::BinaryPrimitives::reorder(backend_t i_backend_type,
                                                               int64_t i_num_dims_left,
                                                               int64_t i_num_dims_right,
                                                               int64_t i_num_dims_out,
                                                               std::map<int64_t, int64_t> const *i_dim_sizes,
                                                               int64_t *io_dim_ids_left,
                                                               int64_t *io_dim_ids_right,
                                                               int64_t *io_dim_ids_out) const
{
  err_t l_err = err_t::DIMENSION_ORDERING_FAILED;

  // determine dimension types of the tensors
  std::vector<einsum_ir::dim_t> l_dim_types_left;
  std::vector<einsum_ir::dim_t> l_dim_types_right;
  std::vector<einsum_ir::dim_t> l_dim_types_out;
  BinaryContraction::dim_types(i_num_dims_left,
                               i_num_dims_right,
                               i_num_dims_out,
                               io_dim_ids_left,
                               io_dim_ids_right,
                               io_dim_ids_out,
                               &l_dim_types_left,
                               &l_dim_types_right,
                               &l_dim_types_out);

  tenord_t l_tensor_ordering = tenord_t::UNDEFINED_TENORD;

  if (i_backend_type == backend_t::TPP)
  {
    l_tensor_ordering = tenord_t::LEFT_BC_BM_BK_BI_KB_MB_CB_RIGHT_BC_BN_BK_BJ_NB_KB_CB_OUT_NATIVE;
  }
  else if (i_backend_type == backend_t::BLAS)
  {
    if (l_dim_types_out.size() > 0 && l_dim_types_out.back() == einsum_ir::dim_t::C)
    {
      l_tensor_ordering = tenord_t::LEFT_BC_BM_BK_BI_CB_KB_MB_RIGHT_BC_BN_BK_BJ_CB_NB_KB_OUT_NATIVE;
    }
    else
    {
      l_tensor_ordering = tenord_t::LEFT_BC_BM_BK_BI_KB_MB_CB_RIGHT_BC_BN_BK_BJ_NB_KB_CB_OUT_NATIVE;
    }
  }
  else if (i_backend_type == backend_t::TBLIS)
  {
    l_tensor_ordering = tenord_t::LEFT_BC_BM_BK_BI_KB_MB_CB_RIGHT_BC_BN_BK_BJ_NB_KB_CB_OUT_NATIVE;
  }
  else
  {
    l_tensor_ordering = tenord_t::LEFT_NATIVE_RIGHT_NATIVE_OUT_NATIVE;
  }

  l_err = reorder(l_tensor_ordering,
                  i_num_dims_left,
                  i_num_dims_right,
                  i_num_dims_out,
                  i_dim_sizes,
                  io_dim_ids_left,
                  io_dim_ids_right,
                  io_dim_ids_out);

  return l_err;
}

void einsum_ir::backend::BinaryPrimitives::compileLoopOrder(std::map<int64_t, dim_t> &io_dim_types,
                                                            std::map<int64_t, int64_t> &io_dim_sizes,
                                                            std::map<int64_t, int64_t> &io_strides_left,
                                                            std::map<int64_t, int64_t> &io_strides_right,
                                                            std::map<int64_t, int64_t> &io_strides_out,
                                                            std::vector<int64_t> const &i_dim_ids_c,
                                                            std::vector<int64_t> const &i_dim_ids_m,
                                                            std::vector<int64_t> const &i_dim_ids_n,
                                                            std::vector<int64_t> const &i_dim_ids_k,
                                                            std::vector<int64_t> const &i_dim_ids_cb,
                                                            std::vector<int64_t> const &i_dim_ids_mb,
                                                            std::vector<int64_t> const &i_dim_ids_nb,
                                                            std::vector<int64_t> const &i_dim_ids_kb,
                                                            std::vector<int64_t> &o_loop_order)
{

  // derive IDs of non-blocked dimensions
  std::vector<int64_t> l_dim_ids_bc;
  std::vector<int64_t> l_dim_ids_bm;
  std::vector<int64_t> l_dim_ids_bn;
  std::vector<int64_t> l_dim_ids_bk;

  l_dim_ids_bc.reserve(i_dim_ids_c.size() - i_dim_ids_cb.size());
  l_dim_ids_bm.reserve(i_dim_ids_m.size() - i_dim_ids_mb.size());
  l_dim_ids_bn.reserve(i_dim_ids_n.size() - i_dim_ids_nb.size());
  l_dim_ids_bk.reserve(i_dim_ids_k.size() - i_dim_ids_kb.size());

  for (size_t l_di = 0; l_di < i_dim_ids_c.size(); l_di++)
  {
    if (std::find(i_dim_ids_cb.begin(), i_dim_ids_cb.end(), i_dim_ids_c[l_di]) == i_dim_ids_cb.end())
    {
      l_dim_ids_bc.push_back(i_dim_ids_c[l_di]);
    }
  }
  for (size_t l_di = 0; l_di < i_dim_ids_m.size(); l_di++)
  {
    if (std::find(i_dim_ids_mb.begin(), i_dim_ids_mb.end(), i_dim_ids_m[l_di]) == i_dim_ids_mb.end())
    {
      l_dim_ids_bm.push_back(i_dim_ids_m[l_di]);
    }
  }
  for (size_t l_di = 0; l_di < i_dim_ids_n.size(); l_di++)
  {
    if (std::find(i_dim_ids_nb.begin(), i_dim_ids_nb.end(), i_dim_ids_n[l_di]) == i_dim_ids_nb.end())
    {
      l_dim_ids_bn.push_back(i_dim_ids_n[l_di]);
    }
  }
  for (size_t l_di = 0; l_di < i_dim_ids_k.size(); l_di++)
  {
    if (std::find(i_dim_ids_kb.begin(), i_dim_ids_kb.end(), i_dim_ids_k[l_di]) == i_dim_ids_kb.end())
    {
      l_dim_ids_bk.push_back(i_dim_ids_k[l_di]);
    }
  }

  // sort dimension depending on strides
  std::sort(l_dim_ids_bc.begin(), l_dim_ids_bc.end(), [&io_strides_out](int64_t a, int64_t b)
            { return io_strides_out[a] > io_strides_out[b]; });
  std::sort(l_dim_ids_bm.begin(), l_dim_ids_bm.end(), [&io_strides_out](int64_t a, int64_t b)
            { return io_strides_out[a] > io_strides_out[b]; });
  std::sort(l_dim_ids_bn.begin(), l_dim_ids_bn.end(), [&io_strides_out](int64_t a, int64_t b)
            { return io_strides_out[a] > io_strides_out[b]; });
  std::sort(l_dim_ids_bk.begin(), l_dim_ids_bk.end(), [&io_strides_right](int64_t a, int64_t b)
            { return io_strides_right[a] > io_strides_right[b]; });

  o_loop_order.clear();

  // add K dimensions to loop order until target size is reached
  int64_t l_size_inner_k = 1;
  while (l_dim_ids_bk.size() > 0)
  {
    int64_t l_dim_id = l_dim_ids_bk.back();
    int64_t l_dim_size = io_dim_sizes[l_dim_id];
    o_loop_order.push_back(l_dim_id);
    l_dim_ids_bk.pop_back();

    if (l_dim_size * l_size_inner_k < m_size_inner_k_loops)
    {
      l_size_inner_k = l_dim_size;
    }
    else
    {
      int64_t l_target = (float_t)m_size_inner_k_loops / (float_t)l_size_inner_k;
      int64_t l_new_size = splitDimension(l_dim_size, l_target);
      if (l_new_size != l_dim_size)
      {
        int64_t l_size_other = l_dim_size / l_new_size;
        int64_t l_id_other = m_next_free_id++;
        int64_t l_stride_left = io_strides_left[l_dim_id] * l_new_size;
        int64_t l_stride_right = io_strides_right[l_dim_id] * l_new_size;

        // apply split
        io_dim_sizes.find(l_dim_id)->second = l_new_size;
        io_dim_types.insert(std::pair<int64_t, dim_t>(l_id_other, einsum_ir::K));
        io_dim_sizes.insert(std::pair<int64_t, int64_t>(l_id_other, l_size_other));
        io_strides_left.insert(std::pair<int64_t, int64_t>(l_id_other, l_stride_left));
        io_strides_right.insert(std::pair<int64_t, int64_t>(l_id_other, l_stride_right));
        l_dim_ids_bk.push_back(l_id_other);
        break;
      }
    }
  }
  // add M and N dimensions with interleaved pattern
  std::map<int64_t, int64_t> *io_strides_in = &io_strides_left;
  std::vector<int64_t> *l_dim_ids_in = &l_dim_ids_bm;
  int64_t m_size_inner_loops = m_size_inner_m_loops;
  bool l_m_dim_next = true;

  while (l_dim_ids_bm.size() && l_dim_ids_bn.size())
  {
    int64_t l_dim_id = l_dim_ids_in->back();
    int64_t l_dim_size = io_dim_sizes[l_dim_id];
    o_loop_order.push_back(l_dim_id);
    l_dim_ids_in->pop_back();

    // split dimension if required
    if (l_dim_size > m_size_inner_loops)
    {
      int64_t l_new_size = splitDimension(l_dim_size, m_size_inner_loops);
      if (l_new_size != l_dim_size)
      {
        dim_t l_dim_type = io_dim_types[l_dim_id];
        int64_t l_size_other = l_dim_size / l_new_size;
        int64_t l_id_other = m_next_free_id++;
        int64_t l_stride_in = io_strides_in->at(l_dim_id) * l_new_size;
        int64_t l_stride_out = io_strides_out[l_dim_id] * l_new_size;

        // apply split
        io_dim_sizes.find(l_dim_id)->second = l_new_size;
        io_dim_types.insert(std::pair<int64_t, dim_t>(l_id_other, l_dim_type));
        io_dim_sizes.insert(std::pair<int64_t, int64_t>(l_id_other, l_size_other));
        io_strides_in->insert(std::pair<int64_t, int64_t>(l_id_other, l_stride_in));
        io_strides_out.insert(std::pair<int64_t, int64_t>(l_id_other, l_stride_out));
        l_dim_ids_in->push_back(l_id_other);
      }
    }

    // swap m and n
    if (l_m_dim_next)
    {
      l_m_dim_next = false;
      io_strides_in = &io_strides_right;
      l_dim_ids_in = &l_dim_ids_bn;
      m_size_inner_loops = m_size_inner_n_loops;
    }
    else
    {
      l_m_dim_next = true;
      io_strides_in = &io_strides_left;
      l_dim_ids_in = &l_dim_ids_bm;
      m_size_inner_loops = m_size_inner_m_loops;
    }
  }

  // add remaining m, n, c, k dimensions
  while (l_dim_ids_bm.size())
  {
    int64_t l_dim_id = l_dim_ids_bm.back();
    o_loop_order.push_back(l_dim_id);
    l_dim_ids_bm.pop_back();
  }
  while (l_dim_ids_bn.size())
  {
    int64_t l_dim_id = l_dim_ids_bn.back();
    o_loop_order.push_back(l_dim_id);
    l_dim_ids_bn.pop_back();
  }
  while (l_dim_ids_bc.size())
  {
    int64_t l_dim_id = l_dim_ids_bc.back();
    o_loop_order.push_back(l_dim_id);
    l_dim_ids_bc.pop_back();
  }
  while (l_dim_ids_bk.size())
  {
    int64_t l_dim_id = l_dim_ids_bk.back();
    o_loop_order.push_back(l_dim_id);
    l_dim_ids_bk.pop_back();
  }

  std::reverse(o_loop_order.begin(), o_loop_order.end());
  // Debug output: Display all variables set/modified in compileLoopOrder function
  std::cout << "\n=== DEBUG compileLoopOrder function ===" << std::endl;

  // Print non-blocked dimension IDs
  std::cout << "l_dim_ids_bc:              ";
  for (size_t i = 0; i < l_dim_ids_bc.size(); ++i)
  {
    std::cout << "[" << i << "]=" << l_dim_ids_bc[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "l_dim_ids_bm:              ";
  for (size_t i = 0; i < l_dim_ids_bm.size(); ++i)
  {
    std::cout << "[" << i << "]=" << l_dim_ids_bm[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "l_dim_ids_bn:              ";
  for (size_t i = 0; i < l_dim_ids_bn.size(); ++i)
  {
    std::cout << "[" << i << "]=" << l_dim_ids_bn[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "l_dim_ids_bk:              ";
  for (size_t i = 0; i < l_dim_ids_bk.size(); ++i)
  {
    std::cout << "[" << i << "]=" << l_dim_ids_bk[i] << " ";
  }
  std::cout << std::endl;

  // Print modified dimension types
  std::cout << "io_dim_types:              ";
  for (const auto &pair : io_dim_types)
  {
    std::cout << "[dim_id=" << pair.first << "]=" << static_cast<int>(pair.second) << " ";
  }
  std::cout << std::endl;

  // Print modified dimension sizes
  std::cout << "io_dim_sizes:              ";
  for (const auto &pair : io_dim_sizes)
  {
    std::cout << "[dim_id=" << pair.first << "]=" << pair.second << " ";
  }
  std::cout << std::endl;

  // Print modified stride maps
  std::cout << "io_strides_left:           ";
  for (const auto &pair : io_strides_left)
  {
    std::cout << "[dim_id=" << pair.first << "]=" << pair.second << " ";
  }
  std::cout << std::endl;

  std::cout << "io_strides_right:          ";
  for (const auto &pair : io_strides_right)
  {
    std::cout << "[dim_id=" << pair.first << "]=" << pair.second << " ";
  }
  std::cout << std::endl;

  std::cout << "io_strides_out:            ";
  for (const auto &pair : io_strides_out)
  {
    std::cout << "[dim_id=" << pair.first << "]=" << pair.second << " ";
  }
  std::cout << std::endl;

  // Print final loop order
  std::cout << "o_loop_order:              ";
  for (size_t i = 0; i < o_loop_order.size(); ++i)
  {
    std::cout << "[" << i << "]=" << o_loop_order[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "=== END DEBUG compileLoopOrder ===\n" << std::endl;
}

int64_t einsum_ir::backend::BinaryPrimitives::splitDimension(int64_t i_dim_size,
                                                             int64_t i_target_size)
{
  // factorization of number
  int64_t l_best_factor = i_dim_size;
  double l_best_distance = abs(log((double)i_dim_size / i_target_size));

  int64_t l_max_factor = 128;
  l_max_factor = i_dim_size / 2 < l_max_factor ? i_dim_size / 2 : l_max_factor;
  for (int64_t l_i = 2; l_i <= l_max_factor; l_i++)
  {
    if (i_dim_size % l_i == 0)
    {
      double l_distance_i = abs(log((double)l_i / i_target_size));
      if (l_best_distance > l_distance_i)
      {
        l_best_factor = l_i;
        l_best_distance = l_distance_i;
      }
    }
  }

  return l_best_factor;
}