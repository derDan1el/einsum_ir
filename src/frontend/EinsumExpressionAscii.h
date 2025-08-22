#ifndef EINSUM_IR_FRONTEND_EINSUM_EXPRESSION_ASCII
#define EINSUM_IR_FRONTEND_EINSUM_EXPRESSION_ASCII

#include <string>
#include <vector>
#include <map>
#include "../constants.h"

namespace einsum_ir
{
  namespace frontend
  {
    class EinsumExpressionAscii;
  }
}

class einsum_ir::frontend::EinsumExpressionAscii
{
public:
  /**
   * Splits an input string by the given separation string.
   *
   * @param i_input input string.
   * @param i_separation separation string.
   * @param o_output output substrings.
   **/
  static void split_string(std::string const &i_input,
                           std::string const &i_separation,
                           std::vector<std::string> &o_output);

  /**
   * Converts an single-character input expression string to standardized format.
   *
   * The input expression string is expected to be in the following format:
   *   "tensor1,tensor2,...,tensorN->tensorN+1"
   * where the dimension ids are single characters.
   *
   * The output expression string will be in the following format:
   *   "[tensor1],[tensor2],...,[tensorN]->[tensorN+1]"
   * where the dimension ids are single characters separated by commas.
   *
   * Example:
   *  "iae,bf,dcba,cg,dh->hgfei"
   * will be converted to:
   *  "[i,a,e],[b,f],[d,c,b,a],[c,g],[d,h]->[h,g,f,e,i]".
   *
   * @param i_expr_string input expression string.
   * @param o_expr_string will be set to standardized expression string.
   **/
  static void schar_to_standard(std::string const &i_expr_string,
                                std::string &o_expr_string);

  /**
   * Converts a standardized expression string to single-character format.
   *
   * The input expression string is expected to be in the following format:
   *   "[tensor1],[tensor2],...,[tensorN]->[tensorN+1]"
   * where the dimension ids are separated by commas.
   *
   * The output expression string will be in the following format:
   *   "tensor1,tensor2,...,tensorN->tensorN+1"
   * where the dimension ids are single characters.
   *
   * Example:
   *   "[i,a,e],[b,f],[d,c,b,a],[c,g],[d,h]->[h,g,f,e,i]"
   * will be converted to:
   *   "IAE,BF,DCBA,CG,DH->HGFEI"
   *
   * @param i_expr_string input expression string.
   * @param o_expr_string will be set to single-character expression string.
   **/
  static void standard_to_schar(std::string const &i_expr_string,
                                std::string &o_expr_string);

  /**
   * Extracts the tensors from an einsum expression.
   *
   * The dimension sizes string is expected to be in one in the following format:
   *   "[tensor1],[tensor2],...,[tensorN]->[tensorN+1]"
   * where the dimension ids are separated by commas.
   *
   * Example #1:
   *   "[i,a,e],[b,f],[d,c,b,a],[c,g],[d,h]->[h,g,f,e,i]"
   * will be parsed to:
   *   ["i,a,e", "b,f", "d,c,b,a", "c,g", "d,h", "h,g,f,e,i"]
   *
   * Example #2:
   *
   *    a = 5, b = 6, c = 7
   *    d = 8, e = 9, f = 10,
   *    g = 11, h = 12, i = 13
   *
   *    "[13,5,9],[6,10],[8,7,6,5],[7,11],[8,12]->[12,11,10,9,13]"
   * will be parsed to:
   *  ["13,5,9", "6,10", "8,7,6,5", "7,11", "8,12", "12,11,10,9,13"]
   *
   * @param i_expr_string einsum expression.
   * @param o_tensors will be set to extracted tensors
   */
  static void parse_tensors(std::string const &i_expr_string,
                            std::vector<std::string> &o_tensors);

  /**
   * Extracts the dimension sizes from the dimension sizes string.
   *
   * The dimension sizes string is expected to be in the following format:
   *  "size1,size2,...,sizeN"
   * where the dimension of tensor with id i has the size size_i.
   *
   * Example:
   *   "32,8,4,2,16,64,8,8,8"
   * will be parsed to:
   *  [32, 8, 4, 2, 16, 64, 8, 8, 8]
   *
   * @param i_dim_sizes_string dimension sizes string.
   * @param o_dim_sizes will be set to extracted dimension sizes.
   **/
  static void parse_dim_sizes(std::string const &i_dim_sizes_string,
                              std::vector<int64_t> &o_dim_sizes);

  /**
   * Extracts the contraction path for an einsum expression.
   *
   * The path is expected to be in the following format:
   *   "(tensor1,tensor2),(tensor2,tensor3),...",
   * where tensor1 and tensor2 are the tensors to be contracted first,
   * tensor2 and tensor3 are the tensors to be contracted second, etc.
   *
   * Example:
   *   "(1,2),(2,3),(0,1),(0,1)"
   * will be parsed to:
   *  [1, 2, 2, 3, 0, 1, 0, 1]
   *
   * @param i_expr_string einsum expression.
   * @param o_path will be set to extracted path.
   **/
  static void parse_path(std::string const &i_expr_string,
                         std::vector<int64_t> &o_path);

