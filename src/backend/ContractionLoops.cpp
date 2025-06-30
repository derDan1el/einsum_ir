#include "ContractionLoops.h"
#include <algorithm>

//#include <iostream> //daniel: remove this
#ifdef _OPENMP
#include <omp.h>
#endif

void einsum_ir::backend::ContractionLoops::init(std::map<int64_t, int64_t> const *i_sizes, // daniel : l_dim_sizes
                                                std::map<int64_t, int64_t> const *i_strides_left,
                                                std::map<int64_t, int64_t> const *i_strides_right,
                                                std::map<int64_t, int64_t> const *i_strides_out_aux,
                                                std::map<int64_t, int64_t> const *i_strides_out,
                                                std::map<int64_t, dim_t> const *i_dim_type,
                                                std::vector<int64_t> *i_loop_ids,
                                                int64_t i_num_bytes_scalar_left,
                                                int64_t i_num_bytes_scalar_right,
                                                int64_t i_num_bytes_scalar_out,
                                                kernel_t i_ktype_first_touch,
                                                kernel_t i_ktype_main,
                                                kernel_t i_ktype_last_touch,
                                                ContractionPackingTpp *i_packing)
{
  m_sizes = i_sizes;

  m_strides_left = i_strides_left;
  m_strides_right = i_strides_right;
  m_strides_out_aux = i_strides_out_aux;
  m_strides_out = i_strides_out;

  m_loop_ids = i_loop_ids;

  m_num_bytes_scalar_left = i_num_bytes_scalar_left;
  m_num_bytes_scalar_right = i_num_bytes_scalar_right;
  m_num_bytes_scalar_out = i_num_bytes_scalar_out;

  m_ktype_first_touch = i_ktype_first_touch;
  m_ktype_main = i_ktype_main;
  m_ktype_last_touch = i_ktype_last_touch;

  m_num_tasks_targeted = 1;

  m_packing = i_packing;

  m_dim_type = i_dim_type;

  m_threading_first_last_touch = false;

  m_compiled = false;

  // Debug output: Display all member variables set in ContractionLoops::init function
/*   std::cout << "\n=== ContractionLoops::init function ===" << std::endl;

  // Print sizes
  std::cout << "m_sizes:                   ";
  if (m_sizes)
  {
    for (const auto &pair : *m_sizes)
    {
      std::cout << "[dim_id=" << pair.first << "]=" << pair.second << " ";
    }
  }
  else
  {
    std::cout << "nullptr";
  }
  std::cout << std::endl;

  // Print strides
  std::cout << "m_strides_left:            ";
  if (m_strides_left)
  {
    for (const auto &pair : *m_strides_left)
    {
      std::cout << "[dim_id=" << pair.first << "]=" << pair.second << " ";
    }
  }
  else
  {
    std::cout << "nullptr";
  }
  std::cout << std::endl;

  std::cout << "m_strides_right:           ";
  if (m_strides_right)
  {
    for (const auto &pair : *m_strides_right)
    {
      std::cout << "[dim_id=" << pair.first << "]=" << pair.second << " ";
    }
  }
  else
  {
    std::cout << "nullptr";
  }
  std::cout << std::endl;

  std::cout << "m_strides_out_aux:         ";
  if (m_strides_out_aux)
  {
    for (const auto &pair : *m_strides_out_aux)
    {
      std::cout << "[dim_id=" << pair.first << "]=" << pair.second << " ";
    }
  }
  else
  {
    std::cout << "nullptr";
  }
  std::cout << std::endl;

  std::cout << "m_strides_out:             ";
  if (m_strides_out)
  {
    for (const auto &pair : *m_strides_out)
    {
      std::cout << "[dim_id=" << pair.first << "]=" << pair.second << " ";
    }
  }
  else
  {
    std::cout << "nullptr";
  }
  std::cout << std::endl;

  // Print loop IDs
  std::cout << "m_loop_ids:                ";
  if (m_loop_ids)
  {
    for (size_t i = 0; i < m_loop_ids->size(); ++i)
    {
      std::cout << "[" << i << "]=" << (*m_loop_ids)[i] << " ";
    }
  }
  else
  {
    std::cout << "nullptr";
  }
  std::cout << std::endl;

  // Print scalar byte sizes
  std::cout << "m_num_bytes_scalar_left:   " << m_num_bytes_scalar_left << std::endl;
  std::cout << "m_num_bytes_scalar_right:  " << m_num_bytes_scalar_right << std::endl;
  std::cout << "m_num_bytes_scalar_out:    " << m_num_bytes_scalar_out << std::endl;

  // Print kernel types
  std::cout << "m_ktype_first_touch:       " << static_cast<int>(m_ktype_first_touch) << std::endl;
  std::cout << "m_ktype_main:              " << static_cast<int>(m_ktype_main) << std::endl;
  std::cout << "m_ktype_last_touch:        " << static_cast<int>(m_ktype_last_touch) << std::endl;

  // Print other member variables
  std::cout << "m_num_tasks_targeted:      " << m_num_tasks_targeted << std::endl;

  std::cout << "m_packing:                 " << (m_packing ? "assigned" : "nullptr") << std::endl;

  // Print dimension types
  std::cout << "m_dim_type:                ";
  if (m_dim_type)
  {
    for (const auto &pair : *m_dim_type)
    {
      std::cout << "[dim_id=" << pair.first << "]=" << static_cast<int>(pair.second) << " ";
    }
  }
  else
  {
    std::cout << "nullptr";
  }
  std::cout << std::endl;

  // Print boolean flags
  std::cout << "m_threading_first_last_touch: " << (m_threading_first_last_touch ? "true" : "false") << std::endl;
  std::cout << "m_compiled:                " << (m_compiled ? "true" : "false") << std::endl;

  std::cout << "=== END ContractionLoops::init ===\n"
            << std::endl; */
}

