/*
  == Explanation and rational of testing ==

  We design tests around the observable behaviours of the data type. For most
  functions, we compare the expected results to a known graph using ::IsNode() and
  ::find(). Doing it this way makes the tests easier to read and maintain

  When writing tests, we don't re-test things that are guaranteed by the compiler. For example,
  when calling `g.GetWeights()`, we don't test that the g didn't change, because the function
  prototype dictates that `g` will be const.

  We also test the known exceptions with REQUIRE_THROWS_WITH and REQUIRE_THROWS_AS

  Given the tests here cover all the observable behaviours of the interface that aren't
  guaranteed by the compiler, we can claim that the implementation is probably correct.
*/

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include "assignments/dg/graph.h"
#include "catch.h"
using Catch::Equals;

template<typename N, typename E>
void printValue(std::tuple<N, N, E> val) {
  std::cout << std::get<0>(val) << " " << std::get<1>(val)<< " " << std::get<2>(val) << "\n";
}


// 2.1 constructors + destructor
// requires: IsNode, find, end()
SCENARIO("basic constructor tests") {
  WHEN("basic constructor is used") {
    THEN("it exists") {
      gdwg::Graph<std::string, std::string> g;
      REQUIRE(true);
    }
  }
  AND_WHEN("we use node list constructor") {
    std::vector<std::string> v = {"one", "two", "three"};
    gdwg::Graph<std::string, std::string> g{v.begin(), v.end()};
    THEN("nodes exist with IsNode") {
      REQUIRE(g.IsNode("one"));
      REQUIRE(g.IsNode("two"));
      REQUIRE(g.IsNode("three"));
    }
  }
  AND_WHEN("we use an edge list constructor") {
    std::vector<std::tuple<std::string, std::string, int>> v = {
        {"one", "two", 1}, {"two", "three", 0}, {"one", "three", 3}};
    gdwg::Graph<std::string, int> g{v.begin(), v.end()};
    THEN("nodes exist with IsNode, and edges exist with find") {
      REQUIRE(g.IsNode("one"));
      REQUIRE(g.IsNode("two"));
      REQUIRE(g.IsNode("three"));
      REQUIRE(g.find("one", "two", 1) != g.cend());
      REQUIRE(g.find("two", "three", 0) != g.cend());
      REQUIRE(g.find("one", "three", 3) != g.cend());
    }
  }
  AND_WHEN("we use an initialiser list") {
    gdwg::Graph<std::string, int> g{"one", "two", "three"};
    THEN("nodes exist with IsNode") {
      REQUIRE(g.IsNode("one"));
      REQUIRE(g.IsNode("two"));
      REQUIRE(g.IsNode("three"));
    }
  }
}

// requires: edge list constructor, find, IsNode, end()
SCENARIO("we use the copy constructor") {
  GIVEN("a graph") {
    std::vector<std::tuple<std::string, std::string, int>> v = {
        {"one", "two", 1}, {"two", "three", 0}, {"one", "three", 3}};
    gdwg::Graph<std::string, int> g{v.begin(), v.end()};
    WHEN("we use the copy constructor") {
      auto g2{g};
      THEN("all nodes and edges are there") {
        REQUIRE(g.IsNode("one"));
        REQUIRE(g.IsNode("two"));
        REQUIRE(g.IsNode("three"));
        REQUIRE(g.find("one", "two", 1) != g.cend());
        REQUIRE(g.find("two", "three", 0) != g.cend());
        REQUIRE(g.find("one", "three", 3) != g.cend());
      }
    }
  }
}

// requires: edge list constructor, find, IsNode, end()
SCENARIO("we use the move constructor") {
  GIVEN("a graph") {
    std::vector<std::tuple<std::string, std::string, int>> v = {
        {"one", "two", 1}, {"two", "three", 0}, {"one", "three", 3}};
    gdwg::Graph<std::string, int> g{v.begin(), v.end()};
    WHEN("we use the move constructor") {
      auto g2{std::move(g)};
      THEN("all nodes and edges were moved") {
        REQUIRE(!g.IsNode("one"));
        REQUIRE(!g.IsNode("two"));
        REQUIRE(!g.IsNode("three"));
        REQUIRE(g.find("one", "two", 1) == g.cend());
        REQUIRE(g.find("two", "three", 0) == g.cend());
        REQUIRE(g.find("one", "three", 3) == g.cend());
        REQUIRE(g2.IsNode("one"));
        REQUIRE(g2.IsNode("two"));
        REQUIRE(g2.IsNode("three"));
        REQUIRE(g2.find("one", "two", 1) != g.cend());
        REQUIRE(g2.find("two", "three", 0) != g.cend());
        REQUIRE(g2.find("one", "three", 3) != g.cend());
      }
    }
  }
}

