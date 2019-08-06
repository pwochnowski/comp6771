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
  std::string v1("A");
  std::string v2("B");
  std::string v3("C");

  auto ab = std::make_tuple(v1, v2, 1);
  auto ac2 = std::make_tuple(v1, v3, 2);
  auto ac1 = std::make_tuple(v1, v3, 0);
  auto ba = std::make_tuple(v2, v1, 0);
  auto bc = std::make_tuple(v2, v3, 0);
  auto ca = std::make_tuple(v3, v1, 1);
  auto cb = std::make_tuple(v3, v2, 2);
  std::vector<std::tuple<std::string, std::string, int>> edges{ab, ac2, ac1, ba, bc, ca, cb};

  auto b = edges.cbegin();
  auto e = edges.cend();
  gdwg::Graph<std::string, int> g_2(b, e);

  auto lastValid = --(g_2.cend());
  auto it1 = lastValid;
  auto it2 = --lastValid;
  auto temp_1 = *it1;
  auto temp_2 = *it2;
  std::cout << std::get<0>(temp_1) << " " << std::get<1>(temp_1) << " " << std::get<2>(temp_1) << "\n";
  std::cout << std::get<0>(temp_2) << " " << std::get<1>(temp_2) << " " << std::get<2>(temp_2) << "\n";
}