  /**
   * Extracts the dimension ids from an einsum expression.
   * In the returned mapping, the key is the dimension name and the value is the dimension id.
   *
   * Example:
   *   "iae,bf,dcba,cg,dh->hgfei"
   * will be parsed to:
   *   'a' -> 0
   *   'b' -> 1
   *   'c' -> 2
   *   'd' -> 3
   *   'e' -> 4
   *   'f' -> 5
   *   'g' -> 6
   *   'h' -> 7
   *   'i' -> 8
   *
   * @param i_expr_string einsum expression.
   * @param o_map_dim_name_to_id will be set to extracted dimension ids.
   **/
  static void parse_dim_ids(std::string const &i_expr_string,
                            std::map<std::string, int64_t> &o_map_dim_name_to_id);

  /**
   * Extracts the data type from a string.
   * The input data type is expected to be in the following format:
   * "FP32" or "FP64" or "CPX_FP32" or "CPX_FP64"
   *
   * @param i_dtype_string data type string.
   * @param o_dtype will be set to extracted data type.
   **/
  static void parse_dtype(std::string const &i_dtype_string,
                          data_t &o_dtype);

  /**
   * Extracts the complex type from a string.
   * The input complex type is expected to be in the following format:
   * "FP32" or "FP64" or "CPX_FP32" or "CPX_FP64"
   *
   * @param i_ctype_string complex type string.
   * @param o_ctype will be set to extracted complex type.
   **/
  static void parse_ctype(std::string const &i_ctype_string,
                          complex_t &o_ctype);

  /**
   * Extractes the loop execution order from a string
   *
   * @param i_loop_string loop execution type string.
   * @param i_map_dim_name_to_id map from dimension name to dimension ids.
   * @param o_loop_order will be set to loop order as dimension ids
   **/
  static void parse_loop_order(std::string const &i_loop_string,
                               std::map<std::string, int64_t> const &i_map_dim_name_to_id,
                               std::vector<int64_t> &o_loop_order);

  /**
   *
   * This function relocates the  k dimension of size 4 in the left tensor in front of the first occurence of
   * a k-dimension, or a c-dimension or if not present, at the beginning of the einsum expression.
   *
   * examples:
   *
   * -(left tensor segment): [a,b,c,d,k] -> [k,a,b,c,d] if a,b,c,d are m dimensions
   * -(left tensor segment): [a,b,c,d,k] -> [a,b,k,c,d] if b ist a k-dimension and c,d are m dimensions
   * -(left tensor segment): [a,b,c,d,k] -> [a,b,c,k,d] if c ist a c-dimension and d is a m dimension
   * -(left tensor segment): [a,b,c,d,k] -> [a,b,c,d,k] if d is a k/c-dimension
   *
   *@param i_expression_string_std input expression string in standard format. [a,b,k],[d,e,k]->[d,e,a,b]
   *@param i_expression_string_schar input expression string in single-character format. abck,dek->deab
   *@param o_expression_string_std_layout output expression string in standard format with relocated k dimension
   *@param i_b_layout if true, the k dimension then the right tensor has B layout
   *
   * @note the goal ist to enlarge the primitve size of the m dimension by relocating the k dimension without
   * permuting the tensor by activating the layout-A Flag. (ONLY USECASE :BF16 input tensor with  k dimension)
   **/
  static void relocate_k_dimension(std::string const &i_expression_string_std,
                                        std::string const &i_expression_string_schar,
                                        std::string &o_expression_string_std_layout,
                                        bool i_b_layout = false);

  /**
   * Sets BF16 layout flags by checking if the left tensor has A/B layout.
   * Checks if the fastest dimension in the left tensor is a k dimension with size 4.
   *
   * @param i_tensor_dim_names_left dimension names of the left tensor
   * @param i_tensor_dim_names_right dimension names of the right tensor  
   * @param i_tensor_dim_names_out dimension names of the output tensor
   * @param i_map_dim_name_to_id mapping from dimension names to ids
   * @param i_dim_sizes_vec vector of dimension sizes
   * @param i_expression_string_std input expression string in standard format. [a,b,k],[d,e,k]->[d,e,a,b]
   * @param i_expression_string_schar input expression string in single-character format. abck,dek->deab
   * @param o_dim_ids_in_left_layout reordered dimension ids for the left tensor
   * @param o_dim_ids_in_right_layout reordered dimension ids for the right tensor
   **/
  static void set_bf16_flags(std::vector<std::string> const &i_tensor_dim_names_left,
                                  std::vector<std::string> const &i_tensor_dim_names_right,
                                  std::vector<std::string> const &i_tensor_dim_names_out,
                                  std::map<std::string, int64_t> const &i_map_dim_name_to_id,
                                  std::vector<int64_t> const &i_dim_sizes_vec,
                                  std::string const &i_expression_string_std,
                                  std::string const &i_expression_string_schar,
                                  std::vector<int64_t> &o_dim_ids_in_left_layout,
                                  std::vector<int64_t> &o_dim_ids_in_right_layout);
};

#endif