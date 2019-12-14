import numpy as np
import vrep
import time
from math import *


def run(path,raising = False):
    index = 0
    for p in path:
        index += 1
        vrep.simxSetObjectPosition(
            clientID, q_target, q_target, p, vrep.simx_opmode_blocking)
        print(p)
        point1 = vrep.simxGetObjectPosition(clientID,q_target,-1,vrep.simx_opmode_blocking)
        point2 = vrep.simxGetObjectPosition(clientID,target,-1,vrep.simx_opmode_blocking)
        print(point1)
        print(point2)
        res, d_pos = vrep.simxGetObjectPosition(
            clientID, base, q_target, vrep.simx_opmode_blocking)
        threshold = 0.02
        while np.linalg.norm(d_pos) > threshold:
            res, d_pos = vrep.simxGetObjectPosition(
                clientID, base, q_target, vrep.simx_opmode_blocking)
            res, t_pos = vrep.simxGetObjectPosition(
                clientID,  q_target, -1, vrep.simx_opmode_blocking)
            res, q_pos = vrep.simxGetObjectPosition(
                clientID, base, -1, vrep.simx_opmode_blocking)
            t_pos[2] = q_pos[2]
            vrep.simxSetObjectPosition(clientID, base,-1,t_pos, vrep.simx_opmode_blocking)
        print("到达目标点", index, point1)


vrep.simxFinish(-1)  # just in case, close all opened connections
clientID = vrep.simxStart('127.0.0.1', 19997, True,
                          True, 5000, 5)  # Connect to V-REP
vrep.simxSetIntegerSignal(clientID,'close_hand',0,vrep.simx_opmode_blocking)
res, target = vrep.simxGetObjectHandle(
    clientID, "Target", vrep.simx_opmode_blocking)
res, q_target = vrep.simxGetObjectHandle(
    clientID, "Quadricopter_target", vrep.simx_opmode_blocking)
res, base = vrep.simxGetObjectHandle(
    clientID, "Quadricopter_base", vrep.simx_opmode_blocking)
if clientID != -1:
    print('Connected to remote API server')
    vrep.simxStartSimulation(clientID, vrep.simx_opmode_blocking)
    res, target_position = vrep.simxGetObjectPosition(
        clientID, target, -1, vrep.simx_opmode_blocking)
    path = []
    path.append([0,0,-0.35])
    path.append([0,0,-0.04])
    run(path)
    vrep.simxSetIntegerSignal(clientID,"close_hand",1,vrep.simx_opmode_blocking)
    time.sleep(1)
    # input("回车键继续")
    path = []
    path.append([0,0,0.8])
    run(path)
    input("回车键结束")
    vrep.simxStopSimulation(clientID, vrep.simx_opmode_blocking)
    vrep.simxFinish(clientID)
else:
    print('Failed connecting to remote API server')
print('Program ended')
