#include "graph.h"

template<typename N, typename E>
gdwg::Graph<N, E>(void) {

}

template<typename N, typename E>
gdwg::Graph<N, E>(typename std::vector<N>::const_iterator start,
    typename std::vector<N>::const_iterator end) {

  std::vector<std::shared_ptr<N> > copies;
  while (start != end) {
    this->g.insert(std::make_shared<N>(*start), gdwg::AdjacencyList());
    start++;
  }


}

template<typename N, typename E>
gdwg::Graph<N, E>(typename std::vector<std::tuple<N, N, E>>::const_iterator,
    typename std::vector<std::tuple<N, N, E>>::const_iterator) {

    }