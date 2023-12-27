#include <cassert>
#include <lacam.hpp>

int main()
{
  {
    const auto scen_filename = "../assets/random-32-32-10-random-1.scen";
    const auto map_filename = "../assets/random-32-32-10.map";
    const auto ins = Instance(scen_filename, map_filename, 3);
    auto dist_table = DistTable(ins);

    assert(dist_table.get(0, ins.goals[0]) == 0);
    assert(dist_table.get(0, ins.starts[0]) == 16);
  }

  return 0;
}
