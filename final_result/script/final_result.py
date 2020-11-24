#!/usr/bin/env python
import rospy
import cv2
import numpy as np
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image
from sensor_msgs.msg import CompressedImage
import math
from pyquaternion import Quaternion

np.set_printoptions(suppress=True)


img = cv2.imread('/home/kwon/navigation_result/savemap.pgm',cv2.IMREAD_GRAYSCALE)
img = cv2.cvtColor(img, cv2.COLOR_GRAY2RGB)

green_color = (0, 0, 255)

def totuple(a):
    try:
        return tuple(totuple(i) for i in a)
    except TypeError:
        return a

def DrawArrow(quaternion, origin):
    global img
    global green_color
    v = (15, 0, 0)              # initial vector   =====================>
    v = np.array(v)
    quatReceived = quaternion   # quaternion   =========================>
    quat1 = Quaternion(quatReceived)
    vRotated = quat1.rotate(v)
    vRotated = vRotated[0:2]


    arrowStart = origin    # origin of turtlebot  ===============>
    arrowEnd = arrowStart + vRotated
    arrowEnd = totuple(map(int,arrowEnd))
    arrowStart = totuple(map(int,arrowStart))


    arrowStart = (arrowStart[0]+int(2/0.02),arrowStart[1]*-1+int(3/0.02))
    arrowEnd = (arrowEnd[0]+int(2/0.02), arrowEnd[1]*-1+int(3/0.02))

    cv2.arrowedLine(img, arrowStart, arrowEnd, green_color, thickness=1)

def FinalResult():
    f = open('/home/kwon/navigation_result/arrow_log.txt', 'r')
    while True:
        line = f.readline()
        if not line: break

        origin = (float(line.split(" ")[0]),)
        origin = origin + (float(line.split(" ")[1]),)

        quaternion = (float(line.split(" ")[5]),)
        quaternion = quaternion + (float(line.split(" ")[2]),)
        quaternion = quaternion + (float(line.split(" ")[3]),)
        quaternion = quaternion + (float(line.split(" ")[4]),)
	print(quaternion)

        DrawArrow(quaternion, origin)
    f.close()

    cv2.imshow('img',img)
    cv2.waitKey(0)



if __name__ == '__main__':
    rospy.init_node('finalResult')
    FinalResult()
