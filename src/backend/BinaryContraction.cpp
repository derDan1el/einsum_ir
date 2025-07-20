#include "BinaryContraction.h"
#include <list>
#include <algorithm>
#include <cassert>
#include <cmath>
//#include <iostream> //daniel wegmachen
#include "ContractionLoops.h"

void einsum_ir::backend::BinaryContraction::dim_types(int64_t i_num_dims_t0,
                                                      int64_t i_num_dims_t1,
                                                      int64_t i_num_dims_t2,
                                                      int64_t const *i_dim_ids_t0, // daniel: dimensions ids vom rrechten tensor , 2. (links,out,rechts) ,3 (links,rechts,out)
                                                      int64_t const *i_dim_ids_t1,
                                                      int64_t const *i_dim_ids_t2,
                                                      dim_t i_dim_type_t2, // daniel: die namen sagen an in welchen tensoren diese dimension auftritt zb t0 = rechts, t1 = out  , t2 = links
                                                      dim_t i_dim_type_t2_t0,
                                                      dim_t i_dim_type_t2_t1,
                                                      dim_t i_dim_type_t2_t0_t1,
                                                      dim_t *o_dim_types_t2)
{
  std::list<int64_t> l_dim_ids_t0(i_dim_ids_t0, // daniel: 0 2 3 1
                                  i_dim_ids_t0 + i_num_dims_t0);

  std::list<int64_t> l_dim_ids_t1(i_dim_ids_t1,
                                  i_dim_ids_t1 + i_num_dims_t1);

  std::list<int64_t> l_dim_ids_t2(i_dim_ids_t2,
                                  i_dim_ids_t2 + i_num_dims_t2);

  int64_t l_pos_t2 = 0;
  while (l_dim_ids_t2.size() > 0)
  {
    int64_t l_id_t2 = l_dim_ids_t2.front(); // daniel der linke tensor wird behandelt
    l_dim_ids_t2.pop_front();
    // daniel : es wird geschaut ob die dimensions ids aus dem linken tensor auch in dem rechten tensor und dem output tensor vorkommen.
    std::list<int64_t>::iterator l_iter_t0 = std::find(l_dim_ids_t0.begin(),
                                                       l_dim_ids_t0.end(),
                                                       l_id_t2);

    std::list<int64_t>::iterator l_iter_t1 = std::find(l_dim_ids_t1.begin(),
                                                       l_dim_ids_t1.end(),
                                                       l_id_t2);

    if (l_iter_t0 == l_dim_ids_t0.end() // daniel: wenn man es weder im output noch im rechten tensor findet
        && l_iter_t1 == l_dim_ids_t1.end())
    {
      o_dim_types_t2[l_pos_t2] = i_dim_type_t2;
    }
    else if (l_iter_t0 != l_dim_ids_t0.end() // daniel: wenn in t0 (rechts) gefunden ab er in t1 (output) nicht
             && l_iter_t1 == l_dim_ids_t1.end())
    {
      o_dim_types_t2[l_pos_t2] = i_dim_type_t2_t0; // daniel: [0] = C, [1] = C, [2] = K (c wurde also in rechts und links aber nicht in output gefunden)
      l_dim_ids_t0.erase(l_iter_t0);
    }
    else if (l_iter_t0 == l_dim_ids_t0.end() // daniel : wenn mans im outputtensor nicht findet
             && l_iter_t1 != l_dim_ids_t1.end())
    {
      o_dim_types_t2[l_pos_t2] = i_dim_type_t2_t1;
      l_dim_ids_t0.erase(l_iter_t1);
    }
    else
    {                                                 // daniel: wenn in allen drei tensoren die dimension vorkomt,  std::vector< einsum_ir::dim_t > nimmt o_dim_types_t2 auf dh
      o_dim_types_t2[l_pos_t2] = i_dim_type_t2_t0_t1; // daniel: (zeile darüber) o_dim_types_t2 weist jeder dimensionsid einen dimensionstyp zu bsp hier in dem else zweig wird einsum_ir::C zugewiesen was bedeutet das es eine dim ist die in allen vorkommt und die erhalten bleibt
      l_dim_ids_t0.erase(l_iter_t0);                  // daniel : man entfernt die gefunde dimensionsid wenn vorhanden aus den beiden anderen tensoren in dem fall rechter und output tensor
      l_dim_ids_t1.erase(l_iter_t1);                  //[0] = C, [1] = C
    }
    l_pos_t2++;
  }
}

