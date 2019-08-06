/*

  == Explanation and rational of testing ==
  We designed tests by checking the return values of non-void functions,
  as well as using helper functions to check the internal state of the Graph
  to ensure that only desirable changes have occurred.

  To ensure test coverage we have tests for every function in the spec and
  test it with as many differing return values as possible


  We test exceptions with REQUIRE_THROWS_WITH and REQUIRE_THROWS_AS

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

TEST_CASE("Constructors") {
  SECTION("Vector of nodes") {
    std::vector<int> v{1, 2 ,3};
    gdwg::Graph<int, int> g{v.begin(), v.end()};
    REQUIRE(g.numNodes() == 3);
    REQUIRE(g.numEdges() == 0);
    REQUIRE(g.IsNode(1));
    REQUIRE(g.IsNode(2));
    REQUIRE(g.IsNode(3));
  }
  SECTION("Vector of edges") {
    std::vector<std::tuple<std::string, std::string, int>> v{
        {"one", "two", 1}, {"two", "three", 0}, {"one", "three", 3}};
    gdwg::Graph<std::string, int> g{v.begin(), v.end()};
    REQUIRE(g.numNodes() == 3);
    REQUIRE(g.numEdges() == 3);
    REQUIRE(g.IsNode("one"));
    REQUIRE(g.IsNode("two"));
    REQUIRE(g.IsNode("three"));
    REQUIRE(g.find("one", "two", 1) != g.cend());
    REQUIRE(g.find("two", "three", 0) != g.cend());
    REQUIRE(g.find("one", "three", 3) != g.cend());
  }
  SECTION("initializer list") {
    gdwg::Graph<int, int> g{1, 2, 3};
    REQUIRE(g.numNodes() == 3);
    REQUIRE(g.numEdges() == 0);
    REQUIRE(g.IsNode(1));
    REQUIRE(g.IsNode(2));
    REQUIRE(g.IsNode(3));
  }
  SECTION("Copy constructor") {
    // By the previous test this is enough to check equivalence with the original graph
    gdwg::Graph<int, int> g{1, 2, 3};
    auto copy{g};
    REQUIRE(copy.numNodes() == 3);
    REQUIRE(copy.numEdges() == 0);
    REQUIRE(copy.IsNode(1));
    REQUIRE(copy.IsNode(2));
    REQUIRE(copy.IsNode(3));
  }
  SECTION("Move Constructor") {
    std::vector<std::tuple<int, int, int>> v = {{1, 2, 0}, {1, 3, 1}, {2, 3, 2}};
    gdwg::Graph<int, int> g{v.begin(), v.end()};
    auto g2{std::move(g)};
    REQUIRE(!g.IsNode(1));
    REQUIRE(!g.IsNode(2));
    REQUIRE(!g.IsNode(3));
    REQUIRE(g.find(1, 2, 0) == g.cend());
    REQUIRE(g.find(1, 3, 1) == g.cend());
    REQUIRE(g.find(2, 3, 2) == g.cend());
    REQUIRE(g2.IsNode(1));
    REQUIRE(g2.IsNode(2));
    REQUIRE(g2.IsNode(3));
    REQUIRE(g2.find(1, 2, 0) != g.cend());
    REQUIRE(g2.find(1, 3, 1) != g.cend());
    REQUIRE(g2.find(2, 3, 2) != g.cend());
  }
}

TEST_CASE("Assignment") {
  SECTION("copy assignment") {
    gdwg::Graph<int, int> g{1, 2, 3};
    auto copy = g;
    REQUIRE(copy.numNodes() == 3);
    REQUIRE(copy.numEdges() == 0);
    REQUIRE(copy.IsNode(1));
    REQUIRE(copy.IsNode(2));
    REQUIRE(copy.IsNode(3));
  }

  SECTION("move assignment") {
    std::vector<std::tuple<int, int, int>> v = {{1, 2, 0}, {1, 3, 1}, {2, 3, 2}};
    gdwg::Graph<int, int> g{v.begin(), v.end()};
    auto g2 = std::move(g);
    REQUIRE(!g.IsNode(1));
    REQUIRE(!g.IsNode(2));
    REQUIRE(!g.IsNode(3));
    REQUIRE(g.find(1, 2, 0) == g.cend());
    REQUIRE(g.find(1, 3, 1) == g.cend());
    REQUIRE(g.find(2, 3, 2) == g.cend());
    REQUIRE(g2.IsNode(1));
    REQUIRE(g2.IsNode(2));
    REQUIRE(g2.IsNode(3));
    REQUIRE(g2.find(1, 2, 0) != g.cend());
    REQUIRE(g2.find(1, 3, 1) != g.cend());
    REQUIRE(g2.find(2, 3, 2) != g.cend());
  }
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

  SECTION("InsertNode") {
    gdwg::Graph<int, int> g;
    int i = 1;
    THEN("insert once") {
      REQUIRE(g.InsertNode(i));
      REQUIRE(g.IsNode(i));
      REQUIRE(g.numNodes() == 1);
    }
    THEN("insert twice") {
      REQUIRE(g.InsertNode(i));
      REQUIRE(!g.InsertNode(i));
      REQUIRE(g.IsNode(i));
      REQUIRE(g.numNodes() == 1);
    }
  }

  SECTION("InsertEdge") {
    gdwg::Graph<int, int> g;
    int a = 1;
    int b = 2;
    g.InsertNode(a);
    g.InsertNode(b);
    REQUIRE(g.numEdges() == 0);
    THEN("insert single edge") {
      REQUIRE(g.InsertEdge(a, b, 0));
      REQUIRE(g.numNodes() == 2);
      REQUIRE(g.numEdges() == 1);
    }
    THEN("insert reflexive edge") {
      REQUIRE(g.InsertEdge(a, a, 0));
      REQUIRE(g.numNodes() == 2);
      REQUIRE(g.numEdges() == 1);
    }
    THEN("insert same edge twice") {
      REQUIRE(g.InsertEdge(a, b, 0));
      REQUIRE(!g.InsertEdge(a, b, 0));
    }
    THEN("insert edge to nonexistent node") {
      const auto err = "Cannot call Graph::InsertEdge when either src or dst node does not exist";
      REQUIRE_THROWS_AS(g.InsertEdge(5, 4, 0), std::runtime_error);
      REQUIRE_THROWS_AS(g.InsertEdge(a, 5, 0), std::runtime_error);
      REQUIRE_THROWS_AS(g.InsertEdge(4, b, 0), std::runtime_error);
      REQUIRE_THROWS_WITH(g.InsertEdge(5, 4, 0), err);
      REQUIRE_THROWS_WITH(g.InsertEdge(a, 5, 0), err);
      REQUIRE_THROWS_WITH(g.InsertEdge(4, b, 0), err);
    }
  }

  SECTION("erase") {
    GIVEN("a graph") {
      gdwg::Graph<std::string, int> g = sampleGraph();

      THEN("standard case") {
        auto val = g.erase(g.find(std::string("hello"), std::string("are"), 2));
        auto exp = std::make_tuple(std::string("hello"), std::string("are"), 8);
        REQUIRE(*val == exp);
      }

      THEN("deleting the last edge of the first of two neighbour") {
        auto val = g.erase(g.find(std::string("hello"), std::string("are"), 8));
        auto exp = std::make_tuple(std::string("hello"), std::string("how"), 5);
        REQUIRE(*val == exp);
      }

      THEN("deleting the only edge of the second of two neighbours") {
        auto val = g.erase(g.find(std::string("hello"), std::string("how"), 5));
        auto exp = std::make_tuple(std::string("how"), std::string("hello"), 4);
        auto expPrev = std::make_tuple(std::string("hello"), std::string("are"), 8);
        REQUIRE(*val == exp);
        REQUIRE(*(--val) == expPrev);
      }

      THEN("deleting the last edge of last vertex") {
        g.DeleteNode(std::string("you?"));
        auto val = g.erase(g.find(std::string("how"), std::string("hello"), 4));
        auto exp = g.cend();
        REQUIRE(val == exp);
        --val;
        auto expPrev = std::make_tuple(std::string("hello"), std::string("how"), 5);
        REQUIRE(*val == expPrev);
      }
    }
  }

  SECTION("find") {
    gdwg::Graph<std::string, int> g = sampleGraph();
    auto val = g.find(std::string("hello"), std::string("how"), 5);
    auto exp = std::make_tuple(std::string("hello"), std::string("how"), 5);
    auto expPrev = std::make_tuple(std::string("hello"), std::string("are"), 8);
    REQUIRE(*val == exp);
    REQUIRE(*(--val) == expPrev);
    REQUIRE(g.find(std::string("hello"), std::string("how"), 9000) == g.cend());
  }

  SECTION("IsNode") {
    gdwg::Graph<std::string, int> g = sampleGraph();
    REQUIRE(g.IsNode(std::string("hello")));
    REQUIRE(!g.IsNode(std::string("")));
  }

  SECTION("IsConnected") {
    gdwg::Graph<std::string, int> g = sampleGraph();
    const auto error_msg =
        "Cannot call Graph::IsConnected if src or dst node don't exist in the graph";
    REQUIRE(g.IsConnected(std::string("hello"), std::string("are")));
    REQUIRE(!g.IsConnected(std::string("are"), std::string("hello")));
    REQUIRE_THROWS_WITH(g.IsConnected("haha", "hello"), error_msg);
    REQUIRE_THROWS_AS(g.IsConnected("haha", "hello"), std::runtime_error);
    REQUIRE_THROWS_WITH(g.IsConnected("are", "haha"), error_msg);
    REQUIRE_THROWS_AS(g.IsConnected("are", "haha"), std::runtime_error);
    REQUIRE_THROWS_WITH(g.IsConnected("haha", "haha"), error_msg);
    REQUIRE_THROWS_AS(g.IsConnected("haha", "haha"), std::runtime_error);
  }

  SECTION("GetConnected") {
    gdwg::Graph<std::string, int> g = sampleGraph();
    auto neighbours = g.GetConnected("hello");
    REQUIRE(neighbours.size() == 2);
    REQUIRE(neighbours[0] == "are");
    REQUIRE(neighbours[1] == "how");
    neighbours = g.GetConnected("you?");
    REQUIRE(neighbours.size() == 0);

  }

  SECTION("GetNodes") {
    gdwg::Graph<std::string, int> g;
    REQUIRE(g.GetNodes().empty());
    g = sampleGraph();
    REQUIRE(g.GetNodes().size() == 4);
  }

  SECTION("GetWeights") {
    gdwg::Graph<std::string, int> g = sampleGraph();
    auto weights = g.GetWeights("hello", "are");
    REQUIRE(weights.size() == 2);
    REQUIRE(weights[0] == 2);
    REQUIRE(weights[1] == 8);
    weights = g.GetWeights("hello", "you?");
    REQUIRE(weights.size() == 0);

  }

  SECTION("Clear") {
    gdwg::Graph<int, int> g;
    g.InsertNode(1);
    g.InsertNode(2);
    g.InsertEdge(1, 2, 0);
    g.Clear();
    REQUIRE(g.numNodes() == 0);
    REQUIRE(g.numEdges() == 0);
  }

  SECTION("DeleteNode") {
    gdwg::Graph<std::string, int> g = sampleGraph();
    int oldN = g.numNodes();
    int oldE = g.numEdges();
    REQUIRE(g.DeleteNode("are"));
    REQUIRE(g.numNodes() == oldN-1);
    REQUIRE(g.numEdges() == oldE-3);
    REQUIRE(!g.DeleteNode("are"));
    REQUIRE(g.numNodes() == oldN-1);
    REQUIRE(g.numEdges() == oldE-3);
  }
}

TEST_CASE("iterators") {
  SECTION("iterator for empty graph") {
    gdwg::Graph<std::string, int> g;
    auto b = g.cbegin();
    auto e = g.cend();
    REQUIRE(b == e);
  }

  SECTION("comparison") {
    auto val1 = std::make_tuple<int, int, int>(1, 2, 1);
    auto val2 = std::make_tuple<int, int, int>(2, 3, 2);
    std::vector<std::tuple<int, int, int>> v{val1, val2};
    gdwg::Graph<int, int> g{v.begin(), v.end()};
    gdwg::Graph<int, int> g2{v.begin(), v.end()};
    REQUIRE(g.begin() == g.begin());
    REQUIRE(g.end() == g.end());
    REQUIRE(g.begin() != g2.begin());
    REQUIRE(g.end() != g2.end());

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

  SECTION("ostream") {
    gdwg::Graph<std::string, int>g = sampleGraph();
    std::cout<<g;

  }
}
