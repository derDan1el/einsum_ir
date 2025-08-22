#include "EinsumExpressionAscii.h"

#include <algorithm>
#include <set>
#include <codecvt>
#include <locale>

void einsum_ir::frontend::EinsumExpressionAscii::split_string(std::string const &i_input,
                                                              std::string const &i_separation,
                                                              std::vector<std::string> &o_output)
{
  std::string l_string = i_input;
  int64_t l_off = 0;
  int64_t l_size_string = l_string.size();
  while (l_off < l_size_string)
  {
    l_off = l_string.find(i_separation);
    if (l_off < 0)
      break;
    o_output.push_back(l_string.substr(0, l_off));
    l_string.erase(0, l_off + i_separation.size());
  }
  if (l_string.size() > 0)
  {
    o_output.push_back(l_string);
  }
}

void einsum_ir::frontend::EinsumExpressionAscii::schar_to_standard(std::string const &i_expr_string,
                                                                   std::string &o_expr_string)
{
  o_expr_string.clear();

  std::string l_expr = i_expr_string;

  l_expr.erase(std::remove(l_expr.begin(),
                           l_expr.end(),
                           ' '),
               l_expr.end());

  std::vector<std::string> l_tensors;
  split_string(l_expr,
               std::string("->"),
               l_tensors);

  std::vector<std::string> l_input_tensors;
  split_string(l_tensors[0],
               std::string(","),
               l_input_tensors);

  for (std::size_t l_te = 0; l_te < l_input_tensors.size(); l_te++)
  {
    std::string l_tensor = l_input_tensors[l_te];
    o_expr_string += "[";
    for (std::size_t l_di = 0; l_di < l_tensor.size(); l_di++)
    {
      o_expr_string += l_tensor[l_di];
      if (l_di < l_tensor.size() - 1)
      {
        o_expr_string += ",";
      }
    }
    o_expr_string += "]";
    if (l_te < l_input_tensors.size() - 1)
    {
      o_expr_string += ",";
    }
  }

  o_expr_string += "->[";

  for (std::size_t l_di = 0; l_di < l_tensors[1].size(); l_di++)
  {
    o_expr_string += l_tensors[1][l_di];
    if (l_di < l_tensors[1].size() - 1)
    {
      o_expr_string += ",";
    }
  }

  o_expr_string += "]";
}