void einsum_ir::backend::BinaryContraction::dim_types(int64_t i_num_dims_left,
                                                      int64_t i_num_dims_right,
                                                      int64_t i_num_dims_out,
                                                      int64_t const *i_dim_ids_left,
                                                      int64_t const *i_dim_ids_right,
                                                      int64_t const *i_dim_ids_out,
                                                      std::vector<einsum_ir::dim_t> *o_dim_types_left,
                                                      std::vector<einsum_ir::dim_t> *o_dim_types_right,
                                                      std::vector<einsum_ir::dim_t> *o_dim_types_out)
{
  o_dim_types_left->resize(i_num_dims_left);
  o_dim_types_right->resize(i_num_dims_right);
  o_dim_types_out->resize(i_num_dims_out);
  // daniel: hier wird 3 mal dim_types aufgerufen für jeden tensor einmal wobei zb I,K,M,C für den linken tensor steht und der output ist ein pointer auf ein vektor der die dimensionstypen speichert die im outputtensor dann auch erhalten werden
  dim_types(i_num_dims_right,
            i_num_dims_out,
            i_num_dims_left,
            i_dim_ids_right,
            i_dim_ids_out,
            i_dim_ids_left,
            einsum_ir::I,
            einsum_ir::K,
            einsum_ir::M,
            einsum_ir::C,
            o_dim_types_left->data()); // daniel: [0] = C, [1] = C, [2] = M, [3] = K

  dim_types(i_num_dims_left,
            i_num_dims_out,
            i_num_dims_right,
            i_dim_ids_left,
            i_dim_ids_out,
            i_dim_ids_right,
            einsum_ir::J,
            einsum_ir::K,
            einsum_ir::N,
            einsum_ir::C,
            o_dim_types_right->data()); // daniel: [0] = C, [1] = K, [2] = N, [3] = C

  dim_types(i_num_dims_left,
            i_num_dims_right,
            i_num_dims_out,
            i_dim_ids_left,
            i_dim_ids_right,
            i_dim_ids_out,
            einsum_ir::UNDEFINED_DIM,
            einsum_ir::M,
            einsum_ir::N,
            einsum_ir::C,
            o_dim_types_out->data()); // daniel: [0] = C, [1] = C, [2] = N
}
// daniel :   filter_dim_ids( i_num_dims_out, einsum_ir::M, o_dim_types_out->data(), i_dim_ids_out, o_dim_ids_m->data() );
int64_t einsum_ir::backend::BinaryContraction::filter_dim_ids(int64_t i_num_dims_tensor,
                                                              dim_t i_dim_type_filter,
                                                              dim_t const *i_dim_types_tensor,
                                                              int64_t const *i_dim_ids_tensor,
                                                              int64_t *o_dim_ids_filtered)
{
  int64_t l_id_filtered = 0;

  for (int64_t l_di = 0; l_di < i_num_dims_tensor; l_di++)
  {
    if (i_dim_types_tensor[l_di] == i_dim_type_filter)
    {                                                             // daniel: wenn dim typ nach dem man filtert gleich der dim typ des tensors an stell ist
      o_dim_ids_filtered[l_id_filtered] = i_dim_ids_tensor[l_di]; // daniel: die id der stelle an der manden typ gefunden hat nach dem man filtert wird gespeichert
      l_id_filtered++;
    }
  }

  return l_id_filtered;
}

