#include "../include/post_processing.hpp"

#include "../include/dist_table.hpp"
#include "../include/planner.hpp"

bool is_feasible_solution(const Instance &ins, const Solution &solution,
                          const int verbose)
{
  if (solution.empty()) return true;

  // check start locations
  if (!is_same_config(solution.front(), ins.starts)) {
    info(1, verbose, "invalid starts");
    return false;
  }

  // check goal locations
  if (!is_same_config(solution.back(), ins.goals)) {
    info(1, verbose, "invalid goals");
    return false;
  }

  for (size_t t = 1; t < solution.size(); ++t) {
    for (size_t i = 0; i < ins.N; ++i) {
      auto v_i_from = solution[t - 1][i];
      auto v_i_to = solution[t][i];
      // check connectivity
      if (v_i_from != v_i_to &&
          std::find(v_i_to->neighbor.begin(), v_i_to->neighbor.end(),
                    v_i_from) == v_i_to->neighbor.end()) {
        info(1, verbose, "invalid move");
        return false;
      }

      // check conflicts
      for (size_t j = i + 1; j < ins.N; ++j) {
        auto v_j_from = solution[t - 1][j];
        auto v_j_to = solution[t][j];
        // vertex conflicts
        if (v_j_to == v_i_to) {
          info(1, verbose, "vertex conflict between agent-", i, " and agent-",
               j, " at vertex-", v_i_to->id, " at timestep ", t);
          return false;
        }
        // swap conflicts
        if (v_j_to == v_i_from && v_j_from == v_i_to) {
          info(1, verbose, "edge conflict");
          return false;
        }
      }
    }
  }

  return true;
}