einsum_ir::err_t einsum_ir::backend::ContractionLoops::compile()
{
  // determine if the outermost C dimension is complex
  m_cpx_outer_c = false;
  m_cpx_outer_c |= ce_cpx_op(m_ktype_first_touch);
  m_cpx_outer_c |= ce_cpx_op(m_ktype_main);
  m_cpx_outer_c |= ce_cpx_op(m_ktype_last_touch);

  // check if a complex C dimension is possible
  if (m_cpx_outer_c && m_dim_type->at(m_loop_ids->at(0)) != einsum_ir::C)
  {
    return err_t::INVALID_CPX_DIM;
  }
  if (m_cpx_outer_c && m_sizes->at(m_loop_ids->at(0)) != 2)
  {
    return err_t::INVALID_CPX_DIM;
  }

  // derive complex strides
  if (m_cpx_outer_c)
  {
    m_cpx_stride_in_left_bytes = m_strides_left->at(m_loop_ids->at(0)) * m_num_bytes_scalar_left;
    m_cpx_stride_in_right_bytes = m_strides_right->at(m_loop_ids->at(0)) * m_num_bytes_scalar_right;
    m_cpx_stride_out_aux_bytes = m_strides_out_aux->at(m_loop_ids->at(0)) * m_num_bytes_scalar_out;
    m_cpx_stride_out_bytes = m_strides_out->at(m_loop_ids->at(0)) * m_num_bytes_scalar_out;
    m_loop_ids->erase(m_loop_ids->begin());
  }
  else
  {
    m_cpx_stride_in_left_bytes = 0;
    m_cpx_stride_in_right_bytes = 0;
    m_cpx_stride_out_aux_bytes = 0;
    m_cpx_stride_out_bytes = 0;
  }
  m_num_loops = m_loop_ids->size();
  //std::cout << "m_num_loops: " << m_num_loops << std::endl;

  // setup packing loop execution
  m_id_loop_packing_left = m_num_loops;
  m_id_loop_packing_right = m_num_loops;
  if (m_packing != nullptr)
  {
    m_id_loop_packing_left -= m_packing->m_packing_loop_offset_left;
    m_id_loop_packing_right -= m_packing->m_packing_loop_offset_right;
  }

  // add dummy loop for non-existing loops such that the inner kernel and packing is executed
  if (m_num_loops == 0 || m_id_loop_packing_left < 0 || m_id_loop_packing_right < 0)
  {
    m_loop_ids->insert(m_loop_ids->begin(), -1);
    m_num_loops++;
    m_id_loop_packing_left++;
    m_id_loop_packing_right++;
  }

  // setup loop structure
  m_loop_dim_type.clear();
  m_loop_sizes.clear();
  m_loop_strides_left.clear();
  m_loop_strides_right.clear();
  m_loop_strides_out_aux.clear();
  m_loop_strides_out.clear();

  m_loop_dim_type.reserve(m_num_loops);
  m_loop_sizes.reserve(m_num_loops);
  m_loop_strides_left.reserve(m_num_loops);
  m_loop_strides_right.reserve(m_num_loops);
  m_loop_strides_out_aux.reserve(m_num_loops);
  m_loop_strides_out.reserve(m_num_loops);

  for (int64_t l_di = 0; l_di < m_num_loops; l_di++)
  {
    int64_t l_dim_id = m_loop_ids->at(l_di);
    m_loop_dim_type.push_back(map_find_default<dim_t>(m_dim_type, l_dim_id, dim_t::UNDEFINED_DIM));
    m_loop_sizes.push_back(map_find_default<int64_t>(m_sizes, l_dim_id, 1));
    m_loop_strides_left.push_back(map_find_default<int64_t>(m_strides_left, l_dim_id, 0));
    m_loop_strides_right.push_back(map_find_default<int64_t>(m_strides_right, l_dim_id, 0));
    m_loop_strides_out_aux.push_back(map_find_default<int64_t>(m_strides_out_aux, l_dim_id, 0));
    m_loop_strides_out.push_back(map_find_default<int64_t>(m_strides_out, l_dim_id, 0));
  }

  // find correct loop for first last touch routine
  for (int64_t l_di = m_num_loops - 1; l_di >= 0; l_di--)
  {
    dim_t l_dim_type = m_loop_dim_type[l_di];
    if (l_di == 0 || l_dim_type != einsum_ir::K)
    {
      m_id_loop_first_last_touch = l_di;
      break;
    }
  }
  // scale with size of data types
  for (int64_t l_lo = 0; l_lo < m_num_loops; l_lo++)
  {
    m_loop_strides_left[l_lo] *= m_num_bytes_scalar_left;
    m_loop_strides_right[l_lo] *= m_num_bytes_scalar_right;
    m_loop_strides_out_aux[l_lo] *= m_num_bytes_scalar_out;
    m_loop_strides_out[l_lo] *= m_num_bytes_scalar_out;
  }

  // compile iteration spaces
  err_t l_err = threading(m_num_tasks_targeted);
  if (l_err != err_t::SUCCESS)
  {
    return err_t::COMPILATION_FAILED;
  }

  m_compiled = true;

  // Debug output: Display all member variables set in ContractionLoops::compile function
  /* std::cout << "\n=== ContractionLoops::compile function ===" << std::endl;

  // Print loop counts and packing loop IDs
  std::cout << "m_num_loops:               " << m_num_loops << std::endl;
  std::cout << "m_id_loop_packing_left:    " << m_id_loop_packing_left << std::endl;
  std::cout << "m_id_loop_packing_right:   " << m_id_loop_packing_right << std::endl;
  std::cout << "m_id_loop_first_last_touch:" << m_id_loop_first_last_touch << std::endl;

  // Print modified loop_ids
  std::cout << "m_loop_ids (final):        ";
  if (m_loop_ids)
  {
    for (size_t i = 0; i < m_loop_ids->size(); ++i)
    {
      std::cout << "[" << i << "]=" << (*m_loop_ids)[i] << " ";
    }
  }
  else
  {
    std::cout << "nullptr";
  }
  std::cout << std::endl;

  // Print loop structure vectors
  std::cout << "m_loop_dim_type:           ";
  for (size_t i = 0; i < m_loop_dim_type.size(); ++i)
  {
    std::cout << "[" << i << "]=" << static_cast<int>(m_loop_dim_type[i]) << " ";
  }
  std::cout << std::endl;

  std::cout << "m_loop_sizes:              ";
  for (size_t i = 0; i < m_loop_sizes.size(); ++i)
  {
    std::cout << "[" << i << "]=" << m_loop_sizes[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_loop_strides_left:       ";
  for (size_t i = 0; i < m_loop_strides_left.size(); ++i)
  {
    std::cout << "[" << i << "]=" << m_loop_strides_left[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_loop_strides_right:      ";
  for (size_t i = 0; i < m_loop_strides_right.size(); ++i)
  {
    std::cout << "[" << i << "]=" << m_loop_strides_right[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_loop_strides_out_aux:    ";
  for (size_t i = 0; i < m_loop_strides_out_aux.size(); ++i)
  {
    std::cout << "[" << i << "]=" << m_loop_strides_out_aux[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_loop_strides_out:        ";
  for (size_t i = 0; i < m_loop_strides_out.size(); ++i)
  {
    std::cout << "[" << i << "]=" << m_loop_strides_out[i] << " ";
  }
  std::cout << std::endl;

  // Print threading information
  std::cout << "m_num_tasks:               " << m_num_tasks << std::endl;
  std::cout << "m_id_first_parallel:       " << m_id_first_parallel << std::endl;

  // Print final compilation flag
  std::cout << "m_compiled:                " << (m_compiled ? "true" : "false") << std::endl;

  std::cout << "=== END ContractionLoops::compile ===\n"
            << std::endl; */

  return err_t::SUCCESS;
}

