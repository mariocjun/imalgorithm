#ifndef ALGOVI_INCLUDE_UTILS_GRAPH_HPP_
#define ALGOVI_INCLUDE_UTILS_GRAPH_HPP_

#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <iostream>
#include "../algorithm_coroutine.hpp"

using namespace std;

namespace ImAlgorithm {

template<typename T>
class Edge;

template<typename T>
class Vertex {
 public:
  explicit Vertex(T data) : data_(data) {}
  [[nodiscard]] T getData() const { return data_; }
  void setData(T data) { data_ = data; }
 private:
  T data_;
};

template<typename T>
class Edge {
 public:
  explicit Edge(Vertex<T> *from, Vertex<T> *to) : from_(from), to_(to) {}
  Vertex<T> *getFrom() const { return from_; }
  Vertex<T> *getTo() const { return to_; }
 private:
  Vertex<T> *from_;
  Vertex<T> *to_;
};

template<typename T>
class Graph {
 private:
  vector<Vertex<T> *> vertices_;
  vector<Edge<T> *> edges_;
  vector <vector<int>> adjMatrix_;
  vector<bool> visited_;
  vector <list<int>> adjList_;

 public:
  explicit Graph(int n) {
    vertices_.resize(n);
    for (int i = 0; i < n; ++i) {
      vertices_[i] = new Vertex(i);
    }
    adjMatrix_ = vector<vector<int>>(n, vector<int>(n, 0));
    adjList_ = vector<list<int>>(n);
    visited_ = vector<bool>(n, false);
  }

  void addEdge(int from, int to) {
    edges_.push_back(new Edge<T>(vertices_[from], vertices_[to]));
    adjMatrix_[from][to] = adjMatrix_[to][from] = 1;
    adjList_[from].push_back(to);
    adjList_[to].push_back(from);
  }

  void printAdjMatrix() {
    for (const auto &row : adjMatrix_) {
      for (const auto &value : row) {
        cout << value << " ";
      }
      cout << "\n";
    }
  }

  void printAdjList() {
    for (int i = 0; i < adjList_.size(); ++i) {
      cout << "Vertex " << i << ": ";
      for (int j : adjList_[i]) {
        cout << j << " ";
      }
      cout << "\n";
    }
  }

  void printGraph() {
    for (auto &edge : edges_) {
      cout << edge->getFrom()->getData() << " -> " << edge->getTo()->getData() << "\n";
    }
  }

  void resetVisited() {
    fill(visited_.begin(), visited_.end(), false);
  }

  AlgorithmCoroutine<Vertex<T> *> DFSGenerator(int start) {
    resetVisited();
    stack<int> s;
    s.push(start);
    visited_[start] = true;
    while (!s.empty()) {
      int v = s.top();
      s.pop();
      co_yield vertices_[v];
      for (int i = 0; i < adjMatrix_[v].size(); ++i) {
        if (adjMatrix_[v][i] == 1 && !visited_[i]) {
          s.push(i);
          visited_[i] = true;
        }
      }
    }
  }

  AlgorithmCoroutine<Vertex<T> *> BFSGenerator(int start) {
    resetVisited();
    queue<int> q;
    q.push(start);
    visited_[start] = true;
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      co_yield vertices_[v];
      for (int i = 0; i < adjMatrix_[v].size(); ++i) {
        if (adjMatrix_[v][i] == 1 && !visited_[i]) {
          q.push(i);
          visited_[i] = true;
        }
      }
    }
  }

  AlgorithmCoroutine<Vertex<T> *> DFSGeneratorRecursive(int start) {
    resetVisited();
    DFSRecursive(start);
  }

  AlgorithmCoroutine<Vertex<T> *> BFSGeneratorRecursive(int start) {
    resetVisited();
    BFSRecursive(start);
  }

  void DFSRecursive(int start) {
    visited_[start] = true;
    co_yield vertices_[start];
    for (int i = 0; i < adjMatrix_[start].size(); ++i) {
      if (adjMatrix_[start][i] == 1 && !visited_[i]) {
        DFSRecursive(i);
      }
    }
  }

  void BFSRecursive(int start) {
    queue<int> q;
    q.push(start);
    visited_[start] = true;
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      co_yield vertices_[v];
      for (int i = 0; i < adjMatrix_[v].size(); ++i) {
        if (adjMatrix_[v][i] == 1 && !visited_[i]) {
          q.push(i);
          visited_[i] = true;
        }
      }
    }
  }

  void DFS(int start) {
    resetVisited();
    stack<int> s;
    s.push(start);
    visited_[start] = true;
    while (!s.empty()) {
      int v = s.top();
      s.pop();
      cout << "Visited vertex: " << v << "\n";
      for (int i = 0; i < adjMatrix_[v].size(); ++i) {
        if (adjMatrix_[v][i] == 1 && !visited_[i]) {
          s.push(i);
          visited_[i] = true;
        }
      }
    }
  }

  void BFS(int start) {
    resetVisited();
    queue<int> q;
    q.push(start);
    visited_[start] = true;
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      cout << "Visited vertex: " << v << "\n";
      for (int i = 0; i < adjMatrix_[v].size(); ++i) {
        if (adjMatrix_[v][i] == 1 && !visited_[i]) {
          q.push(i);
          visited_[i] = true;
        }
      }
    }
  }

  ~Graph() {
    for (auto vertex : vertices_) {
      delete vertex;
    }
    for (auto edge : edges_) {
      delete edge;
    }
  }
};

}  // namespace ImAlgorithm

#endif //ALGOVI_INCLUDE_UTILS_GRAPH_HPP_