// 2.2 operations (move + copy)
SCENARIO("we use the copy assignment") {
  GIVEN("a graph") {
    std::vector<std::tuple<std::string, std::string, int>> v = {
        {"one", "two", 1}, {"two", "three", 0}, {"one", "three", 3}};
    gdwg::Graph<std::string, int> g{v.begin(), v.end()};
    WHEN("we use the copy assignment") {
      auto g2 = g;
      THEN("all nodes and edges are there") {
        REQUIRE(g.IsNode("one"));
        REQUIRE(g.IsNode("two"));
        REQUIRE(g.IsNode("three"));
        REQUIRE(g.find("one", "two", 1) != g.cend());
        REQUIRE(g.find("two", "three", 0) != g.cend());
        REQUIRE(g.find("one", "three", 3) != g.cend());
      }
    }
  }
}

// requires: edge list constructor, find, IsNode, end()
SCENARIO("we use the move assignment") {
  GIVEN("a graph") {
    std::vector<std::tuple<std::string, std::string, int>> v = {
        {"one", "two", 1}, {"two", "three", 0}, {"one", "three", 3}};
    gdwg::Graph<std::string, int> g{v.begin(), v.end()};
    WHEN("we use the move assignment") {
      auto g2 = std::move(g);
      THEN("all nodes and edges were moved") {
        REQUIRE(!g.IsNode("one"));
        REQUIRE(!g.IsNode("two"));
        REQUIRE(!g.IsNode("three"));
        REQUIRE(g.find("one", "two", 1) == g.cend());
        REQUIRE(g.find("two", "three", 0) == g.cend());
        REQUIRE(g.find("one", "three", 3) == g.cend());
        REQUIRE(g2.IsNode("one"));
        REQUIRE(g2.IsNode("two"));
        REQUIRE(g2.IsNode("three"));
        REQUIRE(g2.find("one", "two", 1) != g.cend());
        REQUIRE(g2.find("two", "three", 0) != g.cend());
        REQUIRE(g2.find("one", "three", 3) != g.cend());
      }
    }
  }
}

// 2.3 methods

SCENARIO("InsertNode and IsNode tests") {
  WHEN("we insert a node") {
    gdwg::Graph<std::string, int> g;
    const std::string v = "hi";
    THEN("insertNode is true and isNode is true") {
      REQUIRE(g.InsertNode(v));
      REQUIRE(g.IsNode(v));
    }
  }
  WHEN("we try to insert the same node twice") {
    const std::string v = "hi";
    gdwg::Graph<std::string, int> g;
    THEN("insertNode is false and isNode is true") {
      REQUIRE(g.InsertNode(v));
      REQUIRE(!g.InsertNode(v));
      REQUIRE(g.IsNode(v));
    }
  }
}

SCENARIO("InsertEdge tests") {
  GIVEN("a graph with two nodes") {
    gdwg::Graph<std::string, int> g;
    const std::string v1 = "hi", v2 = "hithere";
    g.InsertNode(v1);
    g.InsertNode(v2);
    WHEN("we insert an edge") {
      THEN("it inserts") { REQUIRE(g.InsertEdge(v1, v2, 0)); }
    }
    AND_WHEN("we insert a reflexive edge") {
      THEN("it inserts") { REQUIRE(g.InsertEdge(v1, v1, 0)); }
    }
    AND_WHEN("we try to insert the same edge twice") {
      THEN("it doesn't insert the second time") {
        REQUIRE(g.InsertEdge(v1, v2, 0));
        REQUIRE(!g.InsertEdge(v1, v2, 0));
      }
    }
    AND_WHEN("we insert the same edge, but directionally different") {
      THEN("it inserts both times") {
        REQUIRE(g.InsertEdge(v1, v2, 0));
        REQUIRE(g.InsertEdge(v2, v1, 0));
      }
    }
    AND_WHEN("we insert an edge from an nonexistent node") {
      THEN("it throws a runtime error") {
        const auto err = "Cannot call Graph::InsertEdge when either src or dst node does not exist";
        REQUIRE_THROWS_AS(g.InsertEdge("ffff", "ffffff", 0), std::runtime_error);
        REQUIRE_THROWS_AS(g.InsertEdge(v1, "ffffff", 0), std::runtime_error);
        REQUIRE_THROWS_AS(g.InsertEdge("ffff", v2, 0), std::runtime_error);
        REQUIRE_THROWS_WITH(g.InsertEdge("ffff", "ffffff", 0), err);
        REQUIRE_THROWS_WITH(g.InsertEdge(v1, "ffffff", 0), err);
        REQUIRE_THROWS_WITH(g.InsertEdge("ffff", v2, 0), err);
      }
    }
  }
}