einsum_ir::err_t einsum_ir::backend::ContractionLoops::threading(int64_t i_num_tasks)
{
  m_num_tasks = i_num_tasks;

  // find the first parallelizable loop
  m_id_first_parallel = 0;
  int64_t l_num_parallel = 0;
  bool l_found_non_k = false;
  for (int64_t l_di = 0; l_di < m_loop_dim_type.size(); l_di++)
  {
    dim_t l_type = m_loop_dim_type[l_di];
    if (l_type == einsum_ir::K && !l_found_non_k)
    {
      m_id_first_parallel++;
    }
    else if (l_type != einsum_ir::K)
    {
      l_found_non_k = true;
      l_num_parallel++;
    }
    else
    {
      break;
    }
  }
  if (l_num_parallel == 0)
  {
    m_id_first_parallel = 0;
  }

  m_iter_spaces.init(m_num_loops,
                     l_num_parallel,
                     nullptr,
                     m_loop_sizes.data() + m_id_first_parallel,
                     m_num_tasks);
  err_t l_err = m_iter_spaces.compile();
  if (l_err != err_t::SUCCESS)
  {
    return err_t::COMPILATION_FAILED;
  }

  m_num_tasks = m_iter_spaces.num_tasks();

  return err_t::SUCCESS;
}

