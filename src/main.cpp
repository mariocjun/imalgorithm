#include "utils/graph.hpp"
#include <iostream>

int main() {
  // Criação de um grafo direcionado
  ImAlgorithm::Graph<int> graph(true);

  // Adicionando vertices
  for(int i = 1; i <= 5; i++) {
    graph.addVertex(i, i*10);
  }

  // Adicionando arestas
  graph.addEdge(1, 2, 5);
  graph.addEdge(2, 3, 10);
  graph.addEdge(3, 4, 15);
  graph.addEdge(4, 5, 20);
  graph.addEdge(5, 1, 25);

  // Impressão de vertices e suas arestas
  for(int i = 1; i <= 5; i++) {
    const auto &adjacent = graph.getNeighbors(i);
    for(const auto vertex : adjacent) {
      std::cout << "Edge from Vertex " << i << " to Vertex " << vertex << std::endl;
    }
  }

  // Atualização do peso da aresta
  graph.updateEdgeWeight(1, 2, 50);
  std::cout << "Updated edge weight from Vertex 1 to Vertex 2: "
            << graph.getEdges().at(std::make_pair(1,2))->getWeight() << std::endl;

  return 0;
}