#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <lacam.hpp>

namespace py = pybind11;

std::vector<std::vector<std::vector<int>>> convertSolution(const Solution& solution) {
    int numAgents = solution[0].size();
    std::vector<std::vector<std::vector<int>>> result(numAgents);
    for (int i = 0; i < solution.size(); i++) {
        for (int j = 0; j < numAgents; j++) {
            std::vector<int> vertexResult = {solution[i][j]->x, solution[i][j]->y};
            result[j].push_back(vertexResult);
        }
    }
    return result;
}

std::vector<std::vector<std::vector<int>>> pySolver(const std::vector<std::vector<int>>& map,
          const std::vector<std::pair<int, int>>& starts,
          const std::vector<std::pair<int, int>>& goals,
          double timeout) {
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

        return convertSolution(solution);
        }

PYBIND11_MODULE(lacam, m) {
    m.def("solve", &pySolver, "Solve the multi-agent pathfinding problem",
          py::arg("map"), py::arg("starts"), py::arg("goals"), py::arg("timeout"));
}