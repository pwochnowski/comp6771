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
void printValue(std::tuple<N, N, E> val) {
  std::cout << std::get<0>(val) << " " << std::get<1>(val)<< " " << std::get<2>(val) << "\n";
}


gdwg::Graph<std::string, int> sampleGraph() {
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
  return g;
}

TEST_CASE("Methods") {
  SECTION("clear graph") {
    gdwg::Graph<std::string, int> g = sampleGraph();
    REQUIRE(g.numNodes() != 0);
    g.Clear();
    REQUIRE(g.numNodes() == 0);
    // Check clearing empty graph is fine
    g.Clear();
    REQUIRE(g.numNodes() == 0);
  }

  SECTION("IsNode") {
    gdwg::Graph<std::string, int> g = sampleGraph();
    REQUIRE(g.IsNode(std::string("hello")));
    REQUIRE(!g.IsNode(std::string("")));
  }


  SECTION("delete node") {
    gdwg::Graph<std::string, int> g = sampleGraph();
    int oldN = g.numNodes();
    int oldE = g.numEdges();
    g.DeleteNode("are");
    REQUIRE(g.numNodes() == oldN-1);
    REQUIRE(g.numEdges() == oldE-3);
  }
}

TEST_CASE("friends") {
  SECTION("equals") {
    GIVEN("a graph") {
      gdwg::Graph<std::string, int> g = sampleGraph();
      THEN("Check equality holds for a copy constructed graph") {
        gdwg::Graph<std::string, int> gCopy(sampleGraph());
        REQUIRE(g == gCopy);
      }
      THEN("Check equality fails with empty graph") {
        gdwg::Graph<std::string, int> empty;
        REQUIRE(g != empty);
      }
    }
  }
}

TEST_CASE("iterators") {
  SECTION("iterator for empty graph") {
    gdwg::Graph<std::string, int> g;
    auto b = g.cbegin();
    auto e = g.cend();
    REQUIRE(b == e);
  }

  SECTION("increment/decrement iterators") {
    GIVEN("Some test graphs") {
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
      gdwg::Graph<std::string, int> g(b, e);

      REQUIRE(g.numNodes() == 3);
      REQUIRE(g.numEdges() == 7);

      THEN("increment from beginning through the entire graph") {
        std::vector<std::tuple<std::string, std::string, int>> exp{ab, ac1, ac2, ba, bc, ca, cb};
        int i = 0;
        for (auto it = g.cbegin(); it != g.cend(); it++) {
          REQUIRE(*it == exp[i++]);
        }
      }

      THEN("decrement from end through the entire graph") {
        std::vector<std::tuple<std::string, std::string, int>> exp{cb, ca, bc, ba, ac2, ac1, ab};
        int i = 0;
        auto it = g.cend();
        do {
          it--;
          REQUIRE(*it == exp[i++]);
        } while (it != g.cbegin());
      }

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
    }
  }
}