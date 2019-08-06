//
// Created by kyngm on 7/08/2019.
//

#include "AdjacencyList.h"

//Constructor for adjacency list iterator
AdjacencyList<N,E>::const_iterator() {
  static AdjacencyList<N, E> dummy;
  *this = dummy.cend();
}

//Copy assignment for iterator
AdjacencyList<N,E>::const_iterator& operator=(const const_iterator& it) {
  this->outer_ = it.outer_;
  this->frontSentinel_ = it.frontSentinel_;
  this->backSentinel_ = it.backSentinel_;
  this->inner_ = it.inner_;
  return *this;
}

//Postfix decrement operator
AdjacencyList<N,E>::const_iterator operator--(int); {
  auto copy{*this};
  --(*this);
  return copy;
}

//Postfix increment operator
AdjacencyList<N,E>::const_iterator operator++(int) {
  auto copy{*this};
  ++(*this);
  return copy;
}