void einsum_ir::backend::BinaryContraction::dim_types_ids(int64_t i_num_dims_left,
                                                          int64_t i_num_dims_right,
                                                          int64_t i_num_dims_out,
                                                          int64_t const *i_dim_ids_left,
                                                          int64_t const *i_dim_ids_right,
                                                          int64_t const *i_dim_ids_out,
                                                          std::vector<einsum_ir::dim_t> *o_dim_types_out,
                                                          std::vector<int64_t> *o_dim_ids_c,
                                                          std::vector<int64_t> *o_dim_ids_m,
                                                          std::vector<int64_t> *o_dim_ids_n,
                                                          std::vector<int64_t> *o_dim_ids_k,
                                                          std::vector<int64_t> *o_dim_ids_i,
                                                          std::vector<int64_t> *o_dim_ids_j)
{
  std::vector<einsum_ir::dim_t> l_dim_types_left;
  std::vector<einsum_ir::dim_t> l_dim_types_right;

  // derive dimension types
  dim_types(i_num_dims_left,
            i_num_dims_right,
            i_num_dims_out,
            i_dim_ids_left,
            i_dim_ids_right,
            i_dim_ids_out,
            &l_dim_types_left,
            &l_dim_types_right,
            o_dim_types_out);

  // count dimensions
  int64_t l_num_dims_c = std::count(o_dim_types_out->begin(), // daniel: [0] = C, [1] = C, [2] = N
                                    o_dim_types_out->end(),
                                    einsum_ir::C);
  int64_t l_num_dims_m = std::count(o_dim_types_out->begin(),
                                    o_dim_types_out->end(),
                                    einsum_ir::M);
  int64_t l_num_dims_n = std::count(o_dim_types_out->begin(),
                                    o_dim_types_out->end(),
                                    einsum_ir::N);
  int64_t l_num_dims_k = std::count(l_dim_types_left.begin(), // daniel: [0] = C, [1] = C, [2] = M, [3] = K
                                    l_dim_types_left.end(),
                                    einsum_ir::K);
  int64_t l_num_dims_i = std::count(l_dim_types_left.begin(),
                                    l_dim_types_left.end(),
                                    einsum_ir::I);
  int64_t l_num_dims_j = std::count(l_dim_types_right.begin(), // daniel: [0] = C, [1] = K, [2] = N, [3] = C
                                    l_dim_types_right.end(),
                                    einsum_ir::J);

  // filter dim ids
  o_dim_ids_c->resize(l_num_dims_c);
  o_dim_ids_m->resize(l_num_dims_m);
  o_dim_ids_n->resize(l_num_dims_n);
  o_dim_ids_k->resize(l_num_dims_k);
  o_dim_ids_i->resize(l_num_dims_i);
  o_dim_ids_j->resize(l_num_dims_j);

  filter_dim_ids(i_num_dims_out, // daniel: die func hat auch einen rückgabeyp (anzahl der occurences) wird aber anscheinend nicht genutzt,
                 einsum_ir::C,
                 o_dim_types_out->data(),
                 i_dim_ids_out,
                 o_dim_ids_c->data()); // orte wo C vorkommt werden gespeichert

  filter_dim_ids(i_num_dims_out,
                 einsum_ir::M,
                 o_dim_types_out->data(),
                 i_dim_ids_out,
                 o_dim_ids_m->data());

  filter_dim_ids(i_num_dims_out,
                 einsum_ir::N,
                 o_dim_types_out->data(),
                 i_dim_ids_out,
                 o_dim_ids_n->data());

  filter_dim_ids(i_num_dims_left,
                 einsum_ir::K,
                 l_dim_types_left.data(),
                 i_dim_ids_left,
                 o_dim_ids_k->data());

  filter_dim_ids(i_num_dims_left,
                 einsum_ir::I,
                 l_dim_types_left.data(),
                 i_dim_ids_left,
                 o_dim_ids_i->data());

  filter_dim_ids(i_num_dims_right,
                 einsum_ir::J,
                 l_dim_types_right.data(),
                 i_dim_ids_right,
                 o_dim_ids_j->data());

  // Debug output: Display input parameters and output variables of dim_types_ids function
/*   std::cout << "\n=== dim_types_ids function ===" << std::endl;

  std::cout << "\nInput parameters:" << std::endl;
  std::cout << "i_num_dims_left:           " << i_num_dims_left << std::endl;
  std::cout << "i_num_dims_right:          " << i_num_dims_right << std::endl;
  std::cout << "i_num_dims_out:            " << i_num_dims_out << std::endl;

  std::cout << "\ni_dim_ids_left:            ";
  for (int64_t i = 0; i < i_num_dims_left; i++)
  {
    std::cout << "[" << i << "]=" << i_dim_ids_left[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "i_dim_ids_right:           ";
  for (int64_t i = 0; i < i_num_dims_right; i++)
  {
    std::cout << "[" << i << "]=" << i_dim_ids_right[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "i_dim_ids_out:             ";
  for (int64_t i = 0; i < i_num_dims_out; i++)
  {
    std::cout << "[" << i << "]=" << i_dim_ids_out[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "===================================\n"
            << std::endl; */
}