void einsum_ir::backend::ContractionLoops::contract(void const *i_tensor_left,
                                                    void const *i_tensor_right,
                                                    void const *i_tensor_out_aux,
                                                    void *io_tensor_out)
{

  if (m_packing != nullptr)
  {
    m_packing->allocate_memory();
  }

  if (m_id_first_parallel > 0)
  {
    contract_iter_non_parallel(0,
                               i_tensor_left,
                               i_tensor_right,
                               i_tensor_out_aux,
                               io_tensor_out,
                               true,
                               true);
  }
  else
  {

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int64_t l_ta = 0; l_ta < m_num_tasks; l_ta++)
    {
      contract_iter(l_ta,
                    0,
                    i_tensor_left,
                    i_tensor_right,
                    i_tensor_out_aux,
                    io_tensor_out,
                    true,
                    true);
    }
  }
}

void einsum_ir::backend::ContractionLoops::contract_iter(int64_t i_id_task,
                                                         int64_t i_id_loop,
                                                         void const *i_ptr_left,
                                                         void const *i_ptr_right,
                                                         void const *i_ptr_out_aux,
                                                         void *i_ptr_out,
                                                         bool i_first_access,
                                                         bool i_last_access)
{

  // DEBUG: Ausführliche Ausgabe von firsts() und sizes()
  //std::cout << "\n=== DEBUG contract_iter - IterationSpaces ===\n";
  //std::cout << "Input parameters:\n";
  //std::cout << "  i_id_task:               " << i_id_task << "\n";
  //std::cout << "  i_id_loop:               " << i_id_loop << "\n";
  //std::cout << "  m_id_first_parallel:     " << m_id_first_parallel << "\n";
  //std::cout << "  array_index:             " << (i_id_loop - m_id_first_parallel) << "\n";
  //std::cout << "  m_num_tasks:             " << m_num_tasks << "\n";
  //std::cout << "  m_num_loops:             " << m_num_loops << "\n";
//
  //// Debug firsts() für ALLE Tasks
  //std::cout << "\nDEBUG firsts() for ALL tasks:\n";
  //for (int64_t task = 0; task < m_num_tasks; task++)
  //{
  //  std::cout << "  Task " << task << " firsts(): ";
  //  int64_t const *firsts_array = m_iter_spaces.firsts(task);
  //  if (firsts_array)
  //  {
  //    // Zeige alle Elemente des firsts Arrays für diese Task
  //    int64_t num_parallel = m_num_loops - m_id_first_parallel;
  //    for (int64_t idx = 0; idx < num_parallel; idx++)
  //    {
  //      std::cout << "[" << idx << "]=" << firsts_array[idx] << " ";
  //    }
  //  }
  //  else
  //  {
  //    std::cout << "nullptr";
  //  }
  //  std::cout << "\n";
  //}
//
  //// Debug sizes() für ALLE Tasks
  //std::cout << "\nDEBUG sizes() for ALL tasks:\n";
  //for (int64_t task = 0; task < m_num_tasks; task++)
  //{
  //  std::cout << "  Task " << task << " sizes():  ";
  //  int64_t const *sizes_array = m_iter_spaces.sizes(task);
  //  if (sizes_array)
  //  {
  //    // Zeige alle Elemente des sizes Arrays für diese Task
  //    int64_t num_parallel = m_num_loops - m_id_first_parallel;
  //    for (int64_t idx = 0; idx < num_parallel; idx++)
  //    {
  //      std::cout << "[" << idx << "]=" << sizes_array[idx] << " ";
  //    }
  //  }
  //  else
  //  {
  //    std::cout << "nullptr";
  //  }
  //  std::cout << "\n";
  //}

  // Debug für die aktuell verwendeten Werte
  //std::cout << "\nCurrent access for THIS function call:\n";
  //std::cout << "  Current task:            " << i_id_task << "\n";
  //std::cout << "  Current loop:            " << i_id_loop << "\n";

  //if (i_id_loop >= m_id_first_parallel)
  //{
  //  int64_t array_idx = i_id_loop - m_id_first_parallel;
  //  int64_t const *current_firsts = m_iter_spaces.firsts(i_id_task);
  //  int64_t const *current_sizes = m_iter_spaces.sizes(i_id_task);
//
  //  std::cout << "  Array index used:        " << array_idx << "\n";
  //  std::cout << "  firsts()[" << array_idx << "]:         " << current_firsts[array_idx] << "\n";
  //  std::cout << "  sizes()[" << array_idx << "]:          " << current_sizes[array_idx] << "\n";
//
  //  // Zeige auch die Loop-Größe aus m_loop_sizes zum Vergleich
  //  std::cout << "  m_loop_sizes[" << i_id_loop << "]:      " << m_loop_sizes[i_id_loop] << "\n";
  //}
  //else
  //{
  //  std::cout << "  This loop is NON-PARALLEL (i_id_loop < m_id_first_parallel)\n";
  //  std::cout << "  Using m_loop_sizes directly\n";
  //  std::cout << "  m_loop_sizes[" << i_id_loop << "]:      " << m_loop_sizes[i_id_loop] << "\n";
  //}

  // Debug zusätzliche IterationSpaces Informationen
  //std::cout << "\nIterationSpaces global info:\n";
  //std::cout << "  num_tasks():             " << m_iter_spaces.num_tasks() << "\n";
  //std::cout << "  num_collapsed():         " << m_iter_spaces.num_collapsed() << "\n";

  //std::cout << "=== END DEBUG contract_iter ===\n\n";

  // derive first element and number of iterations
  // daniel: .firsts(i_id_task) ist eine methodenaufruf mit i_id_task als eingabeparameter.
  // daniel: die funktion gibt einen zeiger auf ein array zurück und auf dieses greigt man mit den eckigen klammern zu
  int64_t l_first = m_iter_spaces.firsts(i_id_task)[i_id_loop - m_id_first_parallel];

  // sizes     //! sizes of the loops / number of iterations an der stelle [i_id_loop - m_id_first_parallel];
  int64_t l_size = m_iter_spaces.sizes(i_id_task)[i_id_loop - m_id_first_parallel];

  bool l_first_access = i_first_access;
  bool l_last_access = i_last_access;

  // issue loop iterations
  for (int64_t l_it = l_first; l_it < l_first + l_size; l_it++)
  {
    char *l_ptr_left = (char *)i_ptr_left;
    char *l_ptr_right = (char *)i_ptr_right;
    char *l_ptr_out_aux = (char *)i_ptr_out_aux;
    char *l_ptr_out = (char *)i_ptr_out;

    l_ptr_left += l_it * m_loop_strides_left[i_id_loop];
    l_ptr_right += l_it * m_loop_strides_right[i_id_loop];
    l_ptr_out_aux += l_it * m_loop_strides_out_aux[i_id_loop] * (l_ptr_out_aux != nullptr);
    l_ptr_out += l_it * m_loop_strides_out[i_id_loop];
    //std::cout << "l_ptr_left += l_it(" << l_it << ") * m_loop_strides_left[i_id_loop(" << i_id_loop << ")] (" << m_loop_strides_left[i_id_loop] << ");" << std::endl;
    //std::cout << "l_ptr_left += l_it(" << l_it << ") * m_loop_strides_right[i_id_loop(" << i_id_loop << ")] (" << m_loop_strides_right[i_id_loop] << ");" << std::endl;
    //std::cout << "l_ptr_left += l_it(" << l_it << ") * m_loop_strides_out[i_id_loop(" << i_id_loop << ")] (" << m_loop_strides_out[i_id_loop] << ");" << std::endl;

    if (m_loop_dim_type[i_id_loop] == einsum_ir::K)
    {
      l_first_access = i_first_access && l_it == 0;
      l_last_access = i_last_access && l_it == m_loop_sizes[i_id_loop] - 1;
    }

    if (m_id_loop_first_last_touch == i_id_loop && l_first_access)
    {
      kernel_first_touch(l_ptr_out_aux,
                         l_ptr_out);
    }

    if (m_id_loop_packing_left == i_id_loop)
    {
      l_ptr_left = m_packing->kernel_pack_left(l_ptr_left);
    }
    if (m_id_loop_packing_right == i_id_loop)
    {
      l_ptr_right = m_packing->kernel_pack_right(l_ptr_right);
    }
    // daniel: main programm?
    if (i_id_loop + 1 < m_num_loops)
    {
      contract_iter(i_id_task,
                    i_id_loop + 1,
                    l_ptr_left,
                    l_ptr_right,
                    l_ptr_out_aux,
                    l_ptr_out,
                    l_first_access,
                    l_last_access);
    }
    else
    {
      // execute main kernel
      kernel_main(l_ptr_left,
                  l_ptr_right,
                  l_ptr_out);
    }
    if (m_id_loop_first_last_touch == i_id_loop && l_last_access)
    {
      kernel_last_touch(l_ptr_out_aux,
                        l_ptr_out);
    }
  }
}

