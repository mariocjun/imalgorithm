#ifndef ALGOVI_INCLUDE_UTILS_GRAPH_HPP_
#define ALGOVI_INCLUDE_UTILS_GRAPH_HPP_

#include <vector>
#include <list>
#include <unordered_map>
#include <memory>
#include <functional>
#include <queue>
#include "algorithm_coroutine.hpp"
#include "graph_item.hpp"

namespace ImAlgorithm {

template<typename T>
class Edge;

struct PairHash {
  template <class T1, class T2>
  std::size_t operator () (const std::pair<T1,T2>& p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second);

    // Mainly for demonstration purposes, i.e. works but is overly simple
    // In the real world, use sth. like boost.hash_combine
    return h1 ^ h2;
  }
};

template<typename T>
class Vertex {
 public:
  explicit Vertex(T data) : data_(data) {}

  [[nodiscard]] T getData() const { return data_; }
  void setData(T data) { data_ = data; }

  void setVisited(bool visited) { visited_ = visited; }
  [[nodiscard]] bool isVisited() const { return visited_; }

 private:
  T data_;
  T discoveryTime_;
  bool onStack_{};
    bool visited_{};
};

template<typename T>
class Edge : public graph_item<T> {
 public:
  explicit Edge(int from, int to, T capacity)
      : graph_item<T>(capacity), from_ID_(from), to_ID_(to) {}

  [[nodiscard]] int getFrom_ID() const { return from_ID_; }
  [[nodiscard]] int getTo_ID() const { return to_ID_; }

  T getWeight() const { return this->getData(); }

  T getData() const override { return this->data_; }
  void setData(T data) override { this->data_ = data; }

 private:
  int from_ID_;
  int to_ID_;
};

template<typename T>
class Graph {
 private:
  std::unordered_map<int, std::unique_ptr<Vertex<T>>> vertices_;
  std::unordered_map<std::pair<int, int>, std::unique_ptr<Edge<T>>, PairHash> edges_;
  std::unordered_map<int, std::list<int>> adjList_;
  bool isDirected_{};

 public:
  explicit Graph(bool isDirected = false)
      : isDirected_(isDirected) {}

  void addVertex(int vertexId, T data) {
    if (vertices_.find(vertexId) != vertices_.end()) {
      throw std::invalid_argument("Vertex already exists.");
    }
    vertices_[vertexId] = std::make_unique<Vertex<T>>(data);
    adjList_[vertexId] = std::list<int>();
  }

  void removeVertex(int vertexId) {
    vertices_.erase(vertexId);
    adjList_.erase(vertexId);
  }

  void addEdge(int from, int to, T capacity) {
    if (edgeExists(from, to)) {
      throw std::invalid_argument("Edge already exists.");
    }
    auto edge = std::make_unique<Edge<T>>(from, to, capacity);
    edges_.emplace(std::make_pair(from, to), std::move(edge));
    adjList_[from].push_back(to);
    if (!isDirected_) {
      adjList_[to].push_back(from);
    }
  }

  void removeEdge(int from, int to) {
    edges_.erase(from);
    adjList_[from].remove(to);
    if (!isDirected_) {
      adjList_[to].remove(from);
    }
  }

  bool vertexExists(int vertexId) const {
    return vertices_.find(vertexId) != vertices_.end();
  }

  bool edgeExists(int from, int to) const {
    return edges_.find(std::make_pair(from, to)) != edges_.end();
  }

  std::vector<std::pair<int, std::unique_ptr<Edge<T>>>> getEdgesSortedByWeight() const {
    std::vector<std::pair<int, std::unique_ptr<Edge<T>>>> edges(edges_.begin(), edges_.end());
    sort(edges.begin(), edges.end(),
         [](const auto& a, const auto& b) {
           return a.second->getWeight() < b.second->getWeight();
         });
    return edges;
  }

  void updateEdgeWeight(int from, int to, T newWeight) {
    if (edges_.find(std::make_pair(from, to)) != edges_.end()) {
      edges_[std::make_pair(from,to)]->setData(newWeight);
    } else {
      throw std::invalid_argument("Edge does not exist.");
    }
  }

  std::list<int> getNeighbors(int vertexId) const {
    if (adjList_.find(vertexId) != adjList_.end()) {
      return adjList_.at(vertexId);
    }
    return {};
  }

  const std::unordered_map<std::pair<int, int>, std::unique_ptr<Edge<int>>, PairHash>& getEdges() const { return edges_; }

  const std::unordered_map<int, std::unique_ptr<Vertex<T>>>& getVertices() const { return vertices_; }

  ~Graph() = default;
};

}

#endif //ALGOVI_INCLUDE_UTILS_GRAPH_HPP_