
# coding: utf-8

# In[1]:


import numpy as np
import cv2
import matplotlib.pyplot as plt
import matplotlib.cm as cm


# In[2]:


# img = cv2.imread('a20-3.png')
# img = cv2.imread('a18-4.png')
img = cv2.imread('a26.png')
img.shape


# In[3]:


# 多边形检测 效果不好  Opencv GUI
# gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
 
# ret,thresh = cv2.threshold(gray,127,255,1)
 
# contours,h = cv2.findContours(thresh,1,2)
 
# for cnt in contours:
#     approx = cv2.approxPolyDP(cnt,0.01*cv2.arcLength(cnt,True),True)
#     print(len(approx))
#     if len(approx)==5:
# #         print "pentagon"
#         cv2.drawContours(img,[cnt],0,255,-1)
#     elif len(approx)==3:
# #         print "triangle"
#         cv2.drawContours(img,[cnt],0,(0,255,0),-1)
#     elif len(approx)==4:
# #         print "square"
#         cv2.drawContours(img,[cnt],0,(0,0,255),-1)
# #     elif len(approx) == 9:
# # #         print "half-circle"
# #         cv2.drawContours(img,[cnt],0,(255,255,0),-1)
#     elif len(approx) > 15:
# #         print "circle"
#         cv2.drawContours(img,[cnt],0,(0,255,255),-1) 
# cv2.imshow('img',img)
# cv2.waitKey(0)


# In[4]:


# loop over the boundaries
boundaries=[]
bg=[(100,74,56),(30,28,30)] # BGR
for base in bg:
    boundaries.append(([x-20 for x in base],[x+20 for x in base]))
# boundaries.append(([0,0,0],[255,255,255]))
# boundaries = [
#     ([1, 15, 100], [50, 56, 200]),
#     ([86, 31, 4], [220, 88, 50]),
#     ([25, 146, 190], [62, 174, 250]),
#     ([103, 86, 65], [145, 133, 128])
# ]
boundaries


# In[5]:


def draw_opencv_image(img):
    plt.figure(figsize=(20,20/1280*720))
    plt.imshow(img)
    plt.show()

def single_color_range(img,boundaries):
    for i,(lower, upper) in enumerate(boundaries):
        # create NumPy arrays from the boundaries
        lower = np.array(lower, dtype = "uint8")
        upper = np.array(upper, dtype = "uint8")

        # find the colors within the specified boundaries and apply
        # the mask
        mask = cv2.inRange(img, lower, upper)
        output = cv2.bitwise_and(img, img, dst=np.ones(img.shape).astype(np.uint8)*255, mask = mask)
    #     output=cv2.bitwise_and(img, np.ones(img.shape).astype(np.uint8)*255, mask=mask)
        output=np.vstack([img, output])

        # show the images

        print("images boundary_"+str(i))
        outputRGB = cv2.cvtColor(output, cv2.COLOR_BGR2RGB)
        plt.figure(figsize=(20,20))
        plt.imshow(outputRGB)
        plt.show()
        
def union_color_range(img,boundaries):
    mask = np.zeros(img.shape[:-1], dtype="uint8")
    for i,(lower, upper) in enumerate(boundaries):
        # create NumPy arrays from the boundaries
        lower = np.array(lower, dtype = "uint8")
        upper = np.array(upper, dtype = "uint8")

        # find the colors within the specified boundaries and apply
        # the mask
        single_mask = cv2.inRange(img, lower, upper)
        mask=np.bitwise_or(mask, single_mask)
    output = cv2.bitwise_and(img, img, dst=np.ones(img.shape).astype(np.uint8)*255, mask = mask)
#     output=cv2.bitwise_and(img, np.ones(img.shape).astype(np.uint8)*255, mask=mask)
    cv2.imwrite('output.png',output)
    
    output=np.vstack([img, output])

    # show the images

    print("images boundary_"+str(i))
    outputRGB = cv2.cvtColor(output, cv2.COLOR_BGR2RGB)
    plt.figure(figsize=(20,20))
    plt.imshow(outputRGB)
    plt.show()
    return mask
mask=union_color_range(img, boundaries)


# In[6]:


two_color_mask=mask


# In[19]:


# 聚类
# X = np.random.randint(25,50,(25,2))
# Y = np.random.randint(60,85,(25,2))
# Z = np.vstack((X,Y))
num_clusters=10

Z=[]
for x in range(two_color_mask.shape[0]):
    for y in range(two_color_mask.shape[1]):
        if two_color_mask[x,y]:
            Z.append((x,y))
            

# convert to np.float32
Z = np.float32(Z)
print('len=',len(Z))
# define criteria and apply kmeans()
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 100, 5.0)
ret,label,center=cv2.kmeans(Z,num_clusters ,None,criteria,10,cv2.KMEANS_RANDOM_CENTERS)

# Now separate the data, Note the flatten()
# A = Z[label.ravel()==0]
# B = Z[label.ravel()==1]
# print(len(A), len(B), A.max(), B.max())

# plt.figure(figsize=(72/10,128/10))
def draw_clusters(clusters):

    plt.figure(figsize=(20,20/1280*720))
    for p in clusters:
        plt.xlim(0,two_color_mask.shape[1])
        plt.ylim(0,two_color_mask.shape[0])
        plt.gca().set_aspect(aspect='equal')
        plt.gca().invert_yaxis()
        plt.scatter(p[:,1],p[:,0],s=0.01)
        
