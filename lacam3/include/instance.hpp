/*
 * instance definition
 */
#pragma once
#include <random>

#include "graph.hpp"
#include "utils.hpp"

struct Instance {
  Graph *G;       // graph
  Config starts;  // initial configuration
  Config goals;   // goal configuration
  const uint N;   // number of agents
  bool delete_graph_after_used;

  Instance(const std::vector<std::vector<int>> &map,
           const std::vector<std::pair<int, int>> &starts,
           const std::vector<std::pair<int, int>> &goals);
  Instance(Graph *_G, const Config &_starts, const Config &_goals, uint _N);
  // random instance generation
  ~Instance();

  // simple feasibility check of instance
  bool is_valid(const int verbose = 0) const;
};

// solution: a sequence of configurations
using Solution = std::vector<Config>;
