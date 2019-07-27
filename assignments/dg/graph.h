#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include<memory>
#include<map>
#include<set>
#include<tuple>

namespace gdwg {

  template <typename N, typename E>
  class AdjacencyList {
    private:
      std::map<std::shared_ptr<N>, std::set<std::shared_ptr<E> > > list;
  };

  template <typename N, typename E>
  class Graph {
    public:
      class const_iterator {};
      Graph();
      Graph(typename std::vector<N>::const_iterator,typename std::vector<N>::const_iterator);
      Graph<N, E>(typename std::vector<std::tuple<N, N, E>>::const_iterator,
          typename std::vector<std::tuple<N, N, E>>::const_iterator);


    private:
      friend AdjacencyList<N, E>;
      std::map<std::shared_ptr<N>, AdjacencyList<N, E> > g;
  };


}  // namespace gdwg



#include "assignments/dg/graph.cpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
