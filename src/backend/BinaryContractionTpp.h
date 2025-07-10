#ifndef EINSUM_IR_BACKEND_BINARY_CONTRACTION_TPP
#define EINSUM_IR_BACKEND_BINARY_CONTRACTION_TPP

#include <libxsmm.h>
#include "BinaryContraction.h"
#include "ContractionLoopsTpp.h"

namespace einsum_ir
{
  namespace backend
  {
    class BinaryContractionTpp;
  }
}

class einsum_ir::backend::BinaryContractionTpp : public BinaryContraction
{
private:
  //! LIBXSMM-based unary first-touch TPP
  libxsmm_meltwfunction_unary m_xmm_kernel_first_touch_unary = nullptr;

  //! LIBXSMM-based binary first-touch TPP
  libxsmm_meltwfunction_binary m_xmm_kernel_first_touch_binary = nullptr;

  //! LIBXSMM-based main TPP which is called in the innermost loop
  libxsmm_xmmfunction m_xmm_kernel_main;

  //! LIBXSMM-based unary last-touch TPP
  libxsmm_meltwfunction_unary m_xmm_kernel_last_touch_unary = nullptr;

  //! LIBXSMM-based binary last-touch TPP
  libxsmm_meltwfunction_binary m_xmm_kernel_last_touch_binary = nullptr;

  //! packing
  ContractionPackingTpp *m_packing = nullptr;

  //! contraction loop interface
  ContractionLoopsTpp m_cont_loops;

  //! BC sizes
  std::vector<int64_t> m_sizes_bc;
  //! BM sizes
  std::vector<int64_t> m_sizes_bm;
  //! BN sizes
  std::vector<int64_t> m_sizes_bn;
  //! BK sizes
  std::vector<int64_t> m_sizes_bk;

  //! BC strides of of the left tensor
  std::vector<int64_t> m_strides_left_bc;
  //! BM strides of the left tensor
  std::vector<int64_t> m_strides_left_bm;
  //! BK strides of the left tensor
  std::vector<int64_t> m_strides_left_bk;
  //! BI strides of the left tensor
  std::vector<int64_t> m_strides_left_bi;

  //! BC strides of the right tensor
  std::vector<int64_t> m_strides_right_bc;
  //! BN strides of the right tensor
  std::vector<int64_t> m_strides_right_bn;
  //! BK strides of the right tensor
  std::vector<int64_t> m_strides_right_bk;
  //! BJ strides of the right tensor
  std::vector<int64_t> m_strides_right_bj;

  //! BC strides of the auxiliary output tensor
  std::vector<int64_t> m_strides_out_aux_bc;
  //! BM strides of the auxiliary output tensor
  std::vector<int64_t> m_strides_out_aux_bm;
  //! BN strides of the auxiliary output tensor
  std::vector<int64_t> m_strides_out_aux_bn;

  //! BC strides of the output tensor
  std::vector<int64_t> m_strides_out_bc;
  //! BM strides of the output tensor
  std::vector<int64_t> m_strides_out_bm;
  //! BN strides of the output tensor
  std::vector<int64_t> m_strides_out_bn;
  //! Batch size for batch-reduce GEMM operations

  /**
   * Converts the given native datatype to a LIBXSMM datatype.
   *
   * @param i_data_type native datatype.
   * @return corresponding LIBXSMM datatype.
   */
  static libxsmm_datatype dtype_to_libxsmm(data_t i_dtype);

  /**
   *  Detects the batch-reduce K dimensions for the given contraction.
   *
   * @param o_batch_reduce_size product of the sizes of the batch-reduce k dimensions.
   * @param o_stride_hint_a stride hint for the left tensor.
   * @param o_stride_hint_b stride hint for the right tensor.
   * @param o_dim_ids_kb blocked k dimension ids.
   * @param i_dim_ids_cb  blocked c dimension ids.
   * @param i_dim_ids_mb  blocked m dimension ids.
   * @param i_dim_ids_nb  blocked n dimension ids.
   * @param i_m_dim_ids_k m dimension ids for k.
   * @param i_m_dim_ids_m m dimension ids for m.
   * @param i_m_dim_ids_n m dimension ids for n.
   * @param o_batch_reduce_k_dims output found batch-reduce k dimension ids.
   * @param i_strides_left strides of the left tensor.
   * @param i_strides_right strides of the right tensor.
   * @param i_dim_sizes dimension sizes.
   *
   *
   * @return product of the sizes of the batch-reduce k dimensions.
   */
  static void detect_batch_reduce_k_dimensions(
      int64_t &o_batch_reduce_size,
      int64_t &o_stride_hint_a,
      int64_t &o_stride_hint_b,
      std::vector<int64_t> &o_dim_ids_kb,
      std::vector<int64_t> &i_dim_ids_cb,
      std::vector<int64_t> &i_dim_ids_mb,
      std::vector<int64_t> &i_dim_ids_nb,
      std::vector<int64_t> &i_m_dim_ids_k,
      std::vector<int64_t> &i_m_dim_ids_m,
      std::vector<int64_t> &i_m_dim_ids_n,
      std::vector<int64_t> &o_batch_reduce_k_dims,
      const std::map<int64_t, int64_t> &i_strides_left,
      const std::map<int64_t, int64_t> &i_strides_right,
      const std::map<int64_t, int64_t> &i_dim_sizes);

public:
  /**
   * Destructor
   **/
  ~BinaryContractionTpp();

  /**
   * Compiles the binary contraction.
   **/
  err_t compile();

  /**
   * Initializes the threading configuration of the contraction.
   *
   * @param i_num_tasks_target number of targeted tasks.
   **/
  void threading(int64_t i_num_tasks_target);

  /**
   * Performs a contraction on the given input data.
   *
   * @param i_tensor_left left input tensor.
   * @param i_tensor_right right input tensor.
   * @param io_tensor_out output tensor.
   **/
  void contract(void const *i_tensor_left,
                void const *i_tensor_right,
                void *io_tensor_out);

  /**
   * Performs a contraction on the given input data.
   *
   * @param i_tensor_left left input tensor.
   * @param i_tensor_right right input tensor.
   * @param i_tensor_out_aux auxiliary data w.r.t. output tensor.
   * @param io_tensor_out output tensor.
   **/
  void contract(void const *i_tensor_left,
                void const *i_tensor_right,
                void const *i_tensor_out_aux,
                void *io_tensor_out);
};

#endif