void einsum_ir::backend::BinaryContraction::strides(int64_t i_num_dims,
                                                    int64_t const *i_dim_ids,
                                                    std::map<int64_t, int64_t> const *i_dim_sizes,
                                                    std::map<int64_t, int64_t> *o_strides)
{
  o_strides->clear();

  int64_t l_id_tensor = i_num_dims - 1;

  int64_t l_stride_tmp = 1;

  while (l_id_tensor >= 0)
  {
    int64_t l_dim_id = i_dim_ids[l_id_tensor];
    int64_t l_dim_size = i_dim_sizes->at(l_dim_id);

    std::pair<int64_t, int64_t> l_pair(l_dim_id,
                                       l_stride_tmp);
    if (l_dim_size < 2)
    {
      l_pair.second = 0;
    }

    o_strides->insert(l_pair);

    l_stride_tmp *= l_dim_size;

    l_id_tensor--;
  }
}
void einsum_ir::backend::BinaryContraction::init(int64_t i_num_dims_left,
                                                 int64_t i_num_dims_right,
                                                 int64_t i_num_dims_out,
                                                 std::map<int64_t, int64_t> const *i_dim_sizes_inner,
                                                 std::map<int64_t, int64_t> const *i_dim_sizes_outer_left,
                                                 std::map<int64_t, int64_t> const *i_dim_sizes_outer_right,
                                                 std::map<int64_t, int64_t> const *i_dim_sizes_outer_out_aux,
                                                 std::map<int64_t, int64_t> const *i_dim_sizes_outer_out,
                                                 int64_t const *i_dim_ids_left,
                                                 int64_t const *i_dim_ids_right,
                                                 int64_t const *i_dim_ids_out,
                                                 data_t i_dtype_left,
                                                 data_t i_dtype_right,
                                                 data_t i_dtype_comp,
                                                 data_t i_dtype_out,
                                                 kernel_t i_ktype_first_touch,
                                                 kernel_t i_ktype_main,
                                                 kernel_t i_ktype_last_touch)
{
  init(i_num_dims_left,
       i_num_dims_right,
       i_num_dims_out,
       i_dim_sizes_inner,
       i_dim_sizes_outer_left,
       i_dim_sizes_outer_right,
       i_dim_sizes_outer_out_aux,
       i_dim_sizes_outer_out,
       nullptr,
       i_dim_ids_left,
       i_dim_ids_right,
       i_dim_ids_out,
       nullptr,
       nullptr,
       nullptr,
       i_dtype_left,
       i_dtype_right,
       i_dtype_comp,
       i_dtype_out,
       false, // TODO : vnni a?
       false, // TODO : vnni b?
       i_ktype_first_touch,
       i_ktype_main,
       i_ktype_last_touch);
}

