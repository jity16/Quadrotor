# coding=utf-8
import vrep
import numpy as np
import time
from math import *
if __name__ == '__main__':
    vrep.simxFinish(-1)  # just in case, close all opened connections
    clientID = vrep.simxStart('127.0.0.1', 19997, True,
                              True, 5000, 5)  # Connect to V-REP
    res, drone = vrep.simxGetObjectHandle(clientID,"drone_zed",vrep.simx_opmode_blocking)
    res, target = vrep.simxGetObjectHandle(clientID,"Quadricopter_target",vrep.simx_opmode_blocking)
    assert drone != 0
    assert target != 0
    try:
        ps = np.arange(0.004,0.02,0.002)
        ds = np.arange(0.5,4,0.5)
        with open("mesh.txt",'w') as f:
            for p in ps:
                for d in ds:
                    f.write("p: "+str(round(p,2))+"\td: "+str(round(d,2))+'\t')
                    print("p: "+str(round(p,2))+"\td: "+str(round(d,2)))
                    vrep.simxSetFloatSignal(clientID,"p",p,vrep.simx_opmode_blocking)
                    vrep.simxSetFloatSignal(clientID,"d",d,vrep.simx_opmode_blocking)
                    vrep.simxStartSimulation(clientID, vrep.simx_opmode_blocking)
                    start = time.time()
                    max_dis = 0
                    while True:
                        res, sp = vrep.simxGetObjectPosition(clientID,drone,target,vrep.simx_opmode_blocking)
                        max_dis =abs(sp[0]) if abs(sp[0]) > max_dis else max_dis
                        res, vel, _ = vrep.simxGetObjectVelocity(clientID,drone,vrep.simx_opmode_blocking)
                        if abs(sp[0])<0.1 and abs(vel[1])<0.1:
                            f.write("time: "+str(round(time.time() - start,2))+'\n')
                            f.write("dis: "+str(round(max_dis,2))+"\n\n")
                            print("time: "+str(round(time.time() - start,2)))
                            print("dis: "+str(round(max_dis,2)))
                            break
                        if max_dis > 2:
                            f.write("dis: "+str(round(max_dis,2))+"\n\n")
                            print("dis: "+str(round(max_dis,2))+"\n")
                            break
                        if time.time() - start > 7:
                            f.write("time: inf\n")
                            f.write("dis: "+str(round(max_dis,2))+"\n\n")
                            print("dis: "+str(round(max_dis,2)))
                            print("time: inf\n")
                            break

                    # f.write("distance: "+str(round(sp[0],2))+'\t'+str(round(sp[1],2))+'\t'+str(round(sp[2],2))+'\n')
                    # f.write("Velocity: "+str(round(vel[0],2))+'\t'+str(round(vel[1],2))+'\t'+str(round(vel[2],2))+'\n\n')
                    print("distance: "+str(round(sp[0],2))+'\t'+str(round(sp[1],2))+'\t'+str(round(sp[2],2)))
                    print("Velocity: "+str(round(vel[0],2))+'\t'+str(round(vel[1],2))+'\t'+str(round(vel[2],2))+'\n')
                    vrep.simxStopSimulation(clientID, vrep.simx_opmode_blocking)
                    time.sleep(0.5)
    except KeyboardInterrupt:
        pass
    finally:
        vrep.simxFinish(clientID)
