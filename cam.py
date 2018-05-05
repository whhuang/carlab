from picamera import PiCamera
from picamera.array import PiRGBArray
from time import sleep
import cv2
import numpy as np
import serial

### Constants ###
framesize = (300,300)
N = 1#3 # Number of cars
IDs = ("A","B","C")

# color ranges (low, high) in [HSV]
#c_red = (np.array([0,190,70]), np.array([10,255,255]))    # red
c_green = (np.array([30,140,30]), np.array([75,255,255]))  # green
c_pink = (np.array([147,75,55]), np.array([180,255,255])) # pink
c_chartreuse = (np.array([27,50,30]), np.array([45, 255, 255])) # chartreuse
#c_orange = (np.array([8,100,70]), np.array([20, 255, 255])) # orange
c_blue = (np.array([95,40,40]), np.array([128, 255, 255])) # blue
color_range = (c_pink, c_chartreuse, c_blue)

### Setup ###
cam = PiCamera(resolution = framesize)
rawCapture = PiRGBArray(cam, size = framesize)

params = cv2.SimpleBlobDetector_Params()
params.minThreshold = 10;
params.maxThreshold = 200;
params.filterByColor = False
params.filterByArea = True
params.minArea = .0004 * (framesize[0]*framesize[1])
params.maxArea = 1 * (framesize[0]*framesize[1])
params.filterByCircularity = False
params.filterByConvexity = False
params.filterByInertia = False
detector = cv2.SimpleBlobDetector(params)

ser = serial.Serial('/dev/ttyUSB0')
ser.baudrate = 9600

sleep(.1)
print 'begin'


# Returns the position and angle of the given car
def extract_vector(features):
    # Step 1: Return -1 if there are not enough features
    if (len(features) < 2):
        return -1, -1, -1, -1, -1

    # Step 2: Sort the features by size
    features = sorted(features, key=lambda point: point.size, reverse = True)
    center = features[0] # center has the largest size
    periph = features[1]
    
    x = 1.0 * center.pt[0] # convert to float
    y = 1.0 * center.pt[1]
    x2 = 1.0 * periph.pt[0]
    y2 = 1.0 * periph.pt[1]
    r = np.arctan2(y2-y, x2-x) # [radians]
    
    return x, y, x2, y2,r


# Returns a list of vectors for all cars
def get_car_vectors():
    
    # Step 1: Get Raw Image
    cam.capture(rawCapture, 'bgr', use_video_port = True)
    im = rawCapture.array
    im_with_features = im
    cv2.imshow('Raw Image', im)

    car_vectors = [] # list of vectors describing each car
    for i in range(0,N): # For each car
        # Step 2: Filter by Color
        im_hsv = cv2.cvtColor(im, cv2.COLOR_BGR2HSV)
        im_mask = cv2.inRange(im_hsv, color_range[i][0], color_range[i][1])
        im_filtered = cv2.bitwise_and(im, im, mask = im_mask)
        # cv2.imshow("Filtered", im_filtered)
        
        # Step 3: Detect Blobs
        features = detector.detect(im_mask)
        x,y,x2,y2,direction = extract_vector(features)
        car_vectors.append([x,y,direction])

        # Step 4: Display Processed Image
        features = sorted(features, key=lambda point: point.size, reverse = True)
        im_with_features = cv2.drawKeypoints(im_with_features, features[0:2], np.array([]), (0,255,0),
                                         cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
        cv2.line(im_with_features, (int(x),int(y)), (int(x2),int(y+(x2-x)*np.tan(direction))), (0,140,255), 2)
        cv2.circle(im_with_features, (framesize[0]/2, framesize[1]/2), 5, (0,165,255),-1)
    cv2.imshow('Car Vectors', im_with_features)
    rawCapture.truncate(0)

    return car_vectors

def car_ref_frame(car_vector): # Converts (tx,ty,r) to the car's reference frame
    tx, ty, direction, r = car_vector

    # Step 1: Make the frame's center the origin
    recentered_x = tx-framesize[0]/2.0
    recentered_y = ty-framesize[1]/2.0

    # Step 2: Rotate tx, ty to the car's reference frame
    tx_rot = np.cos(-direction - np.pi/2) * recentered_x - np.sin(-direction - np.pi/2) * recentered_y
    ty_rot = np.sin(-direction + np.pi/2) * recentered_x + np.cos(-direction + np.pi/2) * recentered_y

    # Step 3: Rotate r and make sure it is on [-pi, pi]
    r_rot = -1 * (r - direction)
    r_rot = r_rot * 180 / np.pi # convert to degrees
    while (r_rot < -180):
        r_rot = r_rot + 360
    while (r_rot > 180):
        r_rot = r_rot - 360

    return [tx_rot, ty_rot, r_rot]




def compress_RGB(rgb_color): # Compress an RGB value to a number on [0,255]
    r_digit = int(rgb_color[0]/25.6) # [0,9]
    g_digit = int(rgb_color[1]/25.6) # [0,9]
    b_digit = int(rgb_color[2]/25.6) # [0,9]

    return r_digit * 100 + g_digit * 10 + b_digit

def tx_pkt(car_vectors, colors):
    pkt_list = [] # a list of the packet fields
    
    for i in range(0, len(car_vectors)): # for each car
        pkt_list.extend(IDs[i])
        pkt_list.extend(int(v) for v in car_ref_frame(car_vectors[i]))
        pkt_list.append(compress_RGB(colors[i]))

    pkt = ",".join(str(f) for f in pkt_list) + ","
    print(pkt)
    ser.write(pkt)

