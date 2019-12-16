import numpy as np
import cv2
from matplotlib import pyplot as plt
import logging

color_ranges=[]
# red
# color.append([np.asarray([170, 110, 110]), np.asarray([210, 140, 140])])
color_ranges.append([np.asarray([0, 80, 150]), np.asarray([5, 100, 220])])
# green
# color.append([np.asarray([122, 162, 127]), np.asarray([152, 202, 157])])
color_ranges.append([np.asarray([60, 60, 160]), np.asarray([80, 80, 230])])
# blue
# color.append([np.asarray([110, 130, 170]), np.asarray([137, 159, 202])])
color_ranges.append([np.asarray([100, 77, 160]), np.asarray([120, 97, 200])])
# deep grey
# color.append([np.asarray([95, 95, 95]), np.asarray([120, 120, 120])])
color_ranges.append([np.asarray([0, 0, 95]), np.asarray([0, 0, 120])])
# shadow grey
# color.append([np.asarray([156, 156, 156]), np.asarray([176, 176, 176])])
color_ranges.append([np.asarray([0, 0, 150]), np.asarray([0, 0, 200])])
# brown
color_ranges.append([np.asarray([10,101,168]),np.asarray([10,103,199])])
# deep brown
color_ranges.append([np.asarray([21,77,143]),np.asarray([22,80,154])])
id_to_color=('red','green','blue','deep grey','shadow grey','brown','deep brown')


def color_mask(img):
    flag, thresh = cv2.threshold(img[:,:,0], 70, 255, cv2.THRESH_BINARY_INV)
    # BLACK MAGIC
    color_p=[]
    color_p.append([np.asarray([19, 40, 123]), np.asarray([28, 60, 143])])
    color_p.append([np.asarray([12, 35, 103]), np.asarray([18, 47, 127])])
    color_p.append([np.asarray([11, 43, 53]), np.asarray([20, 82, 81])])
    color_p.append([np.asarray([23, 24, 64]), np.asarray([35, 88, 128])])
    color_p.append([np.asarray([2, 51, 40]), np.asarray([22, 83, 121])])
    color_p.append([np.asarray([18, 58, 93]), np.asarray([24, 71, 155])])
    color_p.append([np.asarray([28, 26, 107]), np.asarray([34, 50, 166])])
    color_p.append([np.asarray([26, 42, 111]), np.asarray([31, 53, 133])])
    img_hsv = cv2.cvtColor(img, cv2.COLOR_RGB2HSV)
    final_mask=thresh
    for i in range(len(color_p)):
        mask = cv2.inRange(img_hsv, color_p[i][0], color_p[i][1])
        final_mask=np.bitwise_or(final_mask, mask)
    return final_mask
    # flag, thresh = cv2.threshold(img[:,:,0], 70, 255, cv2.THRESH_BINARY_INV)
    # return thresh


def find_contours(mask):
    # Prepocess
    gray = mask
    blur = cv2.GaussianBlur(gray,(1,1),1000)
    flag, thresh = cv2.threshold(blur, 120, 255, cv2.THRESH_BINARY)

    # Find contours
    contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_NONE)

    # Select long perimeters only
    listindex=[i for i in range(len(contours)) if hierarchy[0,i,3]==-1 and cv2.contourArea(contours[i])>10]
    contours=[contours[i] for i in listindex]
    logging.info('INFO: # potential faces = %d'%len(listindex))
    return contours


def perspective_transformation(img, contours):
    num_faces=len(contours)
    warp = [None for i in range(num_faces)]
    bounding_parallelogram_boxes=[None for i in range(len(contours))]
    output_size=(120,120)
    for i in range(num_faces):
        contour = contours[i]
        peri = cv2.arcLength(contour,True)
        for exp_threshold in range(0,20):
            approx = cv2.approxPolyDP(contour,0.04*peri*2**(exp_threshold/2),True)
            if len(approx)==4:
                break
            elif len(approx)<4:
                rect = cv2.minAreaRect(contours[i])
                approx = [cv2.boxPoints(rect)]
                break

        # extract points from nested list
        approx = np.array([item for sublist in approx for item in sublist],np.float32)

        # pair bouding points with target points
        approx=sorted(approx.tolist(),key=lambda x:x[1])
        top_points=sorted(approx[:2])
        bottom_points=sorted(approx[-2:])
        approx=np.array((bottom_points[0],*top_points,bottom_points[1]),np.float32)
        # print('face_id=%d apprxo= %s'%(i ,approx) )

        h = np.array([ [0,output_size[1]],[0,0],[output_size[0],0],
            [output_size[0],output_size[1]] ], np.float32)
        transform = cv2.getPerspectiveTransform(approx,h)
        warp[i] = cv2.warpPerspective(img,transform,output_size)
        bounding_parallelogram_boxes[i]=approx
    return warp, bounding_parallelogram_boxes

def read_rgb_img(filename):
    img = cv2.imread(filename)
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    return img


def which_color(image_block_rgb):
    img_hsv = cv2.cvtColor(image_block_rgb, cv2.COLOR_RGB2HSV)
    color_cnt=np.zeros((len(color_ranges,)))
    for i, color_range in enumerate(color_ranges):
        mask = cv2.inRange(img_hsv, color_range[0], color_range[1])
        color_cnt[i]=cv2.countNonZero(mask)
    color_id = color_cnt.argmax()
    if color_cnt[color_id]<=1:
        return -1
    return color_id


def get_clothes_color(img_rgb, center,size, i):
    if center[1]+size[1]-10>=0 and center[1]+size[1]+10<720 and center[0]+size[0]-10>=0 and center[0]+size[0]+10<1280:
        return which_color(img_rgb[center[1]+size[1]-10:center[1]+size[1]+10,center[0]+size[0]-10:center[0]+size[0]+10])
    else:
        return -2

def get_pants_color(img_rgb, center,size, i):
    if center[1]+size[1]*5-10>=0 and center[1]+size[1]*5+10<720 and center[0]+size[0]*5-10>=0 and center[0]+size[0]*5+10<1280:
        return which_color(img_rgb[center[1]+size[1]*5-10:center[1]+size[1]*5+10,center[0]+size[0]*5-10:center[0]+size[0]*5+10])
    else:
        return -2

def find_target(img_rgb,color_id):
    img_hsv = cv2.cvtColor(img_rgb, cv2.COLOR_RGB2HSV)
    mask = cv2.inRange(img_hsv, color_ranges[color_id][0], color_ranges[color_id][1])
    pos=(mask.flatten()!=0).argmax()
    pos=(pos%mask.shape[1],pos//mask.shape[1])
    return pos

# # from facerecognition.recognition import *
# img=read_rgb_img('/home/eric/Work/vregRobot/contour_detection/a4.png')
# contours=find_contours(color_mask(img))
# faces, boxes=perspective_transformation(img, contours)
# # faces为识别出脸的结果，识别失败为-1
# for i, face in enumerate(faces):
#     # plt.imshow(face)
#     # plt.savefig('output/%d.png'%i)
#     # people_id=recogize_portrait(face)
#     people_color = get_person_color(img, boxes[i], i)
#     print('%d people= box=%s'%(i, boxes[i]))