void einsum_ir::frontend::EinsumExpressionAscii::standard_to_schar(std::string const &i_expr_string,
                                                                   std::string &o_expr_string)
{
  o_expr_string.clear();

  std::string l_expr = i_expr_string;

  l_expr.erase(std::remove(l_expr.begin(),
                           l_expr.end(),
                           ' '),
               l_expr.end());

  l_expr.erase(0, 1);
  l_expr.erase(l_expr.size() - 1, 1);

  std::vector<std::string> l_tensors;
  split_string(l_expr,
               std::string("]->["),
               l_tensors);

  if (l_tensors.size() == 1)
  {
    l_tensors.push_back("");
  }

  std::vector<std::string> l_input_tensors;
  split_string(l_tensors[0],
               std::string("],["),
               l_input_tensors);

  // map: dimension name -> dimension id
  std::map<std::string, int64_t> l_map_dim_name_to_id;

  for (std::size_t l_te = 0; l_te < l_input_tensors.size(); l_te++)
  {
    std::string l_tensor = l_input_tensors[l_te];

    std::vector<std::string> l_tensor_dim_names;
    split_string(l_tensor,
                 std::string(","),
                 l_tensor_dim_names);

    for (std::size_t l_di = 0; l_di < l_tensor_dim_names.size(); l_di++)
    {
      l_map_dim_name_to_id.insert({l_tensor_dim_names[l_di], 0});
    }
  }

  std::vector<std::string> l_dim_names;
  for (std::map<std::string, int64_t>::iterator l_di = l_map_dim_name_to_id.begin(); l_di != l_map_dim_name_to_id.end(); l_di++)
  {
    l_dim_names.push_back(l_di->first);
  }
  std::sort(l_dim_names.begin(),
            l_dim_names.end(),
            [](std::string const &a, std::string const &b)
            {
              try
              {
                return std::stoi(a) < std::stoi(b);
              }
              catch (...)
              {
                return a < b;
              }
            });

  int64_t l_dim_id = 0;
  for (std::size_t l_di = 0; l_di < l_dim_names.size(); l_di++)
  {
    l_map_dim_name_to_id[l_dim_names[l_di]] = l_dim_id;
    l_dim_id++;
  }

  // map: dimension name -> ascii or utf8 character
  std::map<std::string, std::string> l_map_dim_name_to_char;
  for (std::map<std::string, int64_t>::iterator l_di = l_map_dim_name_to_id.begin(); l_di != l_map_dim_name_to_id.end(); l_di++)
  {
    std::string l_dim_name = l_di->first;
    l_dim_id = l_di->second;

    if (l_map_dim_name_to_id.size() <= 52)
    {
      if (l_dim_id < 26)
      {
        l_map_dim_name_to_char.insert({l_dim_name, std::string(1, l_dim_id + 65)});
      }
      else
      {
        l_map_dim_name_to_char.insert({l_dim_name, std::string(1, l_dim_id + 97 - 26)});
      }
    }
    else
    {
      std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> l_conv;
      std::string u8str = l_conv.to_bytes(l_dim_id + 161);
      l_map_dim_name_to_char.insert({l_dim_name, u8str});
    }
  }

  // assemble the output expression
  for (std::size_t l_te = 0; l_te < l_input_tensors.size(); l_te++)
  {
    std::string l_tensor = l_input_tensors[l_te];

    std::vector<std::string> l_tensor_dim_names;
    split_string(l_tensor,
                 std::string(","),
                 l_tensor_dim_names);

    for (std::size_t l_di = 0; l_di < l_tensor_dim_names.size(); l_di++)
    {
      o_expr_string += l_map_dim_name_to_char[l_tensor_dim_names[l_di]];
    }
    if (l_te < l_input_tensors.size() - 1)
    {
      o_expr_string += ",";
    }
  }
  o_expr_string += "->";

  std::vector<std::string> l_output_tensor;
  split_string(l_tensors[1],
               std::string(","),
               l_output_tensor);
  for (std::size_t l_di = 0; l_di < l_output_tensor.size(); l_di++)
  {
    o_expr_string += l_map_dim_name_to_char[l_output_tensor[l_di]];
  }
}

void einsum_ir::frontend::EinsumExpressionAscii::parse_tensors(std::string const &i_expr_string,
                                                               std::vector<std::string> &o_tensors)
{
  o_tensors.clear();

  std::string l_expr = i_expr_string;
  // daniel: "[a,b,c],[a,c,d,b]->[a,b,d]" zu "a,b,c],[a,c,d,b]->[a,b,d"
  l_expr.erase(0, 1);
  l_expr.erase(l_expr.size() - 1, 1);

  l_expr.erase(std::remove(l_expr.begin(),
                           l_expr.end(),
                           ' '),
               l_expr.end());
  std::vector<std::string> l_tensors_tmp;

  split_string(l_expr,
               std::string("]->["),
               l_tensors_tmp);

  if (l_tensors_tmp.size() == 1)
  {
    l_tensors_tmp.push_back("");
  }

  split_string(l_tensors_tmp[0],
               std::string("],["),
               o_tensors);
  o_tensors.push_back(l_tensors_tmp[1]); // daniel: o_tensor aus "[a,b,c],[a,c,d,b]->[a,b,d]" wird [0] = "a,b,c" und [1] = "a,c,d,b" [2] = "a,b,d"
}

void einsum_ir::frontend::EinsumExpressionAscii::parse_dim_sizes(std::string const &i_dim_sizes_string,
                                                                 std::vector<int64_t> &o_dim_sizes)
{
  o_dim_sizes.clear();

  std::string l_sizes = i_dim_sizes_string;

  l_sizes.erase(std::remove(l_sizes.begin(),
                            l_sizes.end(),
                            ' '),
                l_sizes.end());

  std::vector<std::string> l_dim_sizes_tmp;
  split_string(l_sizes,
               std::string(","),
               l_dim_sizes_tmp);

  o_dim_sizes.resize(l_dim_sizes_tmp.size());
  for (std::size_t l_di = 0; l_di < l_dim_sizes_tmp.size(); l_di++)
  {
    o_dim_sizes[l_di] = std::stoi(l_dim_sizes_tmp[l_di]);
  }
}