void einsum_ir::backend::BinaryContraction::init(int64_t i_num_dims_left,
                                                 int64_t i_num_dims_right,
                                                 int64_t i_num_dims_out,
                                                 std::map<int64_t, int64_t> const *i_dim_sizes_inner,
                                                 std::map<int64_t, int64_t> const *i_dim_sizes_outer_left,
                                                 std::map<int64_t, int64_t> const *i_dim_sizes_outer_right,
                                                 std::map<int64_t, int64_t> const *i_dim_sizes_outer_out_aux,
                                                 std::map<int64_t, int64_t> const *i_dim_sizes_outer_out,
                                                 std::vector<int64_t> const *i_loop_ids_ext,
                                                 int64_t const *i_dim_ids_left,
                                                 int64_t const *i_dim_ids_right,
                                                 int64_t const *i_dim_ids_out,
                                                 int64_t const *i_dim_ids_permute_left,
                                                 int64_t const *i_dim_ids_permute_right,
                                                 MemoryManager *i_memory,
                                                 data_t i_dtype_left,
                                                 data_t i_dtype_right,
                                                 data_t i_dtype_comp,
                                                 data_t i_dtype_out,
                                                 bool i_vnni_a,
                                                 bool i_vnni_b,
                                                 kernel_t i_ktype_first_touch,
                                                 kernel_t i_ktype_main,
                                                 kernel_t i_ktype_last_touch)
{
  m_num_dims_left = i_num_dims_left;
  m_num_dims_right = i_num_dims_right;
  m_num_dims_out = i_num_dims_out;

  m_dim_sizes_inner = i_dim_sizes_inner;                 // daniel: wirklich [0] =32,[1] =8,[2] =4,[3] =2, es ist ne map kein array die in klammern sind first values
  m_dim_sizes_outer_left = i_dim_sizes_outer_left;       // daniel: wirklich [0] =32,[1] =8,[2] =4,[3] =2,
  m_dim_sizes_outer_right = i_dim_sizes_outer_right;     // daniel: wirklich [0] =32,[1] =8,[2] =4,[3] =2,
  m_dim_sizes_outer_out_aux = i_dim_sizes_outer_out_aux; // daniel: wirklich [0] =32,[1] =8,[2] =4,[3] =2,
  m_dim_sizes_outer_out = i_dim_sizes_outer_out;         // daniel: wirklich [0] =32,[1] =8,[2] =4,[3] =2,

  m_loop_ids_ext = i_loop_ids_ext; // daniel:  [0] =0,[1] =1,[2] =2,[3] =3

  m_dim_ids_left = i_dim_ids_left;   // daniel: [0] =0,[1] =1,[2] =2
  m_dim_ids_right = i_dim_ids_right; // daniel: [0] =0,[1] =2, [2] =3, [3] =1
  m_dim_ids_out = i_dim_ids_out;     // daniel: [0] =0,[1] =1,[2] =3
  std::vector<int64_t> l_dim_ids_in_left;
  std::vector<int64_t> l_dim_ids_in_right;
  std::vector<int64_t> l_dim_ids_out;

  m_dim_ids_permute_left = i_dim_ids_permute_left;   // daniel: leerer vektor
  m_dim_ids_permute_right = i_dim_ids_permute_right; // daniel: leerer vektor

  m_dtype_left = i_dtype_left;
  m_dtype_right = i_dtype_right;
  m_dtype_comp = i_dtype_comp;
  m_dtype_out = i_dtype_out;

  m_vnni_a = i_vnni_a;
  m_vnni_b = i_vnni_b;

  m_ktype_first_touch = i_ktype_first_touch;
  m_ktype_main = i_ktype_main;
  m_ktype_last_touch = i_ktype_last_touch;

  m_memory = i_memory;

  // Debug output: Display all member variables set in init function
/*   std::cout << "\n=== .init function in BinaryContraction ===" << std::endl;
  std::cout << "m_num_dims_left:           " << m_num_dims_left << std::endl;
  std::cout << "m_num_dims_right:          " << m_num_dims_right << std::endl;
  std::cout << "m_num_dims_out:            " << m_num_dims_out << std::endl;

  std::cout << "\nDimension sizes maps:" << std::endl;
  if (m_dim_sizes_inner)
  {
    std::cout << "m_dim_sizes_inner:         ";
    for (const auto &pair : *m_dim_sizes_inner)
    {
      std::cout << "[" << pair.first << "]=" << pair.second << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "\nLoop order:" << std::endl;
  if (m_loop_ids_ext)
  {
    std::cout << "m_loop_ids_ext:            ";
    for (size_t i = 0; i < m_loop_ids_ext->size(); i++)
    {
      std::cout << "[" << i << "]=" << (*m_loop_ids_ext)[i] << " ";
    }
    std::cout << std::endl;
  }
  else
  {
    std::cout << "m_loop_ids_ext:            nullptr" << std::endl;
  }

  std::cout << "\nDimension IDs:" << std::endl;
  std::cout << "m_dim_ids_left:            ";
  for (int64_t i = 0; i < m_num_dims_left; i++)
  {
    std::cout << "[" << i << "]=" << m_dim_ids_left[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_dim_ids_right:           ";
  for (int64_t i = 0; i < m_num_dims_right; i++)
  {
    std::cout << "[" << i << "]=" << m_dim_ids_right[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_dim_ids_out:             ";
  for (int64_t i = 0; i < m_num_dims_out; i++)
  {
    std::cout << "[" << i << "]=" << m_dim_ids_out[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "\nVNNI and kernel types:" << std::endl;
  std::cout << "m_vnni_a:                  " << (m_vnni_a ? "true" : "false") << std::endl;
  std::cout << "========================================\n"
            << std::endl; */
}