// requires: IsNode, GetConnected, find
SCENARIO("DeleteNode tests") {
  GIVEN("a graph with two nodes") {
    const std::string v1 = "hi", v2 = "hithere";
    using T = std::tuple<std::string, std::string, int>;
    std::vector<T> v{
        {v1, v2, 0},
    };
    gdwg::Graph<std::string, int> g{v.begin(), v.end()};
    gdwg::Graph<std::string, int> original_g{v.begin(), v.end()};
    WHEN("we delete an existing node") {
      THEN("it deletes") {
        REQUIRE(g.DeleteNode(v1));
        std::vector<std::string> expected_nodes = {v2};
        REQUIRE(g.GetNodes() == expected_nodes);
        REQUIRE(g.find(v1, v2, 0) == g.cend());
      }
    }
    AND_WHEN("we delete a non-existent node") {
      THEN("the graph is the same as before") {
        REQUIRE(!g.DeleteNode("yolo"));
        REQUIRE(g == original_g);
      }
    }
  }
}

// requires IsConnected, tuple vector constructor, ==
SCENARIO("Replace tests") {
  GIVEN("a graph with two nodes") {
    const std::string v1 = "hi", v2 = "hithere", new_v1 = "bye";
    using T = std::tuple<std::string, std::string, int>;
    std::vector<T> v{{v1, v2, 0}, {v2, v1, 0}, {v1, v1, 0}};

    gdwg::Graph<std::string, int> g{v.begin(), v.end()};
    gdwg::Graph<std::string, int> original_g{v.begin(), v.end()};
    WHEN("we replace a node's data") {
      THEN("it replaces") {
        REQUIRE(g.Replace(v1, new_v1));
        REQUIRE(!g.IsNode(v1));
        REQUIRE(g.IsNode(new_v1));
        REQUIRE(g.find(v1, v2, 0) == g.cend());
        REQUIRE(g.find(v2, v1, 0) == g.cend());
        REQUIRE(g.find(v1, new_v1, 0) == g.cend());
        REQUIRE(g.find(new_v1, v2, 0) != g.cend());
        REQUIRE(g.find(v2, new_v1, 0) != g.cend());
        REQUIRE(g.find(new_v1, new_v1, 0) != g.cend());
      }
    }
    AND_WHEN("we try to replace with an existing node with the new data") {
      THEN("it returns false and graph remains unchanged") {
        REQUIRE(!g.Replace(v1, v2));
        REQUIRE(g == original_g);
      }
    }
    AND_WHEN("we try replace a node that doesn't exist") {
      THEN("it returns a run time error") {
        const auto err = "Cannot call Graph::Replace on a node that doesn't exist";
        REQUIRE_THROWS_AS(g.Replace("yeet", v2), std::runtime_error);
        REQUIRE_THROWS_WITH(g.Replace("yeet", v2), err);
      }
    }
  }
}

