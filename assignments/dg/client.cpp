#include <iostream>
#include <string>
#include <tuple>

#include "assignments/dg/graph.h"
// #include "graph.hpp"

// Note: At the moment, there is no client.sampleout. Please do your own testing

int main() {
  gdwg::Graph<std::string, int> g;
  g.InsertNode("hello");
  g.InsertNode("how");
  g.InsertNode("are");
  g.InsertNode("you?");

  g.InsertEdge("hello", "how", 5);
  g.InsertEdge("hello", "are", 8);
  g.InsertEdge("hello", "are", 2);

  g.InsertEdge("how", "you?", 1);
  g.InsertEdge("how", "hello", 4);

  g.InsertEdge("are", "you?", 3);

  std::cout << g << '\n';
//  g.Replace("are", "ar");
//  std::cout << g << '\n';
//
//  g.Replace("ar", "are");
//  std::cout << g << '\n';

  g.MergeReplace("how", "are");
  std::cout << g << '\n';

  decltype(g)::const_iterator it = g.cbegin();
  auto val = *it;
  std::cout << std::get<0>(val) << " " << std::get<1>(val)<< " " << std::get<2>(val) << "\n";
  it++;
  // auto v2 = *it;
  // std::cout << std::get<0>(v2) << " " << std::get<1>(v2)<< " " << std::get<2>(v2) << "\n";

  // gdwg::Graph<std::string, int> g2{g};

  // std::cout << g2 << "\n";

  // This is a structured binding.
  // https://en.cppreference.com/w/cpp/language/structured_binding
  // It allows you to unpack your tuple.
  // for (const auto& [from, to, weight] : g) {
  //   std::cout << from << " -> " << to << " (weight " << weight << ")\n";
  // }
  //  g.Replace("are", "4re");
  //  std::cout << g << '\n';
}
