//
// Created by kyngm on 7/08/2019.
//

#ifndef DG_ADJACENCYLIST_H
#define DG_ADJACENCYLIST_H


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

        const_iterator();
        const_iterator& operator=(const const_iterator& it);

        reference operator*() const;
        const_iterator& operator--();
        const_iterator operator--(int);
        const_iterator& operator++();
        const_iterator operator++(int);


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

    void DeleteNode(const N& node);
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
};



#endif //DG_ADJACENCYLIST_H
