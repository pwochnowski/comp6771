#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include<memory>
#include<map>
#include<set>
#include<utility>
#include<tuple>

namespace gdwg {

  template<typename T>
  struct shared_pointer_store {
    shared_pointer_store(const std::shared_ptr<T>& ptr) : ptr_(ptr) {}
    shared_pointer_store(const T& entry) : ptr_(std::make_shared<T>(entry)) {}

    bool operator < (const shared_pointer_store<T>& r) const {
        return *ptr_ < *r.ptr_;
    }
    std::shared_ptr<T> ptr_;
  };

  // template<typename B>
  // struct unique_pointer_store {
  //   unique_pointer_store(const B& entry) {
  //     this->ptr_ = std::unique_ptr<B>(new B(entry));
  //   }

  //   bool operator < (const unique_pointer_store<B>& r) const {
  //       return *ptr_ < *r.ptr_;
  //   }
  //   std::unique_ptr<B> ptr_;
  // };

  template<typename N, typename E>
  class AdjacencyList {
    public:
      AdjacencyList() : list() {};
      ~AdjacencyList() {};


      void addEdge(const shared_pointer_store<N>&, const E&);
      // friend std::ostream& operator<<(std::ostream&, const AdjacencyList<N, E>&);
      friend std::ostream& operator<<(std::ostream& out, const gdwg::AdjacencyList<N, E>& l) {
        for (auto& i : l.list) {
          for (auto& j : i.second) {
            out << "\t" << *(i.first.ptr_) << " | " << *(j.ptr_)<<"\n";
          }
        }
        return out;
      }
    private:
      std::map<shared_pointer_store<N>, std::set<shared_pointer_store<E> > > list;
  };

  template <typename N, typename E>
  class Graph {
    public:
      class const_iterator {};
      Graph();
      // Graph(typename std::vector<N>::const_iterator,typename std::vector<N>::const_iterator);
      // Graph<N, E>(typename std::vector<std::tuple<N, N, E>>::const_iterator,
      //     typename std::vector<std::tuple<N, N, E>>::const_iterator);

      bool InsertNode(const N&);
      bool InsertEdge(const N&, const N&, const E&);
      // friend std::ostream& operator<<(std::ostream&, const Graph<N, E>&);
      friend std::ostream& operator<<(std::ostream& out, const gdwg::Graph<N, E>& g) {
        for (auto& i : g.g) {
          out << *(i.first.ptr_) << " (\n" << i.second << ")\n";
        }
        return out;
      }


    private:
      friend AdjacencyList<N, E>;
      std::map<shared_pointer_store<N>, AdjacencyList<N, E> > g;
  };

  template<typename N, typename E>
  std::ostream& operator<<(std::ostream& out, const gdwg::AdjacencyList<N, E>& l) {
    for (auto& i : l.list) {
      for (auto& j : i.second) {
        out << "\t" << i.first << " | " << j<<"\n";
      }
    }
    return out;
  }




}  // namespace gdwg



#include "assignments/dg/graph.cpp"
// #include "graph.cpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