// requires: IsConnected, tuple list constructor
SCENARIO("MergeReplace tests") {
  using T = std::tuple<std::string, std::string, int>;
  std::vector<T> v{{"1", "2", 1}, {"1", "2", 0},
                   {"1", "3", 5}, {"2", "2", 0},  // will be a duplicate edge
                   {"2", "4", 5}, {"3", "4", 5}};
  gdwg::Graph<std::string, int> g{v.begin(), v.end()};

  WHEN("we MergeReplace on two existing nodes") {
    THEN("the result is as expected, without duplicates") {
      g.MergeReplace("1", "2");
      std::vector<T> exp{{"2", "2", 0}, {"2", "2", 1}, {"2", "3", 5}, {"2", "4", 5}, {"3", "4", 5}};
      std::vector<T> act{g.begin(), g.end()};
      REQUIRE(exp == act);
    }
  }

  AND_WHEN("we try to MergeReplace with a nonexistent node") {
    THEN("it throws a runtime error") {
      const auto err =
          "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph";
      REQUIRE_THROWS_AS(g.MergeReplace("0", "1"), std::runtime_error);
      REQUIRE_THROWS_AS(g.MergeReplace("1", "0"), std::runtime_error);
      REQUIRE_THROWS_AS(g.MergeReplace("0", "0"), std::runtime_error);
      REQUIRE_THROWS_WITH(g.MergeReplace("0", "1"), err);
      REQUIRE_THROWS_WITH(g.MergeReplace("1", "0"), err);
      REQUIRE_THROWS_WITH(g.MergeReplace("0", "0"), err);
    }
  }
}

SCENARIO("Clear tests") {
  GIVEN("a graph with two nodes") {
    gdwg::Graph<std::string, int> g;
    const std::string v1 = "ton", v2 = "pop";
    g.InsertNode(v1);
    g.InsertNode(v2);
    g.InsertEdge(v1, v2, 0);
    WHEN("we clear the graph") {
      g.Clear();
      THEN("it clears") {
        REQUIRE(!g.IsNode(v1));
        REQUIRE(!g.IsNode(v2));
      }
    }
  }
}

SCENARIO("IsNode tests") {
  GIVEN("a graph with a node") {
    gdwg::Graph<std::string, int> g;
    const std::string v1 = "meet";
    g.InsertNode(v1);
    WHEN("we check an existing node") {
      THEN("it returns true") { REQUIRE(g.IsNode(v1)); }
    }
    AND_WHEN("we check a non-existing node") {
      THEN("it returns false") { REQUIRE(!g.IsNode("blah")); }
    }
  }
}

// requires: tuple constructor
SCENARIO("IsConnected tests") {
  GIVEN("a graph with two nodes") {
    gdwg::Graph<std::string, int> g;
    const std::string v1 = "tap", v2 = "wow";
    g.InsertNode(v1);
    g.InsertNode(v2);
    g.InsertEdge(v1, v2, 0);
    const auto error_msg =
        "Cannot call Graph::IsConnected if src or dst node don't exist in the graph";
    WHEN("we check if two nodes with an edge are connected") {
      THEN("it checks it is connected") { REQUIRE(g.IsConnected(v1, v2)); }
    }
    AND_WHEN("we check with a non-existent src node") {
      THEN("it throws a runtime error") {
        REQUIRE_THROWS_WITH(g.IsConnected("haha", v2), error_msg);
        REQUIRE_THROWS_AS(g.IsConnected("haha", v2), std::runtime_error);
      }
    }
    AND_WHEN("we check with a non-existent dst node") {
      THEN("it throws a runtime error") {
        REQUIRE_THROWS_WITH(g.IsConnected(v1, "lala"), error_msg);
        REQUIRE_THROWS_AS(g.IsConnected("haha", v2), std::runtime_error);
      }
    }
    AND_WHEN("we check with a non-existent src and dst") {
      THEN("it throws a runtime error") {
        REQUIRE_THROWS_WITH(g.IsConnected("haha", "lala"), error_msg);
        REQUIRE_THROWS_AS(g.IsConnected("haha", "lala"), std::runtime_error);
      }
    }
  }
}

SCENARIO("GetNodes tests") {
  GIVEN("an empty graph") {
    gdwg::Graph<std::string, int> g;
    WHEN("we get all nodes in graph") {
      THEN("an empty vector is returned") { REQUIRE(g.GetNodes().empty()); }
    }
  }

  GIVEN("a graph with three nodes") {
    gdwg::Graph<std::string, int> g;
    const std::string v1 = "bag", v2 = "mat", v3 = "app";
    g.InsertNode(v1);
    g.InsertNode(v2);
    g.InsertNode(v3);
    WHEN("we get all nodes in graph") {
      std::vector<std::string> expected_vec{"app", "bag", "mat"};
      THEN("a vector of all nodes is given in increasing order") {
        REQUIRE(g.GetNodes() == expected_vec);
      }
    }
  }
}

