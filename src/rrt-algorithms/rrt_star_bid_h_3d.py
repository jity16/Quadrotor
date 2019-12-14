# This file is subject to the terms and conditions defined in
# file 'LICENSE', which is part of this source code package.
import numpy as np
import time

from src.rrt.rrt_star_bid_h import RRTStarBidirectionalHeuristic
from src.search_space.search_space import SearchSpace
from src.utilities.plotting import Plot

X_dimensions = np.array([(-20, 20), (-20, 20), (0, 20)])  # dimensions of Search Space
# obstacles
# Obstacles = np.array(
#     [(20, 20, 20, 40, 40, 40), (20, 20, 60, 40, 40, 80), (20, 60, 20, 40, 80, 40), (60, 60, 20, 80, 80, 40),
#      (60, 20, 20, 80, 40, 40), (60, 20, 60, 80, 40, 80), (20, 60, 60, 40, 80, 80), (60, 60, 60, 80, 80, 80)])
Obstacles=None

for x in X_dimensions:
    for i in range(len(x)):
        x[i]*=10

def set_obstacles(obs):
    global Obstacles
    Obstacles = np.array(obs)
    print('python obs=', obs)
    for x in Obstacles:
        for i in range(len(x)):
            x[i]*=10

def sovle_rrt(src,dst):
    # x_init = (10.7016,0.472867,0.813236 )  # starting location
    # x_goal = (-5,0,1)  # goal location
    x_init=src
    x_goal=dst

    x_init = tuple([x*10 for x in x_init])
    x_goal = tuple([x*10 for x in x_goal])
    X_dimensions[2][0]=x_init[2]
    X_dimensions[2][1]=x_init[2]+1

    print('src, dst= ', x_init, x_goal)

    Q = np.array([(2, 1)])  # length of tree edges
    r = 1  # length of smallest edge to check for intersection with obstacles
    max_samples = 1024  # max number of samples to take before timing out
    rewire_count = 32  # optional, number of nearby branches to rewire
    prc = 0.01  # probability of checking for a connection to goal

    # create Search Space
    X = SearchSpace(X_dimensions, Obstacles)


    # create rrt_search
    # for t in range(100):
    #     a=time.time()
    rrt = RRTStarBidirectionalHeuristic(X, Q, x_init, x_goal, max_samples, r, prc, rewire_count)
    path = rrt.rrt_star_bid_h()
    for j in range(len(path)):
        path[j]=[path[j][i]/10 for i in range(len(path[j]))]
    #     b=time.time()
    #     print('time=', b-a)
    print(path)

    # # plot
    # plot = Plot("rrt_star_bid_h_3d")
    # plot.plot_tree(X, rrt.trees)
    # if path is not None:
    #     plot.plot_path(X, path)
    # plot.plot_obstacles(X, Obstacles)
    # plot.plot_start(X, x_init)
    # plot.plot_goal(X, x_goal)
    # plot.draw(auto_open=True)
    return path
