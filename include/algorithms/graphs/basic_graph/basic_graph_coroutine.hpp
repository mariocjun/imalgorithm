#include "algorithm_coroutine.hpp" // Assegure-se que esse arquivo esteja no seu caminho de inclusão

namespace ImAlgorithm::basic_graph {

enum BasicGraphStepFlag : int { NoOp = 0, HighlightCmp = 1, HighlightSwap = 2 };
using BasicGraphStepFlags = int;
using BasicGraphCoroutine = AlgorithmCoroutine<BasicGraphStepFlags>;

BasicGraphCoroutine basicGraphAlgorithm() noexcept {
  // Representando a lógica do algoritmo do grafo como uma co-rotina.
  // Esse é apenas um exemplo muito simples, sem o contexto expressivo de um algoritmo de gráfico real.

  while (true) {
    co_yield HighlightCmp; // Suponha que estamos comparando dois nós
    // Faça alguma operação de comparação aqui

    co_yield HighlightSwap; // Suponha que estamos trocando dois nós
    // Faça alguma operação de troca aqui
  }

  co_yield NoOp; // Indica o fim do algoritmo
}

}  // namespace ImAlgorithm::basic_graph