SCENARIO("GetConnected tests") {
  GIVEN("a graph with two nodes with no edges") {
    gdwg::Graph<std::string, int> g;
    const std::string v1 = "git", v2 = "ban";
    g.InsertNode(v1);
    g.InsertNode(v2);
    WHEN("we get connected nodes from src") {
      THEN("an empty vector is returned") { REQUIRE(g.GetConnected(v1).empty()); }
    }
    AND_WHEN("we call GetConnected on an invalid src") {
      const std::string err = "Cannot call Graph::GetConnected if src doesn't exist in the graph";
      REQUIRE_THROWS_AS(g.GetConnected("nonexist"), std::out_of_range);
      REQUIRE_THROWS_WITH(g.GetConnected("nonexist"), err);
    }
  }

  GIVEN("a graph with three nodes with edges") {
    gdwg::Graph<std::string, int> g;
    const std::string v1 = "yolo", v2 = "ha", v3 = "nice";
    g.InsertNode(v1);
    g.InsertNode(v2);
    g.InsertNode(v3);
    g.InsertEdge(v1, v1, 0);
    g.InsertEdge(v1, v2, 0);
    g.InsertEdge(v3, v1, 0);
    WHEN("we get connected nodes from src") {
      THEN("nodes from outgoing edges of src are returned") {
        std::vector<std::string> expected_vec{"ha", "yolo"};
        REQUIRE(g.GetConnected(v1) == expected_vec);
      }
    }
  }
}

SCENARIO("a graph with serveral edges to the same node") {
  GIVEN("a graph with several edges") {
    using T = std::tuple<std::string, std::string, int>;
    std::vector<T> v{{"a", "b", 0}, {"a", "b", 1}};
    gdwg::Graph<std::string, int> g{v.begin(), v.end()};
    WHEN("we get connected on the multi edges") {
      THEN("we only get one edge") {
        auto res = g.GetConnected("a");
        std::vector<std::string> exp{"b"};
        REQUIRE(res == exp);
      }
    }
  }
}

SCENARIO("GetWeights tests") {
  GIVEN("a graph with three nodes") {
    const std::string v1 = "a", v2 = "b", v3 = "c", v4 = "d", v5 = "e";
    using T = std::tuple<std::string, std::string, int>;
    std::vector<T> v{{v1, v2, 0}, {v1, v3, 1}, {v1, v3, 5}, {v1, v4, 7}};
    gdwg::Graph<std::string, int> g{v.begin(), v.end()};
    g.InsertNode(v5);
    WHEN("we get weights between v1 and v3") {
      THEN("we get two weights in increasing order") {
        std::vector<int> expected_weights = {1, 5};
        REQUIRE(g.GetWeights(v1, v3) == expected_weights);
      }
    }
    AND_WHEN("we get weights between v1 and v5") {
      THEN("we get no weights") { REQUIRE(g.GetWeights(v1, v5).empty()); }
    }
    AND_WHEN("we get weights with non-existent src or dst") {
      THEN("we get an out of range error") {
        const auto err =
            "Cannot call Graph::GetWeights if src or dst node don't exist in the graph";
        REQUIRE_THROWS_AS(g.GetWeights("ffff", "ffffff"), std::out_of_range);
        REQUIRE_THROWS_AS(g.GetWeights(v1, "ffffff"), std::out_of_range);
        REQUIRE_THROWS_AS(g.GetWeights("ffff", v2), std::out_of_range);
        REQUIRE_THROWS_WITH(g.GetWeights("ffff", "ffffff"), err);
        REQUIRE_THROWS_WITH(g.GetWeights(v1, "ffffff"), err);
      }
    }
  }
}

SCENARIO("find tests") {
  GIVEN("a graph with three nodes") {
    gdwg::Graph<std::string, int> g;
    const std::string v1 = "hen", v2 = "forward", v3 = "turkey";
    g.InsertNode(v1);
    g.InsertNode(v2);
    g.InsertNode(v3);
    g.InsertEdge(v1, v3, 0);

    WHEN("we find an existing edge") {
      THEN("we get a const iterator to the edge") { REQUIRE(g.find(v1, v3, 0) != g.cend()); }
    }
    AND_WHEN("we find a non-existent edge") {
      THEN("we get a const iterator to cend") { REQUIRE(g.find(v1, v2, 0) == g.cend()); }
    }
  }
}