void einsum_ir::backend::ContractionLoops::contract_iter_non_parallel(int64_t i_id_loop,
                                                                      void const *i_ptr_left,
                                                                      void const *i_ptr_right,
                                                                      void const *i_ptr_out_aux,
                                                                      void *i_ptr_out,
                                                                      bool i_first_access,
                                                                      bool i_last_access)
{
  bool l_first_access = i_first_access;
  bool l_last_access = i_last_access;

  // issue loop iterations
  for (int64_t l_it = 0; l_it < m_loop_sizes[i_id_loop]; l_it++)
  {
    char *l_ptr_left = (char *)i_ptr_left;
    char *l_ptr_right = (char *)i_ptr_right;
    char *l_ptr_out_aux = (char *)i_ptr_out_aux;
    char *l_ptr_out = (char *)i_ptr_out;

    l_ptr_left += l_it * m_loop_strides_left[i_id_loop];
    l_ptr_right += l_it * m_loop_strides_right[i_id_loop];
    l_ptr_out_aux += l_it * m_loop_strides_out_aux[i_id_loop] * (l_ptr_out_aux != nullptr);
    l_ptr_out += l_it * m_loop_strides_out[i_id_loop];

    if (m_loop_dim_type[i_id_loop] == einsum_ir::K)
    {
      l_first_access = i_first_access && l_it == 0;
      l_last_access = i_last_access && l_it == m_loop_sizes[i_id_loop] - 1;
    }

    if (m_id_loop_first_last_touch == i_id_loop && l_first_access)
    {
      kernel_first_touch(l_ptr_out_aux,
                         l_ptr_out);
    }

    if (m_id_loop_packing_left == i_id_loop)
    {
      l_ptr_left = m_packing->kernel_pack_left(l_ptr_left);
    }
    if (m_id_loop_packing_right == i_id_loop)
    {
      l_ptr_right = m_packing->kernel_pack_right(l_ptr_right);
    }

    if (i_id_loop + 1 < m_id_first_parallel)
    {
      contract_iter_non_parallel(i_id_loop + 1,
                                 l_ptr_left,
                                 l_ptr_right,
                                 l_ptr_out_aux,
                                 l_ptr_out,
                                 l_first_access,
                                 l_last_access);
    }
    else
    {
#ifdef _OPENMP
#pragma omp parallel for
#endif
      for (int64_t l_ta = 0; l_ta < m_num_tasks; l_ta++)
      {
        contract_iter(l_ta,
                      i_id_loop + 1,
                      l_ptr_left,
                      l_ptr_right,
                      l_ptr_out_aux,
                      l_ptr_out,
                      l_first_access,
                      l_last_access);
      }
    }
    if (m_id_loop_first_last_touch == i_id_loop && l_last_access)
    {
      kernel_last_touch(l_ptr_out_aux,
                        l_ptr_out);
    }
  }
}