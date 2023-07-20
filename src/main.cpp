#include <iostream>
#include <memory>
#include "../include/utils/graph.hpp"

using namespace ImAlgorithm;

int main() {
    cout << "Criando grafo com 5 vértices e 9 arestas...\n" << endl;
    Graph<int> graph(5);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 4);
    graph.addEdge(3, 4);
    graph.addEdge(4, 0);
    graph.addEdge(4, 1);
    graph.addEdge(4, 2);
    graph.addEdge(4, 3);

    cout << "Gerando DFS com pilha..." << endl;
    graph.DFS(0);
    cout << endl;

    cout << "Gerando BFS com fila..." << endl;
    graph.BFS(0);
    cout << endl;

    cout << "Imprimindo lista de adjacência..." << endl;
    graph.printAdjList();
    cout << endl;

    cout << "Imprimindo matriz de adjacência..." << endl;
    graph.printAdjMatrix();
    cout << endl;


    return 0;
}