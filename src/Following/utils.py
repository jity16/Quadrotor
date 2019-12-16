# coding=utf-8
import vrep
import math
from PIL import Image as I
import array
import numpy as np
import logging
import time

recognize = False

logging.getLogger().setLevel(logging.INFO)
vrep.simxFinish(-1)
clientID = vrep.simxStart('127.0.0.1', 19997, True, True, 5000, 5)  # Connect to V-REP

if clientID == -1:
    logging.error("Failed to connect to remote API Server")
    exit()
else:
    logging.info('Connected to remote API server')
# _, v0 = vrep.simxGetObjectHandle(clientID, 'zed_vision0', vrep.simx_opmode_blocking)
# _, drone = vrep.simxGetObjectHandle(clientID, 'drone_zed', vrep.simx_opmode_blocking)
# _, base = vrep.simxGetObjectHandle(clientID, 'Quadricopter_base', vrep.simx_opmode_blocking)
# _, target = vrep.simxGetObjectHandle(clientID, 'Quadricopter_target', vrep.simx_opmode_blocking)
v0, drone, base, target = 503, 479, 500, 476
err, resolution, image = vrep.simxGetVisionSensorImage(clientID, v0, 0, vrep.simx_opmode_streaming)

print('v0 = ', v0, 'drone = ', drone, 'base = ', base, 'target = ', target)


# _, v1 = vrep.simxGetObjectHandle(clientID, 'zed_vision1', vrep.simx_opmode_oneshot_wait)
# vrep.simxStartSimulation(clientID, vrep.simx_opmode_blocking)

def change_color_and_resize(image, resolution):
    for i in range(len(image)):
        image[i] = (image[i] + 256) % 256
    image_buffer = I.frombuffer("RGB", (resolution[0], resolution[1]), bytes(image), "raw", "RGB", 0, 1).rotate(180)
    return np.array(image_buffer)


def get_sensor_image(vision_sensor):
    cnt = 0
    while (vrep.simxGetConnectionId(clientID) != -1):
        err, resolution, image = vrep.simxGetVisionSensorImage(clientID, vision_sensor, 0, vrep.simx_opmode_buffer)
        cnt += 1
        if err == vrep.simx_return_ok:
            image_buffer = change_color_and_resize(image, resolution)
            return image_buffer
        if cnt % 2048 == 0:
            logging.warning('get_sensor_image retry %d tiems' % cnt)
            # image_buffer.save("a%d.png"%(cnt,))


default_height = 3


def calc_angle_by_xy(x, y):
    return math.atan2(y, x) / math.pi * 180


def set_target_position(pos):
    if len(pos) < 4:
        pos.append(default_height)
    now_pos = get_drone_position()
    print('move target from ', round(now_pos[0], 2), ',', round(now_pos[1], 2), ',', round(now_pos[2], 2), ' to ',
          pos[1], ',', pos[2], ',', pos[3], "rotate(deg) :", pos[0])
    vrep.simxSetObjectOrientation(clientID, target, base, [0, 0, math.radians(pos[0])], vrep.simx_opmode_oneshot)
    vrep.simxSetObjectPosition(clientID=clientID,
                               objectHandle=target,
                               relativeToObjectHandle=-1,
                               position=pos[1:],
                               operationMode=vrep.simx_opmode_oneshot)


def rotate_drone(angle):
    vrep.simxSetObjectOrientation(clientID=clientID,
                                  objectHandle=target,
                                  relativeToObjectHandle=base,
                                  eulerAngles=[0, 0, angle],
                                  operationMode=vrep.simx_opmode_blocking)


def rotate_drone_to(angle):
    vrep.simxSetObjectOrientation(clientID=clientID,
                                  objectHandle=target,
                                  relativeToObjectHandle=base,
                                  eulerAngles=[0, 0, math.radians(angle)],
                                  operationMode=vrep.simx_opmode_oneshot)


def get_drone_position():
    return get_position(drone)


def get_target_position():
    return get_position(target)


streamed_handle = set()


def get_position(handle):
    if handle not in streamed_handle:
        error_code, ret = vrep.simxGetObjectPosition(clientID=clientID,
                                                     objectHandle=handle,
                                                     relativeToObjectHandle=-1,
                                                     operationMode=vrep.simx_opmode_streaming_split + 1000)
        streamed_handle.add(handle)
    while True:
        error_code, ret = vrep.simxGetObjectPosition(clientID=clientID,
                                                 objectHandle=handle,
                                                 relativeToObjectHandle=-1,
                                                 operationMode=vrep.simx_opmode_buffer)
        if not error_code:
            break
    print(handle, error_code, ret)
    return ret


def get_drone_angle():
    _, ret = vrep.simxGetObjectOrientation(clientID=clientID,
                                           objectHandle=drone,
                                           relativeToObjectHandle=-1,
                                           operationMode=vrep.simx_opmode_blocking)
    return ret


def distance_between_drone_and_target():
    drone_pos = get_drone_position()
    target_pos = get_target_position()
    return distance(drone_pos, target_pos)


def distance(p1, p2):
    return np.linalg.norm(np.array([d1 - d2 for d1, d2 in zip(p1, p2)]))


if __name__ == '__main__':
    print(calc_angle_by_xy(0.1, 0))
    print(calc_angle_by_xy(0, 0.1))
    print(calc_angle_by_xy(-0.1, 0))
    print(calc_angle_by_xy(0, -0.1))
    print(calc_angle_by_xy(0, 0))


def disconnect():
    # vrep.simxStopSimulation(clientID, vrep.simx_opmode_blocking)
    vrep.simxFinish(clientID)
