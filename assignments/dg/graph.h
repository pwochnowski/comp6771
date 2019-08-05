#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include<memory>
#include<vector>
#include<string>
#include<map>
#include<iostream>
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
    const T& operator*() {
      return *ptr_;
    }
    std::shared_ptr<T> ptr_;
  };

  // template<typename B>
  // struct unique_pointer_store {
  //   unique_pointer_store(const std::unique_ptr<B>& ptr) : ptr_(ptr) {}
  //   unique_pointer_store(const B& entry) : ptr_(std::make_unique<B>(entry)) {}

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

      class const_iterator {
        public:
          using iterator_category = std::bidirectional_iterator_tag;
          using value_type = std::tuple<N, E>;
          using reference = std::tuple<const N&,const E&>;
          using pointer = std::tuple<N, E>*;
          using difference_type = int;

          const_iterator() {
            static AdjacencyList<N, E> dummy;
            *this = dummy.cend();
          }

          const_iterator& operator=(const const_iterator& it) {
            this->outer_ = it.outer_;
            this->frontSentinel_ = it.frontSentinel_;
            this->backSentinel_ = it.backSentinel_;
            this->inner_ = it.inner_;
            return *this;
          }

          reference operator*() const;
          const_iterator& operator--();
          const_iterator operator--(int) {
            auto copy{*this};
            --(*this);
            return copy;
          }
          const_iterator& operator++();
          const_iterator operator++(int) {
            auto copy{*this};
            ++(*this);
            return copy;
          }
          pointer operator->() const { return &(operator*()); }

          friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) {
            return (lhs.outer_ == rhs.outer_ &&
                (lhs.inner_ == rhs.inner_ || lhs.outer_ == rhs.backSentinel_ || lhs.outer_ == rhs.frontSentinel_)
            );
          }
          friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) {
            return !(lhs == rhs);
          }


        private:
          typename std::map<shared_pointer_store<N>, std::set<shared_pointer_store<E> > >::const_iterator outer_;
          typename std::map<shared_pointer_store<N>, std::set<shared_pointer_store<E> > >::const_iterator frontSentinel_;
          typename std::map<shared_pointer_store<N>, std::set<shared_pointer_store<E> > >::const_iterator backSentinel_;
          typename std::set<shared_pointer_store<E> >::const_iterator inner_;


          const_iterator(
              const typename std::map<shared_pointer_store<N>, std::set<shared_pointer_store<E> > >::const_iterator& outer,
              const typename std::map<shared_pointer_store<N>, std::set<shared_pointer_store<E> > >::const_iterator& frontSentinel,
              const typename std::map<shared_pointer_store<N>, std::set<shared_pointer_store<E> > >::const_iterator& backSentinel,
              const typename std::set<shared_pointer_store<E> >::const_iterator& inner
          ) : outer_{outer},
              frontSentinel_{frontSentinel},
              backSentinel_{backSentinel},
              inner_{inner} {}

          friend class AdjacencyList<N, E>;
      };


      void addEdge(const shared_pointer_store<N>&, const E&);

      friend std::ostream& operator<<(std::ostream& out, const gdwg::AdjacencyList<N, E>& l) {
        for (const auto& i : l.list) {
          for (const auto& j : i.second) {
            out << "\t" << *(i.first.ptr_) << " | " << *(j.ptr_)<<"\n";
          }
        }
        return out;
      }

      const_iterator cbegin() const;
      const_iterator cend() const;

      // Helper functions
      bool hasEdge(N v) {
        return list.find(v) != 0;
      }

      int GetSize() const {
        return list.size();
      }
    private:
      std::map<shared_pointer_store<N>, std::set<shared_pointer_store<E> > > list;
  };

  template <typename N, typename E>
  class Graph {
    public:
      class const_iterator {
        public:
          using iterator_category = std::bidirectional_iterator_tag;
          using value_type = std::tuple<N, N, E>;
          using reference = std::tuple<const N&,const N&,const E&>;
          using pointer = std::tuple<N, N, E>*;
          using difference_type = int;

          reference operator*() const;
          const_iterator& operator--();
          const_iterator operator--(int) {
            auto copy{*this};
            --(*this);
            return copy;
          }
          const_iterator& operator++();
          const_iterator operator++(int) {
            auto copy{*this};
            ++(*this);
            return copy;
          }
          pointer operator->() const { return &(operator*()); }

          friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) {
            // std::cout<<"(==) outer: "<< (lhs.outer_ == rhs.outer_)<< "\n(==) inner"
            return (lhs.outer_ == rhs.outer_ &&
                (lhs.inner_ == rhs.inner_ || lhs.outer_ == rhs.backSentinel_ || lhs.outer_ == rhs.frontSentinel_ )
            );
          }
          friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) { return !(lhs == rhs); }

        private:
          typename std::map<shared_pointer_store<N>, AdjacencyList<N, E> >::const_iterator outer_;
          const typename std::map<shared_pointer_store<N>, AdjacencyList<N, E> >::const_iterator frontSentinel_;
          const typename std::map<shared_pointer_store<N>, AdjacencyList<N, E> >::const_iterator backSentinel_;
          typename AdjacencyList<N, E>::const_iterator inner_;

          friend class Graph<N, E>;
          const_iterator(
              typename std::map<shared_pointer_store<N>, AdjacencyList<N, E> >::const_iterator outer,
              const typename std::map<shared_pointer_store<N>, AdjacencyList<N, E> >::const_iterator frontSentinel,
              const typename std::map<shared_pointer_store<N>, AdjacencyList<N, E> >::const_iterator backSentinel,
              typename AdjacencyList<N, E>::const_iterator inner
          ) : outer_{outer},
              frontSentinel_{frontSentinel},
              backSentinel_{backSentinel},
              inner_{inner} {}

      };

      // Iterator methods
      const_iterator cbegin() const;
      const_iterator cend() const;
      // const_reverse_iterator crbegin();


      Graph();
      Graph(typename std::vector<N>::const_iterator&, typename std::vector<N>::const_iterator&);
      Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator& ,
          typename std::vector<std::tuple<N, N, E>>::const_iterator&
      );

      bool InsertNode(const N&);
      bool InsertEdge(const N&, const N&, const E&);

      friend std::ostream& operator<<(std::ostream& out, const gdwg::Graph<N, E>& g) {
        for (auto& i : g.g) {
          out << *(i.first.ptr_) << " (\n" << i.second << ")\n";
        }
        return out;
      }

      // Custom Helpers for tests
      int numNodes() {
        return g.size();
      }

      int numEdges() {
        int res = 0;
        for (const auto& i : g) {
          res += i.second.GetSize();
        }
        return res;
      }



    private:
      friend AdjacencyList<N, E>;
      std::map<shared_pointer_store<N>, AdjacencyList<N, E> > g;
  };

  template<typename N, typename E>
  std::ostream& operator<<(std::ostream& out, const gdwg::AdjacencyList<N, E>& l) {
    for (auto& i : l.list) {
      for (auto& j : i.second) {
        out<< "\t"<< i.first<< " | "<< j<< "\n";
      }
    }
    return out;
  }




}  // namespace gdwg



#include "assignments/dg/graph.cpp"
// #include "graph.cpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
