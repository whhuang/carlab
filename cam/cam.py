from picamera import PiCamera
from picamera.array import PiRGBArray
from time import sleep
import cv2
import numpy as np
import serial

### Constants ###
framesize = (300,300)
N = 3 # Number of cars

l = 50

# color range for each robot (low, high)
# in [HSV], not RGB
c_red = (np.array([0,190,70]), np.array([10,255,255]))    # red
c_green = (np.array([30,140,30]), np.array([75,255,255]))  # green
c_pink = (np.array([147,100,70]), np.array([180,255,255])) # pink
c_chartreuse = (np.array([27,50,30]), np.array([45, 255, 255])) # chartreuse
c_orange = (np.array([8,100,70]), np.array([20, 255, 255])) # orange
color_range = (c_pink, c_chartreuse, c_orange)

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
def extract_vector(keypoints):
    # Step 1: Return -1 if there's not enough keypoints
    if (len(keypoints) < 2):
        return -1, -1, -1, -1, -1

    # Step 2: Sort the Keypoints by size
    keypoints = sorted(keypoints, key=lambda point: point.size, reverse = True)
    center = keypoints[0] # center has the largest size
    periph = keypoints[1]
    
    x = 1.0 * center.pt[0] # convert to float
    y = 1.0 * center.pt[1]
    x2 = 1.0 * periph.pt[0]
    y2 = 1.0 * periph.pt[1]
    r = np.arctan((y2-y)/(x2-x)) # [radians]
    return x, y, x2, y2,r


# Returns a list of vectors for all cars
def get_car_vectors():
    
    # Step 1: Get Raw Image
    cam.capture(rawCapture, 'bgr', use_video_port = True)
    im = rawCapture.array
    im_with_keypoints = im
    cv2.imshow('Raw Image', im)

    car_vectors = [] # list of vectors describing each car
    for i in range(0,N): # For each car
        # Step 2: Filter by Color
        im_hsv = cv2.cvtColor(im, cv2.COLOR_BGR2HSV)
        im_mask = cv2.inRange(im_hsv, color_range[i][0], color_range[i][1])
        im_filtered = cv2.bitwise_and(im, im, mask = im_mask)
        #cv2.imshow("Filtered", im_filtered)
        
        # Step 3: Detect Blobs
        keypoints = detector.detect(im_mask)
        x,y,x2,y2,r = extract_vector(keypoints)
        car_vectors.append([x,y,r])

        # Step 4: Display Processed Image
        keypoints = sorted(keypoints, key=lambda point: point.size, reverse = True)
        im_with_keypoints = cv2.drawKeypoints(im_with_keypoints, keypoints[0:2], np.array([]), (0,255,0),
                                         cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
        cv2.line(im_with_keypoints, (int(x),int(y)), (int(x2),int(y+(x2-x)*np.tan(r))), (0,140,255), 2)

    cv2.imshow('Car Vectors', im_with_keypoints)
    rawCapture.truncate(0)

    return car_vectors


def tx_pkt(car_vectors, velocities, colors):
    pkt_list = [] # a list of the packet fields
    
    for i in range(0, len(car_vectors)): # for each car
        pkt_list.extend(car_vectors[i])
        pkt_list.append(velocities[i])
        pkt_list.extend(colors[i])

    pkt = ",".join(str(f) for f in pkt_list) + ","
    print(pkt)
    ser.write(pkt)


### Loop: Frame Processing ###
#framerate = 1000 # frames / second
#while True:
    # Step 1: Extract Car Vectors from Image
#    car_vectors = get_car_vectors()

    # Step 2: Calculate Packet Fields
#    velocities = (1, 1, 1) # Desired speed of each car
#    colors = ((255,0,0),(0,255,0),(0,0,255)) # RGB colors of each car's LED

    # Step 3: Transmit Packet
#    tx_pkt(car_vectors, velocities, colors)

    
#    if cv2.waitKey(1000/framerate) & 0xFF == ord('q'):
#        break;