void einsum_ir::frontend::EinsumExpressionAscii::parse_path(std::string const &i_expr_string,
                                                            std::vector<int64_t> &o_path)
{
  o_path.clear();

  std::string l_expr = i_expr_string;

  l_expr.erase(std::remove(l_expr.begin(),
                           l_expr.end(),
                           ' '),
               l_expr.end());
  l_expr.erase(std::remove(l_expr.begin(),
                           l_expr.end(),
                           '('),
               l_expr.end());
  l_expr.erase(std::remove(l_expr.begin(),
                           l_expr.end(),
                           ')'),
               l_expr.end());

  std::vector<std::string> l_path_tmp;
  split_string(l_expr,
               std::string(","),
               l_path_tmp);

  o_path.resize(l_path_tmp.size());
  for (std::size_t l_co = 0; l_co < l_path_tmp.size(); l_co++)
  {
    o_path[l_co] = std::stoi(l_path_tmp[l_co]);
  }
}

void einsum_ir::frontend::EinsumExpressionAscii::parse_dim_ids(std::string const &i_expr_string,
                                                               std::map<std::string, int64_t> &o_map_dim_name_to_id)
{
  o_map_dim_name_to_id.clear();
  std::vector<std::string> l_tensors;
  parse_tensors(i_expr_string, l_tensors); // daniel: o_tensor aus "[a,b,c],[a,c,d,b]->[a,b,d]" wird [0] = "a,b,c" und [1] = "a,c,d,b" [2] = "a,b,d"
  int64_t l_num_tensors = l_tensors.size();

  std::set<std::string> l_dim_names_set;
  for (int64_t l_te = 0; l_te < l_num_tensors; l_te++)
  {
    std::string l_tensor = l_tensors[l_te];

    std::vector<std::string> l_tensor_dim_names;
    split_string(l_tensor,
                 std::string(","),
                 l_tensor_dim_names);

    for (std::size_t l_di = 0; l_di < l_tensor_dim_names.size(); l_di++)
    {
      l_dim_names_set.insert(l_tensor_dim_names[l_di]);
    }
  }
  std::vector<std::string> l_dim_names(l_dim_names_set.begin(),
                                       l_dim_names_set.end());
  std::sort(l_dim_names.begin(),
            l_dim_names.end(),
            [](std::string const &a, std::string const &b)
            {
              try
              {
                return std::stoi(a) < std::stoi(b);
              }
              catch (...)
              {
                return a < b;
              }
            });

  for (std::size_t l_di = 0; l_di < l_dim_names.size(); l_di++)
  {
    o_map_dim_name_to_id.insert({l_dim_names[l_di], l_di});
  }
}

void einsum_ir::frontend::EinsumExpressionAscii::parse_dtype(std::string const &i_dtype_string,
                                                             data_t &o_dtype)
{
  if (i_dtype_string == "BF16")
  {
    o_dtype = einsum_ir::BF16;
  }
  else if (i_dtype_string == "FP32")
  {
    o_dtype = einsum_ir::FP32;
  }
  else if (i_dtype_string == "FP64")
  {
    o_dtype = einsum_ir::FP64;
  }
  else if (i_dtype_string == "CPX_FP32")
  {
    o_dtype = einsum_ir::FP32;
  }
  else if (i_dtype_string == "CPX_FP64")
  {
    o_dtype = einsum_ir::FP64;
  }
  else
  {
    o_dtype = einsum_ir::UNDEFINED_DTYPE;
  }
}

void einsum_ir::frontend::EinsumExpressionAscii::parse_ctype(std::string const &i_ctype_string,
                                                             complex_t &o_ctype)
{
  if (i_ctype_string == "BF16")
  {
    o_ctype = einsum_ir::REAL_ONLY;
  }
  else if (i_ctype_string == "FP32")
  {
    o_ctype = einsum_ir::REAL_ONLY;
  }
  else if (i_ctype_string == "FP64")
  {
    o_ctype = einsum_ir::REAL_ONLY;
  }
  else if (i_ctype_string == "CPX_FP32")
  {
    o_ctype = einsum_ir::BATCH_INNER;
  }
  else if (i_ctype_string == "CPX_FP64")
  {
    o_ctype = einsum_ir::BATCH_INNER;
  }
  else
  {
    o_ctype = einsum_ir::UNDEFINED_CTYPE;
  }
}

