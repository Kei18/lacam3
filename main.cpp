#include <argparse/argparse.hpp>
#include <iostream>
#include <lacam.hpp>

Solution solve(const std::vector<std::vector<int>>& map,
               const std::vector<std::pair<int, int>>& starts,
               const std::vector<std::pair<int, int>>& goals, int timeout)
{
  // setup instance
  Instance ins(map, starts, goals);
  if (!ins.is_valid(1)) return {};

  // default solver parameters
  Planner::FLG_SWAP = true;
  Planner::FLG_STAR = true;
  Planner::FLG_MULTI_THREAD = true;
  Planner::PIBT_NUM = 10;
  Planner::FLG_REFINER = true;
  Planner::REFINER_NUM = 4;
  Planner::FLG_SCATTER = true;
  Planner::SCATTER_MARGIN = 10;
  Planner::RANDOM_INSERT_PROB1 = 0.001;
  Planner::RANDOM_INSERT_PROB2 = 0.01;
  Planner::FLG_RANDOM_INSERT_INIT_NODE = false;
  Planner::RECURSIVE_RATE = 0.2;
  Planner::RECURSIVE_TIME_LIMIT = 1000;
  Planner::CHECKPOINTS_DURATION = 5000;

  // solve
  const auto deadline = Deadline(timeout * 1000);
  const auto solution = solve(ins, 0, &deadline, 0);
  const auto comp_time_ms = deadline.elapsed_ms();

  // failure
  if (solution.empty()) return {};

  // check feasibility
  if (!is_feasible_solution(ins, solution, 0)) {
    return {};
  }

  auto get_x = [&](int k) { return k % ins.G->width; };
  auto get_y = [&](int k) { return k / ins.G->width; };

  std::cout << "starts=";
  for (size_t i = 0; i < ins.N; ++i) {
    auto k = ins.starts[i]->index;
    std::cout << "(" << get_x(k) << "," << get_y(k) << "),";
  }
  std::cout << "\ngoals=";
  for (size_t i = 0; i < ins.N; ++i) {
    auto k = ins.goals[i]->index;
    std::cout << "(" << get_x(k) << "," << get_y(k) << "),";
  }
  std::cout << "\nsolution=\n";
  for (size_t t = 0; t < solution.size(); ++t) {
    std::cout << t << ":";
    auto C = solution[t];
    for (auto v : C) {
      std::cout << "(" << get_x(v->index) << "," << get_y(v->index) << "),";
    }
    std::cout << "\n";
  }
  return solution;
}

int main(int argc, char* argv[])
{
  std::vector<std::vector<int>> map = {{0, 0, 0, 0, 0},
                                       {0, 0, 0, 0, 0},
                                       {0, 0, 1, 0, 0},
                                       {0, 0, 0, 0, 0},
                                       {0, 0, 0, 0, 0}};
  std::vector<std::pair<int, int>> starts = {{0, 0}, {1, 2}};
  std::vector<std::pair<int, int>> goals = {{4, 4}, {1, 1}};
  // std::vector<std::pair<int, int>> starts = {{1, 1}};
  // std::vector<std::pair<int, int>> goals = {{2, 2}};

  // for (auto i: map){
  //     for (auto j: i){
  //         std::cout<< " "<< j <<" ";
  //     }
  //     std::cout<<"\n";
  // }

  // std::cout<<starts.size()<<"\n";

  // for (const auto& i : starts) {
  //     auto [x, y] = i;
  //     std::cout << " " << x << " " << y << std::endl;
  // }

  // for (const auto& i : goals) {
  //     auto [x, y] = i;
  //     std::cout << " " << x << " " << y << std::endl;
  // }
  // solve
  const auto solution = solve(map, starts, goals, 5);

  return 0;
}
