
template<typename N, typename E>
gdwg::Graph<N,E>::Graph() {

}

// template<typename N, typename E>
// gdwg::Graph<N, E>::Graph(typename std::vector<N>::const_iterator start,
//     typename std::vector<N>::const_iterator end) {

//   std::vector<std::shared_ptr<N> > copies;
//   while (start != end) {
//     this->InsertNode(*start);
//     start++;
//   }
// }

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