#include <iostream>
#include <vector>
#include <map>

#include <ATen/ATen.h>
#include "backend/UnaryTpp.h"

/**
 *
 * Dieser Code wurde erstellt um die UnaryTpp Permutation zu verstehen und um Support für BF16 zu testen/erstellen.
 * Es wurde ein Beispiel gehardcoded. Ein eigenes Beispiel kann erstellt werden,
 * indem man l_dim_sizes_map ,l_dim_ids_input und l_dim_ids_output ändert.
 *
 * Tensoren die weniger als 4 Dimensionen haben, werden in der Konsole ausgegeben.
 */
int main()
{

std::cout << "\033[96;4mEin Minimalbeispiel für die Permutation eines Tensors.\n\n"
          << "Hier wird das Ergebnis der Permutation durch die UnaryTpp Klasse\n"
          << "mit der permute() Funktion von Pytorch verglichen.\n\n\033[0m" << std::endl;

  /**
   * Es wird ein map benötigt die die Dimensions IDs ihren Größen zuordnet.
   */
  std::map<int64_t, int64_t> l_dim_sizes_map;
  l_dim_sizes_map[0] = 2;
  l_dim_sizes_map[1] = 3;
  l_dim_sizes_map[2] = 4;
  l_dim_sizes_map[3] = 5;

  /**
   * Es werden zwei Vektoren benötigt, die die Anordnung der Dimensionen durch ihre dim_ids beschreiben.
   */
  std::vector<int64_t> l_dim_ids_input = {0, 1, 2, 3};
  std::vector<int64_t> l_dim_ids_output = {3, 2, 1, 0};

  /**
   * l_sizes_/input/output ist für die PyTorch Tensoren notwendig, um sie zu erstellen.
   */
  std::vector<int64_t> l_sizes_input;
  for (int64_t dim_id : l_dim_ids_input)
  {
    l_sizes_input.push_back(l_dim_sizes_map[dim_id]);
  }

  std::vector<int64_t> l_sizes_output;
  for (int64_t dim_id : l_dim_ids_output)
  {
    l_sizes_output.push_back(l_dim_sizes_map[dim_id]);
  }

  /**
   * Tensoren so wie in bench_binary.cpp erstellen.
   * Das der Output Tensor mit randomwerten initialisiert wird muss nicht sein.
   */

  at::Tensor l_tensor_input = at::randn(at::IntArrayRef(l_sizes_input.data(), l_sizes_input.size()), at::ScalarType::BFloat16);
  at::Tensor l_tensor_output = at::zeros(at::IntArrayRef(l_sizes_output.data(), l_sizes_output.size()), at::ScalarType::BFloat16);

  if (l_sizes_input.size() <= 3)
  {

    std::cout << "═══════════════════════════════════ Inputtensor ═══════════════════════════════════" << std::endl;
    std::cout << l_tensor_input << std::endl
              << std::endl;
    std::cout << "════════════════════════════ Outputtensor vor der Permutation ════════════════════════════" << std::endl;
    std::cout << l_tensor_output << std::endl
              << std::endl;
  }
  /**
   * UnaryTpp Objekt erstellen.
   * Die init der UnaryTpp aufrufen.
   */

  einsum_ir::backend::UnaryTpp l_unary_permute;

  // 7. init() Aufruf
  l_unary_permute.init(l_dim_ids_input.size(),   // i_num_dims
                       &l_dim_sizes_map,         // i_dim_sizes (input)
                       l_dim_ids_input.data(),   // i_dim_ids_in
                       l_dim_ids_output.data(),  // i_dim_ids_out
                       einsum_ir::data_t::BF16,  // i_dtype_in
                       einsum_ir::data_t::BF16,  // i_dtype_comp
                       einsum_ir::data_t::BF16,  // i_dtype_out
                       einsum_ir::kernel_t::COPY // i_ktype_main
  );

  /**
   * compile muss aufgerufen werden, damit die Permutation ausgeführt werden kann.
   */
  einsum_ir::err_t l_err = l_unary_permute.compile();
  if (l_err != einsum_ir::err_t::SUCCESS)
  {
    std::cerr << "Fehler beim Kompilieren!" << std::endl;
    return EXIT_FAILURE;
  }

  /**
   * Die eval() Methode führt die Permutation aus.
   * Sie erwartet die Zeiger auf die Daten der Input- und Output-Tensoren.
   */
  l_unary_permute.eval(l_tensor_input.data_ptr(),
                       l_tensor_output.data_ptr());

  at::Tensor l_expected_output = l_tensor_input.permute(l_dim_ids_output);

  if (l_sizes_input.size() <= 3)
  {
    std::cout << "═══════════════════════════════════ UnaryTpp Ergebnis ═══════════════════════════════════" << std::endl;
    std::cout << l_tensor_output << std::endl
              << std::endl;

    std::cout << "═══════════════════════════════════ PyTorch Ergebnis ═══════════════════════════════════" << std::endl;
    std::cout << l_expected_output << std::endl
              << std::endl;
  }

  bool l_equal = at::equal(l_expected_output, l_tensor_output);
  std::cout << "Ergebnisse gleich: ";

  if (!l_equal)
  {
    std::cout << "\033[1;31mNEIN\033[0m" << std::endl;
  }
  else
  {
    std::cout << "\033[1;32mJA\033[0m" << std::endl;
  }

  return EXIT_SUCCESS;
}