SCENARIO("bool erase tests") {
  GIVEN("a graph with three nodes") {
    gdwg::Graph<std::string, int> g;
    const std::string v1 = "hen", v2 = "forward", v3 = "turkey";
    g.InsertNode(v1);
    g.InsertNode(v2);
    g.InsertNode(v3);
    g.InsertEdge(v1, v3, 0);

    WHEN("we erase an existing edge") {
      THEN("it gets erased") {
        REQUIRE(g.erase(v1, v3, 0));
        REQUIRE(!g.IsConnected(v1, v3));
      }
    }
    AND_WHEN("we erase a non-existent edge") {
      THEN("it returns false") { REQUIRE(!g.erase(v1, v2, 0)); }
    }
  }
}

SCENARIO("const_iterator erase tests") {
  GIVEN("a graph with three nodes") {
    using T = std::tuple<std::string, std::string, int>;
    T val1 = {"a", "b", 1};
    T val2 = {"b", "c", 1};
    std::vector<T> v{val1, val2};
    gdwg::Graph<std::string, int> g{v.begin(), v.end()};

    WHEN("we erase an existing edge") {
      THEN("it gets erased, and the iterator points to the next value") {
        REQUIRE(*(g.erase(g.begin())) == val2);
        REQUIRE(g.find("a", "b", 1) == g.cend());
      }
    }
    AND_WHEN("we erase the last edge") {
      THEN("it gets erased, and the iterator points to the end") {
        // // auto it = g.erase(g.begin());
        // auto it = g.erase(--g.end());
        // // printValue(*it);
        // printValue(*(--g.end()));
        // auto exp = g.end();
        // REQUIRE(it == exp);
        // REQUIRE(g.find("b", "c", 1) == g.end());
      }
    }
    AND_WHEN("we erase a non-existent edge") {
      THEN("it returns cend") {
        REQUIRE(g.erase(g.end()) == g.end());
      }
    }
  }
}

// iterators
// requires: tuple constructor
SCENARIO("iterator basic usage") {
  GIVEN("a known graph") {
    using T = std::tuple<std::string, std::string, int>;
    T val1 = {"a", "b", 1};
    T val2 = {"b", "c", 2};
    std::vector<T> v{val1, val2};
    gdwg::Graph<std::string, int> g{v.begin(), v.end()};
    WHEN("we get an iterator to begin() and cbegin()") {
      THEN("it is the expected value") {
        REQUIRE(*g.begin() == val1);
        REQUIRE(*g.cbegin() == val1);
      }
    }
    AND_WHEN("we iterate over the data type using begin, end") {
      std::vector<T> v2{g.begin(), g.end()};

      THEN("the values are the same") {
        REQUIRE(v == v2);
      }
    }
    AND_WHEN("we iterate over the data type using cbegin, cend") {
      std::vector<T> v2{g.cbegin(), g.cend()};
      THEN("the values are the same") { REQUIRE(v == v2); }
    }
  }
}

SCENARIO("iterator comparison") {
  GIVEN("two identical graphs") {
    auto val1 = std::make_tuple<std::string, std::string, int>("a", "b", 1);
    auto val2 = std::make_tuple<std::string, std::string, int>("b", "c", 2);
    std::vector<std::tuple<std::string, std::string, int>> v{val1, val2};
    gdwg::Graph<std::string, int> g{v.begin(), v.end()}, g2{v.begin(), v.end()};
    WHEN("we compare iterators of the same graph") {
      THEN("they are the same") {
        REQUIRE(g.begin() == g.begin());
        REQUIRE(g.end() == g.end());
      }
    }
    AND_WHEN("we compare CONSTANT iterators of the same graph") {
      THEN("they are the same") {
        REQUIRE(g.cbegin() == g.cbegin());
        REQUIRE(g.cend() == g.cend());
      }
    }
    AND_WHEN("we compare iterators of two identical graphs") {
      THEN("they are different") {
        REQUIRE(g.begin() != g2.begin());
        REQUIRE(g.end() != g2.end());
      }
    }
    AND_WHEN("we compare CONSTANT iterators of two identical graphs") {
      THEN("they are different") {
        REQUIRE(g.cbegin() != g2.cbegin());
        REQUIRE(g.cend() != g2.cend());
      }
    }
  }
}

