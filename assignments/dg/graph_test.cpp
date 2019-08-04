/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/

#include<vector>
#include<tuple>

#include "assignments/dg/graph.h"
#include "catch.h"

// TODO(students): Fill this in.
template<typename N, typename E>
void printValue(typename gdwg::Graph<N, E>::const_iterator it) {
  auto val = *it;
  std::cout << std::get<0>(val) << " " << std::get<1>(val)<< " " << std::get<2>(val) << "\n";
}


SCENARIO("increment/decrement iterators") {
  GIVEN("Some test graphs") {
      // Connected graph of 3 vertices
      std::string v1("A");
      std::string v2("B");
      std::string v3("C");

      auto ab = std::make_tuple(v1, v2, 1);
      auto ac = std::make_tuple(v1, v3, 0);
      auto ba = std::make_tuple(v2, v1, 0);
      auto bc = std::make_tuple(v2, v3, 0);
      auto ca = std::make_tuple(v3, v1, 1);
      auto cb = std::make_tuple(v3, v2, 2);
      std::vector<std::tuple<std::string, std::string, int>> edges{ab, ac, ba, bc, ca, cb};
      auto b = edges.cbegin();
      auto e = edges.cend();
      gdwg::Graph<std::string, int> g(b, e);

      REQUIRE(g.numNodes() == 3);
      REQUIRE(g.numEdges() == 6);


      THEN("post-increment should give first value: A->B (wt. 1)") {
        auto it = g.cbegin()++;
        REQUIRE(std::get<0>(*it) == v1);
        REQUIRE(std::get<1>(*it) == v2);
        REQUIRE(std::get<2>(*it) == 1);
      }

      THEN("pre-increment should give second value: A->C (wt. 0)") {
        auto it = ++g.cbegin();
        REQUIRE(std::get<0>(*it) == v1);
        REQUIRE(std::get<1>(*it) == v3);
        REQUIRE(std::get<2>(*it) == 0);
      }

      THEN("pre-decrement from the end should give the last value: C->B (wt. 2)") {
        auto it = --g.cend();
        REQUIRE(std::get<0>(*it) == v3);
        REQUIRE(std::get<1>(*it) == v2);
        REQUIRE(std::get<2>(*it) == 2);
      }

      THEN("increment/decrement should leave iterator the same") {
        auto lastValid = --(g.cend());
        auto it1 = lastValid;
        auto it2 = --lastValid;

        REQUIRE(++it2 == it1);
        REQUIRE(*it2 == *it1);
        REQUIRE(lastValid == --it1);
        REQUIRE(*lastValid == *it1);
      }

      //TODO: cases with empty adjacency lists + edge cases (i.e. transitioning with inner/outer)
  }
}
