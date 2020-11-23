#!/usr/bin/env python

import rospy
import cv2
import numpy as np
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image
from sensor_msgs.msg import CompressedImage

def Gray():
	image = cv2.imread('/home/g0401828t/catkin_ws/src/opencv/images.jpeg', cv2.IMREAD_COLOR)

	cv_image = image.copy()
	dst=cv_image.copy()
        cv_gray = cv2.cvtColor(cv_image, cv2.COLOR_RGB2GRAY)

        gauss_gray = cv2.GaussianBlur(cv_gray,(3,3),3.0)
	canny_gray = cv2.Canny(gauss_gray, 50 ,150)

	lines = cv2.HoughLinesP(canny_gray, 0.1, np.pi / 180, 10, minLineLength = 10, maxLineGap = 200)

	for i in lines:
		cv2.line(dst, (i[0][0], i[0][1]), (i[0][2], i[0][3]), (0, 0, 255), 1)
		gradient = (i[0][3]-i[0][1])/(i[0][2]-i[0][0])
		print(i,(i[0][0], i[0][1]), (i[0][2], i[0][3]))
		print("gradient:",gradient)

	cv2.circle(dst, (70,15), 20, (0,0,255),1)
        cv2.imshow('dst', dst)
	cv2.imshow('cv_image',cv_image)
	cv2.waitKey(0)

if __name__ == '__main__':
    rospy.init_node('gray')
    node = Gray()
    node.main()