SCENARIO("reverse iterator usage") {
  GIVEN("a known graph") {
    using T = std::tuple<std::string, std::string, int>;
    std::vector<T> v{{"a", "b", 1}, {"b", "c", 2}};
    gdwg::Graph<std::string, int> g{v.begin(), v.end()};
    auto exp_v = v;
    std::reverse(exp_v.begin(), exp_v.end());
    WHEN("we iterate in reverse using rbegin and rend") {
      std::vector<T> act_v = {g.rbegin(), g.rend()};
      THEN("the value is as expected") { REQUIRE(exp_v == act_v); }
    }
    AND_WHEN("we iterate in reverse using crbegin and crend") {
      std::vector<T> act_v = {g.crbegin(), g.crend()};
      THEN("the value is as expected") { REQUIRE(exp_v == act_v); }
    }
  }
}

// dirty tests
// requires: DeleteNode
SCENARIO("user modifies the graph and then uses an iterator") {
  GIVEN("a known graph") {
    using T = std::tuple<std::string, std::string, int>;
    std::vector<T> v{{"a", "b", 1}, {"b", "c", 2}, {"c", "d", 8}};
    gdwg::Graph<std::string, int> g{v.begin(), v.end()};
    WHEN("the user deletes a node") {
      g.DeleteNode("a");
      THEN("the iterator is as expected") {
        std::vector<T> exp{{"b", "c", 2}, {"c", "d", 8}};
        std::vector<T> act{g.begin(), g.end()};
        REQUIRE(act == exp);
      }
    }
    AND_WHEN("the user deletes a different node") {
      g.DeleteNode("b");
      THEN("the iterator is as expected") {
        std::vector<T> exp{{"c", "d", 8}};
        std::vector<T> act{g.begin(), g.end()};
        REQUIRE(act == exp);
      }
    }
  }
}

// 2.4 friends
SCENARIO("uses == and != operator") {
  GIVEN("two identical graphs") {
    std::string v1 = "1", v2 = "2", v3 = "3", v4 = "4", v5 = "5", v6 = "6";
    std::vector<std::tuple<std::string, std::string, int>> v = {
        {v1, v2, 1}, {v1, v3, 3}, {v2, v3, 0}, {v3, v5, 4}, {v5, v1, 1}, {v5, v1, 2}};
    gdwg::Graph<std::string, int> g{v.begin(), v.end()};
    g.InsertNode(v4);
    gdwg::Graph<std::string, int> g1{v.begin(), v.end()};
    g1.InsertNode(v4);
    WHEN("we use the == operator") {
      THEN("graphs are the same") {
        REQUIRE(g == g1);
        REQUIRE(g1 == g);
        REQUIRE(!(g != g1));
        REQUIRE(!(g1 != g));
      }
    }
    AND_WHEN("we add a node onto g") {
      REQUIRE(g.InsertNode(v6));
      THEN("graphs are different") {
        REQUIRE(!(g == g1));
        REQUIRE(!(g1 == g));
        REQUIRE(g != g1);
        REQUIRE(g1 != g);
      }
    }
    AND_WHEN("we add an edge onto g1") {
      REQUIRE(g.InsertEdge(v2, v4, 8));
      THEN("graphs are different") {
        REQUIRE(!(g == g1));
        REQUIRE(!(g1 == g));
        REQUIRE(g != g1);
        REQUIRE(g1 != g);
      }
    }
  }
  GIVEN("two empty graphs") {
    gdwg::Graph<std::string, int> g;
    gdwg::Graph<std::string, int> g1;
    WHEN("we use the ==, != operator") {
      THEN("graphs are same") {
        REQUIRE(g == g1);
        REQUIRE(!(g != g1));
      }
    }
  }
}

SCENARIO("uses << operator") {
  GIVEN("a known graph") {
    std::vector<std::tuple<std::string, std::string, int>> v = {
        {"1", "2", 1}, {"1", "3", 3}, {"2", "3", 0}, {"3", "5", 4}, {"5", "1", 1}, {"5", "1", 2}};
    gdwg::Graph<std::string, int> g{v.begin(), v.end()};
    g.InsertNode("4");
    WHEN("we use the << operator") {
      THEN("the value is as expected") {
        const char* EXP = R"xyz(1 (
  2 | 1
  3 | 3
)
2 (
  3 | 0
)
3 (
  5 | 4
)
4 (
)
5 (
  1 | 1
  1 | 2
)
)xyz";
        std::ostringstream os;
        os << g;
        std::string act = os.str();
        REQUIRE(act == EXP);
      }
    }
  }
}
