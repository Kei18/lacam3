#include <cassert>
#include <lacam.hpp>

int main()
{
  {
    const auto map_filename = "../assets/empty-8-8.map";
    const auto start_indexes = std::vector<int>({0, 8});
    const auto goal_indexes = std::vector<int>({9, 1});
    const auto ins = Instance(map_filename, start_indexes, goal_indexes);

    // correct solution
    auto sol = Solution(3);
    sol[0] = Config({ins.G->U[0], ins.G->U[8]});
    sol[1] = Config({ins.G->U[1], ins.G->U[0]});
    sol[2] = Config({ins.G->U[9], ins.G->U[1]});
    assert(is_feasible_solution(ins, sol));

    // invalid start
    sol[0] = Config({ins.G->U[0], ins.G->U[4]});
    sol[1] = Config({ins.G->U[1], ins.G->U[0]});
    sol[2] = Config({ins.G->U[9], ins.G->U[1]});
    assert(!is_feasible_solution(ins, sol));

    // invalid goal
    sol[0] = Config({ins.G->U[0], ins.G->U[8]});
    sol[1] = Config({ins.G->U[1], ins.G->U[0]});
    sol[2] = Config({ins.G->U[10], ins.G->U[1]});
    assert(!is_feasible_solution(ins, sol));

    // invalid transition
    sol[0] = Config({ins.G->U[0], ins.G->U[8]});
    sol[1] = Config({ins.G->U[4], ins.G->U[0]});
    sol[2] = Config({ins.G->U[9], ins.G->U[1]});
    assert(!is_feasible_solution(ins, sol));

    // swap conflict
    sol[0] = Config({ins.G->U[0], ins.G->U[8]});
    sol[1] = Config({ins.G->U[8], ins.G->U[0]});
    sol[2] = Config({ins.G->U[9], ins.G->U[1]});
    assert(!is_feasible_solution(ins, sol));

    // vertex conflict
    sol[0] = Config({ins.G->U[0], ins.G->U[8]});
    sol[1] = Config({ins.G->U[0], ins.G->U[0]});
    sol[2] = Config({ins.G->U[8], ins.G->U[1]});
    sol.push_back(Config({ins.G->U[9], ins.G->U[1]}));
    assert(!is_feasible_solution(ins, sol));
  }

  {
    const auto map_filename = "../assets/empty-8-8.map";
    const auto start_indexes = std::vector<int>({0, 5, 10});
    const auto goal_indexes = std::vector<int>({2, 4, 11});
    const auto ins = Instance(map_filename, start_indexes, goal_indexes);

    // correct solution
    auto sol = Solution(3);
    sol[0] = Config({ins.G->U[0], ins.G->U[5], ins.G->U[10]});
    sol[1] = Config({ins.G->U[1], ins.G->U[4], ins.G->U[11]});
    sol[2] = Config({ins.G->U[2], ins.G->U[4], ins.G->U[11]});

    assert(get_makespan(sol) == 2);
    assert(get_sum_of_costs(sol) == 4);
  }

  return 0;
}
