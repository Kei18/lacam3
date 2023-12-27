#include <cassert>
#include <lacam.hpp>

int main()
{
  {
    const auto scen_filename = "../assets/random-32-32-10-random-1.scen";
    const auto map_filename = "../assets/random-32-32-10.map";
    const auto ins = Instance(scen_filename, map_filename, 3);

    assert(size(ins.starts) == 3);
    assert(size(ins.goals) == 3);
    assert(ins.starts[0]->index == 203);
    assert(ins.goals[0]->index == 583);
  }

  return 0;
}
