import cv2 as cv
import os
import requests
import time
from matplotlib import pyplot as plt
import numpy as np

Ip = "http://192.168.178.221" 
Stream = Ip + "/"
Command = Ip + "/command?cmd="

aruco_dict = cv.aruco.getPredefinedDictionary(cv.aruco.DICT_6X6_250)    # Aruco-marker type
parameters = cv.aruco.DetectorParameters()                              # Detectieparameters
detector = cv.aruco.ArucoDetector(aruco_dict, parameters)               # aruco_detector-object

# Change working directory to the script location
# os.chdir(os.path.dirname(os.path.abspath(__file__)))

path = 'images/Camera/'

def print_image(name, img):
    cv.namedWindow(name, cv.WINDOW_NORMAL)
    w = 1280
    scaler = w/img.shape[1]
    h = int(scaler*img.shape[0])
    cv.resizeWindow(name, w, h)
    return 
    

def command(cmd):
    try:
        requests.get(Command +  cmd, timeout=0.2)
        print(f"[COMMAND] '{cmd}' verzonden naar esp32 en Arduino")
    except Exception as e:
        print(f"[FOUT] Kan commando '{cmd}' niet verzenden:", e) 

def detect_color(hsv, color, img1):
    
    if color == 'red':
        #2 ranges because red is on 2 sides of the HSV color chart
        lowerL = np.array([0  , 100, 100])
        upperL = np.array([10 , 255, 255])
        lowerR = np.array([160, 100, 100])
        upperR = np.array([179, 255, 255])
        
        #create mask
        mask = cv.bitwise_or(cv.inRange(hsv, lowerL, upperL),
                             cv.inRange(hsv, lowerR, upperR)) 
        return contours(mask, img1)
        
    
    elif color == 'green':
        lower = np.array([50, 100, 100])
        upper = np.array([70, 255, 255])

        mask = cv.inRange(hsv, lower, upper)
        return contours(mask, img1)
    
def contours(mask, img1):
    contours, _ = cv.findContours(mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    found = False
    for cnt in contours:
        area = cv.contourArea(cnt)
        
        if area > 85:
            perimeter = cv.arcLength(cnt, True)
            
            if perimeter != 0:
                circularity = 4 * np.pi * (area / (np.square(perimeter)))
                
                if circularity > 0.3:
                    (x, y), radius = cv.minEnclosingCircle(cnt)
                    diameter = radius * 2
                   
                    if diameter > 10 and diameter < 200:
                        center = (int(x), int(y))
                        radius = int(radius)
                        cv.circle(img1, center, radius, (0, 255, 0), 2)
                        found = True
    return found, img1


def detect_marker(gray):
    corners, ids, _ = detector.detectMarkers(gray)
    if ids is not None and len(ids) > 0:
        return True, corners, ids
    return False, None, None

def marker_locating(img, corners, ids):
    cv.aruco.drawDetectedMarkers(img, corners, ids)
    center_img = img.shape[1] // 2
    center_marker = int(np.mean(corners[0][0][:,0]))
    distance = center_marker - center_img


    margin = 50
    cv.line(img, (center_img, 0), (center_img, img.shape[0]), (255, 255, 0), 1)
    cv.line(img, (center_img+margin, 0), (center_img+margin, img.shape[0]), (0, 0, 255), 1)
    cv.line(img, (center_img+margin, 0), (center_img-margin, img.shape[0]), (0, 0, 255), 1)
    cv.circle(img, (center_img, int(np.mean(corners[0][0][:,1]))), 5, (255, 0, 0), -1)

    if distance < -margin:
        task = ("Left")
    elif distance > margin:
        task = ("Right")
    else:
        task = ("Mid")
    
    return task



while True:

    response = requests.get(f"{Ip}/capture", timeout=30)
    if response.status_code != 200:
        continue

    img_array = np.asarray(bytearray(response.content), dtype=np.uint8)
    img1 = cv.imdecode(img_array, cv.IMREAD_COLOR)
    if img1 is None:
        continue  

    # cv.imshow("esp32", img1)
    # if cv.waitKey(1) & 0xFF == ord('q'):
    #     break

    # img1 = cv.imread(os.path.join(path, "code.jpg"), cv.IMREAD_COLOR)
    # print_image('red',img1)

    hsv = cv.cvtColor(img1, cv.COLOR_BGR2HSV)   #RGB to HSV
    gray = cv.cvtColor(img1, cv.COLOR_BGR2GRAY) #RGB to gray
    red_detected, img = detect_color(hsv, 'red', img1)
    green_detected, img = detect_color(hsv, 'green', img1)

    marker_found, corners, ids = detect_marker(gray)

    if red_detected:
        command("r")

    if green_detected:
        command("g")

    if not green_detected and not red_detected:
        if marker_found:
            task = marker_locating(img1, corners, ids)
            command(task)
        

    # print_image("detectie", img)
    cv.imshow("detectie", img)
    cv.waitKey(30)
    


cv.destroyAllWindows()