#ifndef ALGOVI_INCLUDE_UTILS_GRAPHALGORITHM_HPP_
#define ALGOVI_INCLUDE_UTILS_GRAPHALGORITHM_HPP_

#include "graph.hpp"

namespace ImAlgorithm {

template<typename T>
class GraphAlgorithm {
 public:
  virtual T run(const Graph<T> &graph) = 0;  // Método virtual puro
  virtual ~GraphAlgorithm() = default; // Destrutor virtual
};

}

#endif //ALGOVI_INCLUDE_UTILS_GRAPHALGORITHM_HPP_