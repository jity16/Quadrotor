# Make sure to have the server side running in V-REP: 
# in a child script of a V-REP scene, add following command
# to be executed just once, at simulation start:
#
# simRemoteApi.start(19999)
#
# then start simulation, and run this program.
#
# IMPORTANT: for each successful call to simxStart, there
# should be a corresponding call to simxFinish at the end!
import numpy as np
try:
    import vrep
except:
    print ('--------------------------------------------------------------')
    print ('"vrep.py" could not be imported. This means very probably that')
    print ('either "vrep.py" or the remoteApi library could not be found.')
    print ('Make sure both are in the same folder as this file,')
    print ('or appropriately adjust the file "vrep.py"')
    print ('--------------------------------------------------------------')
    print ('')

import time
from math import *
delta = 0.02

def update_paramter(d_position):
    print(d_position)
    x = d_position[0]
    y = d_position[1]
    dirs = np.array([x,-x,y,-y])
    d = np.argmax(dirs)
    print(dirs[d])
    res,p1 = vrep.simxGetFloatSignal(clientID,"p1",vrep.simx_opmode_blocking)
    res,p2 = vrep.simxGetFloatSignal(clientID,"p2",vrep.simx_opmode_blocking)
    res,p3 = vrep.simxGetFloatSignal(clientID,"p3",vrep.simx_opmode_blocking)
    res,p4 = vrep.simxGetFloatSignal(clientID,"p4",vrep.simx_opmode_blocking)
    print(d)
    if d == 0:
        p1 += delta
        p2 -= delta
        p3 -= delta
        p4 += delta
    elif d == 1:
        p1 -= delta
        p2 += delta
        p3 += delta
        p4 -= delta
    elif d == 2:
        p1 += delta
        p2 += delta
        p3 -= delta
        p4 -= delta
    elif d == 3:
        p1 -= delta
        p2 -= delta
        p3 += delta
        p4 += delta
    p = np.array([p1,p2,p3,p4])
    [p1,p2,p3,p4] = p
    print(p1,p2,p3,p4)
    vrep.simxSetFloatSignal(clientID,"p1",p1,vrep.simx_opmode_blocking)
    vrep.simxSetFloatSignal(clientID,"p2",p2,vrep.simx_opmode_blocking)
    vrep.simxSetFloatSignal(clientID,"p3",p3,vrep.simx_opmode_blocking)
    vrep.simxSetFloatSignal(clientID,"p4",p4,vrep.simx_opmode_blocking)




print ('Program started')
vrep.simxFinish(-1) # just in case, close all opened connections
clientID=vrep.simxStart('127.0.0.1',19997,True,True,5000,5) # Connect to V-REP
if clientID!=-1:
    print ('Connected to remote API server')
    vrep.simxSetFloatSignal(clientID,"p1",6.5,vrep.simx_opmode_blocking)
    vrep.simxSetFloatSignal(clientID,"p2",6.8,vrep.simx_opmode_blocking)
    vrep.simxSetFloatSignal(clientID,"p3",7.5,vrep.simx_opmode_blocking)
    vrep.simxSetFloatSignal(clientID,"p4",7.2,vrep.simx_opmode_blocking)
    # Now try to retrieve data in a blocking fashion (i.e. a service call):
    res,objs=vrep.simxGetObjects(clientID,vrep.sim_handle_all,vrep.simx_opmode_blocking)
    if res==vrep.simx_return_ok:
        print ('Number of objects in the scene: ',len(objs))
    else:
        print ('Remote API function call returned with error code: ',res)
    res, base = vrep.simxGetObjectHandle(clientID,"Quadricopter_base",vrep.simx_opmode_blocking)
    res, init_position = vrep.simxGetObjectPosition(clientID,base, -1,vrep.simx_opmode_blocking)
    # Now retrieve streaming data (i.e. in a non-blocking fashion):
    for i in range(10):
        print(i,":")
        vrep.simxStartSimulation(clientID, vrep.simx_opmode_blocking)
        startTime=time.time()
        while time.time()-startTime < 5:
            time.sleep(0.005)
        res, end_position = vrep.simxGetObjectPosition(clientID,base, -1,vrep.simx_opmode_blocking)
        d_position = np.array(end_position) - np.array(init_position)
        update_paramter(d_position)
        vrep.simxStopSimulation(clientID,vrep.simx_opmode_blocking)
        time.sleep(0.5)



    # Now close the connection to V-REP:
    vrep.simxFinish(clientID)
else:
    print ('Failed connecting to remote API server')
print ('Program ended')
