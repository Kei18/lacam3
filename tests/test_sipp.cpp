#include <cassert>
#include <lacam.hpp>

int main()
{
  {
    const auto scen_filename = "../tests/assets/sapp.scen";
    const auto map_filename = "../tests/assets/sapp.map";
    const auto ins = Instance(scen_filename, map_filename, 1);
    auto D = DistTable(ins);
    auto CT = CollisionTable(&ins);
    const auto i = 0;
    auto path1 = sipp(i, ins.starts[i], ins.goals[i], &D, &CT);
    assert(path1 == Path({ins.G->V[0], ins.G->V[1], ins.G->V[2], ins.G->V[3]}));

    // vertex collision
    auto dynamic_obs_traj = Path({ins.G->V[2], ins.G->V[1], ins.G->V[4]});
    CT.enrollPath(1, dynamic_obs_traj);
    auto path2 = sipp(i, ins.starts[i], ins.goals[i], &D, &CT);
    assert(path2 == Path({ins.G->V[0], ins.G->V[0], ins.G->V[1], ins.G->V[2],
                          ins.G->V[3]}));

    // edge collision
    CT.clearPath(1, dynamic_obs_traj);
    dynamic_obs_traj =
        Path({ins.G->V[3], ins.G->V[2], ins.G->V[1], ins.G->V[4]});
    CT.enrollPath(1, dynamic_obs_traj);
    auto path3 = sipp(i, ins.starts[i], ins.goals[i], &D, &CT);
    assert(path3 == Path({ins.G->V[0], ins.G->V[0], ins.G->V[0], ins.G->V[1],
                          ins.G->V[2], ins.G->V[3]}));
  }

  {
    const auto scen_filename = "../tests/assets/sapp2.scen";
    const auto map_filename = "../tests/assets/sapp2.map";
    const auto ins = Instance(scen_filename, map_filename, 1);
    auto D = DistTable(ins);
    auto CT = CollisionTable(&ins);
    const auto i = 0;
    auto path1 = sipp(i, ins.starts[i], ins.goals[i], &D, &CT);
    assert(path1 == Path({ins.G->V[1]}));

    auto dynamic_obs_traj = Path({ins.G->V[0], ins.G->V[1], ins.G->V[2]});
    CT.enrollPath(1, dynamic_obs_traj);
    auto path2 = sipp(i, ins.starts[i], ins.goals[i], &D, &CT);
    assert(path2 == Path({ins.G->V[1], ins.G->V[5], ins.G->V[1]}));

    CT.clearPath(1, dynamic_obs_traj);
    dynamic_obs_traj = Path({ins.G->V[0], ins.G->V[1], ins.G->V[5]});
    CT.enrollPath(1, dynamic_obs_traj);
    auto path3 = sipp(i, ins.starts[i], ins.goals[i], &D, &CT);
    assert(path3 == Path({ins.G->V[1], ins.G->V[2], ins.G->V[1]}));
  }

  return 0;
}
