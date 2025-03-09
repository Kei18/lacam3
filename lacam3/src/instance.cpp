#include "../include/instance.hpp"

Instance::~Instance()
{
  if (delete_graph_after_used) delete G;
}

Instance::Instance(Graph *_G, const Config &_starts, const Config &_goals,
                   uint _N)
    : G(_G), starts(_starts), goals(_goals), N(_N)
{
}
Instance::Instance(const std::vector<std::vector<int>> &map,
                   const std::vector<std::pair<int, int>> &startVectors,
                   const std::vector<std::pair<int, int>> &goalVectors)
    : G(new Graph(map)),
      starts(Config()),
      goals(Config()),
      N(startVectors.size()),
      delete_graph_after_used(true)
{
  for (const auto &start : startVectors) {
    if (start.first < 0 || G->width <= start.first || start.second < 0 ||
        G->height <= start.second) {
      info(1, 0, "Invalid start node coordinates");
      return;
    }
    int index = G->width * start.second + start.first;
    if (G->U[index] != nullptr) {
      starts.push_back(G->U[index]);
    } else {
      info(1, 0, "Invalid start node coordinates");
      return;
    }
  }

  for (const auto &goal : goalVectors) {
    if (goal.first < 0 || G->width <= goal.first || goal.second < 0 ||
        G->height <= goal.second) {
      info(1, 0, "Invalid goal node coordinates");
      return;
    }
    int index = G->width * goal.second + goal.first;
    if (G->U[index] != nullptr) {
      goals.push_back(G->U[index]);
    } else {
      info(1, 0, "Invalid goal node coordinates");
      return;
    }
  }
}

bool Instance::is_valid(const int verbose) const
{
  if (N != starts.size() || N != goals.size()) {
    info(1, verbose, "invalid N, check instance");
    return false;
  }
  return true;
}