clusters=[]
for i in range(num_clusters):
    p=Z[label.ravel()==i]
    clusters.append(p)

print(two_color_mask.shape)    
draw_clusters(clusters)
plt.show()

# Plot the data
# plt.scatter(A[:,0],A[:,1])
# plt.scatter(B[:,0],B[:,1],c = 'r')
# plt.scatter(center[:,0],center[:,1],s = 80,c = 'y', marker = 's')
# plt.xlabel('Height'),plt.ylabel('Weight')


# In[20]:


import random as rng
# Find the rotated rectangles and ellipses for each contour
contours=[]
for cluster in clusters:
    contours.append(cluster[:,(1,0)])
# print(len(contours),[len(x) for x in contours])
# print(contours)
minRect = [None]*len(contours)
for i, c in enumerate(contours):
    minRect[i] = cv2.minAreaRect(c)
#     print('cluster %d %s\nminRect %s'%(i,c,minRect[i]))
    

# Draw contours + rotated rects + ellipses
drawing = np.zeros((img.shape[0], img.shape[1], 3), dtype=np.uint8)

# draw_clusters(clusters)
bouding_rectangle=[]
for i, c in enumerate(contours):
#     color = (rng.randint(0,256), rng.randint(0,256), rng.randint(0,256))
    color = (255, 255, 255)
    # contour
#     contours[i]=contours[i].astype("uint8")
    print(drawing.shape,len(contours))
#     cv2.drawContours(drawing, contours, i, color)

    # rotated rectangle
    if minRect[i][2]<-45:
        minRect[i]=(minRect[i][0],(minRect[i][1][1], minRect[i][1][0]),minRect[i][2]+90)
    box = cv2.boxPoints(minRect[i])
    print('%d\mminRect=%s\nbox=%s'%(i,minRect[i],box))
    box = np.intp(box) #np.intp: Integer used for indexing (same as C ssize_t; normally either int32 or int64)
    bouding_rectangle.append(box)
    print('newbox=%s'%box)
    cv2.drawContours(drawing, [box], 0, color, 𝚝𝚑𝚒𝚌𝚔𝚗𝚎𝚜𝚜=-1)
    
draw_opencv_image(drawing)
#     plt.figure(figsize=(20,20/1280*720))
#     plt.imshow(drawing)
# #     plt.gca().invert_yaxis()
#     plt.show()


# In[15]:


# 只能用于灰度图像deskew
# def deskew(image, angle):
#     image = cv2.bitwise_not(image)
#     non_zero_pixels = cv2.findNonZero(mask)
#     center, wh, theta = cv2.minAreaRect(non_zero_pixels)

#     root_mat = cv2.getRotationMatrix2D(center, angle, 1)
#     rows, cols = image.shape
#     rotated = cv2.warpAffine(image, root_mat, (cols, rows), flags=cv2.INTER_CUBIC)

#     return cv2.getRectSubPix(rotated, (cols, rows), center)
# print(img.shape)
# for bounding_rectangle in minRect: 
#     deskewed_image = deskew(img.copy(), bounding_rectangle[2])
#     draw_opencv_image(deskewed_image)
bouding_rectangle


# In[21]:


for rec in bouding_rectangle:
    pts1 = np.float32([rec[0], rec[1], rec[2], rec[3]])
    pts2 = np.float32([[0, 131], [0, 0], [126, 0], [126, 131]])
    matrix = cv2.getPerspectiveTransform(pts1, pts2)
    result = cv2.warpPerspective(img, matrix, (126, 131))
    result = cv2.cvtColor(result, cv2.COLOR_BGR2RGB)
    plt.imshow(result)
    plt.show()


# In[23]:


# face recognition
import face_recognition

# Create arrays of known face encodings and their names
known_face_encodings = []
known_face_names = []
for i in range(1,7):
    data_image = face_recognition.load_image_file("/home/eric/Work/vregRobot/face_data/%d.png"%(i,))
    data_encoding = face_recognition.face_encodings(data_image)[0]
    known_face_encodings.append(data_encoding)
    known_face_names.append(str(i))

def recogize_portrait(portrait_rgb):
    rgb_frame=protrait
    face_locations = [(0, portrait_rgb.shape[1], portrait_rgb.shape[0], 0)]
    face_encodings = face_recognition.face_encodings(rgb_frame, face_locations)

    # Loop through each face in this frame of video
    for (top, right, bottom, left), face_encoding in zip(face_locations, face_encodings):
        # See if the face is a match for the known face(s)
        matches = face_recognition.compare_faces(known_face_encodings, face_encoding)

        name = "Unknown"

        # If a match was found in known_face_encodings, just use the first one.
        # if True in matches:
        #     first_match_index = matches.index(True)
        #     name = known_face_names[first_match_index]

        # Or instead, use the known face with the smallest distance to the new face
        face_distances = face_recognition.face_distance(known_face_encodings, face_encoding)
        best_match_index = np.argmin(face_distances)
        if matches[best_match_index]:
            name = known_face_names[best_match_index]
        print(name)