void einsum_ir::frontend::EinsumExpressionAscii::parse_loop_order(std::string const &i_loop_string,
                                                                  std::map<std::string, int64_t> const &i_map_dim_name_to_id,
                                                                  std::vector<int64_t> &o_loop_order)
{

  o_loop_order.clear();

  std::string l_loops = i_loop_string;

  l_loops.erase(std::remove(l_loops.begin(),
                            l_loops.end(),
                            ' '),
                l_loops.end());

  std::vector<std::string> l_loop_dims_tmp;
  split_string(l_loops,
               std::string(","),
               l_loop_dims_tmp);

  o_loop_order.reserve(l_loop_dims_tmp.size());
  for (std::size_t l_di = 0; l_di < l_loop_dims_tmp.size(); l_di++)
  {
    o_loop_order.push_back(i_map_dim_name_to_id.at(l_loop_dims_tmp[l_di]));
  }
}

void einsum_ir::frontend::EinsumExpressionAscii::relocate_k_dimension(std::string const &i_expression_string_std,
                                                                           std::string const &i_expression_string_schar,
                                                                           std::string &o_expression_string_std_layout,
                                                                           bool i_b_layout)
{
  o_expression_string_std_layout = i_expression_string_std;
  std::string copy_i_expression_string_schar = i_expression_string_schar;
  auto it_comma = std::find(i_expression_string_schar.begin(),
                            i_expression_string_schar.end(),
                            ',');
  auto it_arrow = std::find(i_expression_string_schar.begin(),
                            i_expression_string_schar.end(),
                            '-');
  // idx von comma und von arrow end:
  int64_t l_comma_idx = it_comma - i_expression_string_schar.begin();
  int64_t l_i = 0;
  for (l_i = l_comma_idx - 2; l_i >= 0; l_i--) // if l_comma_idx - 2 < 0 then left tensor is only "k" abort
  {
    char dim_name = i_expression_string_schar[l_i];
    auto found_right = std::find(it_comma + 1,
                                 it_arrow,
                                 dim_name);
    if (found_right != it_arrow) // found_right != it_arrow means that a k/c dimension was found in the right tensor

    {
      copy_i_expression_string_schar.erase(l_comma_idx - 1, 1);
      copy_i_expression_string_schar.insert(l_i + 1, 1, i_expression_string_schar[l_comma_idx - 1]);
      break;
    }
  }
  if (l_i < 0) // no k/c dimension found in left tensor : put the k dimension at the beginning
  {
    copy_i_expression_string_schar.erase(l_comma_idx - 1, 1);
    copy_i_expression_string_schar.insert(0, 1, i_expression_string_schar[l_comma_idx - 1]);
  }

  if (i_b_layout)
  {
    int64_t l_arrow_idx = it_arrow - i_expression_string_schar.begin();

    for (l_i = l_arrow_idx - 3; l_i > l_comma_idx; l_i--) // l_arrow_idx - 3 means "xnv->" is on x
    {
      char dim_name = i_expression_string_schar[l_i];

      // if dim_name is not in left OR || is in out then not a k dim
      if (std::find(i_expression_string_schar.begin(),
                    it_comma - 2,
                    dim_name) == (it_comma - 2) ||
          std::find(it_arrow + 2,
                    i_expression_string_schar.end(),
                    dim_name) != i_expression_string_schar.end())
      {
        break;
      }
    }
    if (l_i != l_arrow_idx - 3) //
    {
      std::rotate(copy_i_expression_string_schar.begin() + l_i + 1,
                  copy_i_expression_string_schar.begin() + l_arrow_idx - 2,
                  copy_i_expression_string_schar.begin() + l_arrow_idx - 1);
    }
  }

  einsum_ir::frontend::EinsumExpressionAscii::schar_to_standard(copy_i_expression_string_schar,
                                                                o_expression_string_std_layout);
}