einsum_ir::err_t einsum_ir::backend::BinaryContraction::compile_base()
{
  dim_types_ids(m_num_dims_left,  // anzahl dim links
                m_num_dims_right, // anzahl dim rechts
                m_num_dims_out,   // anzahl dim output
                m_dim_ids_left,
                m_dim_ids_right,
                m_dim_ids_out,
                &m_dim_types_out, // daniel: [0] = C, [1] = C, [2] = N oder sowas
                &m_dim_ids_c,     // orte wo C vorkommt werden gespeichert mit ihren dim größen
                &m_dim_ids_m,
                &m_dim_ids_n,
                &m_dim_ids_k,
                &m_dim_ids_i,
                &m_dim_ids_j);

  m_num_dims_c = m_dim_ids_c.size();
  m_num_dims_m = m_dim_ids_m.size();
  m_num_dims_n = m_dim_ids_n.size();
  m_num_dims_k = m_dim_ids_k.size();
  m_num_dims_i = m_dim_ids_i.size();
  m_num_dims_j = m_dim_ids_j.size();

  // derive sizes
  m_sizes_c.resize(m_num_dims_c);
  m_sizes_m.resize(m_num_dims_m);
  m_sizes_n.resize(m_num_dims_n);
  m_sizes_k.resize(m_num_dims_k);
  m_sizes_i.resize(m_num_dims_i);
  m_sizes_j.resize(m_num_dims_j);

  for (int64_t l_c = 0; l_c < m_num_dims_c; l_c++)
  { // daniel: die dim sizes des dim typen C werden gepseichert
    int64_t l_id = m_dim_ids_c[l_c];
    m_sizes_c[l_c] = m_dim_sizes_inner->at(l_id);
  }
  for (int64_t l_m = 0; l_m < m_num_dims_m; l_m++)
  {
    int64_t l_id = m_dim_ids_m[l_m];
    m_sizes_m[l_m] = m_dim_sizes_inner->at(l_id);
  }
  for (int64_t l_n = 0; l_n < m_num_dims_n; l_n++)
  { // speichert einfach von jeder dimensionsart die dim größe in den vector
    int64_t l_id = m_dim_ids_n[l_n];
    m_sizes_n[l_n] = m_dim_sizes_inner->at(l_id);
  }
  for (int64_t l_k = 0; l_k < m_num_dims_k; l_k++)
  {
    int64_t l_id = m_dim_ids_k[l_k];
    m_sizes_k[l_k] = m_dim_sizes_inner->at(l_id);
  }
  for (int64_t l_i = 0; l_i < m_num_dims_i; l_i++)
  {
    int64_t l_id = m_dim_ids_i[l_i];
    m_sizes_i[l_i] = m_dim_sizes_inner->at(l_id);
  }
  for (int64_t l_j = 0; l_j < m_num_dims_j; l_j++)
  {
    int64_t l_id = m_dim_ids_j[l_j];
    m_sizes_j[l_j] = m_dim_sizes_inner->at(l_id);
  }
  // daniel :ergebnis: [0] =einsum_ir::C, [1] =einsum_ir::C, [2] =einsum_ir::K, [3] =einsum_ir::N
  std::map<int64_t, dim_t> l_dim_types;
  for (const int64_t &l_dim : m_dim_ids_c)
  {
    std::pair<int64_t, dim_t> l_pair(l_dim, einsum_ir::C);
    m_dim_types.insert(l_pair);
  }
  for (const int64_t &l_dim : m_dim_ids_m)
  {
    std::pair<int64_t, dim_t> l_pair(l_dim, einsum_ir::M);
    m_dim_types.insert(l_pair);
  }
  for (const int64_t &l_dim : m_dim_ids_n)
  {
    std::pair<int64_t, dim_t> l_pair(l_dim, einsum_ir::N);
    m_dim_types.insert(l_pair);
  }
  for (const int64_t &l_dim : m_dim_ids_k)
  {
    std::pair<int64_t, dim_t> l_pair(l_dim, einsum_ir::K);
    m_dim_types.insert(l_pair);
  }

  if (m_loop_ids_ext != nullptr)
  { // daniel:  [0] =0,[1] =1,[2] =2,[3] =3
    m_loop_ids_int.clear();
    m_loop_ids_int.reserve(m_loop_ids_ext->size());
    m_loop_ids_int.insert(m_loop_ids_int.begin(), m_loop_ids_ext->begin(), m_loop_ids_ext->end());
  }

  // Debug output: Display all variables set in compile_base function
/*   std::cout << "\n=== compile_base function results ===" << std::endl;

  std::cout << "\nDimension IDs by type:" << std::endl;
  std::cout << "m_dim_ids_c:               ";
  for (size_t i = 0; i < m_dim_ids_c.size(); i++)
  {
    std::cout << "[" << i << "]=" << m_dim_ids_c[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_dim_ids_m:               ";
  for (size_t i = 0; i < m_dim_ids_m.size(); i++)
  {
    std::cout << "[" << i << "]=" << m_dim_ids_m[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_dim_ids_n:               ";
  for (size_t i = 0; i < m_dim_ids_n.size(); i++)
  {
    std::cout << "[" << i << "]=" << m_dim_ids_n[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_dim_ids_k:               ";
  for (size_t i = 0; i < m_dim_ids_k.size(); i++)
  {
    std::cout << "[" << i << "]=" << m_dim_ids_k[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_dim_ids_i:               ";
  for (size_t i = 0; i < m_dim_ids_i.size(); i++)
  {
    std::cout << "[" << i << "]=" << m_dim_ids_i[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_dim_ids_j:               ";
  for (size_t i = 0; i < m_dim_ids_j.size(); i++)
  {
    std::cout << "[" << i << "]=" << m_dim_ids_j[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "\nDimension sizes by type:" << std::endl;
  std::cout << "m_sizes_c:                 ";
  for (size_t i = 0; i < m_sizes_c.size(); i++)
  {
    std::cout << "[" << i << "]=" << m_sizes_c[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_sizes_m:                 ";
  for (size_t i = 0; i < m_sizes_m.size(); i++)
  {
    std::cout << "[" << i << "]=" << m_sizes_m[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_sizes_n:                 ";
  for (size_t i = 0; i < m_sizes_n.size(); i++)
  {
    std::cout << "[" << i << "]=" << m_sizes_n[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_sizes_k:                 ";
  for (size_t i = 0; i < m_sizes_k.size(); i++)
  {
    std::cout << "[" << i << "]=" << m_sizes_k[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_sizes_i:                 ";
  for (size_t i = 0; i < m_sizes_i.size(); i++)
  {
    std::cout << "[" << i << "]=" << m_sizes_i[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "m_sizes_j:                 ";
  for (size_t i = 0; i < m_sizes_j.size(); i++)
  {
    std::cout << "[" << i << "]=" << m_sizes_j[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "\nDimension types map:" << std::endl;
  std::cout << "m_dim_types:               ";
  for (const auto &pair : m_dim_types)
  {
    std::cout << "[dim_id=" << pair.first << "]=" << static_cast<int>(pair.second) << " ";
  }
  std::cout << std::endl;

  std::cout << "\nInternal loop order:" << std::endl;
  std::cout << "m_loop_ids_int:            ";
  for (size_t i = 0; i < m_loop_ids_int.size(); i++)
  {
    std::cout << "[" << i << "]=" << m_loop_ids_int[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "=========================================\n"
            << std::endl; */

  return einsum_ir::SUCCESS;
}

