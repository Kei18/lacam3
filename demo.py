import build.bindings.lacam as lacam
import random
import skimage
import numpy as np
import argparse
import time


def house_generator(env_size=(10, 40), obstacle_ratio=10, remove_edge_ratio=6):
    min_size = env_size[0]
    max_size = env_size[1]
    world_size = np.random.randint(min_size, max_size, 1)
    world_size = world_size[0]
    world = np.zeros((world_size, world_size), dtype=np.int32)
    all_x = range(2, world_size - 2)
    all_y = range(2, world_size - 2)
    obs_edge = []
    obs_corner_x = []
    while len(obs_corner_x) < world_size // obstacle_ratio:
        corn_x = random.sample(all_x, 1)
        near_flag = False
        for i in obs_corner_x:
            if abs(i - corn_x[0]) == 1:
                near_flag = True
        if not near_flag:
            obs_corner_x.append(corn_x[0])
    obs_corner_y = []
    while len(obs_corner_y) < world_size // obstacle_ratio:
        corn_y = random.sample(all_y, 1)
        near_flag = False
        for i in obs_corner_y:
            if abs(i - corn_y[0]) == 1:
                near_flag = True
        if not near_flag:
            obs_corner_y.append(corn_y[0])
    obs_corner_x.append(0)
    obs_corner_x.append(world_size - 1)
    obs_corner_y.append(0)
    obs_corner_y.append(world_size - 1)

    for i in obs_corner_x:
        edge = []
        for j in range(world_size):
            world[i][j] = -1
            if j not in obs_corner_y:
                edge.append([i, j])
            if j in obs_corner_y and edge != []:
                obs_edge.append(edge)
                edge = []

    for i in obs_corner_y:
        edge = []
        for j in range(world_size):
            world[j][i] = -1
            if j not in obs_corner_x:
                edge.append([j, i])
            if j in obs_corner_x and edge != []:
                obs_edge.append(edge)
                edge = []

    all_edge_list = range(len(obs_edge))
    remove_edge = random.sample(all_edge_list, len(obs_edge) // remove_edge_ratio)
    for edge_number in remove_edge:
        for current_edge in obs_edge[edge_number]:
            world[current_edge[0]][current_edge[1]] = 0

    for edges in obs_edge:
        if len(edges) == 1 or len(edges) <= world_size // 20:
            for coordinates in edges:
                world[coordinates[0]][coordinates[1]] = 0
    _, count = skimage.measure.label(world, background=-1, connectivity=1, return_num=True)

    while count != 1 and len(obs_edge) > 0:
        door_edge_index = random.sample(range(len(obs_edge)), 1)[0]
        door_edge = obs_edge[door_edge_index]
        door_index = random.sample(range(len(door_edge)), 1)[0]
        door = door_edge[door_index]
        world[door[0]][door[1]] = 0
        _, count = skimage.measure.label(world, background=-1, connectivity=1, return_num=True)
        # if new_count == count:
        #     world[door[0]][door[1]] = -1
        #     obs_edge.remove(door_edge)
        # else:
        obs_edge.remove(door_edge)
        #     count = new_count
    # world = np.zeros((world_size, world_size))
    world[:, -1] = world[:, 0] = -1
    world[-1, :] = world[0, :] = -1

    # nodes_obs = get_map_nodes(world)
    # return world, nodes_obs
    return world


def getFreeCell(world):

    listOfFree = np.swapaxes(np.where(world==0), 0,1)
    np.random.shuffle(listOfFree)
    return (listOfFree[0][0], listOfFree[0][1])

def populateMap(testMap, numAgents):
    tempMap = np.copy(testMap)
    agentPos = []
    goalPos = []
    for i in range(numAgents):
        agentPos.append(tuple(getFreeCell(tempMap)))
        tempMap[agentPos[-1]] = 2
    tempMap = np.copy(testMap)
    for i in range(numAgents):
        goalPos.append(tuple(getFreeCell(tempMap)))
        tempMap[goalPos[-1]] = 3

    return agentPos, goalPos

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--seed", type=int, default=0)
    parser.add_argument("--numAgents", type=int, default=20)
    parser.add_argument("--timeLimitSec", type=float, default=1.0)
    parser.add_argument("--viz", action=argparse.BooleanOptionalAction, default=True)
    args = parser.parse_args()

    random.seed(args.seed)
    np.random.seed(args.seed)
    testMap = house_generator()
    STARTS, GOALS = populateMap(testMap, args.numAgents)

    print(f"mapSize={testMap.shape}, args={args}, start lacam3")
    solution = lacam.solve(testMap.tolist(), STARTS, GOALS, args.timeLimitSec)
    print("solved" if len(solution) > 0 else "failed")

    # simple visualisation
    if args.viz and len(solution) > 0:

        def colored(r, g, b, text):
            return f"\033[38;2;{r};{g};{b}m{text}\033[0m"

        colors = np.random.randint(0, 255, (args.numAgents, 3))

        def getConfigStr(Q):
            s = list(
                map(
                    lambda k: list(map(lambda x: "." if x == 0 else "@", testMap[k])),
                    range(testMap.shape[0]),
                )
            )
            for i, (x, y) in enumerate(Q):
                s[x][y] = colored(*colors[i], "x" if (x, y) != GOALS[i] else "o")
            return "\n".join(["".join(r) for r in s])

        solution = np.array(solution).transpose((1, 0, 2))
        T = len(solution) - 1
        for t, Q in enumerate(solution):
            goalReaching = sum(list(map(lambda x: x.all(), Q == solution[-1])))
            print(
                f"t={t:03d}/{T:03d}, goalReaching={goalReaching:03d}/{args.numAgents:03d}"
            )
            print(getConfigStr(Q) + (f"\033[{len(testMap)+1}A" if t < T else ""))
            time.sleep(0.2)