void einsum_ir::frontend::EinsumExpressionAscii::set_bf16_flags(std::vector<std::string> const &i_tensor_dim_names_left,
                                                                     std::vector<std::string> const &i_tensor_dim_names_right,
                                                                     std::vector<std::string> const &i_tensor_dim_names_out,
                                                                     std::map<std::string, int64_t> const &i_map_dim_name_to_id,
                                                                     std::vector<int64_t> const &i_dim_sizes_vec,
                                                                     std::string const &i_expression_string_std,
                                                                     std::string const &i_expression_string_schar,
                                                                     std::vector<int64_t> &o_dim_ids_in_left_layout,
                                                                     std::vector<int64_t> &o_dim_ids_in_right_layout)
{
  o_dim_ids_in_left_layout.clear();
  o_dim_ids_in_right_layout.clear();
  // schaue ob die fastest dimension beim A Tensor eine k dimension ist mit dimension_size = 4
  std::string last_dim_name_left = i_tensor_dim_names_left.back();
  bool in_right = std::find(i_tensor_dim_names_right.begin(),
                            i_tensor_dim_names_right.end(),
                            last_dim_name_left) != i_tensor_dim_names_right.end();

  bool in_out = std::find(i_tensor_dim_names_out.begin(),
                          i_tensor_dim_names_out.end(),
                          last_dim_name_left) != i_tensor_dim_names_out.end();

  int64_t last_dim_id_left = i_map_dim_name_to_id.at(last_dim_name_left);
  int64_t last_dim_size_left = i_dim_sizes_vec[last_dim_id_left];

  // vorletzte Dimensions == m-Dimension?
  std::string possible_m_dim = i_tensor_dim_names_left[(i_tensor_dim_names_left.size() - 2)];
  bool is_m_dim = std::find(i_tensor_dim_names_right.begin(),
                            i_tensor_dim_names_right.end(),
                            possible_m_dim) == i_tensor_dim_names_right.end();

  if (last_dim_size_left == 4 && in_right && !in_out && is_m_dim)
  {
    std::string possible_n_dim = i_tensor_dim_names_right[(i_tensor_dim_names_right.size() - 2)];

    bool is_n_dim = std::find(i_tensor_dim_names_left.begin(),
                              i_tensor_dim_names_left.end(),
                              possible_n_dim) == i_tensor_dim_names_left.end();

    std::string l_expression_string_std_layout;
    bool b_layout = i_tensor_dim_names_right.back() == last_dim_name_left && is_n_dim;
    if (b_layout)
    {
      relocate_k_dimension(i_expression_string_std,
                                i_expression_string_schar,
                                l_expression_string_std_layout,
                                b_layout);
    }
    else
    { // only A layout
      relocate_k_dimension(i_expression_string_std,
                                i_expression_string_schar,
                                l_expression_string_std_layout,
                                0);
    }

    std::vector<std::string> l_tensors_layout;

    parse_tensors(l_expression_string_std_layout,
                  l_tensors_layout);

    std::vector<std::string> l_tensor_dim_names_left_layout;

    split_string(l_tensors_layout[0],
                 std::string(","),
                 l_tensor_dim_names_left_layout);

    for (std::size_t l_na = 0; l_na < l_tensor_dim_names_left_layout.size(); l_na++)
    {
      std::string l_dim_name_layout = l_tensor_dim_names_left_layout[l_na];
      int64_t l_dim_id_layout = i_map_dim_name_to_id.at(l_dim_name_layout);
      o_dim_ids_in_left_layout.push_back(l_dim_id_layout);
    }

    if (b_layout)
    {
      std::vector<std::string> l_tensor_dim_names_right_layout;

      split_string(l_tensors_layout[1],
                   std::string(","),
                   l_tensor_dim_names_right_layout);

      for (std::size_t l_na = 0; l_na < l_tensor_dim_names_right_layout.size(); l_na++)
      {
        std::string l_dim_name_layout = l_tensor_dim_names_right_layout[l_na];
        int64_t l_dim_id_layout = i_map_dim_name_to_id.at(l_dim_name_layout);
        o_dim_ids_in_right_layout.push_back(l_dim_id_layout);
      }
    }
  }
}
