#include<iostream>
#include<algorithm>

// Constructors
template<typename N, typename E>
gdwg::Graph<N,E>::Graph() {}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<N>::const_iterator start,
    typename std::vector<N>::const_iterator end) {
  while (start != end) {
    this->InsertNode(*start);
    start++;
  }
}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(
      typename std::vector<std::tuple<N, N, E>>::const_iterator start,
      typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
  while (start != end) {
    this->InsertNode(std::get<0>(*start));
    this->InsertNode(std::get<1>(*start));
    this->InsertEdge(std::get<0>(*start), std::get<1>(*start), std::get<2>(*start));
    start++;
  }
}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(std::initializer_list<N> l) {
  for (const auto& node : l) {
    InsertNode(node);
  }
}



template<typename N, typename E>
void gdwg::Graph<N, E>::Clear() {
  // Maybe have to clear each AdjacancyList stored inside
  this->g.clear();
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& n) const {
  shared_pointer_store<N> tmp = shared_pointer_store<N>(n);
  return this->g.count(tmp) != 0;
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::IsConnected(const N& from, const N& to) const {
  shared_pointer_store<N> src = shared_pointer_store<N>(from);
  shared_pointer_store<N> dest = shared_pointer_store<N>(to);
  const auto& val = g.find(src);
  if (val == g.cend() || g.count(dest) == 0) {
    throw std::runtime_error("Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
  }
  return val->second.hasEdge(to);
}

template<typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetNodes() const {
  std::vector<N> res;
  for (const auto& i : g) {
    res.push_back(*(i.first.ptr_));
  }
  return res;
}

template<typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetConnected(const N& node) const {
  shared_pointer_store<N> v = shared_pointer_store<N>(node);
  const auto val = g.find(v);
  if (val == g.cend()) {
    throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");
  }
  return val->second.GetNeighbours();
}

template<typename N, typename E>
std::vector<E> gdwg::Graph<N, E>::GetWeights(const N& from, const N& to) const {
  shared_pointer_store<N> src = shared_pointer_store<N>(from);
  shared_pointer_store<N> dest = shared_pointer_store<N>(to);
  const auto val = g.find(src);
  if (val == g.cend() || g.count(dest) == 0) {
    throw std::out_of_range("Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
  }
  return val->second.GetWeights(to);
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N& from, const N& to, const E& edge) {
  shared_pointer_store<N> src = shared_pointer_store<N>(from);
  shared_pointer_store<N> dest = shared_pointer_store<N>(to);
  if (this->g.count(src) == 0 || this->g.count(dest) == 0) {
    throw std::runtime_error("Cannot call Graph::InsertEdge when either src or dst node does not exist");
  }
  // Find the shared pointer we have already created
  auto store = (this->g.find(dest))->first;
  return this->g[from].addEdge(store, edge);
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
bool gdwg::Graph<N, E>::DeleteNode(const N& node) {
  shared_pointer_store<N> tmp = shared_pointer_store<N>(node);
  if (this->g.count(tmp) == 0) {
    return false;
  }
  // Find the shared pointer we have already created
  // auto store = (this->g.find(tmp))->first;
  this->g.erase(tmp);
  for (auto& it : this->g) {
    it.second.DeleteNode(node);
  }
  // store.reset();
  return true;
}

template<typename N, typename E>
bool gdwg::Graph<N,E>::Replace(const N& oldData, const N& newData) {
  shared_pointer_store<N> old_pointer = shared_pointer_store<N>(oldData);
  shared_pointer_store<N> new_pointer = shared_pointer_store<N>(newData);
  //Check that newData is not already a node in the graph
  if (this->g.count(new_pointer) != 0) {
    return false;
  }
  //Copy the outgoing edges from the old node
  gdwg::AdjacencyList<N,E> l{this->g[old_pointer]};
  //Set the outgoing edges for the new node to be the same as the edges from the old node
  this->g[new_pointer] = l;
  //Go through all other nodes in graph, copy ingoing edges to old node to ingoing edges to new
  //node
  for (auto& it : this->g) {
    auto curr_node = it.first;
    if (curr_node == old_pointer || curr_node == new_pointer) {
      continue;
    }
    //If there is an edge from current node to old node
    if (IsConnected(*curr_node, oldData)) {
      //Get the set of edge weights from current node to old node
      gdwg::AdjacencyList<N,E> temp_list = this->g[curr_node];
      std::set<shared_pointer_store<E>> edge_weights = temp_list.GetEdgeSet(old_pointer);
      //Make edges from current node to new node with the same edge weights
      this->g[curr_node].SetEdgeSet(new_pointer, edge_weights);
    }
  }
  //Delete the old node, along with all incoming and outgoing edges
  DeleteNode(oldData);
  return true;
}

template<typename N, typename E>
void gdwg::Graph<N,E>::MergeReplace(const N& oldData, const N& newData) {
  //TODO: Exception handling
  shared_pointer_store<N> old_pointer = shared_pointer_store<N>(oldData);
  shared_pointer_store<N> new_pointer = shared_pointer_store<N>(newData);
  //Iterate through neighbours of old node (nodes n where oldData -> n)
  for (auto& it : this->g[old_pointer].GetNeighbours()) {
    auto edge_set_old = g[old_pointer].GetEdgeSet(shared_pointer_store<N>(it));
    std::set<shared_pointer_store<E>> edge_set_new;
    if (IsConnected(newData, it)) {
      edge_set_new = g[new_pointer].GetEdgeSet(shared_pointer_store<N>(it));
      //Merge the two edge sets
      edge_set_new.insert(edge_set_old.begin(), edge_set_old.end());
      //Set the edge set to the merged one
    } else {
      //Make the new set equal to the old set
      edge_set_new = edge_set_old;
    }
    g[new_pointer].SetEdgeSet(shared_pointer_store<N>(it), edge_set_new);
  }
  //Iterate through nodes of the graph
  for (auto& it : this->g) {
    auto curr_node = it.first;
    if (curr_node == old_pointer || curr_node == new_pointer) {
      continue;
    }
    //If there is an edge from current node to old node
    if (IsConnected(*curr_node, oldData)) {
      std::set<shared_pointer_store<E>> edge_set_new;
      //Get the set of edge weights from current node to old node
      std::set<shared_pointer_store<E>> edge_set_old = this->g[curr_node].GetEdgeSet(old_pointer);
      //Make edges from current node to new node with the same edge weights
      if (IsConnected(*curr_node, newData)) {
        //If the node was already connected to the new node, then just merge the list of edges
        edge_set_new = this->g[curr_node].GetEdgeSet(new_pointer);
        edge_set_new.insert(edge_set_old.begin(), edge_set_old.end());
      } else {
        //If the node wasn't connected to the new node, connect it
        edge_set_new = edge_set_old;
      }
      this->g[curr_node].SetEdgeSet(new_pointer, edge_set_new);
    }
  }
  DeleteNode(oldData);
  return;
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::find(const N& n1, const N& n2, const E& edge) const {
  gdwg::shared_pointer_store<N> tmp(n1);
  auto outerVal = g.find(tmp);
  if (outerVal == g.end()) {
    return cend();
  }
  auto innerVal = outerVal->second.find(n2, edge);
  if (innerVal == outerVal->second.cend()) {
    return cend();
  }
  return {outerVal, g.cbegin(), g.cend(), innerVal};
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::erase(gdwg::Graph<N,E>::const_iterator it) {
  if (it == cend()) {
    return cend();
  }
  const auto& val = *it;
  const N& n1 = std::get<0>(val);
  const N& n2 = std::get<1>(val);
  const E&  e = std::get<2>(val);
  // auto it = find(n1, n2, e);
  if (find(n1, n2, e) == cend()) {
    return cend();
  }

  gdwg::shared_pointer_store<N> tmp(n1);
  auto outerVal = g.find(tmp);
  if (outerVal == g.end()) {
    return cend();
  }
  auto innerVal = outerVal->second.erase(n2, e);
  std::cout<<"currently "<< *(outerVal->first.ptr_)<<std::endl;
  while (innerVal == outerVal->second.cend()) {
    outerVal++;
    if (outerVal == g.cend()) {
      std::cout<<"return cend\n";
      return cend();
    }
    std::cout<<"moved to next node in graph now at: "<< *(outerVal->first.ptr_)<<std::endl;
    innerVal = outerVal->second.cbegin();
  }
  std::cout<<"return normally\n";
  return {outerVal, g.cbegin(), g.cend(), innerVal};
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cbegin() const {
  if (this->g.cbegin() == this->g.cend()) {
    std::cout<<"List empty yo\n";
    return {this->g.cend(), this->g.cend(), this->g.cend(), {}};
  }
  return {this->g.cbegin(), this->g.cbegin(), this->g.cend(), this->g.cbegin()->second.cbegin()};
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cend() const {
  if (this->g.cbegin() == this->g.cend()) {
    return {this->g.cend(), this->g.cend(), this->g.cend(), {}};
  }
  auto lastValid = --(this->g.cend());
  return {this->g.cend(), this->g.cbegin(), this->g.cend(), lastValid->second.cend()};
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator& gdwg::Graph<N, E>::const_iterator::operator++() {
  // std::cout<<"Incrementing graph inner, for "<< *(outer_->first.ptr_)<<std::endl;
  ++inner_;
  while (outer_ != backSentinel_ && inner_ == outer_->second.cend()) {
    // std::cout<<"Incrementing graph outer"<<std::endl;
    ++outer_;
    if (outer_ != backSentinel_) {
      inner_ = outer_->second.cbegin();
    // } else {
      // inner_ = {};
      // std::cout<<"Reached back sentinel\n";
    }
  }
  // std::cout<<"Incremented graph inner"<<std::endl;
  return *this;
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator& gdwg::Graph<N, E>::const_iterator::operator--() {
  // std::cout<<"Decrementing graph inner\n";
  if (outer_ == backSentinel_) {
    outer_--;
  }
  while (outer_ != frontSentinel_ && inner_ == outer_->second.cbegin() ) {
    if (outer_ != frontSentinel_) {
      --outer_;
      inner_ = outer_->second.cend();
    }
  }
  --inner_;
  // std::cout<<"Decremented graph inner\n";
  return *this;
}

template<typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator::reference gdwg::Graph<N, E>::const_iterator::operator*() const {
  auto neighbor = *inner_;
  return {*(outer_->first.ptr_), std::get<0>(neighbor), std::get<1>(neighbor)};
}

template<typename N, typename E>
typename gdwg::AdjacencyList<N, E>::const_iterator gdwg::AdjacencyList<N, E>::find(const N& n, const E& e) const {
  gdwg::shared_pointer_store<N> tmp(n);
  const auto outerVal = list.find(tmp);
  if (outerVal == list.end()) {
    return cend();
  }
  shared_pointer_store<E> tmpE(e);
  const auto innerVal = outerVal->second.find(tmpE);
  if (innerVal == outerVal->second.end()) {
    return cend();
  }
  return {outerVal, list.cbegin(), list.cend(), innerVal};
}

template<typename N, typename E>
typename gdwg::AdjacencyList<N, E>::const_iterator gdwg::AdjacencyList<N, E>::erase(const N& n, const E& e) {
  gdwg::shared_pointer_store<N> tmp(n);
  auto outerVal = list.find(tmp);
  if (outerVal == list.end()) {
    return cend();
  }
  shared_pointer_store<E> tmpE(e);
  auto innerVal = outerVal->second.find(tmpE);
  if (innerVal == outerVal->second.end()) {
    return cend();
  }
  // std::cout<<"Found edge to "<<*(outerVal->first.ptr_)<< std::endl;
  // std::cout<<"Current inner val: "<<*(innerVal->ptr_)<<std::endl;
  typename std::set<shared_pointer_store<E> >::const_iterator nextInnerVal = outerVal->second.erase(innerVal);
  auto nextOuterVal = outerVal;
  if (nextInnerVal == outerVal->second.end()) {
    if (outerVal->second.size() == 0) {
      // Check if outer is empty
      nextOuterVal = list.erase(outerVal);
    } else {
      // std::cout<<"set still has elements remaining"<<std::endl;
      // Still increment outer
      nextOuterVal = ++outerVal;
    }
    if (nextOuterVal == list.end()) {
      // std::cout<<"at end of list\n"<<std::endl;
      return cend();
    }
    // std::cout<<"deleted empty set, next val: "<< *(nextOuterVal->first.ptr_)<<std::endl;
    nextInnerVal = nextOuterVal->second.cbegin();
  }
  // std::cout<<"Next inner val: "<<(*nextInnerVal->ptr_)<<std::endl;
  return {nextOuterVal, list.cbegin(), list.cend(), nextInnerVal};
}

template<typename N, typename E>
typename gdwg::AdjacencyList<N, E>::const_iterator gdwg::AdjacencyList<N, E>::cbegin() const {
  if (this->list.cbegin() == this->list.cend()) {
    return cend();
  }
  return {this->list.cbegin(), this->list.cbegin(), this->list.cend(), this->list.cbegin()->second.cbegin()};
}

template<typename N, typename E>
typename gdwg::AdjacencyList<N, E>::const_iterator gdwg::AdjacencyList<N, E>::cend() const {
  if (this->list.cbegin() == this->list.cend()) {
    return {this->list.cend(), this->list.cend(), this->list.cend(), {}};
  }
  auto lastValid = --(this->list.cend());
  return {this->list.cend(), this->list.cbegin(), this->list.cend(), lastValid->second.cend()};
}

template<typename N, typename E>
typename gdwg::AdjacencyList<N, E>::const_iterator& gdwg::AdjacencyList<N, E>::const_iterator::operator++() {
  // std::cout<<"Incrementing list inner\n";
  ++inner_;
  while ( outer_ != backSentinel_ && inner_ == outer_->second.cend()) {
    // std::cout<<"incrementing list outer"<<std::endl;
    ++outer_;
    if (outer_ != backSentinel_) {
      inner_ = outer_->second.cbegin();
    }
  }
  // std::cout<<"Incremented list inner\n";
  return *this;
}

template<typename N, typename E>
typename gdwg::AdjacencyList<N, E>::const_iterator& gdwg::AdjacencyList<N, E>::const_iterator::operator--() {
  // std::cout<<"Decrementing list inner\n";
  if (outer_ == backSentinel_) {
    outer_--;
  }
  while (outer_ != frontSentinel_ && inner_ == outer_->second.cbegin()) {
    if (outer_ != frontSentinel_) {
      --outer_;
      inner_ = outer_->second.cend();
    }
  }
  --inner_;
  // std::cout<<"Decremented list inner\n";
  return *this;
}

template<typename N, typename E>
typename gdwg::AdjacencyList<N, E>::const_iterator::reference gdwg::AdjacencyList<N, E>::const_iterator::operator*() const {
  return {*(outer_->first.ptr_), *(inner_->ptr_)};
}

template<typename N, typename E>
bool gdwg::AdjacencyList<N, E>::addEdge(const shared_pointer_store<N>& v, const E& e) {
  // Guaranteed that v is already in the list
  shared_pointer_store<E> tmp{e};
  if (list[v].find(e) != list[v].cend()) {
    return false;
  }
  this->list[v].insert(e);
  return true;
}

template<typename N, typename E>
std::vector<N> gdwg::AdjacencyList<N, E>::GetNeighbours() const {
  std::vector<N> res;
  for (const auto& val : list) {
    res.push_back(*(val.first.ptr_));
  }
  return res;
}

template<typename N, typename E>
std::vector<E> gdwg::AdjacencyList<N, E>::GetWeights(const N& node) const {
  const gdwg::shared_pointer_store<N> v2(node);
  std::vector<E> res;
  const auto val = list.find(v2);
  if (val == list.end()) {
    return res;
  }
  for (const auto& edge : val->second) {
    res.push_back(*(edge.ptr_));
  }
  return res;
}

template<typename N, typename E>
void gdwg::AdjacencyList<N, E>::DeleteNode(const N& node) {
  shared_pointer_store<N> v(node);
  list.erase(v);
};