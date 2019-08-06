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

    bool operator==(const shared_pointer_store<T>& r) const {
      return *ptr_ == *r.ptr_;
    }
    bool operator!=(const shared_pointer_store<T>& r) const { return !(*this == r); }

    bool operator < (const shared_pointer_store<T>& r) const {
        return *ptr_ < *r.ptr_;
    }
    const T& operator*() {
      return *ptr_;
    }
    std::shared_ptr<T> ptr_;
  };

  template<typename T>
  struct unique_pointer_store {
    unique_pointer_store(const std::unique_ptr<T>& ptr) : ptr_(ptr) {}
    unique_pointer_store(const T& entry) : ptr_(std::make_unique<T>(entry)) {}

    bool operator==(const unique_pointer_store<T>& r) const {
      return *ptr_ == *r.ptr_;
    }
    bool operator!=(const unique_pointer_store<T>& r) const { return !(*this == r); }

    bool operator < (const unique_pointer_store<T>& r) const {
        return *ptr_ < *r.ptr_;
    }
    const T& operator*() {
      return *ptr_;
    }
    std::unique_ptr<T> ptr_;
  };

  template<typename N, typename E>
  class AdjacencyList {
    public:
      AdjacencyList() : list() {};
      AdjacencyList(const gdwg::AdjacencyList<N, E>& other) : list{other.list} {};
      AdjacencyList(AdjacencyList<N,E>&& other) noexcept : list{std::move(other.list)} {};
      ~AdjacencyList() = default;

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
      AdjacencyList& operator=(const AdjacencyList& other) {
        this->list = other.list;
        return *this;
      }

      void DeleteNode(const N& node) {
        shared_pointer_store<N> v(node);
        list.erase(v);
      };
      bool addEdge(const shared_pointer_store<N>&, const E&);
      bool hasEdge(const N& node) const {
        shared_pointer_store<N> v(node);
        return list.count(v) != 0;
      }

      std::set<shared_pointer_store<E>> GetEdgeSet(const shared_pointer_store<N>& node) {
        return list[node];
      }
      void SetEdgeSet(const shared_pointer_store<N>& node,
          const std::set<shared_pointer_store<E>>& set) {
        list[node]=set;
      }

      std::vector<N> GetNeighbours() const;
      std::vector<E> GetWeights(const N& v) const;
      const_iterator find(const N&, const E&) const;
      const_iterator erase(const N&, const E&);

      int GetSize() const {
        int ret = 0;
        for (const auto& val : list) {
          ret += val.second.size();
        }
        return ret;
      }


      friend bool operator==(const gdwg::AdjacencyList<N, E>& l1, const gdwg::AdjacencyList<N, E>& l2) {
        if (l1.GetSize() != l2.GetSize()) {
          return false;
        }
        auto it1 = l1.list.cbegin();
        auto it2 = l2.list.cbegin();
        while (it1 != l1.list.cend()) {
          if (*it1 != *it2) {
            return false;
          }
          it1++;
          it2++;
        }
        return true;
      }
      friend bool operator!=(const gdwg::AdjacencyList<N, E>& l1, const gdwg::AdjacencyList<N, E>& l2) {return !(l1 == l2); }
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
      const_iterator begin() { return cbegin(); };
      const_iterator end() { return cend(); };
      // Iterator methods
      const_iterator cbegin() const;
      const_iterator cend() const;

      using const_reverse_iterator = std::reverse_iterator<gdwg::Graph<N,E>::const_iterator>;
      const_reverse_iterator rbegin() { return const_reverse_iterator{end()}; }
      const_reverse_iterator rend() { return const_reverse_iterator{begin()}; }
      const_reverse_iterator rbegin() const { return crbegin(); }
      const_reverse_iterator rend() const { return crend(); }
      const_reverse_iterator crbegin() const { return const_reverse_iterator{cend()}; }
      const_reverse_iterator crend() const { return const_reverse_iterator{cbegin()}; }


      Graph();
      Graph(typename std::vector<N>::const_iterator, typename std::vector<N>::const_iterator);
      Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator,
          typename std::vector<std::tuple<N, N, E>>::const_iterator
      );
      Graph(std::initializer_list<N> l);
      Graph(const gdwg::Graph<N, E>& other) : g{other.g} {};

      gdwg::Graph<N,E>& operator=(const gdwg::Graph<N,E>& other) {
        this->g = other.g;
        return *this;
      }
      Graph<N, E>(Graph<N, E>&& other) noexcept : g{std::move(other.g)} {};
      ~Graph<N, E>() = default;

      bool InsertNode(const N&);
      bool InsertEdge(const N&, const N&, const E&);
      bool DeleteNode(const N&);
      bool Replace(const N& oldData, const N& newData);
      void MergeReplace(const N& oldData, const N& newData);
      bool IsNode(const N&) const;
      void Clear();
      bool IsConnected(const N&, const N&) const;
      std::vector<N> GetNodes() const;
      std::vector<N> GetConnected(const N&) const;
      std::vector<E> GetWeights(const N&, const N&) const;
      const_iterator find(const N&, const N&, const E&) const;
      bool erase(const N& to, const N& from, const E& e) {
        auto it = find(to, from, e);
        if (it == end()) {
          return false;
        }
        erase(it);
        return true;
      }
      const_iterator erase(gdwg::Graph<N,E>::const_iterator it);



      // Friends
      friend bool operator==(const gdwg::Graph<N, E>& g1, const gdwg::Graph<N, E>& g2) {
        if (g1.numNodes() != g2.numNodes()) {
          return false;
        }
        auto it1 = g1.g.cbegin();
        auto it2 = g2.g.cbegin();
        while (it1 != g1.g.cend()) {
          if (*it1 != *it2) {
            return false;
          }
          it1++;
          it2++;
        }
        return true;
      }
      friend bool operator!=(const gdwg::Graph<N, E>& g1, const gdwg::Graph<N, E>& g2) {return !(g1 == g2); }
      friend std::ostream& operator<<(std::ostream& out, const gdwg::Graph<N, E>& g) {
        for (auto& i : g.g) {
          out << *(i.first.ptr_) << " (\n" << i.second << ")\n";
        }
        return out;
      }

      // Custom Helpers for tests
      int numNodes() const {
        return g.size();
      }

      int numEdges() {
        int res = 0;
        for (const auto& val : g) {
          res += val.second.GetSize();
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



#include "assignments/dg/graph.tpp"
// #include "graph.cpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
