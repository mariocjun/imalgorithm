#ifndef ALGOVI_INCLUDE_ALGORITHMS_GRAPHS_DFS_DFS_ALGORITHM_HPP_
#define ALGOVI_INCLUDE_ALGORITHMS_GRAPHS_DFS_DFS_ALGORITHM_HPP_

#include "utils/graph_algorithm.hpp"
#include "utils/graph.hpp"
#include <stack>
#include <unordered_map>
#include <vector>

template<typename T>
class DFSAlgorithm : public ImAlgorithm::GraphAlgorithm<std::vector<int>> {
 public:
  std::vector<int> run(const ImAlgorithm::Graph<T> &graph) override {
    // Inicializando estruturas de dados
    std::stack<int> stack;
    std::unordered_map<int, bool> visited;
    std::vector<int> order;

    // Obtendo todos os vértices do grafo
    auto vertices = graph.getVertices();

    // Marcando todos os vértices como não visitados
    for (auto &vertex : vertices) {
      visited[vertex.first] = false;
    }

    // Para todos os vértices, execute a DFS se ainda não foi visitado
    for (auto &vertex : vertices) {
      if (!visited[vertex.first]) {
        stack.push(vertex.first);

        while (!stack.empty()) {
          int currentVertexId = stack.top();
          stack.pop();

          if (!visited[currentVertexId]) {
            visited[currentVertexId] = true;
            order.push_back(currentVertexId);

            // Obtém todos os vizinhos do vértice atual
            auto neighbors = graph.getNeighbors(currentVertexId);
            for (auto &neighbor : neighbors) {
              if (!visited[neighbor]) {
                stack.push(neighbor);
              }
            }
          }
        }
      }
    }

    return order;  // Retorna a ordem dos vértices visitados
  }
};

#endif //ALGOVI_INCLUDE_ALGORITHMS_GRAPHS_DFS_DFS_ALGORITHM_HPP_