int64_t einsum_ir::backend::BinaryContraction::num_ops()
{
  int64_t l_size_c = 1;
  int64_t l_size_m = 1;
  int64_t l_size_n = 1;
  int64_t l_size_k = 1;

  for (int64_t l_c = 0; l_c < m_num_dims_c; l_c++)
  {
    l_size_c *= m_sizes_c[l_c];
  }
  for (int64_t l_m = 0; l_m < m_num_dims_m; l_m++)
  {
    l_size_m *= m_sizes_m[l_m];
  }
  for (int64_t l_n = 0; l_n < m_num_dims_n; l_n++)
  {
    l_size_n *= m_sizes_n[l_n];
  }
  for (int64_t l_k = 0; l_k < m_num_dims_k; l_k++)
  {
    l_size_k *= m_sizes_k[l_k];
  }

  int64_t l_num_ops = l_size_c * l_size_m * l_size_n * l_size_k * 2;

  if (m_ktype_main == kernel_t::CPX_MADD)
  {
    l_num_ops *= 2; // four matrix mults ignoring previously counted batch dim
  }

  if (m_ktype_first_touch == ZERO || m_ktype_first_touch == CPX_ZERO)
  {
    l_num_ops -= l_size_c * l_size_m * l_size_n;
  }

  return l_num_ops;
}