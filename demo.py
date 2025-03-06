import build.bindings.lacam as lacam
import random
import skimage
import numpy as np


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
    testMap = house_generator()
    
    STARTS, GOALS = populateMap(testMap, 20)
    
    print(lacam.solve(testMap.tolist(), STARTS, GOALS, 5))
    
