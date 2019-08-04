
template<typename N, typename E>
gdwg::Graph<N,E>::Graph() {

}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<N>::const_iterator start,
    typename std::vector<N>::const_iterator end) {

  std::vector<std::shared_ptr<N> > copies;
  while (start != end) {
    this->InsertNode(*start);
    start++;
  }
}

// template<typename N, typename E>
// gdwg::Graph<N, E>::Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator start,
//     typename std::vector<std::tuple<N, N, E>>::const_iterator end) {

// }

template<typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N& from, const N& to, const E& edge) {
  if (this->g.count(from) == 0 || this->g.count(to) == 0) {
    return false;
  }
  // Find the shared pointer we have already created
  auto store = (this->g.find(shared_pointer_store<N>{to}))->first;
  this->g[from].addEdge(store, edge);
  return true;
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N& node) {
  shared_pointer_store<N> tmp = shared_pointer_store<N>(node);
  if (this->g.count(tmp) != 0) {
    return false;
  }
  gdwg::AdjacencyList<N,E> l;
  this->g[tmp] = l;
  return true;
}

template<typename N, typename E>
void gdwg::AdjacencyList<N, E>::addEdge(const shared_pointer_store<N>& v, const E& e) {
  // Guaranteed that v is already in the list
  this->list[v].insert(shared_pointer_store<E>(e));
}


template<typename N, typename E>
std::ostream& operator<<(std::ostream& out, const gdwg::AdjacencyList<N, E>& l) {
  for (auto& i : l.list) {
    for (auto& j : i.second) {
      out << "\t" << i.first << " | " << j<<"\n";
    }
  }
  return out;
}

template<typename N, typename E>
std::ostream& operator<<(std::ostream& out, const gdwg::Graph<N, E>& g) {
  for (auto& i : g.g) {
    out << i.first<< " (\n" << i.second << ")\n";
  }
  return out;
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cbegin() const {
  if (this->g.cbegin() != this->g.cend()) {
    return {this->g.cbegin(), this->g.cend(), this->g.cbegin()->second.cbegin()};
  }
  // Crashes when called on empty graph
  // TODO: fix by writing a default constructor for AdjacencyList::const_iterator
  return {this->g.cbegin(), this->g.cend(), this->g.cbegin()->second.cbegin()};
  // return {this->g.cend(), this->g.cend(), {}};
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cend() const {
   typename std::map<shared_pointer_store<N>, AdjacencyList<N, E> >::const_iterator outerEnd = this->g.cend();
  return {outerEnd, outerEnd, {}};
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator& gdwg::Graph<N, E>::const_iterator::operator++() {
  ++inner_;
  if (inner_ == outer_->second.cend()) {
    ++outer_;
    if (outer_ != sentinel_) {
      inner_ = outer_->second.cbegin();
    }
  }
  return *this;
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator::reference gdwg::Graph<N, E>::const_iterator::operator*() const {
  auto neighbor = *inner_;
  return {*(outer_->first.ptr_), std::get<0>(neighbor), std::get<1>(neighbor)};
}

template<typename N, typename E>
typename gdwg::AdjacencyList<N, E>::const_iterator gdwg::AdjacencyList<N, E>::cbegin() const {
  // What if the first element is empty?
  if (this->list.cbegin() != this->list.cend()) {
    return {this->list.cbegin(), this->list.cend(), this->list.cbegin()->second.cbegin()};
  }
  return cend();
}

template<typename N, typename E>
typename gdwg::AdjacencyList<N, E>::const_iterator gdwg::AdjacencyList<N, E>::cend() const {
  return {this->list.cend(), this->list.cend(), {}};
}

template<typename N, typename E>
typename gdwg::AdjacencyList<N, E>::const_iterator& gdwg::AdjacencyList<N, E>::const_iterator::operator++() {
   ++inner_;
  if (inner_ == outer_->second.cend()) {
    ++outer_;
    if (outer_ != sentinel_) {
      inner_ = outer_->second.cbegin();
    }
  }
  return *this;
}

template<typename N, typename E>
typename gdwg::AdjacencyList<N, E>::const_iterator::reference gdwg::AdjacencyList<N, E>::const_iterator::operator*() const {
  return {*(outer_->first.ptr_), *(inner_->ptr_)};
}