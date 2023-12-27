#include <cassert>
#include <lacam.hpp>

int main()
{
  {
    const auto scen_filename = "../assets/random-32-32-10-random-1.scen";
    const auto map_filename = "../assets/random-32-32-10.map";
    const auto ins = Instance(scen_filename, map_filename, 3);
    auto solution = solve(ins);
    assert(is_feasible_solution(ins, solution));
  }

  {
    const auto scen_filename = "../tests/assets/2x1.scen";
    const auto map_filename = "../tests/assets/2x1.map";
    const auto ins = Instance(scen_filename, map_filename, 2);
    auto solution = solve(ins);
    assert(solution.empty());
  }

  return 0;
}
